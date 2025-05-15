#include "../MODBUS_RTU_Slave.h"

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
void _Boolean::Communication::MODBUS_RTU_Slave::InitializeContiguousRegistersGroup(MODBUS_RTU_ContiguousRegistersGroup* group, const void* registerArray,const ContiguousRegistersLength registerArrayLength, const MODBUS_RTU_Register_Attribute* attributeArray, const MODBUS_RTU_RegisterAddress firstRegisterAddress
#else
void MODBUS_RTU_Slave_InitializeContiguousRegistersGroup(MODBUS_RTU_ContiguousRegistersGroup* group, const void* registerArray,const ContiguousRegistersLength registerArrayLength, const MODBUS_RTU_Register_Attribute* attributeArray, const MODBUS_RTU_RegisterAddress firstRegisterAddress
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                                               , const MODBUS_RTU_WriteTimeCounter writeTimes
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
