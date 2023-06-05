/*
 * TMC8462.c
 *
 *  Created on: 30.05.2017
 *      Author: LK
 */

#include "TMC8462.h"
#include "spiDma.h"

void tmc8462_esc_read (TMC8462TypeDef *tmc8462, uint16_t address)
{
   uint8_t command[4] = {0u};

   /* Prepare the data structure contains read command. */
   command[0] = (uint8_t)(address >> 5U);
   command[1] = (uint8_t)((address << 3U) | TMC8462_CMD_ADDR_EXT);
   command[2] = (uint8_t)(((address >> 8U) & 0xE0U) | (TMC8462_CMD_READ_WAIT << 2U));
   command[3] = (uint8_t)(0xFFU);
   
   /* Send the command to tmc slave. */
   SpiDma_write (&spiDmaModule, (uint8_t *)&command[0], sizeof(command));
}

void tmc8462_esc_write(TMC8462TypeDef *tmc8462, uint16_t address)
{
   uint8_t command[3] = {0u};

   /* Prepare the data structure contains read command. */
   command[0] = (uint8_t)(address >> 5U);
   command[1] = (uint8_t)((address << 3U) | TMC8462_CMD_ADDR_EXT);
   command[2] = (uint8_t)(((address >> 8U) & 0xE0U) | (TMC8462_CMD_WRITE << 2U));

   /* Send the command to tmc slave. */
   SpiDma_write (&spiDmaModule, (uint8_t *)&command[0], sizeof(command));
}

void tmc8462_mfc_read(TMC8462TypeDef *tmc8462, uint16_t address)
{
   uint8_t command[4] = {0u};

   /* Prepare the data structure contains read command. */
   command[0] = (uint8_t)(address >> 5U);
   command[1] = (uint8_t)((address << 3U) | TMC8462_CMD_ADDR_EXT);
   command[2] = (uint8_t)(((address >> 8U) & 0xE0U) | (TMC8462_CMD_READ_WAIT << 2U));
   command[3] = 0xFFU;

	/* Send the command to tmc slave. */
	SpiDma_write (&spiDmaModule, (uint8_t *)&command[0], sizeof(command));
}

void tmc8462_mfc_write(TMC8462TypeDef *tmc8462, uint16_t address)
{
   uint8_t command[3] = {0u};

   /* Prepare the data structure contains read command. */
   command[0] = (uint8_t)(address >> 5U);
   command[1] = (uint8_t)((address << 3U) | TMC8462_CMD_ADDR_EXT);
   command[2] = (uint8_t)(((address >> 8U) & 0xE0U) | (TMC8462_CMD_WRITE << 2U));

	/* Send the command to tmc slave. */
	SpiDma_write(&spiDmaModule, (uint8_t *)&command[0], sizeof(command));
}

void tmc8462_esc_read_data (TMC8462TypeDef *tmc8462, uint8_t *data_ptr, uint16_t address, uint16_t len)
{
   uint16_t iter = 0U;
   uint8_t * tx_dummy_arr = pvPortMalloc(len);

   /* Prepare the tx data dummy. */
   for (iter = 0U; iter < len; iter++)
   {
      tx_dummy_arr[iter] = (iter < (len - 1)) ? 0x00 : 0xFF;
   }
   
   /* Change the CS pin to active state. */
   SpiDma_CsPinChangeState (&spiDmaModule, true);

   /* Send Read command to address. */
   tmc8462_esc_read (tmc8462, address);

   /* Read data. */
   SpiDma_read (&spiDmaModule, data_ptr, tx_dummy_arr, len);

   /* Change the CS pin to inactive state. */
   SpiDma_CsPinChangeState (&spiDmaModule, false);

   /* Free the data. */
   vPortFree (tx_dummy_arr);
}

uint8_t tmc8462_esc_read_8(TMC8462TypeDef *tmc8462, uint16_t address)
{
	uint8_t buffer = 0;

	tmc8462_esc_read_data (tmc8462, &buffer, address, 1);

	return buffer;
}

