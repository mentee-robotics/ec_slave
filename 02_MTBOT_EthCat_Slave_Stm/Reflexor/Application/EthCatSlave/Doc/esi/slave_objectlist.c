#include "esc_coe.h"
#include "utypes.h"
#include <stddef.h>


static const char acName1000[] = "Device Type";
static const char acName1008[] = "Device Name";
static const char acName1009[] = "Hardware Version";
static const char acName100A[] = "Software Version";
static const char acName1018[] = "Identity Object";
static const char acName1018_00[] = "Max SubIndex";
static const char acName1018_01[] = "Vendor ID";
static const char acName1018_02[] = "Product Code";
static const char acName1018_03[] = "Revision Number";
static const char acName1018_04[] = "Serial Number";
static const char acName1600[] = "DataRxs";
static const char acName1600_00[] = "Max SubIndex";
static const char acName1600_01[] = "NoItems";
static const char acName1600_02[] = "DataRx0";
static const char acName1600_03[] = "DataRx1";
static const char acName1600_04[] = "DataRx2";
static const char acName1600_05[] = "DataRx3";
static const char acName1A00[] = "DataTxs";
static const char acName1A00_00[] = "Max SubIndex";
static const char acName1A00_01[] = "NoItems";
static const char acName1A00_02[] = "DataTx0";
static const char acName1A00_03[] = "DataTx1";
static const char acName1A00_04[] = "DataTx2";
static const char acName1A00_05[] = "DataTx3";
static const char acName1C00[] = "Sync Manager Communication Type";
static const char acName1C00_00[] = "Max SubIndex";
static const char acName1C00_01[] = "Communications Type SM0";
static const char acName1C00_02[] = "Communications Type SM1";
static const char acName1C00_03[] = "Communications Type SM2";
static const char acName1C00_04[] = "Communications Type SM3";
static const char acName1C12[] = "Sync Manager 2 PDO Assignment";
static const char acName1C12_00[] = "Max SubIndex";
static const char acName1C12_01[] = "PDO Mapping";
static const char acName1C13[] = "Sync Manager 3 PDO Assignment";
static const char acName1C13_00[] = "Max SubIndex";
static const char acName1C13_01[] = "PDO Mapping";
static const char acName6000[] = "DataTxs";
static const char acName6000_00[] = "Max SubIndex";
static const char acName6000_01[] = "NoItems";
static const char acName6000_02[] = "DataTx0";
static const char acName6000_03[] = "DataTx1";
static const char acName6000_04[] = "DataTx2";
static const char acName6000_05[] = "DataTx3";
static const char acName7004[] = "DataRxs";
static const char acName7004_00[] = "Max SubIndex";
static const char acName7004_01[] = "NoItems";
static const char acName7004_02[] = "DataRx0";
static const char acName7004_03[] = "DataRx1";
static const char acName7004_04[] = "DataRx2";
static const char acName7004_05[] = "DataRx3";

const _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000, 5001, NULL},
};
const _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 240, ATYPE_RO, acName1008, 0, "8 Channels digital communicate"},
};
const _objd SDO1009[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 40, ATYPE_RO, acName1009, 0, "0.0.1"},
};
const _objd SDO100A[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 40, ATYPE_RO, acName100A, 0, "0.0.1"},
};
const _objd SDO1018[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1018_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_01, 0, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_02, 700707, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_03, 2, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_04, 1, &Obj.serial},
};
const _objd SDO1600[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1600_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_01, 0x70040108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_02, 0x70040208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_03, 0x70040308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_04, 0x70040408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_05, 0x70040508, NULL},
};
const _objd SDO1A00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A00_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_01, 0x60000108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_02, 0x60000208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_03, 0x60000308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_04, 0x60000408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_05, 0x60000508, NULL},
};
const _objd SDO1C00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_01, 1, NULL},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_02, 2, NULL},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_03, 3, NULL},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C00_04, 4, NULL},
};
const _objd SDO1C12[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C12_00, 1, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_01, 0x1600, NULL},
};
const _objd SDO1C13[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_00, 1, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_01, 0x1A00, NULL},
};
const _objd SDO6000[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_01, 4, &Obj.DataTxs.NoItems},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName6000_02, 0, &Obj.DataTxs.DataTx0},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName6000_03, 0, &Obj.DataTxs.DataTx1},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName6000_04, 0, &Obj.DataTxs.DataTx2},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName6000_05, 0, &Obj.DataTxs.DataTx3},
};
const _objd SDO7004[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7004_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7004_01, 4, &Obj.DataRxs.NoItems},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName7004_02, 0, &Obj.DataRxs.DataRx0},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName7004_03, 0, &Obj.DataRxs.DataRx1},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName7004_04, 0, &Obj.DataRxs.DataRx2},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName7004_05, 0, &Obj.DataRxs.DataRx3},
};

const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 0, 0, acName1000, SDO1000},
  {0x1008, OTYPE_VAR, 0, 0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 0, 0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 0, 0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD, 4, 0, acName1018, SDO1018},
  {0x1600, OTYPE_RECORD, 5, 0, acName1600, SDO1600},
  {0x1A00, OTYPE_RECORD, 5, 0, acName1A00, SDO1A00},
  {0x1C00, OTYPE_ARRAY, 4, 0, acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY, 1, 0, acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY, 1, 0, acName1C13, SDO1C13},
  {0x6000, OTYPE_RECORD, 5, 0, acName6000, SDO6000},
  {0x7004, OTYPE_RECORD, 5, 0, acName7004, SDO7004},
  {0xffff, 0xff, 0xff, 0xff, NULL, NULL}
};
