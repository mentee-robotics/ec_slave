/**
 * @file spiDma.h
 *
 * TODO: Comment.
 */
#ifndef SPIDMA_H_
#define SPIDMA_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdbool.h>
#include "FreeRTOS.h"
#include "gpio.h"
#include "spi.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief The states machine of this module.
 */
typedef enum 
{
   SPIDMA_ERROR = - 1,
   SPIDMA_IDLE = 0,
   SPIDMA_TX,
   SPIDMA_RX,
} tSpiDmaStates;

/** 
 * The configuration structure of this module.
 */
typedef struct 
{
   GPIO_PinState csActiveLevel;
   GPIO_TypeDef * csPort;
   uint16_t csPin;
   uint32_t csActiveDelayNs; 
} tSpiDmaConfig;

/** 
 * @brief The data structure of this module.
 */
typedef struct 
{
   const tSpiDmaConfig config;            /**< The configuration structures of this module. */ 
   tSpiDmaStates states;                  /**< The state of this module in runtime working. */
   SPI_HandleTypeDef * const spiModule;   /**< Pointer of spi module. */
   uint8_t * ptrTxArr;              	  /**< Pointer to the data array contains the data TX. */
   uint8_t * ptrRxArr;                    /**< Pointer to the data array contains the data RX. */
   uint16_t timeOut;					  /**< The timeout for communication. 1ms/bit */
} tSpiDmaModule;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

extern tSpiDmaModule spiDmaModule;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/**
 * Function used to initialize this module.
 *
 * @param me      - TODO: Comment.   
 *
 * @return The status of of module.
 */
extern bool SpiDma_Init (tSpiDmaModule * const me);

/**
 * @brief Function used to check if the spi module is not busy.
 *
 * @param me      - TODO: Comment.   
 *
 * @return The status of of module.
 *         true   - Module is busy.
 *         false  - Module is ready.
 */
extern void SpiDma_Deinit (tSpiDmaModule * const me);

/**
 * @brief Function used to check if the spi module is not busy.
 *
 * @param me      - TODO: Comment.   
 *
 * @return The status of of module.
 *         true   - Module is busy.
 *         false  - Module is ready.
 */
extern bool SpiDma_isBusy (tSpiDmaModule * const me);

/**
 * @brief Function used to control the CS pin.
 *
 * @param   me    - TODO: Comment.   
 * @param   state The state of output pin.
 *
 * @return The status of of module.
 *         true   - Module is busy.
 *         false  - Module is ready.
 */
extern void SpiDma_CsPinChangeState (tSpiDmaModule * const me, bool state);

/**
 * @brief Function used to write new data to address.
 *
 * @param me        - TODO: Comment.   
 * @param data_ptr  - TODO: Comment.  
 * @param len       - TODO: Comment.  
 *
 * @return The status of update new request.
 *         true - success
 *         false - failure
 */
extern bool SpiDma_write (tSpiDmaModule * const me, uint8_t * data_ptr, uint16_t len);

/**
 * @brief Function used to read new data.
 *
 * @param me        - TODO: Comment.   
 * @param data_ptr  - TODO: Comment.  
 * @param len       - TODO: Comment.  
 *
 * @return The status of update new request.
 *         true - success
 *         false - failure
 */
extern bool SpiDma_read (tSpiDmaModule * const me, uint8_t * data_rx_ptr, uint8_t * data_tx_dummy, uint16_t len);

/**
 * Function called when the translate data complete.
 *
 * @param         void
 * @return        None
 */
extern void SpiDma_WriteCompleteIrq (tSpiDmaModule * const me);

/**
 * Function called when the read data complete.
 *
 * @param         void
 * @return        None
 */
extern void SpiDma_ReadCompleteIrq (tSpiDmaModule * const me);

#endif /* SPIDMA_H_ */
