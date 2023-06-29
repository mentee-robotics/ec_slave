# THE INSTRUCTION USING OF ETHERCAT SLAVE 

## 1. Revision
| Revision | Date | Description |
| --------------- | --------------- | --------------- |
| 0.0.1 | 06/28/2023 | Initialize instruction |
## 2. Purpose 
TBD

## 3. Glossary
TBD

## 4. Object dictionary
Object dictionary can be accessed using PDOs. These are three main groups in which the address space is divided into: <br>
1. Communication Area
2. Md80 address list 
3. Md80 control

### 4.1. Communication area 
0x7000 - md80_Command
| Index | Subindex | Name | Data Type | SDO | PDO | Range | Default | Units |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0x7000 | 0x01 | md80_dev_no | UINT8 | - | WO | 0 - 3 | 0 | - |
| 0x7000 | 0x02 | command | UINT8 | - | WO | - | 0 | - |
| 0x7000 | 0x03 | size | UINT8 | - | WO | 0 - 23 | 0 | - |
| 0x7000 | 0x04 | timestamp | UINT32 | - | WO | - | 0 | - |
| 0x7000 | 0x05 | dataSet0 | UINT32 | - | WO | - | 0 | - |
| 0x7000 | 0x06 | dataSet1 | UINT32 | - | WO | - | 0 | - |
| 0x7000 | 0x07 | dataSet2 | UINT32 | - | WO | - | 0 | - |
| 0x7000 | 0x08 | dataSet3 | UINT32 | - | WO | - | 0 | - |
| 0x7000 | 0x09 | dataSet4 | UINT32 | - | WO | - | 0 | - |

The command table: <br>
| Value | Command | Note |
| --- | --- | --- |
| 0 | None | - |
| 1 | BUS_FRAME_PING_START | Scan the md80s |
| 2 | BUS_FRAME_CANDLE_CONFIG_BAUDRATE | Not support |
| 3 | BUS_FRAME_MD80_ADD | Add the motor id. |
| 4 | BUS_FRAME_MD80_GENERIC_FRAME | - |
| 5 | BUS_FRAME_MD80_CONFIG_CAN | Not support |
| 6 | BUS_FRAME_BEGIN | Start control motor |
| 7 | BUS_FRAME_END | Stop control motor |
| 8 | BUS_FRAME_UPDATE | Not support |
| 9 | BUS_FRAME_RESET | Not support |

0x60001 - md80_Respond
| Index  | Subindex  | Name         | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---          | ---       | --- | --- | ---    | ---       | ---    |
| 0x6001 | 0x01      | md80_dev_no  | UINT8     | -   | RO  | 0 - 3  | 0         | -      |
| 0x6001 | 0x02      | command      | UINT8     | -   | RO  | -      | 0         | -      |
| 0x6001 | 0x03      | size         | UINT8     | -   | RO  | 0 - 23 | 0         | -      |
| 0x6001 | 0x04      | timestamp    | UINT32    | -   | RO  | -      | 0         | -      |
| 0x6001 | 0x05      | dataSet0     | UINT32    | -   | RO  | -      | 0         | -      |
| 0x6001 | 0x06      | dataSet1     | UINT32    | -   | RO  | -      | 0         | -      |
| 0x6001 | 0x07      | dataSet2     | UINT32    | -   | RO  | -      | 0         | -      |
| 0x6001 | 0x08      | dataSet3     | UINT32    | -   | RO  | -      | 0         | -      |
| 0x6001 | 0x09      | dataSet4     | UINT32    | -   | RO  | -      | 0         | -      |

### 4.2. Md80 address
0x6000 - md80_addrs: The list address of motor detected after request scan.
| Index  | Subindex  | Name   | Data Type | SDO | PDO | Range     | Default   | Units  |
| ---    | ---       | ---    | ---       | --- | --- | ---       | ---       | ---    |
| 0x6001 | 0x01      | md80_0 | UINT32    | -   | RO  | 10 - 2000 | 0         | -      |
| 0x6001 | 0x02      | md80_1 | UINT32    | -   | RO  | 10 - 2000 | 0         | -      |
| 0x6001 | 0x03      | md80_2 | UINT32    | -   | RO  | 10 - 2000 | 0         | -      |
| 0x6001 | 0x04      | md80_3 | UINT32    | -   | RO  | 10 - 2000 | 0         | -      |

### 4.3. MD80 Control
0x7001 - md80_0_DataControl: TBD
| Index  | Subindex  | Name      | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---       | ---       | --- | --- | ---    | ---       | ---    |
| 0x7001 | 0x01      | Position  | UINT8     | -   | WO  | 0 - 628| 0         | read   |
| 0x7001 | 0x02      | Velocity  | UINT8     | -   | WO  | -      | 0         | rad/s  |
| 0x7001 | 0x03      | Torque    | UINT8     | -   | WO  | TBD    | 0         | Nm     |
| 0x7001 | 0x04      | Mode      | UINT32    | -   | WO  | TBD    | 0         | -      |
| 0x7001 | 0x05      | enable    | UINT32    | -   | WO  | 0 - 1  | 0         | bool   |

