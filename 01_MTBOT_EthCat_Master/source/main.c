/** INCLUDE */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include "ethercat.h"
#include "log.h"

/** IMPORTANT DEFINE */

/** TYPE DEFINE */

/** GLOBAL VARIABLES DEFINITION */

/** FUNCTION DECLERATION */
void Module_Init(void); 

/** FUNCTION DEFINITION */
void Module_Init (void)
{
   /* Log module */
   log_set_quiet (false);
   log_set_level (LOG_DEBUG);
}

void main (void)
{
   log_trace ("Start the ethercat master.\n");

   /* Initialize all modules. */
   Module_Init();

   /* Create thread to handle slave error handling in OP mode. */

   /* Start cyclic part. */

}