/**
 * @file lan9252.h
 *
 * TODO: Comment.
 */
#ifndef LAN9252_H_
#define LAN9252_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define LAN9252_ESC_CMD_SERIAL_WRITE     (0x02u)
#define LAN9252_ESC_CMD_SERIAL_READ      (0x03u)
#define LAN9252_ESC_CMD_SERIAL_FAST_READ (0x0Bu)
#define LAN9252_ESC_CMD_RESET_CTL        (0x01F8u)    /**< Reset register.                                           */
#define LAN9252_ESC_CMD_HW_CFG           (0x0074u)    /**< Hardware configuration register.                          */
#define LAN9252_ESC_CMD_BYTE_TEST        (0x0064u)    /**< Byte order test register.                                 */
#define LAN9252_ESC_CMD_ID_REV           (0x0050u)    /**< Chip ID and revision.                                     */
#define LAN9252_ESC_CMD_IRQ_CFG          (0x0054u)    /**< Interrupt configuration.                                  */
#define LAN9252_ESC_CMD_INT_EN           (0x005Cu)    /**< Interrupt enable.                                         */

#define LAN9252_ESC_RESET_DIGITAL        (0x00000001u)
#define LAN9252_ESC_RESET_ETHERCAT       (0x00000040u)
#define LAN9252_ESC_RESET_CTRL_RST       (LAN9252_ESC_RESET_ETHERCAT | LAN9252_ESC_RESET_DIGITAL)

#define LAN9252_ESC_HW_CFG_READY         (0x08000000u)
#define LAN9252_ESC_BYTE_TEST_OK         (0x87654321u)

#define LAN9252_ESC_PRAM_RD_FIFO_REG     (0x0000u)
#define LAN9252_ESC_PRAM_WR_FIFO_REG     (0x0020u)
#define LAN9252_ESC_PRAM_RD_ADDR_LEN_REG (0x0308u)
#define LAN9252_ESC_PRAM_RD_CMD_REG      (0x030Cu)
#define LAN9252_ESC_PRAM_WR_ADDR_LEN_REG (0x0310u)
#define LAN9252_ESC_PRAM_WR_CMD_REG      (0x0314u)
#define LAN9252_ESC_PRAM_CMD_BUSY        (0x80000000u)
#define LAN9252_ESC_PRAM_CMD_ABORT       (0x40000000u)
#define LAN9252_ESC_PRAM_CMD_AVAIL       (0x00000001u)
#define LAN9252_ESC_PRAM_CMD_CNT(x)      (((x) >> 8u) & 0x1Fu)
#define LAN9252_ESC_PRAM_SIZE(x)         ((x) << 16u)
#define LAN9252_ESC_PRAM_ADDR(x)         ((x) << 0)

#define LAN9252_ESC_CSR_DATA_REG         (0x0300)
#define LAN9252_ESC_CSR_CMD_REG          (0x0304)
#define LAN9252_ESC_CSR_CMD_BUSY         (0x80000000)
#define LAN9252_ESC_CSR_CMD_READ         (0x80000000 | 0x40000000)
#define LAN9252_ESC_CSR_CMD_WRITE        (0x80000000)
#define LAN9252_ESC_CSR_CMD_SIZE(x)      ((x) << 16)

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

void lan9252_Init (void);
void lan9252_Deinit (void);
void lan9252_read_data (uint8_t *data_ptr, uint16_t address, uint16_t len);
uint8_t lan9252_read_8 (uint16_t address);
uint16_t lan9252_read_16 (uint16_t address);
uint32_t lan9252_read_32 (uint16_t address);
void lan9252_write_data (uint8_t *data_ptr, uint16_t address, uint16_t len);
void lan9252_write_8 (uint16_t address, uint8_t value);
void lan9252_write_16 (uint16_t address, uint16_t value);
void lan9252_write_32 (uint16_t address, uint32_t value);

#endif /* LAN9252_H_ */
