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
#include "TMC8462.h"
#include "TMC8462_Register.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

static ConfigurationTypeDef tmc8462_config_esc;
static ConfigurationTypeDef tmc8462_config_mfc;
static TMC8462TypeDef tmc8462;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

/* ESC read CSR function. */
static void ESC_read_hw (uint16_t address, void *buf, uint16_t len)
{
   /* Read data from address on tmc slave. */
   tmc8462_esc_read_data (&tmc8462, buf, address, len);
}

/* ESC read CSR function. */
static void ESC_write_hw (uint16_t address, void *buf, uint16_t len)
{
   /* Write data from address on tmc slave. */
   tmc8462_esc_write_data (&tmc8462, buf, address, len);
}

/* ESC read process data ram. */
static void ESC_read_param (uint16_t address, void *buf, uint16_t len)
{
   /* FIXME: Update the contains of this function. */
}

/* ESC write process data ram. */
static void ESC_write_param (uint16_t address, void *buf, uint16_t len)
{
   /* FIXME: Update the contains of this function. */
}

/** ESC read function used by the Slave stack.
 *
 * @param[in]   address     - address of ESC register to read
 * @param[out]  buf         - pointer to buffer to read in
 * @param[in]   len         - number of bytes to read
 */
void ESC_read (uint16_t address, void *buf, uint16_t len)
{
   uint16_t size = 0u;
   uint8_t *temp_buf = (uint8_t *)buf;

   /* Select Read function depending on address, process data ram or not. */
   if (address >= (uint16_t)TMC8462_ESC_PROCESS_DATA_RAM)
   {
      /* Process read pram. */
      ESC_read_hw (address, temp_buf, len);
   }
   else
   {
      while (0u < len)
      {
         /* We write maximum 4 bytes at the time. */
         size = (len > 4U) ? 4U : len;

         /* If we got an odd address size is 1, 01b 11b is captured. */
         if (BIT0 == (address & BIT0))
         {
            size = 1u;
         }
         /* If add 1xb and size != 1 and 3, allow size 2. */
         else if (BIT1 == (address & BIT1))
         {
            size = (BIT0 == (size & BIT0)) ? 1u : 2u;
         }
         /* size 3 not valid. */
         else if (3u == size)
         {
            size = 1;
         }

         /* Read data from slave. */
         ESC_read_hw (address, temp_buf, size);

         /* Next address. */
         len -= size;
         temp_buf += size;
         address += size;
      }
   }

   /* Always providing AlEvent on every read or write. */
   ESC_read_hw (ESCREG_ALEVENT, (void* )&ESCvar.ALevent, sizeof(ESCvar.ALevent));
   ESCvar.ALevent = etohs (ESCvar.ALevent);
}

/** ESC write function used by the Slave stack.
 *
 * @param[in]   address     - address of ESC register to write
 * @param[out]  buf         - pointer to buffer to read in
 * @param[in]   len         - number of bytes to read
 */
void ESC_write (uint16_t address, void *buf, uint16_t len)
{
   uint16_t size = 0u;
   uint8_t *temp_buf = (uint8_t *)buf;

   /* Select Write function depending on address, process data ram or not */
   if (address >= (uint16_t)TMC8462_ESC_PROCESS_DATA_RAM)
   {
      /* Process read pram. */
      ESC_write_hw (address, temp_buf, len);
   }
   else 
   {
      while (0u < len)
      {
         /* We write maximum 4 bytes at the time */
         size = (len > 4u) ? 4u : len;

         /* If we got an odd address size is 1 , 01b 11b is captured */
         if(address & BIT0)
         {
            size = 1;
         }
         /* If address 1xb and size != 1 and 3 , allow size 2 else size 1 */
         else if (address & BIT1)
         {
            size = (size & BIT0) ? 1u : 2u;
         }
         /* size 3 not valid */
         else if (3u == size)
         {
            size = 1u;
         }

         /* Write data to slave. */
         ESC_write_hw (address, temp_buf, size);

         /* next address */
         len -= size;
         temp_buf += size;
         address += size;
      }
   }

   /* Always providing AlEvent on every read or write. */
   ESC_write_hw (ESCREG_ALEVENT, (void* )&ESCvar.ALevent, sizeof(ESCvar.ALevent));
   ESCvar.ALevent = etohs (ESCvar.ALevent);
}

void ESC_reset (void)
{

}

void ESC_init (const esc_cfg_t * config)
{
   /* Initialize the hw driver corresponding ic etherCat used in this project. */
   tmc8462_initConfig (&tmc8462, &tmc8462_config_esc, &tmc8462_config_mfc);

   /* TODO: Add function to reset data link layer of slave. */
}

void ESC_interrupt_enabled (uint32_t mask)
{

}

void ESC_interrupt_disable (uint32_t mask)
{

}