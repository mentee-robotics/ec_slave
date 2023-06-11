/**
 * @file can_iso_tp.h
 *
 * @details Contains all the types data structure used with this device.
 */

#ifndef CAN_ISO_TP_H_
#define CAN_ISO_TP_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <string.h>

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define SUPPORT_CAN_FD
#define OP_OK 0
#define OP_NOK 1

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef struct can_iso_tp_link_t *can_iso_tp_link_t_p;

struct CAN_msg_id
{
   uint32_t id : 29;      /*< Frame id */
   uint32_t isExt : 1;    /*< 0: std frame, 1: ext frame */
   uint32_t isCANFD : 1;  /*< 0: classical frame, 1: CANFD frame  */
   uint32_t isRemote : 1; /*< 0: data frame, 1: remote frame */
};

struct CAN_msg
{
   struct CAN_msg_id id;
   uint8_t dlc;
#ifdef SUPPORT_CAN_FD
   uint8_t data[64];
#else
   uint8_t data[8];
#endif
};

typedef enum
{
   N_OK,
   N_TIMEOUT_A,
   N_TIMEOUT_BS,
   N_TIMEOUT_CR,
   N_WRONG_SN,
   N_INVALID_FS,
   N_UNEXP_PDU,
   N_WFT_OVRN,
   N_BUFFER_OVFLW,
   N_ERROR
} CAN_ISO_TP_RESAULT;

struct can_iso_tp_init_t
{
   void *usr_arg;                                                                                                           // used by user
   struct CAN_msg_id tx_id;                                                                                                 // tx id when tx tp frame
   struct CAN_msg_id rx_id;                                                                                                 // rx id when rx tp frame
   struct CAN_msg_id funtion_id;                                                                                            // tx id when tx tp function frame
   int (*L_Data_request)(can_iso_tp_link_t_p link, const struct CAN_msg *msg);                                              // Used to call the low level driver to send CAN messages
   void (*N_USData_indication)(can_iso_tp_link_t_p link, const uint8_t payload[], uint32_t size, CAN_ISO_TP_RESAULT error); // Reporting Received TP Data to Upper layer
   void (*N_USData_confirm)(can_iso_tp_link_t_p link, const uint8_t payload[], uint32_t size, CAN_ISO_TP_RESAULT error);    // Used to report TP data completion results to the upper layer
   uint8_t *rx_buff;                                                                                                        // For multi-frame reception, point to rx buff
   uint32_t rx_buff_len;                                                                                                    // For multi-frame reception, rx buff length
   // Timeout parameter
   uint16_t N_As; // Maximum time for the sender to transmit data to the receiver, default 1000
   uint16_t N_Ar; // Maximum time for the receiver to transmit flow control to the sender, default 1000
   uint16_t N_Bs; // The maximum time that the sender receives a flow control frame after successfully sending the first frame, 1000 by default.
   // uint16_t N_Br;//Maximum time between receiving end and sending flow control after receiving the first frame
   // uint16_t N_Cs;//Maximum time that the receiving end controls the sending flow to the receiving end
   uint16_t N_Cr;    // The maximum time from sending successful flow control to receiving continuous frames, 1000 by default.
   uint8_t N_WFTmax; // Maximum number of waiting times during sending
   uint8_t FC_BS;    // BS parameters of flow control in receiving process
   uint8_t STmin;    // STmin parameters of flow control in receiving process
   uint8_t TX_DLC;   // DLC value of sending message, only used for CAN-FD
   uint8_t frame_pad;
   // Optional: Print run information
   void (*print_debug)(const char *str);
};

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

uint8_t dlc2len(uint8_t dlc);
int lenToMinDlc(uint16_t len);
int iso_can_tp_create(can_iso_tp_link_t_p link, struct can_iso_tp_init_t* init);
void iso_can_tp_poll(can_iso_tp_link_t_p link, unsigned int user_ms);
int iso_can_tp_L_Data_indication(can_iso_tp_link_t_p link, const struct CAN_msg* msg);
int iso_can_tp_L_Data_confirm(can_iso_tp_link_t_p link, const struct CAN_msg* msg, int8_t error);
int iso_can_tp_N_USData_request(can_iso_tp_link_t_p link, uint8_t isFunction, const uint8_t payload[], uint32_t size);

#endif /* CAN_ISO_TP_H_ */
