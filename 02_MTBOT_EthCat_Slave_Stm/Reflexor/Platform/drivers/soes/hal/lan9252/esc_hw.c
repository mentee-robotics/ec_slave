/**
 * @file esc_hw.c
 *
 * @brief ESC hardware layer functions for TMC8462 through STM32F411.
 * 
 * @details Contains all function to read and write commands to the ESC. Used to read/write ESC registers and memory.
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "esc.h"
#include "esc_hw.h"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "lan9252.h"
#include "cmsis_os.h"

#define BIT(x)	(1U << (x))

/* ESC read CSR function */
static void ESC_read_csr (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;

   value = (LAN9252_ESC_CSR_CMD_READ | LAN9252_ESC_CSR_CMD_SIZE(len) | address);
   lan9252_write_32 (LAN9252_ESC_CSR_CMD_REG, value);

   do
   {
      value = lan9252_read_32 (LAN9252_ESC_CSR_CMD_REG);
   } while (value & LAN9252_ESC_CSR_CMD_BUSY);

   value = lan9252_read_32 (LAN9252_ESC_CSR_DATA_REG);
   memcpy(buf, (uint8_t *)&value, len);
}

/* ESC write CSR function */
static void ESC_write_csr (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;

   memcpy((uint8_t*)&value, buf,len);
   lan9252_write_32 (LAN9252_ESC_CSR_DATA_REG, value);
   value = (LAN9252_ESC_CSR_CMD_WRITE | LAN9252_ESC_CSR_CMD_SIZE(len) | address);
   lan9252_write_32 (LAN9252_ESC_CSR_CMD_REG, value);

   do
   {
      value = lan9252_read_32 (LAN9252_ESC_CSR_CMD_REG);
   } while(value & LAN9252_ESC_CSR_CMD_BUSY);
}

/* ESC read process data ram function */
static void ESC_read_pram (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;
   uint8_t * temp_buf = buf;
   uint16_t quotient, remainder, byte_offset = 0;
   uint8_t fifo_cnt, fifo_size, fifo_range, first_byte_position, temp_len;
   uint8_t buffer[256] = {0};
   int i, size;

   lan9252_write_32 (LAN9252_ESC_PRAM_RD_CMD_REG, LAN9252_ESC_PRAM_CMD_ABORT);

   do
   {
      value = lan9252_read_32 (LAN9252_ESC_PRAM_RD_CMD_REG);
   } while(value & LAN9252_ESC_PRAM_CMD_BUSY);

   lan9252_write_32 (LAN9252_ESC_PRAM_RD_ADDR_LEN_REG, (LAN9252_ESC_PRAM_SIZE(len) | LAN9252_ESC_PRAM_ADDR(address)));
   lan9252_write_32 (LAN9252_ESC_PRAM_RD_CMD_REG, LAN9252_ESC_PRAM_CMD_BUSY);

   /* Find out first byte position and adjust the copy from that
    * according to LAN9252 datasheet and MicroChip SDK code
    */
   first_byte_position = (address & 0x03);

   /* Transfer data */
   while (len > 0)
   {
      /* Wait for read availability */
      if (byte_offset > 0)
      {
         quotient = len/4;
         remainder = len - quotient*4;
      }
      else
      {
         quotient = (len + first_byte_position)/4;
         remainder = (len + first_byte_position) - quotient*4;
      }

      if (remainder != 0)
      {
         quotient++;
      }

      fifo_range = MIN(quotient,16);

      do
      {
         value = lan9252_read_32 (LAN9252_ESC_PRAM_RD_CMD_REG);
      }while(!(value & LAN9252_ESC_PRAM_CMD_AVAIL) || (LAN9252_ESC_PRAM_CMD_CNT(value) < fifo_range));

      /* Fifo size */
      fifo_size = LAN9252_ESC_PRAM_CMD_CNT(value);

      /* Transfer data size */
      size = 4*fifo_size;

      /* Allocate buffer */
      // buffer = (uint8_t *)pvPortMalloc(size);

      /* Reset fifo count */
      fifo_cnt = fifo_size;

      /* Reset buffer */
      memset(buffer,0,size);

      /* Transfer batch of data */
      lan9252_read_data ((uint8_t *)buffer, LAN9252_ESC_PRAM_RD_FIFO_REG, size);

      i = 0;
      while (fifo_cnt > 0 && len > 0)
      {
         value = buffer[i] | (buffer[i+1] << 8) | (buffer[i+2] << 16) | (buffer[i+3] << 24);

         if (byte_offset > 0)
         {
            temp_len = (len > 4) ? 4: len;
            memcpy(temp_buf + byte_offset ,&value, temp_len);
         }
         else
         {
            temp_len = (len > (4 - first_byte_position)) ? (4 - first_byte_position) : len;
            memcpy(temp_buf ,((uint8_t *)&value + first_byte_position), temp_len);
         }

         i += 4;
         fifo_cnt--;
         len -= temp_len;
         byte_offset += temp_len;
      }

      // vPortFree(buffer);
   }
}

