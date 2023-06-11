/**
 * @file spiDma.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <string.h>
#include "spiDma.h"
#include "event_groups.h"
#include "Bits.h"
#include "FreeRTOS.h"
#include "timers.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/* The size of spi tx and rx buffer. */
#define SPIDMA_TX_SIZE  (5U)
#define SPIDMA_RX_SIZE  (256U)

/* Bits mask event of this module. */
#define SPIDMA_TX_DONE (BIT0)
#define SPIDMA_RX_DONE (BIT1)

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/** Initialize the data structure array for TX and RX. */
static uint8_t SPIDMA_Tx_Buffer[SPIDMA_TX_SIZE] __attribute__ ((section(".spiDma_module_space")));
static uint8_t SPIDMA_Rx_Buffer[SPIDMA_RX_SIZE] __attribute__ ((section(".spiDma_module_space")));

/** Event group bits contains all event relate to this module. */
static EventGroupHandle_t spiDma_event = NULL;

/** Contains the configuration for this module. */
tSpiDmaModule spiDmaModule = 
{
   .config.csActiveLevel = GPIO_PIN_RESET,
   .config.csPin = SPI1_CS_Pin,
   .config.csPort = SPI1_CS_GPIO_Port,
   .states = SPIDMA_IDLE,
   .spiModule = &hspi1,
   .ptrRxArr = (uint8_t *)&SPIDMA_Rx_Buffer[0],
   .ptrTxArr = (uint8_t *)&SPIDMA_Tx_Buffer[0],
   .timeOut = 1,
};

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

bool SpiDma_Init (tSpiDmaModule * const me)
{
   bool ret = true;

   /* Create the spiDma event group. */
   spiDma_event = xEventGroupCreate();

   /* Was the event group created successfully? */
   if (NULL == spiDma_event) {
      ret = false;
   }

   /* Clear the buffer. */
   bzero (SPIDMA_Rx_Buffer, SPIDMA_RX_SIZE);
   bzero (SPIDMA_Tx_Buffer, SPIDMA_TX_SIZE);

   return ret;
}

void SpiDma_Deinit (tSpiDmaModule * const me)
{

}

bool SpiDma_isBusy (tSpiDmaModule * const me)
{
   return ((me->states != SPIDMA_IDLE) ? true : false);
}

void SpiDma_CsPinChangeState (tSpiDmaModule * const me, bool state)
{
   GPIO_PinState pinSt = GPIO_PIN_RESET;

   pinSt = (true == state) ? (me->config.csActiveLevel) : 
      ((GPIO_PIN_RESET == me->config.csActiveLevel) ? GPIO_PIN_SET : GPIO_PIN_RESET);
   
   HAL_GPIO_WritePin (me->config.csPort, me->config.csPin, pinSt);
}

bool SpiDma_write (tSpiDmaModule * const me, uint8_t * data_ptr, uint16_t len)
{
   bool ret = true;
   EventBits_t bits = (EventBits_t)0U; 

   /* Verify the parameters input. */
   if (NULL == me)
   {
      /* Pointer to the module SpiDma is NULL. */
      ret = false;
   }
   else if (NULL == data_ptr)
   {
      /* Pointer to data array is NULL. */
      ret = false;
   }
   else if (0 >= len)
   {
      /* The length of data input is equal 0. */
      ret = false;
   }

   /* If the params input are valid, send data to DMA to transfer this to slave.*/
   if (ret)
   {
      /* Copy the data to TX buffer. */
      memcpy (me->ptrTxArr, data_ptr, len);

      /* Start transmit data via DMA. */
      HAL_SPI_Transmit_DMA (me->spiModule, me->ptrTxArr, len);

      /* Change state into TX when the Spi  */
      me->states = SPIDMA_TX;

      /* Blocking the currently executing task until SPI transmit data is finished. */
      bits = xEventGroupWaitBits (spiDma_event, SPIDMA_TX_DONE, pdTRUE, pdFALSE, (me->timeOut/portTICK_PERIOD_MS));

      /* Update new state. */

      if (SPIDMA_TX_DONE != (bits & SPIDMA_TX_DONE))
      {
         ret = false;
      }
   }

   /* Return state. */
   return ret;
}

bool SpiDma_read (tSpiDmaModule * const me, uint8_t * data_rx_ptr, uint8_t * data_tx_dummy, uint16_t len)
{
   bool ret = true;
   EventBits_t bits = (EventBits_t)0U; 

   /* Verify the parameters input. */
   if (NULL == me)
   {
      /* Pointer to the module SpiDma is NULL. */
      ret = false;
   }
   else if ((NULL == data_rx_ptr) || (NULL == data_tx_dummy))
   {
      /* Pointer to data array is NULL. */
      ret = false;
   }
   else if (0 >= len)
   {
      /* The length of data input is equal 0. */
      ret = false;
   }
   
   /* If the params input are valid, send data to DMA to transfer dummy to read data from slave.*/
   if (ret)
   {
      /* Copy the data to TX buffer. */
      memcpy (me->ptrTxArr, data_tx_dummy, len);

      /* Start transmit data via DMA. */
      HAL_SPI_TransmitReceive_DMA (me->spiModule, me->ptrTxArr, me->ptrRxArr, len);

      /* Change state into TX. */
      me->states = SPIDMA_RX;

      /* Blocking the currently executing task until SPI transmit data is finished. */
      bits = xEventGroupWaitBits (spiDma_event, SPIDMA_RX_DONE, pdTRUE, pdFALSE, (me->timeOut/portTICK_PERIOD_MS));

      /* Update new state. */
      if (SPIDMA_RX_DONE != (bits & SPIDMA_RX_DONE))
      {
         ret = false;
      }

      /* Copy data to data array return. */
      memcpy (data_rx_ptr, me->ptrRxArr, len);
   }

   /* Return state. */
   return ret;
}


void SpiDma_WriteCompleteIrq (tSpiDmaModule * const me)
{
   /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

   /* Write data complete, change the state of this module to TxEnd. */
   me->states = SPIDMA_IDLE;

   /* Set bit to inform the TX is finish. */
   xEventGroupSetBitsFromISR (spiDma_event, SPIDMA_TX_DONE, &xHigherPriorityTaskWoken);
}  

void SpiDma_ReadCompleteIrq (tSpiDmaModule * const me)
{
   /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;

   /* Read data complete, change the state of this module to RxEnd. */
   me->states = SPIDMA_IDLE;

   /* Set bit to inform the RX is finish. */
   xEventGroupSetBitsFromISR (spiDma_event, SPIDMA_RX_DONE, &xHigherPriorityTaskWoken);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
   SpiDma_WriteCompleteIrq (&spiDmaModule);
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
   SpiDma_ReadCompleteIrq (&spiDmaModule);
}
