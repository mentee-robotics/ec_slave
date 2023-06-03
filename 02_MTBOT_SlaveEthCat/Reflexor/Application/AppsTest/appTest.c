/**
 * @file appTest.c
 *
 * TODO: Comment
 */

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include "appTest.h"
#include "AppsTest/CanIdle/appTest_canIdle.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define APPTEST_CANIDLE (0)
#define APPTEST_ETHCAT (0)
#define APPTEST_TMC8462 (0)
#define APPTEST_SOES (0)
#define APPTEST_ (0)

/*---------------------------------------------------------------------------------------------------------------------
 *                                                VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DEFINATIONS
 *-------------------------------------------------------------------------------------------------------------------*/

void AppTest_Init ()
{
#if APPTEST_TMC8462 == 1
   AppTest_TMC8462A_Init();
#endif

#if APPTEST_CANIDLE == 1

#endif
}

void AppTest_MainFunction ()
{
#if APPTEST_TMC8462 == 1
   AppTest_TMC8462A_MainFunction();
#endif

#if APPTEST_CANIDLE == 1
   AppTest_CanIdle_MainFunction();
#endif
}

void testTask (void const * arg)
{
   /* Initialize all the test app. */
   AppTest_Init();

   /* Infinite loop */
   for (;;)
   {
      AppTest_MainFunction();
      osDelay(10);
   }
}