uint16_t tmc8462_esc_read_16(TMC8462TypeDef *tmc8462, uint16_t address)
{
	uint8_t data[2];

	tmc8462_esc_read_data(tmc8462, data, address, 2);

	return _8_16(data[1], data[0]);
}

uint32_t tmc8462_esc_read_32(TMC8462TypeDef *tmc8462, uint16_t address)
{
	uint8_t data[4];

	tmc8462_esc_read_data(tmc8462, data, address, 4);

	return _8_32(data[3], data[2], data[1], data[0]);
}

void tmc8462_esc_write_data (TMC8462TypeDef *tmc8462, uint8_t *data_ptr, uint16_t address, uint16_t len)
{   
   /* Change the CS pin to active state. */
   SpiDma_CsPinChangeState (&spiDmaModule, true);

	/* Send request write. */
	tmc8462_esc_write (tmc8462, address);

	/* Send data to slave. */
	SpiDma_write (&spiDmaModule, data_ptr, len);

   /* Change the CS pin to inactive state. */
   SpiDma_CsPinChangeState (&spiDmaModule, false);
}

void tmc8462_esc_write_8 (TMC8462TypeDef *tmc8462, uint16_t address, uint8_t value)
{
	tmc8462_esc_write_data (tmc8462, &value, address, 1);
}

void tmc8462_esc_write_16 (TMC8462TypeDef *tmc8462, uint16_t address, uint16_t value)
{
	uint8_t data[2];

	data[0] = BYTE(value, 0);
	data[1] = BYTE(value, 1);
	tmc8462_esc_write_data (tmc8462, data, address, 2);
}

void tmc8462_esc_write_32 (TMC8462TypeDef *tmc8462, uint16_t address, uint32_t value)
{
	uint8_t data[4];

	data[0] = BYTE(value, 0);
	data[1] = BYTE(value, 1);
	data[2] = BYTE(value, 2);
	data[3] = BYTE(value, 3);
	tmc8462_esc_write_data (tmc8462, data, address, 4);
}

void tmc8462_mfc_read_data (TMC8462TypeDef *tmc8462, uint8_t *data_ptr, uint16_t address, uint16_t len)
{
   uint16_t iter = 0U;
   uint8_t * tx_dummy_arr = pvPortMalloc(len);

   /* Prepare the tx data dummy. */
   for (iter = 0U; iter < len; iter++)
   {
      tx_dummy_arr[iter] = (iter < (len - 1)) ? 0x00 : 0xFF;
   }
   
   /* Change the CS pin to active state. */
   SpiDma_CsPinChangeState (&spiDmaModule, true);

   /* Send Read command to address. */
   tmc8462_mfc_read (tmc8462, address);

   /* Read data. */
   SpiDma_read (&spiDmaModule, data_ptr, tx_dummy_arr, len);

   /* Change the CS pin to inactive state. */
   SpiDma_CsPinChangeState (&spiDmaModule, false);

   /* Free the data. */
   vPortFree (tx_dummy_arr);
}

void tmc8462_mfc_read_32 (TMC8462TypeDef *tmc8462, uint16_t address, uint32_t *value)
{
	uint8_t data[4];

	tmc8462_mfc_read_data(tmc8462, data, address, 4);

	*value = _8_32(data[3], data[2], data[1], data[0]);
}

void tmc8462_mfc_read_64 (TMC8462TypeDef *tmc8462, uint16_t address, uint64_t *value)
{
	uint8_t data[8];

	tmc8462_mfc_read_data(tmc8462, data, address, 8);

	*value = _8_64((uint64_t)data[7], (uint64_t)data[6], (uint64_t)data[5], (uint64_t)data[4], (uint64_t)data[3], (uint64_t)data[2], (uint64_t)data[1], (uint64_t)data[0]);
}

