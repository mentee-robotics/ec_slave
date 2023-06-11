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
      uint8_t NoItems;
      uint8_t DataTx0;
      uint8_t DataTx1;
      uint8_t DataTx2;
      uint8_t DataTx3;
   } DataTxs;

   /* Outputs */

   struct
   {
      uint8_t NoItems;
      uint8_t DataRx0;
      uint8_t DataRx1;
      uint8_t DataRx2;
      uint8_t DataRx3;
   } DataRxs;

} _Objects;

extern _Objects Obj;

#endif /* __UTYPES_H__ */
