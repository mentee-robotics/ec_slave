#include <stdint.h>
#include "can_iso_tp_mcu_lock.h"

void mcu_lock_init(mcu_lock_t *lock)
{
   *lock = 0;
}

unsigned int mcu_lock_try_lock(register mcu_lock_t *lock)
{
   register volatile cpu_status_t cpu_sr;
   register unsigned int ret = (0 == 1);
   if (0 == *lock)
   {
      MCU_LOCK_ENTER_CRITICAL;
      if (0 == *lock)
      {
         *lock = 1;
         ret = (1 == 1);
      }
      MCU_LOCK_EXIT_CRITICAL;
   }
   return ret;
}

void mcu_lock_unlock(mcu_lock_t *lock)
{
   /*cpu_status_t cpu_sr;
   if(true == *lock)
   {
      MCU_LOCK_ENTER_CRITICAL;
      if(true == *lock)
      {
         *lock = false;
      }
      MCU_LOCK_EXIT_CRITICAL;
   }*/
   *lock = 0;
}