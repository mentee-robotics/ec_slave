#include "can_iso_tp_fifoqueue.h"
#include "can_iso_tp_mcu_lock.h"

/*Queue Init*/
uint8_t
QueueInit(struct FifoQueue *Queue, ElemType *dat, uint16_t queue_size)
{
   uint8_t ret = (0 == 1);
   Queue->queue_size = queue_size;
   Queue->dat = dat;
   if ((Queue->dat != (ElemType *)0) && (Queue->queue_size != 0))
   {
      Queue->front = 0;
      Queue->rear = 0;
      ;
      Queue->count = 0;
      ret = (1 == 1);
   }
   return ret;
}

/* Queue In */
uint8_t
QueueIn(register struct FifoQueue *Queue, ElemType sdat)
{
   MCU_LOCK_ENTER_CRITICAL;
   if ((Queue->front == Queue->rear) && (Queue->count == Queue->queue_size))
   {
      MCU_LOCK_EXIT_CRITICAL;
      return QueueFull;
   }
   else
   {
      Queue->dat[Queue->rear] = sdat;
      Queue->rear = (Queue->rear + 1) % Queue->queue_size;
      Queue->count = Queue->count + 1;
      MCU_LOCK_EXIT_CRITICAL;
      return QueueOperateOk;
   }
}

/* Queue Out*/
uint8_t
QueueOut(register struct FifoQueue *Queue, ElemType *sdat)
{
   MCU_LOCK_ENTER_CRITICAL;
   if ((Queue->front == Queue->rear) && (Queue->count == 0))
   {
      MCU_LOCK_EXIT_CRITICAL;
      return QueueEmpty;
   }
   else
   {
      *sdat = Queue->dat[Queue->front];
      Queue->front = (Queue->front + 1) % Queue->queue_size;
      Queue->count = Queue->count - 1;
      MCU_LOCK_EXIT_CRITICAL;
      return QueueOperateOk;
   }
}