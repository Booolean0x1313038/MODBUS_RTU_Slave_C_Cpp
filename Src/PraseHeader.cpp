#include "../MODBUS_RTU_Slave.h"

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
bool _Boolean::Communication::MODBUS_RTU_Slave::PraseHeader(const uint8_t *dataPtr, const uint8_t length)
#else
bool MODBUS_RTU_Slave_PraseHeader(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR, const uint8_t *dataPtr, const uint8_t length)
#endif
{
    int index;
    // 检查数据长度, MODBUS 帧长度至少为 8, 除去 CRC 则至少为 6, 头长度为 4 或 6
    if (length < 6)
    {
        MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_DATA_LENGTH_IS_INVALID;
    }
    else
    {
        // 格式化输入数据
        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_DeviceAddress = *dataPtr;
        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_FunctionCode = (MODBUS_RTU_FunctionCodes) * (dataPtr + 1);
#ifdef MODBUS_RTU_SLAVE_BIG_ENDIAN
        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RegisterAddress = *(uint16_t *)(dataPtr + 2);
        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount = *(uint16_t *)(dataPtr + 4);
#else
        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RegisterAddress = _ToBigEndian(*(uint16_t *)(dataPtr + 2));
        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount = _ToBigEndian(*(uint16_t *)(dataPtr + 4));
#endif
        // 检查报文中的设备地址
        if (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_DeviceAddress != MODBUS_RTU_SLAVE_THISPTR->_DeviceAddress && MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_DeviceAddress != MODBUS_RTU_DEVICE_ADDRESS_BROADCAST)
        {
            MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_NOT_THIS_DEVICE;
        }
        else
        {
            // Summary
            // Definition:
            //     TargetRegOffset = RegisterAddress - AreaFirstAddress
            //     RequestAmount   = MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount
            // Description:
            //     Code                 功能码
            //     Name                 功能码名称
            //     R/W                  操作类型 读 / 写
            //     Amount               操作类型 单个 / 多个
            //     Size                 操作类型 目标寄存器的类型
            //     TargetRegOffsetAddr  要操作的第一个寄存器相对于寄存器数组首个寄存器的偏移地址
            //     TargetRegsBytes      要操作的字节数
            //     BitOffset            操作线圈时的位偏移
            //     DataBytesIn_Frame    读取或写入时数据区的字节数
            //     RequestFrameBytes    请求帧的期望长度
            //     ReturnFrameBytes     返回帧的长度
            //  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            // | Code | Name                             | R/W |  Amount  |  Size  | TargetRegOffsetAddr |  TargetRegsBytes  |      BitOffset      |      DataBytesIn_Frame      |   RequestFrameBytes   |      ReturnFrameBytes       |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // |      |                                  |     |          |        |                     | (RequestAmount +  |                     |                             |                       |                             |
            // | 0x01 | Read Multiple Coils              |  R  | Multiple |  1Bit  | TargetRegOffset / 8 | TargetRegOffset % | TargetRegOffset % 8 | (RequestAmount - 1) / 8 + 1 |           8           |    DataBytesIn_Frame + 7    |
            // |      |                                  |     |          |        |                     |  8 - 1) / 8 + 1   |                     |                             |                       |                             |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // |      |                                  |     |          |        |                     | (RequestAmount +  |                     |                             |                       |                             |
            // | 0x02 | Read Multiple Discrete Inputs    |  R  | Multiple |  1Bit  | TargetRegOffset / 8 | TargetRegOffset % | TargetRegOffset % 8 | (RequestAmount - 1) / 8 + 1 |           8           |    DataBytesIn_Frame + 7    |
            // |      |                                  |     |          |        |                     |  8 - 1) / 8 + 1   |                     |                             |                       |                             |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // | 0x03 | Read Multiple Holding Registers  |  R  | Multiple | 16Bits | TargetRegOffset * 2 | RequestAmount * 2 |          0          |      RequestAmount * 2      |           8           |    DataBytesIn_Frame + 5    |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // | 0x04 | Read Multiple Input Registers    |  R  | Multiple | 16Bits | TargetRegOffset * 2 | RequestAmount * 2 |          0          |      RequestAmount * 2      |           8           |    DataBytesIn_Frame + 5    |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // |      |                                  |     |          |        |                     | (RequestAmount +  |                     |                             |                       |                             |
            // | 0x05 | Write Single Coil                |  W  |  Single  |  1Bit  | TargetRegOffset / 8 | TargetRegOffset % | TargetRegOffset % 8 |              2              |           8           |              8              |
            // |      |                                  |     |          |        |                     |  8 - 1) / 8 + 1   |                     |                             |                       |                             |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // | 0x06 | Write Single Holding Registers   |  W  |  Single  | 16Bits | TargetRegOffset * 2 | RequestAmount * 2 |          0          |              2              |           8           |              8              |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // |      |                                  |     |          |        |                     | (RequestAmount +  |                     | Ceiling(Ceiling(            |                       |                             |
            // | 0x0F | Write Multiple Coils             |  W  | Multiple |  1Bit  | TargetRegOffset / 8 | TargetRegOffset % | TargetRegOffset % 8 | RequestAmount - 1) /        | DataBytesIn_Frame + 9 |              9              |
            // |      |                                  |     |          |        |                     |  8 - 1) / 8 + 1   |                     | 2) * 2                      |                       |                             |
            // |------|----------------------------------|-----|----------|--------|---------------------|-------------------|---------------------|-----------------------------|-----------------------|-----------------------------|
            // | 0x10 | Write Multiple Holding Registers |  W  | Multiple | 16Bits | TargetRegOffset * 2 | RequestAmount * 2 |          0          |      RequestAmount * 2      | DataBytesIn_Frame + 9 |              8              |
            //  -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// 检查报文中的功能码是否受支持
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
            uint8_t funcCodeAttribute = _GetFunctionCodeAttribute(MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_FunctionCode);
#else
            uint8_t funcCodeAttribute = _MODBUS_RTU_Slave_GetFunctionCodeAttribute(MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_FunctionCode);
#endif
            if (funcCodeAttribute == 0)
            {
                MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_FUNCTIONCODE_IS_NOT_SUPPORT;
            }
            else
            {
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits = (funcCodeAttribute & MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_16BITS);
#endif

                // 检查寄存器地址
                MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex = -1;
                for (index = 0; index < MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArrayLength; index++) // 遍历所有分区
                {

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                    if (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RegisterAddress >= MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].FirstRegisterAddress &&
                        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RegisterAddress < MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].FirstRegisterAddress +
                                                                           ((MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].RegisterType == MODBUS_RTU_REGISTER_TYPE_UINT16) ?
                                                                                (MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].Bytes >> 1) :
                                                                                (MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].Bytes << 3)))
