#ifndef CAN_ISO_TP_MCU_LOCK_
#define CAN_ISO_TP_MCU_LOCK_

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"

typedef unsigned int mcu_lock_t;
typedef unsigned int cpu_status_t;

//In PC test environment, no interruption is needed.
#define MCU_LOCK_ENTER_CRITICAL taskENTER_CRITICAL()
#define MCU_LOCK_EXIT_CRITICAL taskEXIT_CRITICAL()


void mcu_lock_init(mcu_lock_t *lock);
unsigned int mcu_lock_try_lock(register mcu_lock_t *lock);
void mcu_lock_unlock(mcu_lock_t *lock);

#endif /* CAN_ISO_TP_MCU_LOCK_ */
