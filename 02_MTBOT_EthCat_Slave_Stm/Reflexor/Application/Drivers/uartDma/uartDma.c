/**
 * @file uartDma.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "dma.h"
#include "uartDma.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/* The size of Uart tx buffer. */
#define UARTDMA_TX_SIZE  (1024U)

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/** Initialize the data structure array for TX and RX. */
static uint8_t UartDma_Tx_Buffer[UARTDMA_TX_SIZE] __attribute__ ((section(".uartDma_module_space")));

/** Contains the configuration for this module. */
tUartDmaModule uartDmaModule = 
{
   .huart = &huart1,
   .p_tx_buff_data = &UartDma_Tx_Buffer[0],
   .tx_dma_current_len = 0,
};

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void  uartDma_init (void)
{
   /* Initialize ringbuffer. */
   lwrb_init((volatile lwrb_t *)&uartDmaModule.tx_buff, uartDmaModule.p_tx_buff_data, UARTDMA_TX_SIZE);
}

void  uartDma_rx_check (void)
{

}

void  uartDma_process_data (const void* data, size_t len)
{

}

void  uartDma_send_string (const char* str, size_t len)
{

#if UARTDMA_USE_TX_DMA == 1
   /* Enter Critical section. */
   taskENTER_CRITICAL();

   if (lwrb_get_free(&uartDmaModule.tx_buff) >= len)
   {
      lwrb_write (&uartDmaModule.tx_buff, str, len);
      uartDma_start_tx_dma_transfer ();
   }

   /* Exit Critical section. */
   taskEXIT_CRITICAL();
#else 

#endif 
}

uint8_t  uartDma_start_tx_dma_transfer(void)
{
   uint8_t started = 0;
   if (uartDmaModule.tx_dma_current_len == 0)
   {
      uartDmaModule.tx_dma_current_len = lwrb_get_linear_block_read_length(&uartDmaModule.tx_buff);

      if (uartDmaModule.tx_dma_current_len > 0)
      {
         /* Limit the maximal size to transmit at a time. */
         uartDmaModule.tx_dma_current_len = 
            (uartDmaModule.tx_dma_current_len > 32) ? 32 : uartDmaModule.tx_dma_current_len;


         /* Configure DMA. */
         started = HAL_UART_Transmit_DMA((UART_HandleTypeDef *)uartDmaModule.huart,
                               (uint8_t *) lwrb_get_linear_block_read_address(&uartDmaModule.tx_buff),
                               uartDmaModule.tx_dma_current_len);
      }
   }

   return started;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   lwrb_skip(&uartDmaModule.tx_buff, uartDmaModule.tx_dma_current_len);
   uartDmaModule.tx_dma_current_len = 0;

   /* Send next data remaining in tx buffer. */
   uartDma_start_tx_dma_transfer();
}

void  uartDma_main (void)
{
   
}