0x7002 - md80_1_DataControl: TBD
| Index  | Subindex  | Name      | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---       | ---       | --- | --- | ---    | ---       | ---    |
| 0x7002 | 0x01      | Position  | UINT8     | -   | WO  | 0 - 628| 0         | read   |
| 0x7002 | 0x02      | Velocity  | UINT8     | -   | WO  | -      | 0         | rad/s  |
| 0x7002 | 0x03      | Torque    | UINT8     | -   | WO  | TBD    | 0         | Nm     |
| 0x7002 | 0x04      | Mode      | UINT32    | -   | WO  | TBD    | 0         | -      |
| 0x7002 | 0x05      | enable    | UINT32    | -   | WO  | 0 - 1  | 0         | bool   |

0x7003 - md80_2_DataControl: TBD
| Index  | Subindex  | Name      | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---       | ---       | --- | --- | ---    | ---       | ---    |
| 0x7003 | 0x01      | Position  | UINT8     | -   | WO  | 0 - 628| 0         | read   |
| 0x7003 | 0x02      | Velocity  | UINT8     | -   | WO  | -      | 0         | rad/s  |
| 0x7003 | 0x03      | Torque    | UINT8     | -   | WO  | TBD    | 0         | Nm     |
| 0x7003 | 0x04      | Mode      | UINT32    | -   | WO  | TBD    | 0         | -      |
| 0x7003 | 0x05      | enable    | UINT32    | -   | WO  | 0 - 1  | 0         | bool   |

0x7004 - md80_3_DataControl: TBD
| Index  | Subindex  | Name      | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---       | ---       | --- | --- | ---    | ---       | ---    |
| 0x7004 | 0x01      | Position  | UINT8     | -   | WO  | 0 - 628| 0         | 0.01 rad/bit |
| 0x7004 | 0x02      | Velocity  | UINT8     | -   | WO  | -      | 0         | rad/s  |
| 0x7004 | 0x03      | Torque    | UINT8     | -   | WO  | TBD    | 0         | Nm     |
| 0x7004 | 0x04      | Mode      | UINT32    | -   | WO  | TBD    | 0         | -      |
| 0x7004 | 0x05      | enable    | BOOLEAN   | -   | WO  | 0 - 1  | 0         | bool   |

0x6002 - md80_0_DataReturn: TBD
| Index  | Subindex  | Name         | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---          | ---       | --- | --- | ---    | ---       | ---    |
| 0x6002 | 0x01      | Mode         | UINT8     | -   | RO  | TBD    | 0         | read   |
| 0x6002 | 0x02      | Position     | UINT16    | -   | RO  | -      | 0         | rad/s  |
| 0x6002 | 0x03      | Velocity     | UINT16    | -   | RO  | TBD    | 0         | Nm     |
| 0x6002 | 0x04      | Torque       | UINT16    | -   | RO  | TBD    | 0         | -      |
| 0x6002 | 0x05      | Temperature  | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6002 | 0x06      | Error        | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6002 | 0x07      | enabled      | BOOLEAN   | -   | RO  | TBD    | 0         | TBD    |

0x6003 - md80_1_DataReturn: TBD
| Index  | Subindex  | Name         | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---          | ---       | --- | --- | ---    | ---       | ---    |
| 0x6003 | 0x01      | Mode         | UINT8     | -   | RO  | TBD    | 0         | read   |
| 0x6003 | 0x02      | Position     | UINT16    | -   | RO  | -      | 0         | rad/s  |
| 0x6003 | 0x03      | Velocity     | UINT16    | -   | RO  | TBD    | 0         | Nm     |
| 0x6003 | 0x04      | Torque       | UINT16    | -   | RO  | TBD    | 0         | -      |
| 0x6003 | 0x05      | Temperature  | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6003 | 0x06      | Error        | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6003 | 0x07      | enabled      | BOOLEAN   | -   | RO  | TBD    | 0         | TBD    |

0x6004 - md80_2_DataReturn: TBD
| Index  | Subindex  | Name         | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---          | ---       | --- | --- | ---    | ---       | ---    |
| 0x6004 | 0x01      | Mode         | UINT8     | -   | RO  | TBD    | 0         | read   |
| 0x6004 | 0x02      | Position     | UINT16    | -   | RO  | -      | 0         | rad/s  |
| 0x6004 | 0x03      | Velocity     | UINT16    | -   | RO  | TBD    | 0         | Nm     |
| 0x6004 | 0x04      | Torque       | UINT16    | -   | RO  | TBD    | 0         | -      |
| 0x6004 | 0x05      | Temperature  | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6004 | 0x06      | Error        | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6004 | 0x07      | enabled      | BOOLEAN   | -   | RO  | TBD    | 0         | TBD    |

0x6005 - md80_3_DataReturn: TBD
| Index  | Subindex  | Name         | Data Type | SDO | PDO | Range  | Default   | Units  |
| ---    | ---       | ---          | ---       | --- | --- | ---    | ---       | ---    |
| 0x6005 | 0x01      | Mode         | UINT8     | -   | RO  | TBD    | 0         | read   |
| 0x6005 | 0x02      | Position     | UINT16    | -   | RO  | -      | 0         | rad/s  |
| 0x6005 | 0x03      | Velocity     | UINT16    | -   | RO  | TBD    | 0         | Nm     |
| 0x6005 | 0x04      | Torque       | UINT16    | -   | RO  | TBD    | 0         | -      |
| 0x6005 | 0x05      | Temperature  | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6005 | 0x06      | Error        | UINT8     | -   | RO  | TBD    | 0         | TBD    |
| 0x6005 | 0x07      | enabled      | BOOLEAN   | -   | RO  | TBD    | 0         | TBD    |

## 5. Sequence initialize slave.
TBD

## 6. Sample
TBD