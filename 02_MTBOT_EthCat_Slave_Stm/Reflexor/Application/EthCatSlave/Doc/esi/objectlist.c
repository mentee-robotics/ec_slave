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
static const char acName1600[] = "md80_Command";
static const char acName1600_00[] = "Max SubIndex";
static const char acName1600_01[] = "md80_dev_no";
static const char acName1600_02[] = "command";
static const char acName1600_03[] = "size";
static const char acName1600_04[] = "counter";
static const char acName1600_05[] = "dataSet0";
static const char acName1600_06[] = "dataSet1";
static const char acName1600_07[] = "dataSet2";
static const char acName1600_08[] = "dataSet3";
static const char acName1600_09[] = "dataSet4";
static const char acName1601[] = "md80_0_DataControl";
static const char acName1601_00[] = "Max SubIndex";
static const char acName1601_01[] = "Position";
static const char acName1601_02[] = "Velocity";
static const char acName1601_03[] = "Torque";
static const char acName1601_04[] = "Mode";
static const char acName1601_05[] = "enable";
static const char acName1601_06[] = "counter";
static const char acName1602[] = "md80_1_DataControl";
static const char acName1602_00[] = "Max SubIndex";
static const char acName1602_01[] = "Position";
static const char acName1602_02[] = "Velocity";
static const char acName1602_03[] = "Torque";
static const char acName1602_04[] = "Mode";
static const char acName1602_05[] = "enable";
static const char acName1602_06[] = "counter";
static const char acName1603[] = "md80_2_DataControl";
static const char acName1603_00[] = "Max SubIndex";
static const char acName1603_01[] = "Position";
static const char acName1603_02[] = "Velocity";
static const char acName1603_03[] = "Torque";
static const char acName1603_04[] = "Mode";
static const char acName1603_05[] = "enable";
static const char acName1603_06[] = "counter";
static const char acName1604[] = "md80_3_DataControl";
static const char acName1604_00[] = "Max SubIndex";
static const char acName1604_01[] = "Position";
static const char acName1604_02[] = "Velocity";
static const char acName1604_03[] = "Torque";
static const char acName1604_04[] = "Mode";
static const char acName1604_05[] = "enable";
static const char acName1604_06[] = "counter";
static const char acName1A00[] = "md80_addrs";
static const char acName1A00_00[] = "Max SubIndex";
static const char acName1A00_01[] = "md80_0";
static const char acName1A00_02[] = "md80_1";
static const char acName1A00_03[] = "md80_2";
static const char acName1A00_04[] = "md80_3";
static const char acName1A00_05[] = "md80_4";
static const char acName1A00_06[] = "md80_5";
static const char acName1A01[] = "md80_Respond";
static const char acName1A01_00[] = "Max SubIndex";
static const char acName1A01_01[] = "md80_dev_no";
static const char acName1A01_02[] = "command";
static const char acName1A01_03[] = "size";
static const char acName1A01_04[] = "timestamp";
static const char acName1A01_05[] = "dataRet0";
static const char acName1A01_06[] = "dataRet1";
static const char acName1A01_07[] = "dataRet2";
static const char acName1A01_08[] = "dataRet3";
static const char acName1A01_09[] = "dataRet4";
static const char acName1A02[] = "md80_0_DataReturn";
static const char acName1A02_00[] = "Max SubIndex";
static const char acName1A02_01[] = "Mode";
static const char acName1A02_02[] = "Position";
static const char acName1A02_03[] = "Velocity";
static const char acName1A02_04[] = "Torque";
static const char acName1A02_05[] = "Temperature";
static const char acName1A02_06[] = "Error";
static const char acName1A02_07[] = "enabled";
static const char acName1A02_08[] = "counter";
static const char acName1A02_09[] = "timestamp";
static const char acName1A03[] = "md80_1_DataReturn";
static const char acName1A03_00[] = "Max SubIndex";
static const char acName1A03_01[] = "Mode";
static const char acName1A03_02[] = "Position";
static const char acName1A03_03[] = "Velocity";
static const char acName1A03_04[] = "Torque";
static const char acName1A03_05[] = "Temperature";
static const char acName1A03_06[] = "Error";
static const char acName1A03_07[] = "enabled";
static const char acName1A03_08[] = "counter";
static const char acName1A03_09[] = "timestamp";
static const char acName1A04[] = "md80_2_DataReturn";
static const char acName1A04_00[] = "Max SubIndex";
static const char acName1A04_01[] = "Mode";
static const char acName1A04_02[] = "Position";
static const char acName1A04_03[] = "Velocity";
static const char acName1A04_04[] = "Torque";
static const char acName1A04_05[] = "Temperature";
static const char acName1A04_06[] = "Error";
static const char acName1A04_07[] = "enabled";
static const char acName1A04_08[] = "counter";
static const char acName1A04_09[] = "timestamp";
static const char acName1A05[] = "md80_3_DataReturn";
static const char acName1A05_00[] = "Max SubIndex";
static const char acName1A05_01[] = "Mode";
static const char acName1A05_02[] = "Position";
static const char acName1A05_03[] = "Velocity";
static const char acName1A05_04[] = "Torque";
static const char acName1A05_05[] = "Temperature";
static const char acName1A05_06[] = "Error";
static const char acName1A05_07[] = "enabled";
static const char acName1A05_08[] = "counter";
static const char acName1A05_09[] = "timestamp";
static const char acName1A06[] = "md80_Last_Command_Received";
static const char acName1A06_00[] = "Max SubIndex";
static const char acName1A06_01[] = "command";
static const char acName1A06_02[] = "counter";
static const char acName1A06_03[] = "timestamp";
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
static const char acName1C12_05[] = "PDO Mapping";
static const char acName1C13[] = "Sync Manager 3 PDO Assignment";
static const char acName1C13_00[] = "Max SubIndex";
static const char acName1C13_01[] = "PDO Mapping";
static const char acName1C13_02[] = "PDO Mapping";
static const char acName1C13_03[] = "PDO Mapping";
static const char acName1C13_04[] = "PDO Mapping";
static const char acName1C13_05[] = "PDO Mapping";
static const char acName1C13_06[] = "PDO Mapping";
static const char acName1C13_07[] = "PDO Mapping";
static const char acName6000[] = "md80_addrs";
static const char acName6000_00[] = "Max SubIndex";
static const char acName6000_01[] = "md80_0";
static const char acName6000_02[] = "md80_1";
static const char acName6000_03[] = "md80_2";
static const char acName6000_04[] = "md80_3";
static const char acName6000_05[] = "md80_4";
static const char acName6000_06[] = "md80_5";
static const char acName6001[] = "md80_Respond";
static const char acName6001_00[] = "Max SubIndex";
static const char acName6001_01[] = "md80_dev_no";
static const char acName6001_02[] = "command";
static const char acName6001_03[] = "size";
static const char acName6001_04[] = "timestamp";
static const char acName6001_05[] = "dataRet0";
static const char acName6001_06[] = "dataRet1";
static const char acName6001_07[] = "dataRet2";
static const char acName6001_08[] = "dataRet3";
static const char acName6001_09[] = "dataRet4";
static const char acName6002[] = "md80_0_DataReturn";
static const char acName6002_00[] = "Max SubIndex";
static const char acName6002_01[] = "Mode";
static const char acName6002_02[] = "Position";
static const char acName6002_03[] = "Velocity";
static const char acName6002_04[] = "Torque";
static const char acName6002_05[] = "Temperature";
static const char acName6002_06[] = "Error";
static const char acName6002_07[] = "enabled";
static const char acName6002_08[] = "counter";
static const char acName6002_09[] = "timestamp";
static const char acName6003[] = "md80_1_DataReturn";
static const char acName6003_00[] = "Max SubIndex";
static const char acName6003_01[] = "Mode";
static const char acName6003_02[] = "Position";
static const char acName6003_03[] = "Velocity";
static const char acName6003_04[] = "Torque";
static const char acName6003_05[] = "Temperature";
static const char acName6003_06[] = "Error";
static const char acName6003_07[] = "enabled";
static const char acName6003_08[] = "counter";
static const char acName6003_09[] = "timestamp";
static const char acName6004[] = "md80_2_DataReturn";
static const char acName6004_00[] = "Max SubIndex";
static const char acName6004_01[] = "Mode";
static const char acName6004_02[] = "Position";
static const char acName6004_03[] = "Velocity";
static const char acName6004_04[] = "Torque";
static const char acName6004_05[] = "Temperature";
static const char acName6004_06[] = "Error";
static const char acName6004_07[] = "enabled";
static const char acName6004_08[] = "counter";
static const char acName6004_09[] = "timestamp";
static const char acName6005[] = "md80_3_DataReturn";
static const char acName6005_00[] = "Max SubIndex";
static const char acName6005_01[] = "Mode";
static const char acName6005_02[] = "Position";
static const char acName6005_03[] = "Velocity";
static const char acName6005_04[] = "Torque";
static const char acName6005_05[] = "Temperature";
static const char acName6005_06[] = "Error";
static const char acName6005_07[] = "enabled";
static const char acName6005_08[] = "counter";
static const char acName6005_09[] = "timestamp";
static const char acName6006[] = "md80_Last_Command_Received";
static const char acName6006_00[] = "Max SubIndex";
static const char acName6006_01[] = "command";
static const char acName6006_02[] = "counter";
static const char acName6006_03[] = "timestamp";
static const char acName7000[] = "md80_Command";
static const char acName7000_00[] = "Max SubIndex";
static const char acName7000_01[] = "md80_dev_no";
static const char acName7000_02[] = "command";
static const char acName7000_03[] = "size";
static const char acName7000_04[] = "counter";
static const char acName7000_05[] = "dataSet0";
static const char acName7000_06[] = "dataSet1";
static const char acName7000_07[] = "dataSet2";
static const char acName7000_08[] = "dataSet3";
static const char acName7000_09[] = "dataSet4";
static const char acName7001[] = "md80_0_DataControl";
static const char acName7001_00[] = "Max SubIndex";
static const char acName7001_01[] = "Position";
static const char acName7001_02[] = "Velocity";
static const char acName7001_03[] = "Torque";
static const char acName7001_04[] = "Mode";
static const char acName7001_05[] = "enable";
static const char acName7001_06[] = "counter";
static const char acName7002[] = "md80_1_DataControl";
static const char acName7002_00[] = "Max SubIndex";
static const char acName7002_01[] = "Position";
static const char acName7002_02[] = "Velocity";
static const char acName7002_03[] = "Torque";
static const char acName7002_04[] = "Mode";
static const char acName7002_05[] = "enable";
static const char acName7002_06[] = "counter";
static const char acName7003[] = "md80_2_DataControl";
static const char acName7003_00[] = "Max SubIndex";
static const char acName7003_01[] = "Position";
static const char acName7003_02[] = "Velocity";
static const char acName7003_03[] = "Torque";
static const char acName7003_04[] = "Mode";
static const char acName7003_05[] = "enable";
static const char acName7003_06[] = "counter";
static const char acName7004[] = "md80_3_DataControl";
static const char acName7004_00[] = "Max SubIndex";
static const char acName7004_01[] = "Position";
static const char acName7004_02[] = "Velocity";
static const char acName7004_03[] = "Torque";
static const char acName7004_04[] = "Mode";
static const char acName7004_05[] = "enable";
static const char acName7004_06[] = "counter";

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
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1600_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_01, 0x70000108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_02, 0x70000208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_03, 0x70000308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_04, 0x70000420, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_05, 0x70000520, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_06, 0x70000620, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_07, 0x70000720, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_08, 0x70000820, NULL},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1600_09, 0x70000920, NULL},
};
const _objd SDO1601[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1601_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_01, 0x70010110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_02, 0x70010210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_03, 0x70010310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_04, 0x70010408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_05, 0x70010508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1601_06, 0x70010620, NULL},
};
const _objd SDO1602[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1602_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_01, 0x70020110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_02, 0x70020210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_03, 0x70020310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_04, 0x70020408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_05, 0x70020508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1602_06, 0x70020620, NULL},
};
const _objd SDO1603[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1603_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_01, 0x70030110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_02, 0x70030210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_03, 0x70030310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_04, 0x70030408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_05, 0x70030508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1603_06, 0x70030620, NULL},
};
const _objd SDO1604[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1604_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1604_01, 0x70040110, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1604_02, 0x70040210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1604_03, 0x70040310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1604_04, 0x70040408, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1604_05, 0x70040508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1604_06, 0x70040620, NULL},
};
const _objd SDO1A00[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A00_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_01, 0x60000120, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_02, 0x60000220, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_03, 0x60000320, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_04, 0x60000420, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_05, 0x60000520, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A00_06, 0x60000620, NULL},
};
const _objd SDO1A01[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A01_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_01, 0x60010108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_02, 0x60010208, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_03, 0x60010308, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_04, 0x60010420, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_05, 0x60010520, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_06, 0x60010620, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_07, 0x60010720, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_08, 0x60010820, NULL},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A01_09, 0x60010920, NULL},
};
const _objd SDO1A02[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A02_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_01, 0x60020108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_02, 0x60020210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_03, 0x60020310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_04, 0x60020410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_05, 0x60020508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_06, 0x60020608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_07, 0x60020708, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_08, 0x60020820, NULL},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A02_09, 0x60020920, NULL},
};
const _objd SDO1A03[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A03_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_01, 0x60030108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_02, 0x60030210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_03, 0x60030310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_04, 0x60030410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_05, 0x60030508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_06, 0x60030608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_07, 0x60030708, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_08, 0x60030820, NULL},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A03_09, 0x60030920, NULL},
};
const _objd SDO1A04[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A04_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_01, 0x60040108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_02, 0x60040210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_03, 0x60040310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_04, 0x60040410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_05, 0x60040508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_06, 0x60040608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_07, 0x60040708, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_08, 0x60040820, NULL},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A04_09, 0x60040920, NULL},
};
const _objd SDO1A05[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A05_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_01, 0x60050108, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_02, 0x60050210, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_03, 0x60050310, NULL},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_04, 0x60050410, NULL},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_05, 0x60050508, NULL},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_06, 0x60050608, NULL},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_07, 0x60050708, NULL},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_08, 0x60050820, NULL},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A05_09, 0x60050920, NULL},
};
const _objd SDO1A06[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1A06_00, 3, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A06_01, 0x60060120, NULL},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A06_02, 0x60060220, NULL},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName1A06_03, 0x60060320, NULL},
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
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C12_00, 5, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_01, 0x1600, NULL},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_02, 0x1601, NULL},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_03, 0x1602, NULL},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_04, 0x1603, NULL},
  {0x05, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C12_05, 0x1604, NULL},
};
const _objd SDO1C13[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName1C13_00, 7, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_01, 0x1A00, NULL},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_02, 0x1A01, NULL},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_03, 0x1A02, NULL},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_04, 0x1A03, NULL},
  {0x05, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_05, 0x1A04, NULL},
  {0x06, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_06, 0x1A05, NULL},
  {0x07, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName1C13_07, 0x1A06, NULL},
};
const _objd SDO6000[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6000_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_01, 0, &Obj.md80_addrs[0]},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_02, 0, &Obj.md80_addrs[1]},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_03, 0, &Obj.md80_addrs[2]},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_04, 0, &Obj.md80_addrs[3]},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_05, 0, &Obj.md80_addrs[4]},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO | ATYPE_TXPDO, acName6000_06, 0, &Obj.md80_addrs[5]},
};
const _objd SDO6001[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_01, 0, &Obj.md80_Respond.md80_dev_no},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_02, 0, &Obj.md80_Respond.command},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6001_03, 0, &Obj.md80_Respond.size},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6001_04, 0, &Obj.md80_Respond.timestamp},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6001_05, 0, &Obj.md80_Respond.dataRet0},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6001_06, 0, &Obj.md80_Respond.dataRet1},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6001_07, 0, &Obj.md80_Respond.dataRet2},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6001_08, 0, &Obj.md80_Respond.dataRet3},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6001_09, 0, &Obj.md80_Respond.dataRet4},
};
const _objd SDO6002[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_01, 0, &Obj.md80_0_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6002_02, 0, &Obj.md80_0_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6002_03, 0, &Obj.md80_0_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6002_04, 0, &Obj.md80_0_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_05, 0, &Obj.md80_0_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_06, 0, &Obj.md80_0_DataReturn.Error},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6002_07, 0, &Obj.md80_0_DataReturn.enabled},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6002_08, 0, &Obj.md80_0_DataReturn.counter},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6002_09, 0, &Obj.md80_0_DataReturn.timestamp},
};
const _objd SDO6003[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_01, 0, &Obj.md80_1_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6003_02, 0, &Obj.md80_1_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6003_03, 0, &Obj.md80_1_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6003_04, 0, &Obj.md80_1_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_05, 0, &Obj.md80_1_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_06, 0, &Obj.md80_1_DataReturn.Error},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6003_07, 0, &Obj.md80_1_DataReturn.enabled},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6003_08, 0, &Obj.md80_1_DataReturn.counter},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6003_09, 0, &Obj.md80_1_DataReturn.timestamp},
};
const _objd SDO6004[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6004_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6004_01, 0, &Obj.md80_2_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6004_02, 0, &Obj.md80_2_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6004_03, 0, &Obj.md80_2_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6004_04, 0, &Obj.md80_2_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6004_05, 0, &Obj.md80_2_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RW, acName6004_06, 0, &Obj.md80_2_DataReturn.Error},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6004_07, 0, &Obj.md80_2_DataReturn.enabled},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6004_08, 0, &Obj.md80_2_DataReturn.counter},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6004_09, 0, &Obj.md80_2_DataReturn.timestamp},
};
const _objd SDO6005[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6005_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6005_01, 0, &Obj.md80_3_DataReturn.Mode},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6005_02, 0, &Obj.md80_3_DataReturn.Position},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6005_03, 0, &Obj.md80_3_DataReturn.Velocity},
  {0x04, DTYPE_UNSIGNED16, 16, ATYPE_RO, acName6005_04, 0, &Obj.md80_3_DataReturn.Torque},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6005_05, 0, &Obj.md80_3_DataReturn.Temperature},
  {0x06, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6005_06, 0, &Obj.md80_3_DataReturn.Error},
  {0x07, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6005_07, 0, &Obj.md80_3_DataReturn.enabled},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6005_08, 0, &Obj.md80_3_DataReturn.counter},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6005_09, 0, &Obj.md80_3_DataReturn.timestamp},
};
const _objd SDO6006[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName6006_00, 3, NULL},
  {0x01, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6006_01, 0, &Obj.md80_Last_Command_Received.command},
  {0x02, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6006_02, 0, &Obj.md80_Last_Command_Received.counter},
  {0x03, DTYPE_UNSIGNED32, 32, ATYPE_RO, acName6006_03, 0, &Obj.md80_Last_Command_Received.timestamp},
};
const _objd SDO7000[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7000_00, 9, NULL},
  {0x01, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7000_01, 0, &Obj.md80_Command.md80_dev_no},
  {0x02, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7000_02, 0, &Obj.md80_Command.command},
  {0x03, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7000_03, 0, &Obj.md80_Command.size},
  {0x04, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7000_04, 0, &Obj.md80_Command.counter},
  {0x05, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7000_05, 0, &Obj.md80_Command.dataSet0},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7000_06, 0, &Obj.md80_Command.dataSet1},
  {0x07, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7000_07, 0, &Obj.md80_Command.dataSet2},
  {0x08, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7000_08, 0, &Obj.md80_Command.dataSet3},
  {0x09, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7000_09, 0, &Obj.md80_Command.dataSet4},
};
const _objd SDO7001[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7001_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7001_01, 0, &Obj.md80_0_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7001_02, 0, &Obj.md80_0_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7001_03, 0, &Obj.md80_0_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7001_04, 0, &Obj.md80_0_DataControl.Mode},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7001_05, 0, &Obj.md80_0_DataControl.enable},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7001_06, 0, &Obj.md80_0_DataControl.counter},
};
const _objd SDO7002[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7002_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7002_01, 0, &Obj.md80_1_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7002_02, 0, &Obj.md80_1_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7002_03, 0, &Obj.md80_1_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7002_04, 0, &Obj.md80_1_DataControl.Mode},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7002_05, 0, &Obj.md80_1_DataControl.enable},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7002_06, 0, &Obj.md80_1_DataControl.counter},
};
const _objd SDO7003[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7003_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7003_01, 0, &Obj.md80_2_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7003_02, 0, &Obj.md80_2_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7003_03, 0, &Obj.md80_2_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7003_04, 0, &Obj.md80_2_DataControl.Mode},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7003_05, 0, &Obj.md80_2_DataControl.enable},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7003_06, 0, &Obj.md80_2_DataControl.counter},
};
const _objd SDO7004[] =
{
  {0x00, DTYPE_UNSIGNED8, 8, ATYPE_RO, acName7004_00, 6, NULL},
  {0x01, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7004_01, 0, &Obj.md80_3_DataControl.Position},
  {0x02, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7004_02, 0, &Obj.md80_3_DataControl.Velocity},
  {0x03, DTYPE_UNSIGNED16, 16, ATYPE_WO, acName7004_03, 0, &Obj.md80_3_DataControl.Torque},
  {0x04, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7004_04, 0, &Obj.md80_3_DataControl.Mode},
  {0x05, DTYPE_UNSIGNED8, 8, ATYPE_WO, acName7004_05, 0, &Obj.md80_3_DataControl.enable},
  {0x06, DTYPE_UNSIGNED32, 32, ATYPE_WO, acName7004_06, 0, &Obj.md80_3_DataControl.counter},
};

