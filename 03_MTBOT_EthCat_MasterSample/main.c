#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>     /* clock_gettime() */
#include <sys/mman.h> /* mlockall() */
#include <sched.h>    /* sched_setscheduler() */
#include <stdbool.h>
#include "math.h"

/****************************************************************************/

#include "ecrt.h"
#include "utypes.h"

/****************************************************************************/

/** Task period in ns. */
#define PERIOD_NS (1000000)

/* Constants */
#define NSEC_PER_SEC (1000000000)
#define FREQUENCY ((NSEC_PER_SEC / PERIOD_NS)/2) // 0.5s
#define SDOS_TOTAL_MAX_MEMBER (21U)

#define MAX_SAFE_STACK (8 * 1024) /* The maximum stack size which is \
                                    guranteed safe to access without \
                                    faulting */


/* Define number of Node etherCat. */
#define ETHCAT_NODE_NUM (1U)

/* Size of cmd. */
#define ETHCAT_SIZE_MSG (20u)

/****************************************************************************/

typedef enum
{
   IDLE = 0,
   POSITION = 1,
   VELOCITY = 2,
   DEPRECATED = 3,
   IMPEDANCE = 4
} tMotorMode;

typedef enum
{
   BUS_FRAME_NONE = 0,
   BUS_FRAME_PING_START = 1,
   BUS_FRAME_CANDLE_CONFIG_BAUDRATE = 2,
   BUS_FRAME_MD80_ADD = 3,
   BUS_FRAME_MD80_GENERIC_FRAME = 4,
   BUS_FRAME_MD80_CONFIG_CAN = 5,
   BUS_FRAME_BEGIN = 6,
   BUS_FRAME_END = 7,
   BUS_FRAME_UPDATE = 8,
   BUS_FRAME_RESET = 9,
} tBusFrameId;

typedef enum
{
   MD80_FRAME_FLASH_LED = 0x00,
   MD80_FRAME_MOTOR_ENABLE = 0x01,
   MD80_FRAME_CONTROL_SELECT = 0x02,
   MD80_FRAME_ZERO_ENCODER = 0x03,
   MD80_FRAME_BASE_CONFIG = 0x04,
   MD80_FRAME_GET_INFO = 0x05,
   MD80_FRAME_SET_BANDWIDTH = 0x06,
   MD80_FRAME_POS_CONTROL = 0x10,
   MD80_FRAME_VEL_CONTROL = 0x11,
   MD80_FRAME_IMP_CONTROL = 0x12,
   MD80_FRAME_RESTART = 0x13,
   MD80_FRAME_SET_MOTION_TARGETS = 0x14,
   MD80_FRAME_CAN_CONFIG = 0x20,
   MD80_FRAME_CAN_SAVE = 0x21,
   MD80_FRAME_WRITE_REGISTER = 0x40,
   MD80_FRAME_READ_REGISTER = 0x41,
   MD80_FRAME_DIAGNOSTIC = 0x69,
   MD80_FRAME_CALIBRATION = 0x70,
   MD80_FRAME_CALIBRATION_OUTPUT = 0x71,
   MD80_RESPONSE_DEFAULT = 0xA0
} tMd80CmdId;

typedef enum
{
   ETHCATNODE_IDLE = 0,
   ETHCATNODE_DEV_SCAN = 1,
   ETHCATNODE_DEV_WAITING_SCAN = 2,
   ETHCATNODE_DEV_ADD = 3,
   ETHCATNODE_DEV_CONFIG = 4,
   ETHCATNODE_DEV_CONTROL = 5,
   ETHCATNODE_DEV_SET_ZERO = 6,
   ETHCATNODE_DEV_ENABLE = 7,
   ETHCATNODE_DEV_SET_MAX_TORQUE = 8,
   ETHCATNODE_DEV_SET_MAX_VEL = 9,
   ETHCATNODE_DEV_SET_MAX_CURR = 10,
   ETHCATNODE_DEV_CONFIG_IMP = 11,
   ETHCATNODE_DEV_BEGIN = 12,
   ETHCATNODE_DEV_END = 13,
   ETHCATNODE_DEV_SET_IMP_MODE = 14,
   ETHCATNODE_DEV_RST = 15,
   ETHCATNODE_DEV_CONFIG1M_DIS_WATCHDOG = 16,
   ETHCATNODE_DEV_MD80_RST = 17,
} tEthNodeStates;

typedef struct
{
   tEthNodeStates state;
   uint8_t aliasId;
   uint8_t posId;
   uint32_t vectorId;
   uint32_t productId;
   ec_master_t *master;
   ec_domain_t *domain;
   uint8_t *domain_pd;
   ec_domain_state_t domain_state;
   ec_pdo_entry_reg_t *domain_regs;
   ec_slave_config_t *slave_cfg;
   ec_slave_config_state_t slave_state;
   ec_sync_info_t *sync_info;
   ec_sdo_request_t * sdos_respond[9];
   ec_sdo_request_t * sdos_command[9];
   ec_sdo_request_t * sdos_lastCommand[3];
   _Objects *data;
   uint8_t numMd80;
   uint32_t md80Addr[6u];
   uint8_t md80Id;
   float angleRadSet;
   float angleRadMax;
   float angleRadDt;
   float rotationCycle;
   uint8_t cmd[ETHCAT_SIZE_MSG];
} tEthNodeData;

/****************************************************************************/



/****************************************************************************/

// process data
static uint8_t *domain1_pd = NULL;

/* Alias and position of EtherCat slave 1. */
#define mtbotLanNode1 0, 0                         /* Alias, Position.       */
#define MTBOT_LAN9252_NODE1 0x00000002, 0x000ab789 /* VectorID ,Product Id.  */
#define mtbotLanNode2 0, 1                         /* Alias, Position.       */
#define MTBOT_LAN9252_NODE2 0x00000002, 0x000ab789 /* VectorID ,Product Id.  */

static _Objects  mtBotDataNode1;
static _Objects  mtBotDataNode2;

