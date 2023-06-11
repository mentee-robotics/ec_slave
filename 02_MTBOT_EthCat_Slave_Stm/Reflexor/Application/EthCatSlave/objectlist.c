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
static const char acName1600[] = "md80_0_DataControl";
static const char acName1600_00[] = "Max SubIndex";
static const char acName1600_01[] = "Position";
static const char acName1600_02[] = "Velocity";
static const char acName1600_03[] = "Torque";
static const char acName1600_04[] = "Mode";
static const char acName1600_05[] = "enable";
static const char acName1600_06[] = "Padding 5";
static const char acName1601[] = "md80_1_DataControl";
static const char acName1601_00[] = "Max SubIndex";
static const char acName1601_01[] = "Position";
static const char acName1601_02[] = "Velocity";
static const char acName1601_03[] = "Torque";
static const char acName1601_04[] = "Mode";
static const char acName1601_05[] = "enable";
static const char acName1601_06[] = "Padding 6";
static const char acName1602[] = "md80_2_DataControl";
static const char acName1602_00[] = "Max SubIndex";
static const char acName1602_01[] = "Position";
static const char acName1602_02[] = "Velocity";
static const char acName1602_03[] = "Torque";
static const char acName1602_04[] = "Mode";
static const char acName1602_05[] = "enable";
static const char acName1602_06[] = "Padding 7";
static const char acName1603[] = "md80_3_DataControl";
static const char acName1603_00[] = "Max SubIndex";
static const char acName1603_01[] = "Position";
static const char acName1603_02[] = "Velocity";
static const char acName1603_03[] = "Torque";
static const char acName1603_04[] = "Mode";
static const char acName1603_05[] = "enable";
static const char acName1603_06[] = "Padding 8";
static const char acName1A00[] = "md80_0_DataReturn";
static const char acName1A00_00[] = "Max SubIndex";
static const char acName1A00_01[] = "Mode";
static const char acName1A00_02[] = "Position";
static const char acName1A00_03[] = "Velocity";
static const char acName1A00_04[] = "Torque";
static const char acName1A00_05[] = "Temperature";
static const char acName1A00_06[] = "Error";
static const char acName1A00_07[] = "enabled";
static const char acName1A00_08[] = "Padding 1";
static const char acName1A01[] = "md80_1_DataReturn";
static const char acName1A01_00[] = "Max SubIndex";
static const char acName1A01_01[] = "Mode";
static const char acName1A01_02[] = "Position";
static const char acName1A01_03[] = "Velocity";
static const char acName1A01_04[] = "Torque";
static const char acName1A01_05[] = "Temperature";
static const char acName1A01_06[] = "Error";
static const char acName1A01_07[] = "enabled";
static const char acName1A01_08[] = "Padding 2";
static const char acName1A02[] = "md80_2_DataReturn";
static const char acName1A02_00[] = "Max SubIndex";
static const char acName1A02_01[] = "Mode";
static const char acName1A02_02[] = "Position";
static const char acName1A02_03[] = "Velocity";
static const char acName1A02_04[] = "Torque";
static const char acName1A02_05[] = "Temperature";
static const char acName1A02_06[] = "Error";
static const char acName1A02_07[] = "enabled";
static const char acName1A02_08[] = "Padding 3";
static const char acName1A03[] = "md80_3_DataReturn";
static const char acName1A03_00[] = "Max SubIndex";
static const char acName1A03_01[] = "Mode";
static const char acName1A03_02[] = "Position";
static const char acName1A03_03[] = "Velocity";
static const char acName1A03_04[] = "Torque";
static const char acName1A03_05[] = "Temperature";
static const char acName1A03_06[] = "Error";
static const char acName1A03_07[] = "enabled";
static const char acName1A03_08[] = "Padding 4";
static const char acName1C00[] = "Sync Manager Communication Type";
static const char acName1C00_00[] = "Max SubIndex";
static const char acName1C00_01[] = "Communications Type SM0";
static const char acName1C00_02[] = "Communications Type SM1";
static const char acName1C00_03[] = "Communications Type SM2";
static const char acName1C00_04[] = "Communications Type SM3";
static const char acName1C12[] = "Sync Manager 2 PDO Assignment";
static const char acName1C12_00[] = "Max SubIndex";
static const char acName1C12_01[] = "PDO Mapping";
static const char acName1C12_02[] = "PDO Mapping";
static const char acName1C12_03[] = "PDO Mapping";
static const char acName1C12_04[] = "PDO Mapping";
static const char acName1C13[] = "Sync Manager 3 PDO Assignment";
static const char acName1C13_00[] = "Max SubIndex";
static const char acName1C13_01[] = "PDO Mapping";
static const char acName1C13_02[] = "PDO Mapping";
static const char acName1C13_03[] = "PDO Mapping";
static const char acName1C13_04[] = "PDO Mapping";
static const char acName2001[] = "md80_addrs";
static const char acName2001_00[] = "Max SubIndex";
static const char acName2001_01[] = "md80_0";
static const char acName2001_02[] = "md80_1";
static const char acName2001_03[] = "md80_2";
static const char acName2001_04[] = "md80_3";
static const char acName2001_05[] = "md80_4";
static const char acName2001_06[] = "md80_5";
static const char acName2001_07[] = "md80_6";
static const char acName2001_08[] = "md80_7";
static const char acName2002[] = "md80_Command";
static const char acName2002_00[] = "Max SubIndex";
static const char acName2002_01[] = "md80_dev_no";
static const char acName2002_02[] = "command";
static const char acName2002_03[] = "size";
static const char acName2002_04[] = "dataSet0";
static const char acName2002_05[] = "dataSet1";
static const char acName2002_06[] = "dataSet2";
static const char acName2002_07[] = "dataSet3";
static const char acName2002_08[] = "dataSet4";
static const char acName2002_09[] = "dataSet5";
static const char acName2002_10[] = "dataSet6";
static const char acName2002_11[] = "dataSet7";
static const char acName2002_12[] = "dataSet8";
static const char acName2002_13[] = "dataSet9";
static const char acName2002_14[] = "dataSet10";
static const char acName2002_15[] = "dataSet11";
static const char acName2002_16[] = "dataSet12";
static const char acName2002_17[] = "dataSet13";
static const char acName2002_18[] = "dataSet14";
static const char acName2002_19[] = "dataSet15";
static const char acName2002_20[] = "dataSet16";
static const char acName2002_21[] = "dataSet17";
static const char acName2002_22[] = "dataSet18";
static const char acName2002_23[] = "dataSet19";
static const char acName2002_24[] = "dataSet20";
static const char acName2002_25[] = "dataSet21";
static const char acName2002_26[] = "dataSet22";
static const char acName2002_27[] = "dataSet23";
static const char acName2003[] = "md80_Respond";
static const char acName2003_00[] = "Max SubIndex";
static const char acName2003_01[] = "md80_dev_no";
static const char acName2003_02[] = "command";
static const char acName2003_03[] = "size";
static const char acName2003_04[] = "dataRet0";
static const char acName2003_05[] = "dataRet1";
static const char acName2003_06[] = "dataRet2";
static const char acName2003_07[] = "dataRet3";
static const char acName2003_08[] = "dataRet4";
static const char acName2003_09[] = "dataRet5";
static const char acName2003_10[] = "dataRet6";
static const char acName2003_11[] = "dataRet7";
static const char acName2003_12[] = "dataRet8";
static const char acName2003_13[] = "dataRet9";
static const char acName2003_14[] = "dataRet10";
static const char acName2003_15[] = "dataRet11";
static const char acName2003_16[] = "dataRet12";
static const char acName2003_17[] = "dataRet13";
static const char acName2003_18[] = "dataRet14";
static const char acName2003_19[] = "dataRet15";
static const char acName2003_20[] = "dataRet16";
static const char acName2003_21[] = "dataRet17";
static const char acName2003_22[] = "dataRet18";
static const char acName2003_23[] = "dataRet19";
static const char acName2003_24[] = "dataRet20";
static const char acName2003_25[] = "dataRet21";
static const char acName2003_26[] = "dataRet22";
static const char acName2003_27[] = "dataRet23";
static const char acName6000[] = "md80_0_DataReturn";
static const char acName6000_00[] = "Max SubIndex";
static const char acName6000_01[] = "Mode";
static const char acName6000_02[] = "Position";
static const char acName6000_03[] = "Velocity";
static const char acName6000_04[] = "Torque";
static const char acName6000_05[] = "Temperature";
static const char acName6000_06[] = "Error";
static const char acName6000_07[] = "enabled";
static const char acName6001[] = "md80_1_DataReturn";
static const char acName6001_00[] = "Max SubIndex";
static const char acName6001_01[] = "Mode";
static const char acName6001_02[] = "Position";
static const char acName6001_03[] = "Velocity";
static const char acName6001_04[] = "Torque";
static const char acName6001_05[] = "Temperature";
static const char acName6001_06[] = "Error";
static const char acName6001_07[] = "enabled";
static const char acName6002[] = "md80_2_DataReturn";
static const char acName6002_00[] = "Max SubIndex";
static const char acName6002_01[] = "Mode";
static const char acName6002_02[] = "Position";
static const char acName6002_03[] = "Velocity";
static const char acName6002_04[] = "Torque";
static const char acName6002_05[] = "Temperature";
static const char acName6002_06[] = "Error";
static const char acName6002_07[] = "enabled";
static const char acName6003[] = "md80_3_DataReturn";
static const char acName6003_00[] = "Max SubIndex";
static const char acName6003_01[] = "Mode";
static const char acName6003_02[] = "Position";
static const char acName6003_03[] = "Velocity";
static const char acName6003_04[] = "Torque";
static const char acName6003_05[] = "Temperature";
static const char acName6003_06[] = "Error";
static const char acName6003_07[] = "enabled";
static const char acName7000[] = "md80_0_DataControl";
static const char acName7000_00[] = "Max SubIndex";
static const char acName7000_01[] = "Position";
static const char acName7000_02[] = "Velocity";
static const char acName7000_03[] = "Torque";
static const char acName7000_04[] = "Mode";
static const char acName7000_05[] = "enable";
static const char acName7001[] = "md80_1_DataControl";
static const char acName7001_00[] = "Max SubIndex";
static const char acName7001_01[] = "Position";
static const char acName7001_02[] = "Velocity";
static const char acName7001_03[] = "Torque";
static const char acName7001_04[] = "Mode";
static const char acName7001_05[] = "enable";
static const char acName7002[] = "md80_2_DataControl";
static const char acName7002_00[] = "Max SubIndex";
static const char acName7002_01[] = "Position";
static const char acName7002_02[] = "Velocity";
static const char acName7002_03[] = "Torque";
static const char acName7002_04[] = "Mode";
static const char acName7002_05[] = "enable";
static const char acName7003[] = "md80_3_DataControl";
static const char acName7003_00[] = "Max SubIndex";
static const char acName7003_01[] = "Position";
static const char acName7003_02[] = "Velocity";
static const char acName7003_03[] = "Torque";
static const char acName7003_04[] = "Mode";
static const char acName7003_05[] = "enable";

