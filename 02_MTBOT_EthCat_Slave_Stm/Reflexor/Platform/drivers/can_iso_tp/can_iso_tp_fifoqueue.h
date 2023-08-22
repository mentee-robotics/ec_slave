#ifndef FIFOQUEUE_H
#define FIFOQUEUE_H

#include <stdint.h>

typedef void *ElemType;

#define QueueFull 0
#define QueueEmpty 1
#define QueueOperateOk 2

struct FifoQueue
{
   // init by user
   ElemType *dat;
   uint16_t queue_size;
   // init by QueueInit
   uint16_t front;
   uint16_t rear;
   uint16_t count;
};

/*Queue Initalize*/
extern uint8_t QueueInit(struct FifoQueue *Queue, ElemType *dat, uint16_t queue_size);
/* Queue In */
extern uint8_t QueueIn(struct FifoQueue *Queue, ElemType sdat);
/* Queue Out */
extern uint8_t QueueOut(struct FifoQueue *Queue, ElemType *sdat);

#endif