const static ec_pdo_entry_reg_t domain1_regs[] = {
    /* Array address of MD80 detected on FDCAN bus. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 1, &mtBotDataNode1.md80_addrs[0]},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 2, &mtBotDataNode1.md80_addrs[1]},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 3, &mtBotDataNode1.md80_addrs[2]},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 4, &mtBotDataNode1.md80_addrs[3]},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 5, &mtBotDataNode1.md80_addrs[4]},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 6, &mtBotDataNode1.md80_addrs[5]},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 7, &mtBotDataNode1.md80_addrs[6]},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6000, 8, &mtBotDataNode1.md80_addrs[7]},
    /* Return data of MD80 ID 0. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 1, &mtBotDataNode1.md80_0_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 2, &mtBotDataNode1.md80_0_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 3, &mtBotDataNode1.md80_0_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 4, &mtBotDataNode1.md80_0_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 5, &mtBotDataNode1.md80_0_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 6, &mtBotDataNode1.md80_0_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 7, &mtBotDataNode1.md80_0_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 8, &mtBotDataNode1.md80_0_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6001, 9, &mtBotDataNode1.md80_0_DataReturn.timestamp},
    /* Return data of MD80 ID 1. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 1, &mtBotDataNode1.md80_1_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 2, &mtBotDataNode1.md80_1_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 3, &mtBotDataNode1.md80_1_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 4, &mtBotDataNode1.md80_1_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 5, &mtBotDataNode1.md80_1_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 6, &mtBotDataNode1.md80_1_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 7, &mtBotDataNode1.md80_1_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 8, &mtBotDataNode1.md80_1_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6002, 9, &mtBotDataNode1.md80_1_DataReturn.timestamp},
    /* Return data of MD80 ID 2. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 1, &mtBotDataNode1.md80_2_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 2, &mtBotDataNode1.md80_2_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 3, &mtBotDataNode1.md80_2_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 4, &mtBotDataNode1.md80_2_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 5, &mtBotDataNode1.md80_2_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 6, &mtBotDataNode1.md80_2_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 7, &mtBotDataNode1.md80_2_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 8, &mtBotDataNode1.md80_2_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6003, 9, &mtBotDataNode1.md80_2_DataReturn.timestamp},
    /* Return data of MD80 ID 3. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 1, &mtBotDataNode1.md80_3_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 2, &mtBotDataNode1.md80_3_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 3, &mtBotDataNode1.md80_3_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 4, &mtBotDataNode1.md80_3_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 5, &mtBotDataNode1.md80_3_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 6, &mtBotDataNode1.md80_3_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 7, &mtBotDataNode1.md80_3_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 8, &mtBotDataNode1.md80_3_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6004, 9, &mtBotDataNode1.md80_3_DataReturn.timestamp},
    /* Return data of MD80 ID 4. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 1, &mtBotDataNode1.md80_4_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 2, &mtBotDataNode1.md80_4_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 3, &mtBotDataNode1.md80_4_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 4, &mtBotDataNode1.md80_4_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 5, &mtBotDataNode1.md80_4_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 6, &mtBotDataNode1.md80_4_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 7, &mtBotDataNode1.md80_4_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 8, &mtBotDataNode1.md80_4_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6005, 9, &mtBotDataNode1.md80_4_DataReturn.timestamp},
    /* Return data of MD80 ID 5. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 1, &mtBotDataNode1.md80_5_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 2, &mtBotDataNode1.md80_5_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 3, &mtBotDataNode1.md80_5_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 4, &mtBotDataNode1.md80_5_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 5, &mtBotDataNode1.md80_5_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 6, &mtBotDataNode1.md80_5_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 7, &mtBotDataNode1.md80_5_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 8, &mtBotDataNode1.md80_5_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6006, 9, &mtBotDataNode1.md80_5_DataReturn.timestamp},
    /* Return data of MD80 ID 6. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 1, &mtBotDataNode1.md80_6_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 2, &mtBotDataNode1.md80_6_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 3, &mtBotDataNode1.md80_6_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 4, &mtBotDataNode1.md80_6_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 5, &mtBotDataNode1.md80_6_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 6, &mtBotDataNode1.md80_6_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 7, &mtBotDataNode1.md80_6_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 8, &mtBotDataNode1.md80_6_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6007, 9, &mtBotDataNode1.md80_6_DataReturn.timestamp},
    /* Return data of MD80 ID 7. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 1, &mtBotDataNode1.md80_7_DataReturn.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 2, &mtBotDataNode1.md80_7_DataReturn.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 3, &mtBotDataNode1.md80_7_DataReturn.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 4, &mtBotDataNode1.md80_7_DataReturn.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 5, &mtBotDataNode1.md80_7_DataReturn.Temperature},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 6, &mtBotDataNode1.md80_7_DataReturn.Error},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 7, &mtBotDataNode1.md80_7_DataReturn.enabled},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 8, &mtBotDataNode1.md80_7_DataReturn.counter},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x6008, 9, &mtBotDataNode1.md80_7_DataReturn.timestamp},

    /* Return data of MD80 ID 0. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7001, 1, &mtBotDataNode1.md80_0_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7001, 2, &mtBotDataNode1.md80_0_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7001, 3, &mtBotDataNode1.md80_0_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7001, 4, &mtBotDataNode1.md80_0_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7001, 5, &mtBotDataNode1.md80_0_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7001, 6, &mtBotDataNode1.md80_0_DataControl.counter},
    /* Return data of MD80 ID 1. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7002, 1, &mtBotDataNode1.md80_1_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7002, 2, &mtBotDataNode1.md80_1_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7002, 3, &mtBotDataNode1.md80_1_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7002, 4, &mtBotDataNode1.md80_1_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7002, 5, &mtBotDataNode1.md80_1_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7002, 6, &mtBotDataNode1.md80_1_DataControl.counter},
    /* Return data of MD80 ID 2. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7003, 1, &mtBotDataNode1.md80_2_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7003, 2, &mtBotDataNode1.md80_2_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7003, 3, &mtBotDataNode1.md80_2_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7003, 4, &mtBotDataNode1.md80_2_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7003, 5, &mtBotDataNode1.md80_2_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7003, 6, &mtBotDataNode1.md80_2_DataControl.counter},
    /* Return data of MD80 ID 3. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7004, 1, &mtBotDataNode1.md80_3_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7004, 2, &mtBotDataNode1.md80_3_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7004, 3, &mtBotDataNode1.md80_3_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7004, 4, &mtBotDataNode1.md80_3_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7004, 5, &mtBotDataNode1.md80_3_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7004, 6, &mtBotDataNode1.md80_3_DataControl.counter},
    /* Return data of MD80 ID 4. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7005, 1, &mtBotDataNode1.md80_4_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7005, 2, &mtBotDataNode1.md80_4_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7005, 3, &mtBotDataNode1.md80_4_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7005, 4, &mtBotDataNode1.md80_4_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7005, 5, &mtBotDataNode1.md80_4_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7005, 6, &mtBotDataNode1.md80_4_DataControl.counter},
    /* Return data of MD80 ID 5. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7006, 1, &mtBotDataNode1.md80_5_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7006, 2, &mtBotDataNode1.md80_5_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7006, 3, &mtBotDataNode1.md80_5_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7006, 4, &mtBotDataNode1.md80_5_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7006, 5, &mtBotDataNode1.md80_5_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7006, 6, &mtBotDataNode1.md80_5_DataControl.counter},
    /* Return data of MD80 ID 6. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7007, 1, &mtBotDataNode1.md80_6_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7007, 2, &mtBotDataNode1.md80_6_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7007, 3, &mtBotDataNode1.md80_6_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7007, 4, &mtBotDataNode1.md80_6_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7007, 5, &mtBotDataNode1.md80_6_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7007, 6, &mtBotDataNode1.md80_6_DataControl.counter},
    /* Return data of MD80 ID 7. */
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7008, 1, &mtBotDataNode1.md80_7_DataControl.Position},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7008, 2, &mtBotDataNode1.md80_7_DataControl.Velocity},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7008, 3, &mtBotDataNode1.md80_7_DataControl.Torque},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7008, 4, &mtBotDataNode1.md80_7_DataControl.Mode},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7008, 5, &mtBotDataNode1.md80_7_DataControl.enable},
    {mtbotLanNode1, MTBOT_LAN9252_NODE1, 0x7008, 6, &mtBotDataNode1.md80_7_DataControl.counter},
    {}};

