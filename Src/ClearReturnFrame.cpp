#include "../MODBUS_RTU_Slave.h"

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
void _Boolean::Communication::MODBUS_RTU_Slave::ClearReturnFrame()
#else
void MODBUS_RTU_Slave_ClearReturnFrame(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR)
#endif
{
    MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[0] = 0;
    MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength = 0;
}
