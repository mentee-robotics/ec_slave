#include "lan9252.h"
#include "spiDma.h"

static void lan9252_cmd_read(uint16_t address)
{
   uint8_t command[4] = {0u};

   /* Prepare the data structure contains read command. */
   command[0] = (uint8_t)LAN9252_ESC_CMD_SERIAL_FAST_READ;
   command[1] = (uint8_t)((address >> 8u) & 0xFF);
   command[2] = (uint8_t)(address & 0xFF);
   command[3] = (uint8_t)(0x01);

   /* Send the command to Lan9252. */
   SpiDma_write(&spiDmaModule, (uint8_t *)&command[0], sizeof(command));
}

static void lan9252_cmd_write(uint16_t address)
{
   uint8_t command[3] = {0u};

   /* Prepare the data structure contains read command. */
   command[0] = (uint8_t)LAN9252_ESC_CMD_SERIAL_WRITE;
   command[1] = (uint8_t)((address >> 8u) & 0xFF);
   command[2] = (uint8_t)(address & 0xFF);

   /* Send the command to tmc slave. */
   SpiDma_write(&spiDmaModule, (uint8_t *)&command[0], sizeof(command));
}

void lan9252_Init(void)
{
   /* Initialize the SPI DMA module. */
   SpiDma_Init(&spiDmaModule);

   /* Verify the connection with Lan9252. */

   /* Reset all register status of lan9252. */
}

void lan9252_Deinit(void)
{
   SpiDma_Deinit(&spiDmaModule);
}

void lan9252_read_data(uint8_t *data_ptr, uint16_t address, uint16_t len)
{
   uint8_t iter = 0u;
   uint8_t tx_dummy_arr[128];

   /* Prepare the tx data dummy. */
   for (iter = 0U; iter < len; iter++)
   {
      tx_dummy_arr[iter] = 0xFFu;
   }

   /* Change the CS pin to active state. */
   SpiDma_CsPinChangeState(&spiDmaModule, true);

   /* Send Read command to address. */
   lan9252_cmd_read(address);

   /* Read data. */
   SpiDma_read(&spiDmaModule, data_ptr, tx_dummy_arr, len);

   /* Change the CS pin to inactive state. */
   SpiDma_CsPinChangeState(&spiDmaModule, false);
}

uint8_t lan9252_read_8(uint16_t address)
{
   uint8_t buffer = 0u;

   lan9252_read_data(&buffer, address, sizeof(buffer));

   return buffer;
}

uint16_t lan9252_read_16(uint16_t address)
{
   uint8_t buffer[2] = {0u};
   uint16_t ret = 0u;

   lan9252_read_data(buffer, address, sizeof(buffer));

   ret = (((uint16_t)buffer[1] << 8u) | ((uint16_t)buffer[0] << 0u));

   return ret;
}

uint32_t lan9252_read_32(uint16_t address)
{
   uint32_t ret = 0u;
   uint8_t buffer[4] = {0u};

   lan9252_read_data(buffer, address, sizeof(buffer));

   ret = (((uint32_t)buffer[3] << 24u) |
          ((uint32_t)buffer[2] << 16u) |
          ((uint32_t)buffer[1] << 8u) |
          ((uint32_t)buffer[0] << 0u));

   return ret;
}

void lan9252_write_data(uint8_t *data_ptr, uint16_t address, uint16_t len)
{
   /* Change the CS pin to active state. */
   SpiDma_CsPinChangeState(&spiDmaModule, true);

   /* Send request write. */
   lan9252_cmd_write(address);

   /* Send data to lan9252. */
   SpiDma_write (&spiDmaModule, data_ptr, len);

   /* Change the CS pin to inactive state. */
   SpiDma_CsPinChangeState(&spiDmaModule, false);
}

void lan9252_write_8(uint16_t address, uint8_t value)
{
   lan9252_write_data(&value, address, 1);
}

void lan9252_write_16(uint16_t address, uint16_t value)
{
   uint8_t data[2] = {0u};

   data[0] = (uint8_t)(value & 0xFF);
   data[1] = (uint8_t)((value >> 8u) & 0xFF);

   lan9252_write_data (data, address, 2);
}

void lan9252_write_32 (uint16_t address, uint32_t value)
{
   uint8_t data[4] = {0u};

   data[0] = (uint8_t)(value & 0xFF);
   data[1] = (uint8_t)((value >> 8u) & 0xFF);
   data[2] = (uint8_t)((value >> 16u) & 0xFF);
   data[3] = (uint8_t)((value >> 24u) & 0xFF);

   lan9252_write_data (data, address, 4u);
}