const static ec_pdo_entry_reg_t domain2_regs[] = {
    /* Array address of MD80 detected on FDCAN bus. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 1, &mtBotDataNode2.md80_addrs[0]},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 2, &mtBotDataNode2.md80_addrs[1]},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 3, &mtBotDataNode2.md80_addrs[2]},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 4, &mtBotDataNode2.md80_addrs[3]},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 5, &mtBotDataNode2.md80_addrs[4]},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 6, &mtBotDataNode2.md80_addrs[5]},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 7, &mtBotDataNode2.md80_addrs[6]},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6000, 8, &mtBotDataNode2.md80_addrs[7]},
    /* Return data of MD80 ID 0. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 1, &mtBotDataNode2.md80_0_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 2, &mtBotDataNode2.md80_0_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 3, &mtBotDataNode2.md80_0_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 4, &mtBotDataNode2.md80_0_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 5, &mtBotDataNode2.md80_0_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 6, &mtBotDataNode2.md80_0_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 7, &mtBotDataNode2.md80_0_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 8, &mtBotDataNode2.md80_0_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6001, 9, &mtBotDataNode2.md80_0_DataReturn.timestamp},
    /* Return data of MD80 ID 1. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 1, &mtBotDataNode2.md80_1_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 2, &mtBotDataNode2.md80_1_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 3, &mtBotDataNode2.md80_1_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 4, &mtBotDataNode2.md80_1_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 5, &mtBotDataNode2.md80_1_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 6, &mtBotDataNode2.md80_1_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 7, &mtBotDataNode2.md80_1_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 8, &mtBotDataNode2.md80_1_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6002, 9, &mtBotDataNode2.md80_1_DataReturn.timestamp},
    /* Return data of MD80 ID 2. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 1, &mtBotDataNode2.md80_2_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 2, &mtBotDataNode2.md80_2_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 3, &mtBotDataNode2.md80_2_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 4, &mtBotDataNode2.md80_2_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 5, &mtBotDataNode2.md80_2_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 6, &mtBotDataNode2.md80_2_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 7, &mtBotDataNode2.md80_2_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 8, &mtBotDataNode2.md80_2_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6003, 9, &mtBotDataNode2.md80_2_DataReturn.timestamp},
    /* Return data of MD80 ID 3. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 1, &mtBotDataNode2.md80_3_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 2, &mtBotDataNode2.md80_3_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 3, &mtBotDataNode2.md80_3_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 4, &mtBotDataNode2.md80_3_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 5, &mtBotDataNode2.md80_3_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 6, &mtBotDataNode2.md80_3_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 7, &mtBotDataNode2.md80_3_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 8, &mtBotDataNode2.md80_3_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6004, 9, &mtBotDataNode2.md80_3_DataReturn.timestamp},
    /* Return data of MD80 ID 4. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 1, &mtBotDataNode2.md80_4_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 2, &mtBotDataNode2.md80_4_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 3, &mtBotDataNode2.md80_4_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 4, &mtBotDataNode2.md80_4_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 5, &mtBotDataNode2.md80_4_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 6, &mtBotDataNode2.md80_4_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 7, &mtBotDataNode2.md80_4_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 8, &mtBotDataNode2.md80_4_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6005, 9, &mtBotDataNode2.md80_4_DataReturn.timestamp},
    /* Return data of MD80 ID 5. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 1, &mtBotDataNode2.md80_5_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 2, &mtBotDataNode2.md80_5_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 3, &mtBotDataNode2.md80_5_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 4, &mtBotDataNode2.md80_5_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 5, &mtBotDataNode2.md80_5_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 6, &mtBotDataNode2.md80_5_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 7, &mtBotDataNode2.md80_5_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 8, &mtBotDataNode2.md80_5_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6006, 9, &mtBotDataNode2.md80_5_DataReturn.timestamp},
    /* Return data of MD80 ID 6. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 1, &mtBotDataNode2.md80_6_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 2, &mtBotDataNode2.md80_6_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 3, &mtBotDataNode2.md80_6_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 4, &mtBotDataNode2.md80_6_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 5, &mtBotDataNode2.md80_6_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 6, &mtBotDataNode2.md80_6_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 7, &mtBotDataNode2.md80_6_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 8, &mtBotDataNode2.md80_6_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6007, 9, &mtBotDataNode2.md80_6_DataReturn.timestamp},
    /* Return data of MD80 ID 7. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 1, &mtBotDataNode2.md80_7_DataReturn.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 2, &mtBotDataNode2.md80_7_DataReturn.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 3, &mtBotDataNode2.md80_7_DataReturn.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 4, &mtBotDataNode2.md80_7_DataReturn.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 5, &mtBotDataNode2.md80_7_DataReturn.Temperature},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 6, &mtBotDataNode2.md80_7_DataReturn.Error},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 7, &mtBotDataNode2.md80_7_DataReturn.enabled},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 8, &mtBotDataNode2.md80_7_DataReturn.counter},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x6008, 9, &mtBotDataNode2.md80_7_DataReturn.timestamp},

    /* Return data of MD80 ID 0. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7001, 1, &mtBotDataNode2.md80_0_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7001, 2, &mtBotDataNode2.md80_0_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7001, 3, &mtBotDataNode2.md80_0_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7001, 4, &mtBotDataNode2.md80_0_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7001, 5, &mtBotDataNode2.md80_0_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7001, 6, &mtBotDataNode2.md80_0_DataControl.counter},
    /* Return data of MD80 ID 1. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7002, 1, &mtBotDataNode2.md80_1_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7002, 2, &mtBotDataNode2.md80_1_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7002, 3, &mtBotDataNode2.md80_1_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7002, 4, &mtBotDataNode2.md80_1_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7002, 5, &mtBotDataNode2.md80_1_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7002, 6, &mtBotDataNode2.md80_1_DataControl.counter},
    /* Return data of MD80 ID 2. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7003, 1, &mtBotDataNode2.md80_2_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7003, 2, &mtBotDataNode2.md80_2_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7003, 3, &mtBotDataNode2.md80_2_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7003, 4, &mtBotDataNode2.md80_2_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7003, 5, &mtBotDataNode2.md80_2_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7003, 6, &mtBotDataNode2.md80_2_DataControl.counter},
    /* Return data of MD80 ID 3. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7004, 1, &mtBotDataNode2.md80_3_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7004, 2, &mtBotDataNode2.md80_3_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7004, 3, &mtBotDataNode2.md80_3_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7004, 4, &mtBotDataNode2.md80_3_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7004, 5, &mtBotDataNode2.md80_3_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7004, 6, &mtBotDataNode2.md80_3_DataControl.counter},
    /* Return data of MD80 ID 4. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7005, 1, &mtBotDataNode2.md80_4_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7005, 2, &mtBotDataNode2.md80_4_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7005, 3, &mtBotDataNode2.md80_4_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7005, 4, &mtBotDataNode2.md80_4_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7005, 5, &mtBotDataNode2.md80_4_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7005, 6, &mtBotDataNode2.md80_4_DataControl.counter},
    /* Return data of MD80 ID 5. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7006, 1, &mtBotDataNode2.md80_5_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7006, 2, &mtBotDataNode2.md80_5_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7006, 3, &mtBotDataNode2.md80_5_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7006, 4, &mtBotDataNode2.md80_5_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7006, 5, &mtBotDataNode2.md80_5_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7006, 6, &mtBotDataNode2.md80_5_DataControl.counter},
    /* Return data of MD80 ID 6. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7007, 1, &mtBotDataNode2.md80_6_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7007, 2, &mtBotDataNode2.md80_6_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7007, 3, &mtBotDataNode2.md80_6_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7007, 4, &mtBotDataNode2.md80_6_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7007, 5, &mtBotDataNode2.md80_6_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7007, 6, &mtBotDataNode2.md80_6_DataControl.counter},
    /* Return data of MD80 ID 7. */
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7008, 1, &mtBotDataNode2.md80_7_DataControl.Position},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7008, 2, &mtBotDataNode2.md80_7_DataControl.Velocity},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7008, 3, &mtBotDataNode2.md80_7_DataControl.Torque},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7008, 4, &mtBotDataNode2.md80_7_DataControl.Mode},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7008, 5, &mtBotDataNode2.md80_7_DataControl.enable},
    {mtbotLanNode2, MTBOT_LAN9252_NODE2, 0x7008, 6, &mtBotDataNode2.md80_7_DataControl.counter},
    {}};

// EtherCAT
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};

static ec_domain_t *domain = NULL;
static ec_domain_state_t domain_state = {};

static ec_slave_config_t *sc_lan9252_Node1 = NULL;
static ec_slave_config_state_t sc_lan9252_Nod1_in_state = {};

/*****************************************************************************/

