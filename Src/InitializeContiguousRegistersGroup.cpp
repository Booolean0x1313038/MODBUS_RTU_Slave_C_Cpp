#include "../MODBUS_RTU_Slave.h"

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
/// @brief 执行操作
/// @param dataPtr 接收到的数据
/// @param length 数据长度
void _Boolean::Communication::MODBUS_RTU_Slave::InitializeContiguousRegistersGroup(MODBUS_RTU_ContiguousRegistersGroup* group, const void* registerArray,const uint16_t registerArrayLength, const uint8_t* attributeArray, const uint16_t firstRegisterAddress
#else
void MODBUS_RTU_Slave_InitializeContiguousRegistersGroup(MODBUS_RTU_ContiguousRegistersGroup* group, const void* registerArray,const uint16_t registerArrayLength, const uint8_t* attributeArray, const uint16_t firstRegisterAddress
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                                               , const uint8_t writeTimes
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                               , const MODBUS_RTU_Register_Type type
#endif
                                               )
{
    group->Registers = registerArray;
    group->RegistersAttribute = attributeArray;
    group->FirstRegisterAddress = firstRegisterAddress;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
    group->WriteTimes = writeTimes;
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
    group->RegisterType = type;
    group->Bytes = type == MODBUS_RTU_REGISTER_TYPE_BIT ? registerArrayLength : (registerArrayLength << 1);
#else
    group->Bytes = registerArrayLength << 1;
#endif
}