/* ESC write process data ram function */
static void ESC_write_pram (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;
   uint8_t * temp_buf = buf;
   uint16_t quotient, remainder, byte_offset = 0;
   uint8_t fifo_cnt, fifo_size, fifo_range, first_byte_position, temp_len;
   uint8_t buffer[256] = {0};
   int i, size;

   lan9252_write_32 (LAN9252_ESC_PRAM_WR_CMD_REG, LAN9252_ESC_PRAM_CMD_ABORT);

   do
   {
      value = lan9252_read_32 (LAN9252_ESC_PRAM_WR_CMD_REG);
   } while(value & LAN9252_ESC_PRAM_CMD_BUSY);

   lan9252_write_32 (LAN9252_ESC_PRAM_WR_ADDR_LEN_REG, (LAN9252_ESC_PRAM_SIZE(len) | LAN9252_ESC_PRAM_ADDR(address)));
   lan9252_write_32 (LAN9252_ESC_PRAM_WR_CMD_REG, LAN9252_ESC_PRAM_CMD_BUSY);

   /* Find out first byte position and adjust the copy from that
    * according to LAN9252 datasheet and MicroChip SDK code
    */
   first_byte_position = (address & 0x03);

   /* Transfer data */
   while (len > 0)
   {
      /* Wait for write availabiliy */
      if (byte_offset > 0)
      {
         quotient = len/4;
         remainder = len - quotient*4;
      }
      else
      {
         quotient = (len + first_byte_position)/4;
         remainder = (len + first_byte_position) - quotient*4;
      }
      if (remainder != 0)
      {
         quotient++;
      }
      fifo_range = MIN(quotient,16);
      do
      {
         value = lan9252_read_32 (LAN9252_ESC_PRAM_WR_CMD_REG);
      }while(!(value & LAN9252_ESC_PRAM_CMD_AVAIL) || (LAN9252_ESC_PRAM_CMD_CNT(value) < fifo_range));

      /* Fifo size */
      fifo_size = LAN9252_ESC_PRAM_CMD_CNT(value);

      /* Transfer data size */
      size = 4*fifo_size;

      /* Allocate buffer */
      // buffer = (uint8_t *)pvPortMalloc(size);

      /* Reset fifo count */
      fifo_cnt = fifo_size;

      /* Reset buffer */
      memset(buffer,0,size);

      i = 0;
      while (fifo_cnt > 0 && len > 0)
      {
         value = 0;
         if (byte_offset > 0)
         {
            temp_len = (len > 4) ? 4: len;
            memcpy(&value, (temp_buf + byte_offset), temp_len);
         }
         else
         {
            temp_len = (len > (4 - first_byte_position)) ? (4 - first_byte_position) : len;
            memcpy(((uint8_t *)&value + first_byte_position), temp_buf, temp_len);
         }

         buffer[i] = (value & 0xFF);
         buffer[i+1] = ((value >> 8) & 0xFF);
         buffer[i+2] = ((value >> 16) & 0xFF);
         buffer[i+3] = ((value >> 24) & 0xFF);

         i += 4;
         fifo_cnt--;
         len -= temp_len;
         byte_offset += temp_len;
      }

      /* Transfer batch of data */
      lan9252_write_data ((uint8_t *)buffer, LAN9252_ESC_PRAM_WR_FIFO_REG, size);
      // vPortFree(buffer);
   }
}

/** ESC read function used by the Slave stack.
 *
 * @param[in]   address     = address of ESC register to read
 * @param[out]  buf         = pointer to buffer to read in
 * @param[in]   len         = number of bytes to read
 */
void ESC_read (uint16_t address, void *buf, uint16_t len)
{
   /* Select Read function depending on address, process data ram or not */
   if (address >= 0x1000)
   {
      ESC_read_pram (address, buf, len);
   }
   else
   {
      uint16_t size;
      uint8_t *temp_buf = (uint8_t *)buf;

      while(len > 0)
      {
         /* We write maximum 4 bytes at the time */
         size = (len > 4) ? 4 : len;
         /* Make size aligned to address according to LAN9252 datasheet
          * Table 12-14 EtherCAT CSR Address VS size and MicroChip SDK code
          */
         /* If we got an odd address size is 1 , 01b 11b is captured */
         if(address & BIT(0))
         {
            size = 1;
         }
         /* If address 1xb and size != 1 and 3 , allow size 2 else size 1 */
         else if (address & BIT(1))
         {
            size = (size & BIT(0)) ? 1 : 2;
         }
         /* size 3 not valid */
         else if (size == 3)
         {
            size = 1;
         }
         /* else size is kept AS IS */
         ESC_read_csr(address, temp_buf, size);

         /* next address */
         len -= size;
         temp_buf += size;
         address += size;
      }
   }
   /* To mimic the ET1100 always providing AlEvent on every read or write */
   ESC_read_csr(ESCREG_ALEVENT,(void *)&ESCvar.ALevent,sizeof(ESCvar.ALevent));
   ESCvar.ALevent = etohs (ESCvar.ALevent);

}