static ec_pdo_entry_info_t mtbotNode1_pdo_entries[] = {
    {0x6000, 1, 32}, /* Md80_0 address. */
    {0x6000, 2, 32}, /* Md80_1 address. */
    {0x6000, 3, 32}, /* Md80_2 address. */
    {0x6000, 4, 32}, /* Md80_3 address. */
    {0x6000, 5, 32}, /* Md80_4 address. */
    {0x6000, 6, 32}, /* Md80_5 address. */
    {0x6000, 7, 32}, /* Md80_6 address. */
    {0x6000, 8, 32}, /* Md80_7 address. */

    {0x6001, 1, 8},  // channel 1 status
    {0x6001, 2, 16}, // channel 1 status
    {0x6001, 3, 16}, // channel 1 status
    {0x6001, 4, 16}, // channel 1 status
    {0x6001, 5, 8},  // channel 1 status
    {0x6001, 6, 8},  // channel 1 status
    {0x6001, 7, 8},  // channel 1 status
    {0x6001, 8, 32}, // channel 1 status
    {0x6001, 9, 32}, // channel 1 status

    {0x6002, 1, 8},  // channel 2 status
    {0x6002, 2, 16}, // channel 2 status
    {0x6002, 3, 16}, // channel 2 status
    {0x6002, 4, 16}, // channel 2 status
    {0x6002, 5, 8},  // channel 2 status
    {0x6002, 6, 8},  // channel 2 status
    {0x6002, 7, 8},  // channel 2 status
    {0x6002, 8, 32}, // channel 1 status
    {0x6002, 9, 32}, // channel 1 status

    {0x6003, 1, 8},  // channel 3 status
    {0x6003, 2, 16}, // channel 3 status
    {0x6003, 3, 16}, // channel 3 status
    {0x6003, 4, 16}, // channel 3 status
    {0x6003, 5, 8},  // channel 3 status
    {0x6003, 6, 8},  // channel 3 status
    {0x6003, 7, 8},  // channel 3 status
    {0x6003, 8, 32}, // channel 1 status
    {0x6003, 9, 32}, // channel 1 status

    {0x6004, 1, 8},  // channel 4 status
    {0x6004, 2, 16}, // channel 4 status
    {0x6004, 3, 16}, // channel 4 status
    {0x6004, 4, 16}, // channel 4 status
    {0x6004, 5, 8},  // channel 4 status
    {0x6004, 6, 8},  // channel 4 status
    {0x6004, 7, 8},  // channel 4 status
    {0x6004, 8, 32}, // channel 1 status
    {0x6004, 9, 32}, // channel 1 status

    {0x6005, 1, 8},  // channel 4 status
    {0x6005, 2, 16}, // channel 4 status
    {0x6005, 3, 16}, // channel 4 status
    {0x6005, 4, 16}, // channel 4 status
    {0x6005, 5, 8},  // channel 4 status
    {0x6005, 6, 8},  // channel 4 status
    {0x6005, 7, 8},  // channel 4 status
    {0x6005, 8, 32}, // channel 1 status
    {0x6005, 9, 32}, // channel 1 status

    {0x6006, 1, 8},  // channel 4 status
    {0x6006, 2, 16}, // channel 4 status
    {0x6006, 3, 16}, // channel 4 status
    {0x6006, 4, 16}, // channel 4 status
    {0x6006, 5, 8},  // channel 4 status
    {0x6006, 6, 8},  // channel 4 status
    {0x6006, 7, 8},  // channel 4 status
    {0x6006, 8, 32}, // channel 1 status
    {0x6006, 9, 32}, // channel 1 status

    {0x6007, 1, 8},  // channel 4 status
    {0x6007, 2, 16}, // channel 4 status
    {0x6007, 3, 16}, // channel 4 status
    {0x6007, 4, 16}, // channel 4 status
    {0x6007, 5, 8},  // channel 4 status
    {0x6007, 6, 8},  // channel 4 status
    {0x6007, 7, 8},  // channel 4 status
    {0x6007, 8, 32}, // channel 1 status
    {0x6007, 9, 32}, // channel 1 status

    {0x6008, 1, 8},  // channel 4 status
    {0x6008, 2, 16}, // channel 4 status
    {0x6008, 3, 16}, // channel 4 status
    {0x6008, 4, 16}, // channel 4 status
    {0x6008, 5, 8},  // channel 4 status
    {0x6008, 6, 8},  // channel 4 status
    {0x6008, 7, 8},  // channel 4 status
    {0x6008, 8, 32}, // channel 1 status
    {0x6008, 9, 32}, // channel 1 status

    {0x7001, 1, 16}, // channel 1 status
    {0x7001, 2, 16}, // channel 1 status
    {0x7001, 3, 16}, // channel 1 status
    {0x7001, 4, 8},  // channel 1 status
    {0x7001, 5, 8},  // channel 1 status
    {0x7001, 6, 32}, // channel 1 status

    {0x7002, 1, 16}, // channel 2 status
    {0x7002, 2, 16}, // channel 2 status
    {0x7002, 3, 16}, // channel 2 status
    {0x7002, 4, 8},  // channel 2 status
    {0x7002, 5, 8},  // channel 2 status
    {0x7002, 6, 32}, // channel 1 status

    {0x7003, 1, 16}, // channel 3 status
    {0x7003, 2, 16}, // channel 3 status
    {0x7003, 3, 16}, // channel 3 status
    {0x7003, 4, 8},  // channel 3 status
    {0x7003, 5, 8},  // channel 3 status
    {0x7003, 6, 32}, // channel 1 status

    {0x7004, 1, 16}, // channel 4 status
    {0x7004, 2, 16}, // channel 4 status
    {0x7004, 3, 16}, // channel 4 status
    {0x7004, 4, 8},  // channel 4 status
    {0x7004, 5, 8},  // channel 4 status
    {0x7004, 6, 32}, // channel 1 status

    {0x7005, 1, 16}, // channel 4 status
    {0x7005, 2, 16}, // channel 4 status
    {0x7005, 3, 16}, // channel 4 status
    {0x7005, 4, 8},  // channel 4 status
    {0x7005, 5, 8},  // channel 4 status
    {0x7005, 6, 32}, // channel 1 status

    {0x7006, 1, 16}, // channel 4 status
    {0x7006, 2, 16}, // channel 4 status
    {0x7006, 3, 16}, // channel 4 status
    {0x7006, 4, 8},  // channel 4 status
    {0x7006, 5, 8},  // channel 4 status
    {0x7006, 6, 32}, // channel 1 status

    {0x7007, 1, 16}, // channel 4 status
    {0x7007, 2, 16}, // channel 4 status
    {0x7007, 3, 16}, // channel 4 status
    {0x7007, 4, 8},  // channel 4 status
    {0x7007, 5, 8},  // channel 4 status
    {0x7007, 6, 32}, // channel 1 status

    {0x7008, 1, 16}, // channel 4 status
    {0x7008, 2, 16}, // channel 4 status
    {0x7008, 3, 16}, // channel 4 status
    {0x7008, 4, 8},  // channel 4 status
    {0x7008, 5, 8},  // channel 4 status
    {0x7008, 6, 32}, // channel 1 status
};

static ec_pdo_entry_info_t mtbotNode2_pdo_entries[] = {
    {0x6000, 1, 32}, /* Md80_0 address. */
    {0x6000, 2, 32}, /* Md80_1 address. */
    {0x6000, 3, 32}, /* Md80_2 address. */
    {0x6000, 4, 32}, /* Md80_3 address. */
    {0x6000, 5, 32}, /* Md80_4 address. */
    {0x6000, 6, 32}, /* Md80_5 address. */
    {0x6000, 7, 32}, /* Md80_6 address. */
    {0x6000, 8, 32}, /* Md80_7 address. */

    {0x6001, 1, 8},  // channel 1 status
    {0x6001, 2, 16}, // channel 1 status
    {0x6001, 3, 16}, // channel 1 status
    {0x6001, 4, 16}, // channel 1 status
    {0x6001, 5, 8},  // channel 1 status
    {0x6001, 6, 8},  // channel 1 status
    {0x6001, 7, 8},  // channel 1 status
    {0x6001, 8, 32}, // channel 1 status
    {0x6001, 9, 32}, // channel 1 status

    {0x6002, 1, 8},  // channel 2 status
    {0x6002, 2, 16}, // channel 2 status
    {0x6002, 3, 16}, // channel 2 status
    {0x6002, 4, 16}, // channel 2 status
    {0x6002, 5, 8},  // channel 2 status
    {0x6002, 6, 8},  // channel 2 status
    {0x6002, 7, 8},  // channel 2 status
    {0x6002, 8, 32}, // channel 1 status
    {0x6002, 9, 32}, // channel 1 status

    {0x6003, 1, 8},  // channel 3 status
    {0x6003, 2, 16}, // channel 3 status
    {0x6003, 3, 16}, // channel 3 status
    {0x6003, 4, 16}, // channel 3 status
    {0x6003, 5, 8},  // channel 3 status
    {0x6003, 6, 8},  // channel 3 status
    {0x6003, 7, 8},  // channel 3 status
    {0x6003, 8, 32}, // channel 1 status
    {0x6003, 9, 32}, // channel 1 status

    {0x6004, 1, 8},  // channel 4 status
    {0x6004, 2, 16}, // channel 4 status
    {0x6004, 3, 16}, // channel 4 status
    {0x6004, 4, 16}, // channel 4 status
    {0x6004, 5, 8},  // channel 4 status
    {0x6004, 6, 8},  // channel 4 status
    {0x6004, 7, 8},  // channel 4 status
    {0x6004, 8, 32}, // channel 1 status
    {0x6004, 9, 32}, // channel 1 status

    {0x6005, 1, 8},  // channel 4 status
    {0x6005, 2, 16}, // channel 4 status
    {0x6005, 3, 16}, // channel 4 status
    {0x6005, 4, 16}, // channel 4 status
    {0x6005, 5, 8},  // channel 4 status
    {0x6005, 6, 8},  // channel 4 status
    {0x6005, 7, 8},  // channel 4 status
    {0x6005, 8, 32}, // channel 1 status
    {0x6005, 9, 32}, // channel 1 status

    {0x6006, 1, 8},  // channel 4 status
    {0x6006, 2, 16}, // channel 4 status
    {0x6006, 3, 16}, // channel 4 status
    {0x6006, 4, 16}, // channel 4 status
    {0x6006, 5, 8},  // channel 4 status
    {0x6006, 6, 8},  // channel 4 status
    {0x6006, 7, 8},  // channel 4 status
    {0x6006, 8, 32}, // channel 1 status
    {0x6006, 9, 32}, // channel 1 status

    {0x6007, 1, 8},  // channel 4 status
    {0x6007, 2, 16}, // channel 4 status
    {0x6007, 3, 16}, // channel 4 status
    {0x6007, 4, 16}, // channel 4 status
    {0x6007, 5, 8},  // channel 4 status
    {0x6007, 6, 8},  // channel 4 status
    {0x6007, 7, 8},  // channel 4 status
    {0x6007, 8, 32}, // channel 1 status
    {0x6007, 9, 32}, // channel 1 status

    {0x6008, 1, 8},  // channel 4 status
    {0x6008, 2, 16}, // channel 4 status
    {0x6008, 3, 16}, // channel 4 status
    {0x6008, 4, 16}, // channel 4 status
    {0x6008, 5, 8},  // channel 4 status
    {0x6008, 6, 8},  // channel 4 status
    {0x6008, 7, 8},  // channel 4 status
    {0x6008, 8, 32}, // channel 1 status
    {0x6008, 9, 32}, // channel 1 status

    {0x7001, 1, 16}, // channel 1 status
    {0x7001, 2, 16}, // channel 1 status
    {0x7001, 3, 16}, // channel 1 status
    {0x7001, 4, 8},  // channel 1 status
    {0x7001, 5, 8},  // channel 1 status
    {0x7001, 6, 32}, // channel 1 status

    {0x7002, 1, 16}, // channel 2 status
    {0x7002, 2, 16}, // channel 2 status
    {0x7002, 3, 16}, // channel 2 status
    {0x7002, 4, 8},  // channel 2 status
    {0x7002, 5, 8},  // channel 2 status
    {0x7002, 6, 32}, // channel 1 status

    {0x7003, 1, 16}, // channel 3 status
    {0x7003, 2, 16}, // channel 3 status
    {0x7003, 3, 16}, // channel 3 status
    {0x7003, 4, 8},  // channel 3 status
    {0x7003, 5, 8},  // channel 3 status
    {0x7003, 6, 32}, // channel 1 status

    {0x7004, 1, 16}, // channel 4 status
    {0x7004, 2, 16}, // channel 4 status
    {0x7004, 3, 16}, // channel 4 status
    {0x7004, 4, 8},  // channel 4 status
    {0x7004, 5, 8},  // channel 4 status
    {0x7004, 6, 32}, // channel 1 status

    {0x7005, 1, 16}, // channel 4 status
    {0x7005, 2, 16}, // channel 4 status
    {0x7005, 3, 16}, // channel 4 status
    {0x7005, 4, 8},  // channel 4 status
    {0x7005, 5, 8},  // channel 4 status
    {0x7005, 6, 32}, // channel 1 status

    {0x7006, 1, 16}, // channel 4 status
    {0x7006, 2, 16}, // channel 4 status
    {0x7006, 3, 16}, // channel 4 status
    {0x7006, 4, 8},  // channel 4 status
    {0x7006, 5, 8},  // channel 4 status
    {0x7006, 6, 32}, // channel 1 status

    {0x7007, 1, 16}, // channel 4 status
    {0x7007, 2, 16}, // channel 4 status
    {0x7007, 3, 16}, // channel 4 status
    {0x7007, 4, 8},  // channel 4 status
    {0x7007, 5, 8},  // channel 4 status
    {0x7007, 6, 32}, // channel 1 status

    {0x7008, 1, 16}, // channel 4 status
    {0x7008, 2, 16}, // channel 4 status
    {0x7008, 3, 16}, // channel 4 status
    {0x7008, 4, 8},  // channel 4 status
    {0x7008, 5, 8},  // channel 4 status
    {0x7008, 6, 32}, // channel 1 status
};