const _objd SDO1000[] =
{
  {0x0, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1000, 5001, NULL},
};
const _objd SDO1008[] =
{
  {0x0, DTYPE_VISIBLE_STRING, 152, ATYPE_RO, acName1008, 0, "MTBOT_Ethercat_Node"},
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
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_01, 2, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_02, 702345, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_03, 2, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1018_04, 1, &Obj.serial},
};
const _objd SDO1600[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1600_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_01, 0x70000110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_02, 0x70000210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_03, 0x70000310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_04, 0x70000408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_05, 0x70000501, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_06, 0x00000007, NULL},
};
const _objd SDO1601[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1601_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_01, 0x70010110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_02, 0x70010210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_03, 0x70010310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_04, 0x70010408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_05, 0x70010501, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_06, 0x00000007, NULL},
};
const _objd SDO1602[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1602_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_01, 0x70020110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_02, 0x70020210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_03, 0x70020310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_04, 0x70020408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_05, 0x70020501, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_06, 0x00000007, NULL},
};
const _objd SDO1603[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1603_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_01, 0x70030110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_02, 0x70030210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_03, 0x70030310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_04, 0x70030408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_05, 0x70030501, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_06, 0x00000007, NULL},
};
const _objd SDO1A00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A00_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_01, 0x60000108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_02, 0x60000210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_03, 0x60000310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_04, 0x60000410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_05, 0x60000508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_06, 0x60000608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_07, 0x60000701, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_08, 0x00000007, NULL},
};
const _objd SDO1A01[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A01_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_01, 0x60010108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_02, 0x60010210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_03, 0x60010310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_04, 0x60010410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_05, 0x60010508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_06, 0x60010608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_07, 0x60010701, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_08, 0x00000007, NULL},
};
const _objd SDO1A02[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A02_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_01, 0x60020108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_02, 0x60020210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_03, 0x60020310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_04, 0x60020410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_05, 0x60020508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_06, 0x60020608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_07, 0x60020701, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_08, 0x00000007, NULL},
};
const _objd SDO1A03[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A03_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_01, 0x60030108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_02, 0x60030210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_03, 0x60030310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_04, 0x60030410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_05, 0x60030508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_06, 0x60030608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_07, 0x60030701, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_08, 0x00000007, NULL},
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
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C12_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_01, 0x1600, NULL},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_02, 0x1601, NULL},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_03, 0x1602, NULL},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_04, 0x1603, NULL},
};
const _objd SDO1C13[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_00, 4, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_01, 0x1A00, NULL},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_02, 0x1A01, NULL},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_03, 0x1A02, NULL},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_04, 0x1A03, NULL},
};
const _objd SDO2001[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2001_00, 8, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_01, 0, &Obj.md80_addrs[0]},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_02, 0, &Obj.md80_addrs[1]},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_03, 0, &Obj.md80_addrs[2]},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_04, 0, &Obj.md80_addrs[3]},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_05, 0, &Obj.md80_addrs[4]},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_06, 0, &Obj.md80_addrs[5]},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_07, 0, &Obj.md80_addrs[6]},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName2001_08, 0, &Obj.md80_addrs[7]},
};
const _objd SDO2002[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2002_00, 27, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_01, 0, &Obj.md80_Command[0]},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_02, 0, &Obj.md80_Command[1]},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_03, 0, &Obj.md80_Command[2]},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_04, 0, &Obj.md80_Command[3]},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_05, 0, &Obj.md80_Command[4]},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_06, 0, &Obj.md80_Command[5]},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_07, 0, &Obj.md80_Command[6]},
  {0x08, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_08, 0, &Obj.md80_Command[7]},
  {0x09, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_09, 0, &Obj.md80_Command[8]},
  {0x10, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_10, 0, &Obj.md80_Command[9]},
  {0x11, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_11, 0, &Obj.md80_Command[10]},
  {0x12, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_12, 0, &Obj.md80_Command[11]},
  {0x13, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_13, 0, &Obj.md80_Command[12]},
  {0x14, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_14, 0, &Obj.md80_Command[13]},
  {0x15, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_15, 0, &Obj.md80_Command[14]},
  {0x16, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_16, 0, &Obj.md80_Command[15]},
  {0x17, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_17, 0, &Obj.md80_Command[16]},
  {0x18, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_18, 0, &Obj.md80_Command[17]},
  {0x19, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_19, 0, &Obj.md80_Command[18]},
  {0x20, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_20, 0, &Obj.md80_Command[19]},
  {0x21, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_21, 0, &Obj.md80_Command[20]},
  {0x22, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_22, 0, &Obj.md80_Command[21]},
  {0x23, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_23, 0, &Obj.md80_Command[22]},
  {0x24, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_24, 0, &Obj.md80_Command[23]},
  {0x25, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_25, 0, &Obj.md80_Command[24]},
  {0x26, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_26, 0, &Obj.md80_Command[25]},
  {0x27, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName2002_27, 0, &Obj.md80_Command[26]},
};
const _objd SDO2003[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_00, 27, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_01, 0, &Obj.md80_Respond[0]},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_02, 0, &Obj.md80_Respond[1]},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_03, 0, &Obj.md80_Respond[2]},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_04, 0, &Obj.md80_Respond[3]},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_05, 0, &Obj.md80_Respond[4]},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_06, 0, &Obj.md80_Respond[5]},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_07, 0, &Obj.md80_Respond[6]},
  {0x08, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_08, 0, &Obj.md80_Respond[7]},
  {0x09, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_09, 0, &Obj.md80_Respond[8]},
  {0x10, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_10, 0, &Obj.md80_Respond[9]},
  {0x11, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_11, 0, &Obj.md80_Respond[10]},
  {0x12, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_12, 0, &Obj.md80_Respond[11]},
  {0x13, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_13, 0, &Obj.md80_Respond[12]},
  {0x14, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_14, 0, &Obj.md80_Respond[13]},
  {0x15, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_15, 0, &Obj.md80_Respond[14]},
  {0x16, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_16, 0, &Obj.md80_Respond[15]},
  {0x17, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_17, 0, &Obj.md80_Respond[16]},
  {0x18, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_18, 0, &Obj.md80_Respond[17]},
  {0x19, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_19, 0, &Obj.md80_Respond[18]},
  {0x20, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_20, 0, &Obj.md80_Respond[19]},
  {0x21, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_21, 0, &Obj.md80_Respond[20]},
  {0x22, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_22, 0, &Obj.md80_Respond[21]},
  {0x23, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_23, 0, &Obj.md80_Respond[22]},
  {0x24, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_24, 0, &Obj.md80_Respond[23]},
  {0x25, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_25, 0, &Obj.md80_Respond[24]},
  {0x26, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_26, 0, &Obj.md80_Respond[25]},
  {0x27, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName2003_27, 0, &Obj.md80_Respond[26]},
};
const _objd SDO6000[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_00, 7, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_01, 0, &Obj.md80_0_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6000_02, 0, &Obj.md80_0_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6000_03, 0, &Obj.md80_0_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6000_04, 0, &Obj.md80_0_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_05, 0, &Obj.md80_0_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName6000_06, 0, &Obj.md80_0_DataReturn.Error},
  {0x07, DTYPE_BOOLEAN, 1, ATYPE_RO, acName6000_07, 0, &Obj.md80_0_DataReturn.enabled},
};
const _objd SDO6001[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_00, 7, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_01, 0, &Obj.md80_1_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6001_02, 0, &Obj.md80_1_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6001_03, 0, &Obj.md80_1_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6001_04, 0, &Obj.md80_1_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_05, 0, &Obj.md80_1_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_06, 0, &Obj.md80_1_DataReturn.Error},
  {0x07, DTYPE_BOOLEAN, 1, ATYPE_RO, acName6001_07, 0, &Obj.md80_1_DataReturn.enabled},
};
const _objd SDO6002[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_00, 7, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_01, 0, &Obj.md80_2_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6002_02, 0, &Obj.md80_2_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6002_03, 0, &Obj.md80_2_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6002_04, 0, &Obj.md80_2_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_05, 0, &Obj.md80_2_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_06, 0, &Obj.md80_2_DataReturn.Error},
  {0x07, DTYPE_BOOLEAN, 1, ATYPE_RO, acName6002_07, 0, &Obj.md80_2_DataReturn.enabled},
};
const _objd SDO6003[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_00, 7, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_01, 0, &Obj.md80_3_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6003_02, 0, &Obj.md80_3_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6003_03, 0, &Obj.md80_3_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6003_04, 0, &Obj.md80_3_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_05, 0, &Obj.md80_3_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_06, 0, &Obj.md80_3_DataReturn.Error},
  {0x07, DTYPE_BOOLEAN, 1, ATYPE_RO, acName6003_07, 0, &Obj.md80_3_DataReturn.enabled},
};
const _objd SDO7000[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7000_01, 0, &Obj.md80_0_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7000_02, 0, &Obj.md80_0_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7000_03, 0, &Obj.md80_0_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7000_04, 0, &Obj.md80_0_DataControl.Mode},
  {0x05, DTYPE_BOOLEAN, 1, ATYPE_WO, acName7000_05, 0, &Obj.md80_0_DataControl.enable},
};
const _objd SDO7001[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7001_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7001_01, 0, &Obj.md80_1_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7001_02, 0, &Obj.md80_1_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7001_03, 0, &Obj.md80_1_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7001_04, 0, &Obj.md80_1_DataControl.Mode},
  {0x05, DTYPE_BOOLEAN, 1, ATYPE_WO, acName7001_05, 0, &Obj.md80_1_DataControl.enable},
};
const _objd SDO7002[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7002_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7002_01, 0, &Obj.md80_2_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7002_02, 0, &Obj.md80_2_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7002_03, 0, &Obj.md80_2_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7002_04, 0, &Obj.md80_2_DataControl.Mode},
  {0x05, DTYPE_BOOLEAN, 1, ATYPE_WO, acName7002_05, 0, &Obj.md80_2_DataControl.enable},
};
const _objd SDO7003[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7003_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7003_01, 0, &Obj.md80_3_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7003_02, 0, &Obj.md80_3_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7003_03, 0, &Obj.md80_3_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7003_04, 0, &Obj.md80_3_DataControl.Mode},
  {0x05, DTYPE_BOOLEAN, 1, ATYPE_WO, acName7003_05, 0, &Obj.md80_3_DataControl.enable},
};

