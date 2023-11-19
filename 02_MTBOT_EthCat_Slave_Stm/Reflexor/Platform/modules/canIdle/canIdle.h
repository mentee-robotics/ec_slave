/**
 * @file canIdle.h
 *
 * TODO: Comment.
 */
#ifndef CANIDLE_H_
#define CANIDLE_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "md80.h"
#include "canIdle.h"
#include "ethCat.h"
#include "utypes.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define CANIDLE_COMMAND_LENGTH      (27U)
#define CANIDLE_COMMAND_LENGTH      (27U)
#define CANIDLE_DATA_LENTH          (24U)

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

/* TODO: Comment */
typedef enum 
{
   CANIDLE_STOP = 0,               /**< App is not working, receive any command from host except the Begin command. */
   CANIDLE_RUN,                    /**< App is running. */
} tCanIdle_WorkState;

/* TODO: Comment */
typedef enum 
{
   CANIDLE_IDLE = 0,               /**< TODO: Comment.  */
   CANIDLE_COMMAND_ENTER,          /**< TODO: Comment.  */
   CANIDLE_COMMAND,                /**< TODO: Comment.  */
   CANIDLE_COMMAND_EXIT,           /**< TODO: Comment.  */
} tCanIdle_States;

/* TODO: Comment */
typedef struct
{
   uint8_t motorId;
   tMd80_BusFrameId frId;
   uint8_t size;
   uint8_t data[CANIDLE_DATA_LENTH];
} tCanIdle_Msg;

/* TODO: Comment */
typedef struct 
{
   tCanIdle_Msg hostReq;
   tCanIdle_States state;
   tCanIdle_WorkState workState;
   bool isNewCommand;
   tCanIdle_Msg cmd;
   tCanIdle_Msg rsp;
   uint8_t numMd80Det;
} tCanIdle_Data;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/ 

extern tCanIdle_Data canIdle_Module;

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

extern void canIdle_UpdateCmd (uint8_t md80id, uint8_t command, uint8_t size, uint8_t * cmd);
extern void canIdle_Init (tMd80_Baudrate);
extern void canIdle_MainFunction (void);

#endif /* CANIDLE_H_  */