static ec_pdo_info_t mtbotNode1_pdos[] = {
    {0x1A00, 80, mtbotNode1_pdo_entries},
    {0x1600, 48, mtbotNode1_pdo_entries + 80}};

static ec_sync_info_t mtbotNode1_syncs[] = {
    {2, EC_DIR_OUTPUT, 1, mtbotNode1_pdos + 1},
    {3, EC_DIR_INPUT, 1, mtbotNode1_pdos},
    {0xff}};

static ec_pdo_info_t mtbotNode2_pdos[] = {
    {0x1A00, 80, mtbotNode2_pdo_entries},
    {0x1600, 48, mtbotNode2_pdo_entries + 80}};

static ec_sync_info_t mtbotNode2_syncs[] = {
    {2, EC_DIR_OUTPUT, 1, mtbotNode2_pdos + 1},
    {3, EC_DIR_INPUT, 1, mtbotNode2_pdos},
    {0xff}};

/* Define number of node etherCat. */
static tEthNodeData ethNode[ETHCAT_NODE_NUM] = {
    /* Node 1. */
    {
        .domain_regs = domain1_regs,
        .data = &mtBotDataNode1,
        .aliasId = 0,
        .posId = 0,
        .vectorId = 0x00000002U,
        .productId = 0x000ab789U,
        .sync_info = (ec_sync_info_t *)&mtbotNode1_syncs[0],
        .angleRadDt = 0.0f,
        .angleRadMax = 2 * M_PI,
        .angleRadDt = 0.01f,
        .rotationCycle = true,
    },
    /* Node 2. */
    {
        .domain_regs = domain2_regs,
        .data = &mtBotDataNode2,
        .aliasId = 0,
        .posId = 1,
        .vectorId = 0x00000002U,
        .productId = 0x000ab789U,
        .sync_info = (ec_sync_info_t *)&mtbotNode2_syncs[0],
        .angleRadDt = 0.0f,
        .angleRadMax = 2 * M_PI,
        .angleRadDt = 0.01f,
        .rotationCycle = true,
    }
};

static uint32_t counter = 0U;
static uint8_t countEachTimeSendData = 0U;

/*****************************************************************************/

void check_domain_state(tEthNodeData *node)
{
   ec_domain_state_t ds;

   ecrt_domain_state(node->domain, &ds);

   if (ds.working_counter != node->domain_state.working_counter)
   {
      printf("Domain: WC %u.\n", ds.working_counter);
   }
   if (ds.wc_state != node->domain_state.wc_state)
   {
      printf("Domain: State %u.\n", ds.wc_state);
   }

   node->domain_state = ds;
}

/*****************************************************************************/

void check_master_state(void)
{
   ec_master_state_t ms;

   ecrt_master_state(master, &ms);

   if (ms.slaves_responding != master_state.slaves_responding)
   {
      printf("%u slave(s).\n", ms.slaves_responding);
   }
   if (ms.al_states != master_state.al_states)
   {
      printf("AL states: 0x%02X.\n", ms.al_states);
   }
   if (ms.link_up != master_state.link_up)
   {
      printf("Link is %s.\n", ms.link_up ? "up" : "down");
   }

   master_state = ms;
}

/*****************************************************************************/

void check_slave_config_states(tEthNodeData *node)
{
   ec_slave_config_state_t s;

   ecrt_slave_config_state(node->slave_cfg, &s);

   if (s.al_state != node->slave_state.al_state)
   {
      printf("LAN9252: State 0x%02X.\n", s.al_state);
   }
   if (s.online != node->slave_state.online)
   {
      printf("LAN9252: %s.\n", s.online ? "online" : "offline");
   }
   if (s.operational != node->slave_state.operational)
   {
      printf("LAN9252: %soperational.\n", s.operational ? "" : "Not ");
   }

   node->slave_state = s;
}

/*****************************************************************************/

uint32_t read_sdo(ec_sdo_request_t * const sdo)
{
   uint32_t dataRet = 0u;

   /* Read response from Reflexor. */
   switch (ecrt_sdo_request_state(sdo))
   {
   case EC_REQUEST_UNUSED:        // request was not used yet
      ecrt_sdo_request_read(sdo); // trigger first read
      break;
   case EC_REQUEST_BUSY:
      printf("Read Still busy...\n");
      break;
   case EC_REQUEST_SUCCESS:
      switch (ecrt_sdo_request_data_size(sdo))
      {
      case 1:
         dataRet = EC_READ_U8(ecrt_sdo_request_data(sdo));
         break;
      case 2:
         dataRet = EC_READ_U16(ecrt_sdo_request_data(sdo));
         break;
      case 4:
         dataRet = EC_READ_U32(ecrt_sdo_request_data(sdo));
         break;
      default:
         break;
      }
      printf("Read SDO value: 0x%04X\n", dataRet);
      ecrt_sdo_request_read(sdo); // trigger next read

      break;
   case EC_REQUEST_ERROR:
      printf("Read Failed to read SDO!\n");
      ecrt_sdo_request_read(sdo); // retry reading
      break;
   }

   return dataRet;
}

/*****************************************************************************/

void write_sdo(ec_sdo_request_t * const sdo, uint32_t dataReq)
{
   int ret = -1;

   /* Read response from Reflexor. */
   switch (ecrt_sdo_request_state(sdo))
   {
   case EC_REQUEST_BUSY:
      printf("Still busy...\n");
      ret = -1;
      break;
   case EC_REQUEST_UNUSED:
   case EC_REQUEST_SUCCESS:
      switch (ecrt_sdo_request_data_size(sdo))
      {
      case 1:
         EC_WRITE_U8(ecrt_sdo_request_data(sdo), (dataReq));
         break;
      case 2:
         EC_WRITE_U16(ecrt_sdo_request_data(sdo), (dataReq));
         break;
      case 4:
         EC_WRITE_U32(ecrt_sdo_request_data(sdo), (dataReq));
         break;
      default:
         break;
      }
      printf("Write: SDO value: 0x%04X\n", dataReq);
      ecrt_sdo_request_write(sdo);

      ret = 0;

      break;
   case EC_REQUEST_ERROR:
      printf("Write: Failed to write SDO!\n");
      ecrt_sdo_request_write(sdo); // retry reading
      ret = -1;
      break;
   }

   return ret;
}

/*****************************************************************************/

