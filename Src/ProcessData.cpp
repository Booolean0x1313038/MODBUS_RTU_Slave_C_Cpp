#include "../MODBUS_RTU_Slave.h"

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
bool _Boolean::Communication::MODBUS_RTU_Slave::ProcessData( const uint8_t *dataPtr, const uint16_t length)
#else
bool MODBUS_RTU_Slave_ProcessData(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR, const uint8_t *dataPtr, const uint16_t length)
#endif
{
    // 检查是否已经检查 Header
    if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex == -1)
    {
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
        PraseHeader(dataPtr, length);
#else
        MODBUS_RTU_Slave_PraseHeader(MODBUS_RTU_SLAVE_THISPTR, dataPtr, length);
#endif
    }
    if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex != -1)
    {
        // 检查数据长度
        if (length < MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes)
        {
            MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_DATA_LENGTH_IS_INVALID;
        }
        else
        {
            // 检查 CRC
            if ((*(dataPtr + MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes - 2) | ((uint16_t)*(dataPtr + MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes - 1) << 8)) !=
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                CalculateCRC(dataPtr, MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes - 2))
#else
                MODBUS_RTU_Slave_CalculateCRC(dataPtr, MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes - 2))
#endif
            {
                MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_CRC_WRONG;
            }
            else
            {

                uint8_t *registerPtr = (uint8_t *)MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex].Registers + MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersMemoryAddressOffset;
                if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Write) // 写入操作
                {
                    uint8_t bytes = 0;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                    bool writeRegister = MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex].WriteTimes <= 1; // 寄存器区域需要多次写入
                    if (!writeRegister)																			  // 发送相同数据的次数不足, 需要判断数据是否相同
                    {
                        uint16_t checkLength = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes - 4;
                        bool copyData = MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrameLength != checkLength;
                        if (!copyData) // 上次有数据且两次数据长度是一样的, 有可能两次数据是一样的
                        {
                            copyData = memcmp(MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrame, dataPtr + 2, MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrameLength) != 0; // 保存的是 [2 ~ -2)内的内存, 即功能码后面到 CRC 前面 (bubsok功能码和 CRC), 0 时内存是一样的
                            if (!copyData)																				 // 两次数据是一样的
                            {
                                MODBUS_RTU_SLAVE_THISPTR->_Counter_RecvDataIsSame++;
                            }
                        }
                        if (copyData) // 两次数据不一样, 保存这一次的
                        {
                            MODBUS_RTU_SLAVE_THISPTR->_Counter_RecvDataIsSame = 1;
                            MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrameLength = checkLength;
                            memcpy(MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrame, dataPtr + 2, MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrameLength);
                        }
                        writeRegister = MODBUS_RTU_SLAVE_THISPTR->_Counter_RecvDataIsSame == MODBUS_RTU_SLAVE_THISPTR->_ContiguousRegistersGroupArray[MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetCoiledRegistersAreaIndex].WriteTimes;
                    }
                    if (!writeRegister)
                    {
                        MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_WRITE_TIME_NOT_MATCH;
                    }
                    else
                    {
                        MODBUS_RTU_SLAVE_THISPTR->_LastRecvFrameLength = 0;
#endif

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                        if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits) // 写入 16 Bits
                        {
#endif
                            bytes = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes;
                            uint16_t *srcPtr = (uint16_t *)(dataPtr + (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple ? 7 : 4));
                            uint16_t *dstPtr = (uint16_t *)(registerPtr);
#ifdef MODBUS_RTU_SLAVE_BIG_ENDIAN
                            memcpy(dstPtr, srcPtr, MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes);
#else
                            for (int i = 0; i < MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount; ++i)
                            {
                                *(dstPtr++) = _ToBigEndian(*(srcPtr++));
                            }
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                        }
                        else if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple) // 写入多个 1 Bit
                        {
                            // Description:
                            // 预处理: SRC中可能有些位不需要写入, 要把这部分置零
                            // 处理前 8 - offset 位
                            //     dst[0] [0, offset)     位不变, [offset, 8) 位归零
                            //     dst[0] [offset, 8)     位存入  src[0] (0, 8 - offset) 位
                            // 处理后 offset 位
                            //     dst[1] [0, 8 - offset) 位归零, [8 - offset, 8) 位不变
                            //     dst[1] [0, 8 - offset) 位存入  src[0] (8 - offset, 8) 位
                            uint8_t *srcPtr = MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame;
                            uint8_t *dstPtr = registerPtr;
                            bytes = ((MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount - 1) >> 3) + 1;
                            // 准备数据: 将寄存器指定区域置零
                            // Details:
                            //     Step0. 使用 ReturnFrame 作为临时区域
                            //     Step1. 将 ReturnFrame 前 RequestAmount 位设置为 0xFF
                            //     Step2. 根据 TargetRegistersBitOffset 将 ReturnFrame 向左移位, 高位移出的数据要补到下一个字节的低位
                            //     Step3. 寄存器要操作的区域与 ReturnFrame 进行 AND 运算
                            memset(srcPtr, 0xFF, bytes);
                            if (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount & 0b111)
                            {
                                srcPtr[MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount >> 3] &= 0xFF >> (8 - (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount & 0b111));
                            }
                            srcPtr[bytes] = 0; // 下一位也要置零, 因为数据可能会移到这里
                            for (int i = bytes; i > 0; --i)
                            {
                                srcPtr[i] |= srcPtr[i - 1] >> (8 - MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset);
                                srcPtr[i - 1] <<= MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset;
                                dstPtr[i] &= ~srcPtr[i];
                            }
                            dstPtr[0] &= ~srcPtr[0];
                            // 赋值
                            // Details: 原理与 "准备数据" 类似,
                            //     Step1 变为: 将源数据前 RequestAmount 位拷贝到 ReturnFrame
                            //     Step3 变为: 寄存器要操作的区域与 ReturnFrame 进行 OR 运算
                            memcpy(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, dataPtr + 7, bytes + (bytes & 1));
                            srcPtr[bytes + (bytes & 1)] = 0;
                            if (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount & 0b111)
                            {
                                srcPtr[MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount >> 3] &= 0xFF >> (8 - (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount & 0b111));
                            }
                            // 准备数据_移位_寄存器指定位归零
                            srcPtr[bytes] = 0;
                            for (int i = bytes; i > 0; --i)
                            {
                                srcPtr[i] |= srcPtr[i - 1] >> (8 - MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset);
                                srcPtr[i - 1] <<= MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset;
                                dstPtr[i] |= srcPtr[i];
                            }
                            dstPtr[0] |= srcPtr[0];
                        }
                        else // 写入 1 个 1 Bit
                        {
                            switch (_ToBigEndian(*(uint16_t *)(dataPtr + 4)))
                            {
                            case 0:
                            {
                                *registerPtr &= ~(1 << MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset);
                                break;
                            }
                            case 0xFF00:
                            {
                                *registerPtr |= (1 << MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset);
                                break;
                            }
                            default:
                            {
                                MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_WRITE_SINGLE_COIL_DATA_IS_INCORRECT;
                                return false;
                            }
                            }
                        }
