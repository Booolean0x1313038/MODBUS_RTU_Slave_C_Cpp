#include "../MODBUS_RTU_Slave.h"

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
_Boolean::Communication::MODBUS_RTU_Slave::MODBUS_RTU_Slave(MODBUS_RTU_DeviceAddress deviceAddress, MODBUS_RTU_ContiguousRegistersGroup *coiledRegisters, ContiguousRegistersLength coiledRegistersAmount)

#else
void MODBUS_RTU_Slave_Initialize(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR, MODBUS_RTU_DeviceAddress deviceAddress, MODBUS_RTU_ContiguousRegistersGroup *coiledRegisters, ContiguousRegistersLength coiledRegistersAmount)
#endif
{
    MODBUS_RTU_SLAVE_THISPTR->_DeviceAddress = deviceAddress;
    MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray = coiledRegisters;
    MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArrayLength = coiledRegistersAmount;
    MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength = 0;
    MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_UNKNOWN;
    MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex = -1;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
    MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrameLength = 0;
    MODBUS_RTU_SLAVE_THISPTR->_Counter_RecvDataIsSame = 0;
#endif
}