void reflexor_sdos_write (const tEthNodeData * const slv)
{
   /* Send all data structure of command to Reflexor. */
   write_sdo (slv->sdos_command[0], slv->data->md80_Command.md80_dev_no);
   write_sdo (slv->sdos_command[1], slv->data->md80_Command.command);
   write_sdo (slv->sdos_command[2], slv->data->md80_Command.size);
   write_sdo (slv->sdos_command[3], slv->data->md80_Command.counter);
   write_sdo (slv->sdos_command[4], slv->data->md80_Command.dataSet0);
   write_sdo (slv->sdos_command[5], slv->data->md80_Command.dataSet1);
   write_sdo (slv->sdos_command[6], slv->data->md80_Command.dataSet2);
   write_sdo (slv->sdos_command[7], slv->data->md80_Command.dataSet3);
   write_sdo (slv->sdos_command[8], slv->data->md80_Command.dataSet4);
}

void reflexor_sdos_read (tEthNodeData * const slv)
{
   /* Send all data structure of command to Reflexor. */
   slv->data->md80_Respond.md80_dev_no = read_sdo (slv->sdos_respond[0]);
   slv->data->md80_Respond.command = read_sdo (slv->sdos_respond[1]);   
   slv->data->md80_Respond.size = read_sdo (slv->sdos_respond[2]);
   slv->data->md80_Respond.timestamp = read_sdo (slv->sdos_respond[3]);   
   slv->data->md80_Respond.dataRet0 = read_sdo (slv->sdos_respond[4]);
   slv->data->md80_Respond.dataRet1 = read_sdo (slv->sdos_respond[5]);
   slv->data->md80_Respond.dataRet2 = read_sdo (slv->sdos_respond[6]);
   slv->data->md80_Respond.dataRet3 = read_sdo (slv->sdos_respond[7]);
   slv->data->md80_Respond.dataRet4 = read_sdo (slv->sdos_respond[8]);
   slv->data->md80_Last_Command_Received.command = read_sdo (slv->sdos_lastCommand[0]);
   slv->data->md80_Last_Command_Received.counter = read_sdo (slv->sdos_lastCommand[1]);
   slv->data->md80_Last_Command_Received.timestamp = read_sdo (slv->sdos_lastCommand[2]);
}

/*****************************************************************************/


bool isAllowReq = false;
uint32_t counterCmd = 0;