#endif

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                    }
#endif
                    if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_Multiple)
                    {
                        memcpy(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, dataPtr, 6);
                        MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[6] = bytes;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                        uint16_t crcValue = CalculateCRC(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, 3);
#else
                        uint16_t crcValue = MODBUS_RTU_Slave_CalculateCRC(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, 3);
#endif
                        MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength - 2] = crcValue & 0xFF;
                        MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength - 1] = crcValue >> 8;
                    }
                    else
                    {
                        memcpy(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, dataPtr, MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength);
                    }
                }
                else // 读取操作
                {
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                    if (MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_FunctionCodeAttribute_16Bits) // 读取 16 Bits, 返回帧 0-2 分别为 00设备地址 01功能码 02字节数
                    {
#endif
                        memcpy(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, dataPtr, 2);
                        MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[2] = MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes;
                        uint16_t *dstPtr = (uint16_t *)(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame + 3);
                        uint16_t *srcPtr = (uint16_t *)(registerPtr);
#ifdef MODBUS_RTU_SLAVE_BIG_ENDIAN
                        memcpy(dstPtr, srcPtr, MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes);
#else
                        for (int i = 0; i < MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount; ++i)
                        {
                            *(dstPtr++) = _ToBigEndian(*(srcPtr++));
                        }
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                    }
                    else // 读取 1 Bit, 返回帧 0-5 分别为 00设备地址 01功能码 0203地址 04字节数, 要移位
                    {
                        memcpy(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, dataPtr, 4);
                        MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[4] = ((MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount - 1) >> 3) + 1;
                        memcpy(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame + 5, registerPtr, MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes);
                        // 移位:
                        // 第 n 个字节右移 Offset 位
                        // 第 n+1 个字节的低 Offset 位拷贝到第 m 个字节的低 Offset 位
                        //     Details: 取 n+1 个字节的低 Offset 位: nTemp ~(0b11111111 << Offset) & nValue
                        //              将 低 Offset 位放到 高 Offset 位上: nTemp <<= (8 - Offset)
                        //              拷贝: nValue |= nTemp;
                        for (uint8_t *ptr = MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame + 5; ptr < MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame + 5 + MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes; ++ptr)
                        {
                            *ptr >>= MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset;
                            *ptr |= (*(ptr + 1) & ~(0xFF << MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset)) << (8 - MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBitOffset);
                        }
                        // 访问的寄存器数 / 8 得到返回的字节数, 其中最后一个字节可能包含不访问的数据, 要将这部分置零
                        if (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount & 0b111)
                        {
                            *(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame + 4 + MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_TargetRegistersBytes) &= ~(0xFF << (MODBUS_RTU_SLAVE_THISPTR->_Prase_RequestFrame_RequestAmount & 0b111));
                        }
                    }
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
                    uint16_t crcValue = CalculateCRC(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, 3);
#else
                    uint16_t crcValue = MODBUS_RTU_Slave_CalculateCRC(MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame, 3);
#endif
                    MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength - 2] = crcValue & 0xFF;
                    MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame[MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength - 1] = crcValue >> 8;
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                    MODBUS_RTU_SLAVE_THISPTR->_Counter_RecvDataIsSame = 0;
#endif
                }
                MODBUS_RTU_SLAVE_THISPTR->_LastResult = MODBUS_RTU_RESULT_OK;
            }
        }
    }
    return MODBUS_RTU_SLAVE_THISPTR->_LastResult == MODBUS_RTU_RESULT_OK;
}
