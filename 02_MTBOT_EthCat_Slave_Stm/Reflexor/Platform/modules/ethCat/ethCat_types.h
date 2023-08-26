/**
 * @file ethCat_types.h
 *
 * TODO: Comment.
 */
#ifndef ETHCAT_TYPES_H_
#define ETHCAT_TYPES_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "utypes.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define ETHCAT_LENGTH_DATAMAX (24U)
#define ETHCAT_MD80_RESPOND (0x2000U)
#define ETHCAT_MD80_COMMAND (0x2001U)
#define ETHCAT_MD80_LAST_COMMAND (0x2002U)

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

/* TODO: Comment */
typedef struct {
   uint16_t Position;
   uint16_t Velocity;
   uint16_t Torque;
   uint8_t Mode;
   uint8_t enable;
   uint32_t counter;
} tEthCat_md80Control;

/* TODO: Comment */
typedef struct {
   uint8_t Mode;
   uint16_t Position;
   uint16_t Velocity;
   uint16_t Torque;
   uint8_t Temperature;
   uint8_t Error;
   uint8_t enabled;
   uint32_t counter;
   uint32_t timestamp;
} tEthCat_md80Return;

/* TODO: Comment */
typedef struct
{
   uint8_t md80_dev_no;
   uint8_t command;
   uint8_t size;
   uint32_t counter;
   uint8_t data[ETHCAT_LENGTH_DATAMAX];
} tEthCat_md80Com;

/* TODO: Comment */
typedef struct
{
   uint8_t md80_dev_no;
   uint8_t command;
   uint8_t size;
   uint32_t timestamp;
   uint8_t data[ETHCAT_LENGTH_DATAMAX];
} tEthCat_md80Res;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

#endif /* ETHCAT_TYPES_H_ */
