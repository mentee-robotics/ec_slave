#ifndef __UTYPES_H__
#define __UTYPES_H__

#include "cc.h"

/* Object dictionary storage */

typedef struct
{
   /* Identity */

   uint32_t serial;

   /* Inputs */

   struct
   {
      uint32_t Mode;
      uint32_t Position;
      uint32_t Velocity;
      uint32_t Torque;
      uint32_t Temperature;
      uint8_t Error;
   } Dev_0_DataReturn;

   /* Outputs */

   struct
   {
      uint32_t Position;
      uint32_t Velocity;
      uint32_t Torque;
      uint32_t Mode;
   } Dev_0_DataControl;

} _Objects;

extern _Objects Obj;

#endif /* __UTYPES_H__ */