const _objectlist SDOobjects[] =
{
  {0x1000, OTYPE_VAR, 0, 0, acName1000, SDO1000},
  {0x1008, OTYPE_VAR, 0, 0, acName1008, SDO1008},
  {0x1009, OTYPE_VAR, 0, 0, acName1009, SDO1009},
  {0x100A, OTYPE_VAR, 0, 0, acName100A, SDO100A},
  {0x1018, OTYPE_RECORD, 4, 0, acName1018, SDO1018},
  {0x1600, OTYPE_RECORD, 9, 0, acName1600, SDO1600},
  {0x1601, OTYPE_RECORD, 6, 0, acName1601, SDO1601},
  {0x1602, OTYPE_RECORD, 6, 0, acName1602, SDO1602},
  {0x1603, OTYPE_RECORD, 6, 0, acName1603, SDO1603},
  {0x1604, OTYPE_RECORD, 6, 0, acName1604, SDO1604},
  {0x1A00, OTYPE_RECORD, 6, 0, acName1A00, SDO1A00},
  {0x1A01, OTYPE_RECORD, 9, 0, acName1A01, SDO1A01},
  {0x1A02, OTYPE_RECORD, 9, 0, acName1A02, SDO1A02},
  {0x1A03, OTYPE_RECORD, 9, 0, acName1A03, SDO1A03},
  {0x1A04, OTYPE_RECORD, 9, 0, acName1A04, SDO1A04},
  {0x1A05, OTYPE_RECORD, 9, 0, acName1A05, SDO1A05},
  {0x1A06, OTYPE_RECORD, 3, 0, acName1A06, SDO1A06},
  {0x1C00, OTYPE_ARRAY, 4, 0, acName1C00, SDO1C00},
  {0x1C12, OTYPE_ARRAY, 5, 0, acName1C12, SDO1C12},
  {0x1C13, OTYPE_ARRAY, 7, 0, acName1C13, SDO1C13},
  {0x6000, OTYPE_ARRAY, 6, 0, acName6000, SDO6000},
  {0x6001, OTYPE_RECORD, 9, 0, acName6001, SDO6001},
  {0x6002, OTYPE_RECORD, 9, 0, acName6002, SDO6002},
  {0x6003, OTYPE_RECORD, 9, 0, acName6003, SDO6003},
  {0x6004, OTYPE_RECORD, 9, 0, acName6004, SDO6004},
  {0x6005, OTYPE_RECORD, 9, 0, acName6005, SDO6005},
  {0x6006, OTYPE_RECORD, 3, 0, acName6006, SDO6006},
  {0x7000, OTYPE_RECORD, 9, 0, acName7000, SDO7000},
  {0x7001, OTYPE_RECORD, 6, 0, acName7001, SDO7001},
  {0x7002, OTYPE_RECORD, 6, 0, acName7002, SDO7002},
  {0x7003, OTYPE_RECORD, 6, 0, acName7003, SDO7003},
  {0x7004, OTYPE_RECORD, 6, 0, acName7004, SDO7004},
  {0xffff, 0xff, 0xff, 0xff, NULL, NULL}
};