void cyclic_task()
{
   uint8_t iter = 0u;

   // receive process data
   ecrt_master_receive(master);

   for (iter = 0u; iter < ETHCAT_NODE_NUM; iter++)
   {
      ecrt_domain_process(ethNode[iter].domain);
   }

   // check process data state
   for (iter = 0u; iter < ETHCAT_NODE_NUM; iter++)
   {
      check_domain_state(&ethNode[iter]);
   }

   if (counter)
   {
      counter--;
   }
   else
   { // do this at 1 Hz
      counter = FREQUENCY;

      // calculate new process data
      countEachTimeSendData += 1U;

      // check for master state (optional)
      check_master_state();

      // check for slave configuration state(s) (optional)
      for (iter = 0u; iter < ETHCAT_NODE_NUM; iter++)
      {
         tEthNodeStates nextState = ethNode[iter].state;
         check_slave_config_states(&ethNode[iter]);

         /* Update the response. */
         reflexor_sdos_read (&ethNode[iter]);

         printf("EthNode slave %d: MD80 0 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.counter));
         printf("EthNode slave %d: MD80 0 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.timestamp));

#if 1
         switch (ethNode[iter].state)
         {
         case ETHCATNODE_IDLE:
            nextState = ETHCATNODE_DEV_RST;
            break;

         case ETHCATNODE_DEV_RST:
            printf("EthNode: %d StepInit 0: Reset dev.\n", iter);
            /* Request scan motor on can bus. */
            ethNode[iter].data->md80_Command.md80_dev_no = 0;
            ethNode[iter].data->md80_Command.counter = ++counterCmd;
            ethNode[iter].data->md80_Command.size = 0;
            ethNode[iter].data->md80_Command.command = BUS_FRAME_RESET;
            reflexor_sdos_write(&ethNode[iter]);
            nextState = ETHCATNODE_DEV_SCAN;
            break;

         case ETHCATNODE_DEV_SCAN:
            printf("EthNode: %d StepInit 1: Scan Motor on bus.\n", iter);
            /* Request scan motor on can bus. */
            ethNode[iter].data->md80_Command.md80_dev_no = 0;
            ethNode[iter].data->md80_Command.counter = ++counterCmd;
            ethNode[iter].data->md80_Command.size = 0;
            ethNode[iter].data->md80_Command.command = BUS_FRAME_PING_START;
            reflexor_sdos_write(&ethNode[iter]);

            nextState = ETHCATNODE_DEV_WAITING_SCAN;
            break;

         case ETHCATNODE_DEV_WAITING_SCAN:
            if (ethNode[iter].data->md80_Respond.command == BUS_FRAME_PING_START)
            {
               printf("EthNode: %d StepInit 2: Check motor on can bus.\n", iter);
               /* Waiting response scan. */
               for (uint8_t i = 0u; i < 8U; i++)
               {
                  ethNode[iter].md80Addr[i] = EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_addrs[i]);

                  if (ethNode[iter].md80Addr[i] != 0)
                  {
                     ethNode[iter].numMd80++;
                     printf("EthNode: %d Motor %d with address 0x%d\n", iter, ethNode[iter].numMd80, ethNode[iter].md80Addr[i]);
                  }
               }
            }

            if (ethNode[iter].numMd80 > 0)
            {
               nextState = ETHCATNODE_DEV_ADD;

               ethNode[iter].md80Id = 0u;
            }

            break;

         case ETHCATNODE_DEV_ADD:
            isAllowReq = false;
            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_ADD) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_DEV_SET_IMP_MODE;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d Add motor md80 id: %d and addres 0x%d.\n", iter, ethNode[iter].md80Id, ethNode[iter].md80Addr[ethNode[iter].md80Id]);

               /* Add motor command. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_ADD;
               ethNode[iter].data->md80_Command.size = 4;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = (uint32_t)(ethNode[iter].md80Addr[ethNode[iter].md80Id] & 0xFFFF);
               reflexor_sdos_write(&ethNode[iter]);
               
               isAllowReq = false;
            }

            break;

         case ETHCATNODE_DEV_CONFIG1M_DIS_WATCHDOG:
            isAllowReq = false;
            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
#ifdef MD80_RESET
                     nextState = ETHCATNODE_DEV_MD80_RST;
#else 
                     nextState = ETHCATNODE_DEV_SET_IMP_MODE;
#endif
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d Set mdId: %d with baudrate 1M and disable timeout.\n", iter, ethNode[iter].md80Id);
               uint32_t baudrate = 1U;
               uint16_t newTimeout = 0u;
               uint8_t canTermination = 0u;

               ethNode[iter].cmd[0] = MD80_FRAME_CAN_CONFIG;
               ethNode[iter].cmd[1] = 0x00;
               memcpy((void *)&ethNode[iter].cmd[4], (void *)&baudrate, sizeof(uint32_t));
               memcpy((void *)&ethNode[iter].cmd[8], (void *)&newTimeout, sizeof(uint16_t));
               memcpy((void *)&ethNode[iter].cmd[10], (void *)&canTermination, sizeof(uint16_t));

               /* Add motor command. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 12;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = *(uint32_t *)&ethNode[iter].cmd[0];
               ethNode[iter].data->md80_Command.dataSet1 = *(uint32_t *)&ethNode[iter].cmd[4];
               ethNode[iter].data->md80_Command.dataSet1 = *(uint8_t *)&ethNode[iter].cmd[8];
               reflexor_sdos_write(&ethNode[iter]);
               
               isAllowReq = false;
            }

            break;

#ifdef MD80_RESET
         case ETHCATNODE_DEV_MD80_RST:
            isAllowReq = false;
            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_IDLE;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d Reset mdId: %d\n", iter);

               ethNode[iter].cmd[0] = MD80_FRAME_RESTART;
               ethNode[iter].cmd[1] = 0x00;
               memcpy((void *)&ethNode[iter].cmd[2], (void *)&baudrate, sizeof(uint32_t));
               memcpy((void *)&ethNode[iter].cmd[6], (void *)&newTimeout, sizeof(uint16_t));
               memcpy((void *)&ethNode[iter].cmd[8], (void *)&canTermination, sizeof(uint8_t));

               /* Add motor command. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 2;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = *(uint32_t *)&ethNode[iter].cmd[0];
               reflexor_sdos_write(&ethNode[iter]);
               
               isAllowReq = false;
            }

            break;
#endif
         case ETHCATNODE_DEV_SET_ZERO:
            isAllowReq = false;
            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_DEV_SET_MAX_CURR;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d: Set zero position for motor id: %d.\n", iter, ethNode[iter].md80Id);

               /* Set zero position. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 4;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = MD80_FRAME_ZERO_ENCODER;
               reflexor_sdos_write(&ethNode[iter]);

               isAllowReq = false;
            }

            break;

         case ETHCATNODE_DEV_SET_MAX_CURR:

            float currentMax = 1.0f;

            isAllowReq = false;
            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_DEV_SET_MAX_TORQUE;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d: Configure Current max for motor id: %d.\n", iter, ethNode[iter].md80Id);

               ethNode[iter].cmd[0] = MD80_FRAME_BASE_CONFIG;
               memcpy((void *)&ethNode[iter].cmd[1], (void *)&currentMax, sizeof(float));

               /* Set zero position. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 8;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = *(uint32_t *)&ethNode[iter].cmd[0];
               ethNode[iter].data->md80_Command.dataSet1 = *(uint32_t *)&ethNode[iter].cmd[4];
               reflexor_sdos_write(&ethNode[iter]);

               isAllowReq = false;
            }

            break;

         case ETHCATNODE_DEV_SET_MAX_TORQUE:
            uint16_t torque = 10;

            isAllowReq = false;
            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_DEV_BEGIN;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d: Configure Torque max for motor id: %d.\n", iter, ethNode[iter].md80Id);

               ethNode[iter].cmd[0] = MD80_FRAME_SET_BANDWIDTH;
               memcpy((void *)&ethNode[iter].cmd[1], (void *)&torque, sizeof(uint16_t));

               /* Set zero position. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 4;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = *(uint32_t *)&ethNode[iter].cmd[0];
               reflexor_sdos_write(&ethNode[iter]);

               isAllowReq = false;
            }

            break;

         case ETHCATNODE_DEV_CONFIG_IMP:

            float kp = 10.0f;
            float ki = 1.0f;
            isAllowReq = false;

            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_DEV_SET_ZERO;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d: Configure kp and ki for IMP mode for motor id: %d.\n", iter, ethNode[iter].md80Id);

               ethNode[iter].cmd[0] = MD80_FRAME_IMP_CONTROL;
               memcpy((void *)&ethNode[iter].cmd[1], (void *)&kp, sizeof(float));
               memcpy((void *)&ethNode[iter].cmd[5], (void *)&ki, sizeof(float));

               /* Set zero position. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 12;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = *(uint32_t *)&ethNode[iter].cmd[0];
               ethNode[iter].data->md80_Command.dataSet1 = *(uint32_t *)&ethNode[iter].cmd[4];
               ethNode[iter].data->md80_Command.dataSet2 = *(uint32_t *)&ethNode[iter].cmd[8];
               reflexor_sdos_write(&ethNode[iter]);

               isAllowReq = false;
            }

            break;

         case ETHCATNODE_DEV_SET_IMP_MODE:
            isAllowReq = false;

            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) &&
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))               
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_DEV_ENABLE;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d: Set IMP mode for motor id: %d.\n", iter, ethNode[iter].md80Id);

               ethNode[iter].cmd[0] = MD80_FRAME_CONTROL_SELECT;
               ethNode[iter].cmd[1] = IMPEDANCE;

               /* Set zero position. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 4;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = *(uint32_t *)&ethNode[iter].cmd[0];
               reflexor_sdos_write(&ethNode[iter]);

               isAllowReq = false;
            }

            break;

         case ETHCATNODE_DEV_ENABLE:
            isAllowReq = false;

            if (ethNode[iter].md80Id < ethNode[iter].numMd80)
            {
               if ((ethNode[iter].data->md80_Respond.command == BUS_FRAME_MD80_GENERIC_FRAME) && 
                   (ethNode[iter].data->md80_Respond.md80_dev_no == ethNode[iter].md80Id))
               {
                  ethNode[iter].md80Id++;
                  isAllowReq = true;

                  if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
                  {
                     nextState = ETHCATNODE_DEV_CONFIG_IMP;
                     ethNode[iter].md80Id = 0u;
                     isAllowReq = false;
                  }
               }
               else if (ethNode[iter].md80Id == 0) 
               {
                  isAllowReq = true;
               }
            }

            if (true == isAllowReq)
            {
               printf("EthNode: %d: Enable motor id: %d.\n", iter, ethNode[iter].md80Id);

               ethNode[iter].cmd[0] = MD80_FRAME_MOTOR_ENABLE;
               ethNode[iter].cmd[1] = 1;

               /* Set zero position. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 2;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = *(uint32_t *)&ethNode[iter].cmd[0];
               reflexor_sdos_write(&ethNode[iter]);

               isAllowReq = false;
            }

            break;

         case ETHCATNODE_DEV_BEGIN:

            printf("EthNode: %d : Start control.\n", iter);
            /* Request scan motor on can bus. */
            ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
            ethNode[iter].data->md80_Command.command = BUS_FRAME_BEGIN;
            ethNode[iter].data->md80_Command.size = 0;
            ethNode[iter].data->md80_Command.counter = ++counterCmd;
            reflexor_sdos_write(&ethNode[iter]);

            nextState = ETHCATNODE_DEV_CONTROL;

            break;

         case ETHCATNODE_DEV_CONTROL:
            counter = FREQUENCY / 1000U;

            if (ethNode[iter].numMd80 > ethNode[iter].md80Id)
            {
               printf("EthNode: %d: Flash led motor id: %d.\n", iter, ethNode[iter].md80Id);

               /* Set zero position. */
               ethNode[iter].data->md80_Command.md80_dev_no = ethNode[iter].md80Id;
               ethNode[iter].data->md80_Command.command = BUS_FRAME_MD80_GENERIC_FRAME;
               ethNode[iter].data->md80_Command.size = 1;
               ethNode[iter].data->md80_Command.counter = ++counterCmd;
               ethNode[iter].data->md80_Command.dataSet0 = MD80_FRAME_FLASH_LED;
               reflexor_sdos_write(&ethNode[iter]);

               ethNode[iter].md80Id++;
            }
            else if (ethNode[iter].numMd80 <= ethNode[iter].md80Id)
            {
               ethNode[iter].md80Id = 0u;
            }

            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 0 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 0 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 0 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 0 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 0 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.Mode));
               printf("EthNode slave %d: MD80 0 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.enabled));
               printf("EthNode slave %d: MD80 0 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.counter));
               printf("EthNode slave %d: MD80 0 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.timestamp));
            }

            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 1 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 1 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 1 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 1 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 1 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.Mode));
               printf("EthNode slave %d: MD80 1 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.enabled));
               printf("EthNode slave %d: MD80 1 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.counter));
               printf("EthNode slave %d: MD80 1 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.timestamp));
            }

            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 2 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 2 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 2 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 2 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 2 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.Mode));
               printf("EthNode slave %d: MD80 2 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.enabled));
               printf("EthNode slave %d: MD80 2 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.counter));
               printf("EthNode slave %d: MD80 2 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.timestamp));
            }

            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 3 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 3 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 3 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 3 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 3 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.Mode));
               printf("EthNode slave %d: MD80 3 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.enabled));
               printf("EthNode slave %d: MD80 3 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.counter));
               printf("EthNode slave %d: MD80 3 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.timestamp));
            }

            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 4 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 4 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 4 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 4 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 4 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.Mode));
               printf("EthNode slave %d: MD80 4 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.enabled));
               printf("EthNode slave %d: MD80 4 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.counter));
               printf("EthNode slave %d: MD80 4 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.timestamp));
            }
            
            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 5 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 5 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 5 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 5 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 5 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.Mode));
               printf("EthNode slave %d: MD80 5 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.enabled));
               printf("EthNode slave %d: MD80 5 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.counter));
               printf("EthNode slave %d: MD80 5 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.timestamp));
            }
            
            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 6 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 6 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 6 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 6 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 6 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.Mode));
               printf("EthNode slave %d: MD80 6 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.enabled));
               printf("EthNode slave %d: MD80 6 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.counter));
               printf("EthNode slave %d: MD80 6 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.timestamp));
            }

            // read process data
            if (EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.counter))
            {
               printf("EthNode slave %d: MD80 7 - Position value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.Position) / 100.0f);
               printf("EthNode slave %d: MD80 7 - Velocity value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.Velocity) / 100.0f);
               printf("EthNode slave %d: MD80 7 - Torque value %f\n", iter, (float)EC_READ_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.Torque) / 100.0f);
               printf("EthNode slave %d: MD80 7 - Temperature value %f\n", iter, (float)EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.Temperature)) / 100.0f;
               printf("EthNode slave %d: MD80 7 - Mode value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.Mode));
               printf("EthNode slave %d: MD80 7 - Enabled value %u\n", iter, EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.enabled));
               printf("EthNode slave %d: MD80 7 - counter %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.counter));
               printf("EthNode slave %d: MD80 7 - time (ms) %u\n", iter, EC_READ_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.timestamp));
            }

            if (ethNode[iter].rotationCycle == true)
            {
               ethNode[iter].angleRadSet += ethNode[iter].angleRadDt;
            }
            else
            {
               ethNode[iter].angleRadSet -= ethNode[iter].angleRadDt;
            }

            if (ethNode[iter].angleRadSet >= ethNode[iter].angleRadMax)
            {
               ethNode[iter].rotationCycle = false;
            }
            else if (ethNode[iter].angleRadSet <= 0)
            {
               ethNode[iter].rotationCycle = true;
            }

            // write process data
            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataControl.Velocity, 1000);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataControl.Torque, 1000);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_0_DataControl.Mode, 0);
            }
            // write process data
            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataControl.Velocity, 400);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataControl.Torque, 100);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_1_DataControl.Mode, 0);
            }

            // write process data
            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataControl.Velocity, 400);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataControl.Torque, 100);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_2_DataControl.Mode, 0);
            }

            // write process data
            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataControl.Velocity, 400);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataControl.Torque, 100);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_3_DataControl.Mode, 0);
            }
            break;

            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataControl.Velocity, 400);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataControl.Torque, 100);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_4_DataControl.Mode, 0);
            }
            break;

            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataControl.Velocity, 400);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataControl.Torque, 100);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_5_DataControl.Mode, 0);
            }
            break;

            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataControl.Velocity, 400);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataControl.Torque, 100);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_6_DataControl.Mode, 0);
            }
            break;

            if (0u == EC_READ_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataReturn.Error))
            {
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataControl.Mode, 4);
               EC_WRITE_U8(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataControl.enable, 1);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataControl.Position, (uint32_t)(ethNode[iter].angleRadSet * 100));
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataControl.Velocity, 400);
               EC_WRITE_U16(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataControl.Torque, 100);
            }
            else
            {
               EC_WRITE_U32(ethNode[iter].domain_pd + ethNode[iter].data->md80_7_DataControl.Mode, 0);
            }
            break;

         default:
            break;
         }