const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 0, 0, acName1000, SDO1000},
  {0x1008, OTYPE_VAR, 0, 0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 0, 0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 0, 0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD, 4, 0, acName1018, SDO1018},
  {0x1600, OTYPE_RECORD, 6, 0, acName1600, SDO1600},
  {0x1601, OTYPE_RECORD, 6, 0, acName1601, SDO1601},
  {0x1602, OTYPE_RECORD, 6, 0, acName1602, SDO1602},
  {0x1603, OTYPE_RECORD, 6, 0, acName1603, SDO1603},
  {0x1A00, OTYPE_RECORD, 8, 0, acName1A00, SDO1A00},
  {0x1A01, OTYPE_RECORD, 8, 0, acName1A01, SDO1A01},
  {0x1A02, OTYPE_RECORD, 8, 0, acName1A02, SDO1A02},
  {0x1A03, OTYPE_RECORD, 8, 0, acName1A03, SDO1A03},
  {0x1C00, OTYPE_ARRAY, 4, 0, acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY, 4, 0, acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY, 4, 0, acName1C13, SDO1C13},
  {0x2001, OTYPE_ARRAY, 8, 0, acName2001, SDO2001},
  {0x2002, OTYPE_ARRAY, 27, 0, acName2002, SDO2002},
  {0x2003, OTYPE_ARRAY, 27, 0, acName2003, SDO2003},
  {0x6000, OTYPE_RECORD, 7, 0, acName6000, SDO6000},
  {0x6001, OTYPE_RECORD, 7, 0, acName6001, SDO6001},
  {0x6002, OTYPE_RECORD, 7, 0, acName6002, SDO6002},
  {0x6003, OTYPE_RECORD, 7, 0, acName6003, SDO6003},
  {0x7000, OTYPE_RECORD, 5, 0, acName7000, SDO7000},
  {0x7001, OTYPE_RECORD, 5, 0, acName7001, SDO7001},
  {0x7002, OTYPE_RECORD, 5, 0, acName7002, SDO7002},
  {0x7003, OTYPE_RECORD, 5, 0, acName7003, SDO7003},
  {0xffff, 0xff, 0xff, 0xff, NULL, NULL}
};