void tmc8462_mfc_read_auto (TMC8462TypeDef *tmc8462, uint16_t address, uint8_t *value)
{
	if(address == TMC8462_MFC_SPI_RX_DATA || address == TMC8462_MFC_SPI_TX_DATA || address == TMC8462_MFC_PWM4) {
		// 64 bit
		tmc8462_mfc_read_data (tmc8462, value, address, 8);
	} else {
		// 32 bit
		tmc8462_mfc_read_data (tmc8462, value, address, 4);
	}
}

void tmc8462_mfc_write_data (TMC8462TypeDef *tmc8462, uint8_t *data_ptr, uint16_t address, uint16_t len)
{
   /* Change the CS pin to active state. */
   SpiDma_CsPinChangeState (&spiDmaModule, true);

	/* Send request write. */
	tmc8462_esc_write (tmc8462, address);

	/* Send data to slave. */
	SpiDma_write (&spiDmaModule, data_ptr, len);

   /* Change the CS pin to inactive state. */
   SpiDma_CsPinChangeState (&spiDmaModule, false);
}

void tmc8462_mfc_write_32(TMC8462TypeDef *tmc8462, uint16_t address, uint32_t value)
{
	uint8_t data[4];

	data[0] = BYTE(value, 0);
	data[1] = BYTE(value, 1);
	data[2] = BYTE(value, 2);
	data[3] = BYTE(value, 3);

	tmc8462_mfc_write_data(tmc8462, data, address, 4);
}

void tmc8462_mfc_write_64(TMC8462TypeDef *tmc8462, uint16_t address, uint64_t value)
{
	uint8_t data[8];

	data[0] = BYTE(value, 0);
	data[1] = BYTE(value, 1);
	data[2] = BYTE(value, 2);
	data[3] = BYTE(value, 3);
	data[4] = BYTE(value, 4);
	data[5] = BYTE(value, 5);
	data[6] = BYTE(value, 6);
	data[7] = BYTE(value, 7);

	tmc8462_mfc_write_data(tmc8462, data, address, 8);
}

void tmc8462_mfc_write_auto(TMC8462TypeDef *tmc8462, uint16_t address, uint8_t *value)
{
	if(address == TMC8462_MFC_SPI_RX_DATA || address == TMC8462_MFC_SPI_TX_DATA || address == TMC8462_MFC_PWM4) {
		// 64 bit
		tmc8462_mfc_write_data(tmc8462, value, address, 8);
	} else {
		// 32 bit
		tmc8462_mfc_write_data(tmc8462, value, address, 4);
	}
}

void tmc8462_initConfig(TMC8462TypeDef *tmc8462, ConfigurationTypeDef *tmc8462_config_esc, ConfigurationTypeDef *tmc8462_config_mfc)
{
	tmc8462->config_esc = tmc8462_config_esc;
	tmc8462->config_mfc = tmc8462_config_mfc;

	/* Initialize the SPI_DMA module. */
	SpiDma_Init (&spiDmaModule);

	/* Verify the connection with tmc. */
	while (TMC8462_FIELD_READ(tmc8462, tmc8462_esc_read_16, TMC8462_ESC_PDI_CTRL, TMC8462_ESC_PDI_MODE_MASK, TMC8462_ESC_PDI_MODE_SHIFT) != TMC8462_PDI_SPI_SLAVE);

	/* Reset all register status of slave. */
	tmc8462_esc_write_16(tmc8462, TMC8462_ESC_AL_STATUS, FIELD_SET(TMC8462_EC_STATE_INIT, TMC8462_ESC_AL_ERROR_MASK, TMC8462_ESC_AL_ERROR_SHIFT, true));
	tmc8462_esc_write_16(tmc8462, TMC8462_ESC_AL_CODE, 0x0000);
	tmc8462_esc_write_16(tmc8462, TMC8462_ESC_AL_EVENT_MASK_LO, 0xFF0E);
}