#endif
         ethNode[iter].state = nextState;

         ecrt_domain_queue(ethNode[iter].domain);
      }
   }
   ecrt_master_send(master);
}

/****************************************************************************/

void stack_prefault(void)
{
   unsigned char dummy[MAX_SAFE_STACK];

   memset(dummy, 0, MAX_SAFE_STACK);
}

/****************************************************************************/

int main(int argc, char **argv)
{
   ec_slave_config_t *sc;
   struct timespec wakeup_time;
   int ret = 0;
   uint8_t iter = 0u;

   /* Request master. */
   master = ecrt_request_master(0);
   if (!master)
   {
      return -1;
   }

   /* Create domain with master above. */
   for (iter = 0u; iter < ETHCAT_NODE_NUM; iter++)
   {
      ethNode[iter].domain = ecrt_master_create_domain(master);
      if (!ethNode[iter].domain)
      {
         return -1;
      }
   }

   /* Initialize the EthCat node. */
   for (iter = 0u; iter < ETHCAT_NODE_NUM; iter++)
   {
      /* TODO: Comment. */
      ethNode[iter].master = master;

      /* Slave configuration. */
      ethNode[iter].slave_cfg = ecrt_master_slave_config(ethNode[iter].master,
                                                         ethNode[iter].aliasId,
                                                         ethNode[iter].posId,
                                                         ethNode[iter].vectorId,
                                                         ethNode[iter].productId);
      if (!ethNode[iter].slave_cfg)
      {
         fprintf(stderr, "Failed to get slave configuration.\n");
         return -1;
      }

      printf("Configuring PDOs...\n");
      if (ecrt_slave_config_pdos(ethNode[iter].slave_cfg, EC_END, ethNode[iter].sync_info))
      {
         fprintf(stderr, "Failed to configure PDOs.\n");
         return -1;
      }

      printf("Configuring SDOs...\n");
      ethNode[iter].sdos_respond[0] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           1U,
                                                                           sizeof(mtBotDataNode1.md80_Respond.md80_dev_no));

      ethNode[iter].sdos_respond[1] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           2,
                                                                           sizeof(mtBotDataNode1.md80_Respond.command));
      ethNode[iter].sdos_respond[2] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           3,
                                                                           sizeof(mtBotDataNode1.md80_Respond.size));
      ethNode[iter].sdos_respond[3] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           4,
                                                                           sizeof(mtBotDataNode1.md80_Respond.timestamp));
      ethNode[iter].sdos_respond[4] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           5,
                                                                           sizeof(mtBotDataNode1.md80_Respond.dataRet0));
      ethNode[iter].sdos_respond[5] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           6,
                                                                           sizeof(mtBotDataNode1.md80_Respond.dataRet1));
      ethNode[iter].sdos_respond[6] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           7,
                                                                           sizeof(mtBotDataNode1.md80_Respond.dataRet2));
      ethNode[iter].sdos_respond[7] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           8,
                                                                           sizeof(mtBotDataNode1.md80_Respond.dataRet3));
      ethNode[iter].sdos_respond[8] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2000U,
                                                                           9,
                                                                           sizeof(mtBotDataNode1.md80_Respond.dataRet4));
      /* Configuration SDO for command. */
      ethNode[iter].sdos_command[0] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           1,
                                                                           sizeof(mtBotDataNode1.md80_Command.md80_dev_no));

      ethNode[iter].sdos_command[1] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           2,
                                                                           sizeof(mtBotDataNode1.md80_Command.command));

      ethNode[iter].sdos_command[2] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           3,
                                                                           sizeof(mtBotDataNode1.md80_Command.size));

      ethNode[iter].sdos_command[3] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           4,
                                                                           sizeof(mtBotDataNode1.md80_Command.counter));

      ethNode[iter].sdos_command[4] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           5,
                                                                           sizeof(mtBotDataNode1.md80_Command.dataSet0));

      ethNode[iter].sdos_command[5] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           6,
                                                                           sizeof(mtBotDataNode1.md80_Command.dataSet1));
      ethNode[iter].sdos_command[6] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           7,
                                                                           sizeof(mtBotDataNode1.md80_Command.dataSet2));

      ethNode[iter].sdos_command[7] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           8,
                                                                           sizeof(mtBotDataNode1.md80_Command.dataSet3));

      ethNode[iter].sdos_command[8] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                           0x2001U,
                                                                           9,
                                                                           sizeof(mtBotDataNode1.md80_Command.dataSet4));

      /* Last command received of Reflexor. */
      ethNode[iter].sdos_lastCommand[0] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                               0x2002U,
                                                                               1,
                                                                               sizeof(mtBotDataNode1.md80_Last_Command_Received.command));
      ethNode[iter].sdos_lastCommand[1] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                               0x2002U,
                                                                               2,
                                                                               sizeof(mtBotDataNode1.md80_Last_Command_Received.counter));
      ethNode[iter].sdos_lastCommand[2] = ecrt_slave_config_create_sdo_request(ethNode[iter].slave_cfg,
                                                                               0x2002U,
                                                                               3,
                                                                               sizeof(mtBotDataNode1.md80_Last_Command_Received.timestamp));

      if (ecrt_domain_reg_pdo_entry_list(ethNode[iter].domain, ethNode[iter].domain_regs))
      {
         fprintf(stderr, "PDO entry registration failed!\n");
         return -1;
      }                                                                         
   }
   printf("Activating master...\n");
   if (ecrt_master_activate(master))
   {
      return -1;
   }

   for (iter = 0u; iter < ETHCAT_NODE_NUM; iter++)
   {
      if (!(ethNode[iter].domain_pd = ecrt_domain_data(ethNode[iter].domain)))
      {
         return -1;
      }
   }

   /* Set priority */
   struct sched_param param = {};
   param.sched_priority = sched_get_priority_max(SCHED_FIFO);

   printf("Using priority %i.", param.sched_priority);
   if (sched_setscheduler(0, SCHED_FIFO, &param) == -1)
   {
      perror("sched_setscheduler failed");
   }

   /* Lock memory */
   if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
   {
      fprintf(stderr, "Warning: Failed to lock memory: %s\n",
              strerror(errno));
   }
   stack_prefault();

   printf("Starting RT task with dt=%u ns.\n", PERIOD_NS);

   clock_gettime(CLOCK_MONOTONIC, &wakeup_time);
   wakeup_time.tv_sec += 1; /* start in future */
   wakeup_time.tv_nsec = 0;

   while (1)
   {
      ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                            &wakeup_time, NULL);
      if (ret)
      {
         fprintf(stderr, "clock_nanosleep(): %s\n", strerror(ret));
         break;
      }

      cyclic_task();

      wakeup_time.tv_nsec += PERIOD_NS;
      while (wakeup_time.tv_nsec >= NSEC_PER_SEC)
      {
         wakeup_time.tv_nsec -= NSEC_PER_SEC;
         wakeup_time.tv_sec++;
      }
   }

   return ret;
}