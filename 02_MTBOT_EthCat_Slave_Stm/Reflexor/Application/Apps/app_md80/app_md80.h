/**
 * @file app_md80.h
 *
 * TODO: Comment.
 */
#ifndef APP_MD80_H_
#define APP_MD80_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "md80/md80.h"
#include "canIdle.h"
#include "ethCat.h"
#include "utypes.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define APPMD80_COMMAND_LENGTH      (27U)
#define APPMD80_COMMAND_LENGTH      (27U)
#define APPMD80_DATA_LENTH          (24U)

#define APP_MD80_TOTAL_DEV_SUPPORT  (4U)

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef enum 
{
   APP_MD80_STOP = 0,               /**< App is not working, receive any command from host except the Begin command. */
   APP_MD80_RUN,                    /**< App is running. */
} tAppMd80_WorkState;

typedef enum 
{
   APP_MD80_IDLE = 0,               /**< TODO: Comment.  */
   APP_MD80_COMMAND_ENTER,          /**< TODO: Comment.  */
   APP_MD80_COMMAND,                /**< TODO: Comment.  */
   APP_MD80_COMMAND_EXIT,           /**< TODO: Comment.  */
} tAppMd80_States;

typedef struct
{
   uint8_t motorId;
   tMd80_BusFrameId frId;
   uint8_t size;
   uint8_t data[APPMD80_DATA_LENTH];
} tAppMd80_Msg;

typedef struct 
{
   tAppMd80_Msg hostReq;
   tAppMd80_States state;
   tAppMd80_WorkState workState;
   bool isNewCommand;
   tAppMd80_Msg cmd;
   tAppMd80_Msg rsp;
   tMd80_Device * pMd80[APP_MD80_TOTAL_DEV_SUPPORT];
   uint8_t numMd80Det;
} tAppMd80_Data;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

void App_Md80_UpdateCmd (uint8_t md80id, uint8_t command, uint8_t size, uint8_t * cmd); 
void App_Md80_Init (void);
void App_Md80_MainFunction (void);

#endif /* APP_MD80_H_  */
