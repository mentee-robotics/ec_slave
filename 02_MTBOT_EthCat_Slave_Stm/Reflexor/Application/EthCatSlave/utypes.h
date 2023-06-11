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
      uint8_t Mode;
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Temperature;
      uint8_t Error;
      uint8_t enabled;
   } md80_0_DataReturn;
   struct
   {
      uint8_t Mode;
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Temperature;
      uint8_t Error;
      uint8_t enabled;
   } md80_1_DataReturn;
   struct
   {
      uint8_t Mode;
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Temperature;
      uint8_t Error;
      uint8_t enabled;
   } md80_2_DataReturn;
   struct
   {
      uint8_t Mode;
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Temperature;
      uint8_t Error;
      uint8_t enabled;
   } md80_3_DataReturn;

   /* Outputs */

   struct
   {
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Mode;
      uint8_t enable;
   } md80_0_DataControl;
   struct
   {
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Mode;
      uint8_t enable;
   } md80_1_DataControl;
   struct
   {
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Mode;
      uint8_t enable;
   } md80_2_DataControl;
   struct
   {
      uint16_t Position;
      uint16_t Velocity;
      uint16_t Torque;
      uint8_t Mode;
      uint8_t enable;
   } md80_3_DataControl;

   /* Parameters */

   uint32_t md80_addrs[8];
   uint8_t md80_Command[27];
   uint8_t md80_Respond[27];
} _Objects;

extern _Objects Obj;

#endif /* __UTYPES_H__ */