#else
                    if (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RegisterAddress >= MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].FirstRegisterAddress &&
                        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RegisterAddress < MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].FirstRegisterAddress + (MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[index].Bytes >> 1))
#endif

                    {
                        // 起始地址命中第 index 个分区
                        MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex = index;
                        break;
                    }
                }
                if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex == -1) // 没有命中任何分区
                {
                    MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_REGISTER_NOT_EXIST;
                }
                else
                {
                    MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Write = (funcCodeAttribute & MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_WRITE);
                    MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple = (funcCodeAttribute & MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_MULTIPLE);
                    int TargetRegOffset = MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RegisterAddress - MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex].FirstRegisterAddress;

                    if (!MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple)
                    {
                        MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount = 1;
                    }
                    // 检查寄存器区的类型是否可以执行操作
                    // 计算读取或写入的数据的长度
                    // 计算目标寄存器的偏移地址
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                    if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits != (MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex].RegisterType == MODBUS_RTU_REGISTER_TYPE_UINT16))
                    {
                        MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_REGISTER_TYPE_IS_INCORRECT;
                    }
                    else
                    {

                        MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes =MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits ? (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount << 1) : (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount + TargetRegOffset - ((TargetRegOffset >> 3) << 3) - 1) / 8 + 1;
                        MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersMemoryAddressOffset =MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits ? (TargetRegOffset << 1) : (TargetRegOffset >> 3);
#else
                    {
                        MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes =(MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount << 1);
                        MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersMemoryAddressOffset =(TargetRegOffset << 1);
#endif

                        // 检查目标寄存器范围是否超出本机寄存器范围
                        if (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount == 0 ||
                            (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersMemoryAddressOffset + MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes) > MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex].Bytes)
                        {
                            MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_REGISTER_OUT_OF_RANGE;
                        }
                        else
                        {
                            // 检查 Operation
                            uint8_t registersAttribute = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Write ? MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE : MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE;
                            MODBUS_RTU_ResultCode failedResultCode = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Write ? MODBUS_RTU_RESULT_REGISTER_NOT_WRITEABLE : MODBUS_RTU_RESULT_REGISTER_NOT_READABLE;

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                            uint16_t rangeStart = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersMemoryAddressOffset >> (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits ? 1 : 0);
                            uint16_t rangeEnd = rangeStart + (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes >> (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits ? 1 : 0));
#else
                            uint16_t rangeStart = TargetRegOffset;
                            uint16_t rangeEnd = rangeStart + MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount;
#endif
                            bool wrongAttribute = false;
                            for (index = rangeStart; index < rangeEnd; ++index)
                            {
                                if ((MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex].RegistersAttribute[index] & registersAttribute) == 0)
                                {
                                    wrongAttribute = true;
                                    break;
                                }
                            }
                            if (wrongAttribute)
                            {
                                MODBUS_RTU_SLAVE_THISPTR->_LastResult = failedResultCode;
                            }
                            else
                            {
                                // 计算位偏移
                                // 计算读取或写入时请求帧或返回帧中数据的字节数
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits ? 0 : TargetRegOffset % 8;
                                uint16_t DataBytesIn_Frame =
                                    MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple ?
                                        (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits ?
                                             (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount << 1) :
                                             ((MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount - 1) >> 3) + 1) :
                                        2;
#else
                                uint16_t DataBytesIn_Frame = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple ? MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes: 2;
#endif


                                // 计算预期的请求帧长度
                                MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Write && MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple ? DataBytesIn_Frame + 9 : 8;
                                // 计算返回帧的长度
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL

                                MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Write ?
                                                                 ((MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple & MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits) ? 8 : 9)
                                                                                                                   : DataBytesIn_Frame + (DataBytesIn_Frame & 1) + (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits ? 5 : 7);
#else

                                MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Write ? (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple ? 8 : 9) : DataBytesIn_Frame + 5;
#endif

                                MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_OK;
                            }
                        }
                    }
                }
            }
        }
    }
    if (MODBUS_RTU_SLAVE_THISPTR->_LastResult != MODBUS_RTU_RESULT_OK)
    {
        MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex = -1;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
        _GenerateExceptionFrame();
#else
        _MODBUS_RTU_Slave_GenerateExceptionFrame(MODBUS_RTU_SLAVE_THISPTR);
#endif
        return false;
    }
    return true;
}
