/**
 * @file appTest_canIdle.h
 *
 * TODO: Comment.
 */
#ifndef APPTEST_CANIDLE_H_
#define APPTEST_CANIDLE_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "md80.h"
#include "canM_Types.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef enum 
{
   APPTEST_CANIDLE_NO = 0,
   APPTEST_CANIDLE_BLINK,
   APPTEST_CANIDLE_SCAN,
   APPTEST_CANIDLE_ADD_MD80,
   APPTEST_CANIDLE_CONFIGURE_BAUDRATE,
   APPTEST_CANIDLE_MD80_SET_BAUDRATE,
   APPTEST_CANIDLE_MD80_SAVE,
   APPTEST_CANIDLE_MD80_RESET,
   APPTEST_CANIDLE_EXAMPLE1,
   APPTEST_CANIDLE_EXAMPLE2,
   APPTEST_CANIDLE_EXAMPLE3,
   APPTEST_CANIDLE_EXAMPLE4,
   APPTEST_CANIDLE_EXAMPLE5,
   APPTEST_CANIDLE_EXAMPLE6,
   APPTEST_CANIDLE_EXAMPLE7,
   APPTEST_CANIDLE_EXAMPLE8,
   APPTEST_CANIDLE_EXAMPLE9,
} tAppTest_CanIdle_Case;

/* The data structure contains used for this test. */
typedef struct 
{
   uint16_t md80id;
   tMd80_Device dev;
   tAppTest_CanIdle_Case testCase;
   float angleRad;
   float dt;
   float maxAngleRad;
   bool rotationCycle;
   bool isCanIdleEnabled;
   tMd80_Baudrate md80BaudrateSet;
   uint16_t newTimeoutMs;
   bool isCanTermination;
} tAppTest_CanIdle;

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

void AppTest_CanIdle_Init ();
void AppTest_CanIdle_MainFunction ();

#endif /* APPTEST_CANIDLE_H_ */