/** ESC write function used by the Slave stack.
 *
 * @param[in]   address     = address of ESC register to write
 * @param[out]  buf         = pointer to buffer to write from
 * @param[in]   len         = number of bytes to write
 */
void ESC_write (uint16_t address, void *buf, uint16_t len)
{
   /* Select Write function depending on address, process data ram or not */
   if (address >= 0x1000)
   {
      ESC_write_pram(address, buf, len);
   }
   else
   {
      uint16_t size;
      uint8_t *temp_buf = (uint8_t *)buf;

      while(len > 0)
      {
         /* We write maximum 4 bytes at the time */
         size = (len > 4) ? 4 : len;
         /* Make size aligned to address according to LAN9252 datasheet
          * Table 12-14 EtherCAT CSR Address VS size  and MicroChip SDK code
          */
         /* If we got an odd address size is 1 , 01b 11b is captured */
         if(address & BIT(0))
         {
            size = 1;
         }
         /* If address 1xb and size != 1 and 3 , allow size 2 else size 1 */
         else if (address & BIT(1))
         {
            size = (size & BIT(0)) ? 1 : 2;
         }
         /* size 3 not valid */
         else if (size == 3)
         {
            size = 1;
         }
         /* else size is kept AS IS */
         ESC_write_csr(address, temp_buf, size);

         /* next address */
         len -= size;
         temp_buf += size;
         address += size;
      }
   }

   /* To mimic the ET1x00 always providing AlEvent on every read or write */
   ESC_read_csr(ESCREG_ALEVENT,(void *)&ESCvar.ALevent,sizeof(ESCvar.ALevent));
   ESCvar.ALevent = etohs (ESCvar.ALevent);
}

/* Un-used due to evb-lan9252-digio not havning any possability to
 * reset except over SPI.
 */
void ESC_reset (void)
{

}

void ESC_init (const esc_cfg_t * config)
{
   uint32_t value;
   uint32_t counter = 0;
   uint32_t timeout = 1000; // wait 100msec

   // start initialization
   // Reset the ecat core here due to evb-lan9252-digio not having any GPIO for that purpose.
   lan9252_write_32(LAN9252_ESC_CMD_RESET_CTL, LAN9252_ESC_RESET_CTRL_RST);

   // Wait until reset command has been executed
   do
   {
      osDelay(100);
      counter++;
      value = lan9252_read_32(LAN9252_ESC_CMD_RESET_CTL);
   } while ((value & LAN9252_ESC_RESET_CTRL_RST) && (counter < timeout));

   // Perform byte test
   do
   {
      osDelay(100);
      counter++;
      value = lan9252_read_32(LAN9252_ESC_CMD_BYTE_TEST);
   } while ((value != LAN9252_ESC_BYTE_TEST_OK) && (counter < timeout));

   // Check hardware is ready
   do
   {
      osDelay(100);
      counter++;
      value = lan9252_read_32(LAN9252_ESC_CMD_HW_CFG);
   } while (!(value & LAN9252_ESC_HW_CFG_READY) && (counter < timeout));

   // Check if timeout occured
   if (counter < timeout)
   {
      // Read the chip identification and revision
      value = lan9252_read_32(LAN9252_ESC_CMD_ID_REV);
      DPRINT("Detected chip %x Rev %u \n", ((value >> 16) & 0xFFFF), (value & 0xFFFF));

      // Set AL event mask
      value = (ESCREG_ALEVENT_CONTROL |
               ESCREG_ALEVENT_SMCHANGE |
               ESCREG_ALEVENT_SM0 |
               ESCREG_ALEVENT_SM1);
      ESC_ALeventmaskwrite(value);
   }
   else
   {
      DPRINT("Timeout occurred during reset \n");
   }
}

void ESC_interrupt_enable (uint32_t mask)
{
   // Enable interrupt for SYNC0 or SM2 or SM3
   uint32_t user_int_mask = ESCREG_ALEVENT_DC_SYNC0 |
                            ESCREG_ALEVENT_SM2 |
                            ESCREG_ALEVENT_SM3;
   if (mask & user_int_mask)
   {
      ESC_ALeventmaskwrite(ESC_ALeventmaskread() | (mask & user_int_mask));
   }

   // Set LAN9252 interrupt pin driver as push-pull active high
   lan9252_write_32 (LAN9252_ESC_CMD_IRQ_CFG, 0x00000111);

   // Enable LAN9252 interrupt
   lan9252_write_32 (LAN9252_ESC_CMD_INT_EN, 0x00000001);
}

void ESC_interrupt_disable (uint32_t mask)
{
   // Enable interrupt for SYNC0 or SM2 or SM3
   uint32_t user_int_mask = ESCREG_ALEVENT_DC_SYNC0 |
                            ESCREG_ALEVENT_SM2 |
                            ESCREG_ALEVENT_SM3;

   if (mask & user_int_mask)
   {
      // Disable interrupt from SYNC0
      ESC_ALeventmaskwrite (ESC_ALeventmaskread() & ~(mask & user_int_mask));
   }


   // Disable LAN9252 interrupt
   lan9252_write_32 (LAN9252_ESC_CMD_INT_EN, 0x00000000);
}
