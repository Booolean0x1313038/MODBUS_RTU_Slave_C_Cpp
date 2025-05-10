#ifndef __MODBUS_RTU_SLAVE_H__
#define __MODBUS_RTU_SLAVE_H__

#include <stdbool.h>
#include "MODBUS_RTU_Slave_Configuration.h"

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef uint16_t MODBUS_RTU_Register_UINT16;
typedef uint8_t MODBUS_RTU_RegisterAttribute;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
typedef unsigned char MODBUS_RTU_Register_BIT;
#endif

/// @brief 功能码
typedef enum
{
    /// @brief 读取多个线圈 (1 Bit)
    MODBUS_RTU_FUNCTIONCODE_READ_MULTIPLE_COILS = 0x01,
    /// @brief 读取多个离散输入 (1 Bit)
    MODBUS_RTU_FUNCTIONCODE_READ_MULTIPLE_DISCRETE_INPUTS = 0x02,
    /// @brief 读取多个保持寄存器 (16 Bits)
    MODBUS_RTU_FUNCTIONCODE_READ_MULTIPLE_HOLDING_REGISTERS = 0x03,
    /// @brief 读取多个输入寄存器 (16 Bits)
    MODBUS_RTU_FUNCTIONCODE_READ_MULTIPLE_INPUT_REGISTERS = 0x04,
    /// @brief 写入单个线圈 (1 Bit)
    MODBUS_RTU_FUNCTIONCODE_WRITE_SINGLE_COIL = 0x05,
    /// @brief 写入单个保持寄存器 (16 Bit)
    MODBUS_RTU_FUNCTIONCODE_WRITE_SINGLE_HOLDING_REGISTER = 0x06,
    /// @brief 读取异常状态 (仅串行线)
    /// @warning 不受支持
    MODBUS_FunctionCode_SerialLineOnly_ReadExceptionStatus = 0x07,
    /// @brief 通信系统诊断 (仅串行线)
    /// @warning 不受支持
    MODBUS_FunctionCode_SerialLineOnly_Diagnostics = 0x08,
    /// @brief  获取通信事件计数器 (仅串行线)
    /// @warning 不受支持
    MODBUS_FunctionCode_SerialLineOnly_GetCommunicateEventCounter0 = 0x0B,
    /// @brief  获取通信事件日志 (仅串行线)
    /// @warning 不受支持
    MODBUS_FunctionCode_SerialLineOnly_GetCommunicateEventLog = 0x0C,
    /// @brief  写多个线圈 (1 Bit)
    MODBUS_RTU_FUNCTIONCODE_WRITE_MULTIPLE_COILS = 0x0F,
    /// @brief  写多个保持寄存器 (16 Bits)
    MODBUS_RTU_FUNCTIONCODE_WRITE_MULTIPLE_HOLDING_REGISTERS = 0x10,
    /// @brief 报告服务器ID (仅串行线)
    /// @warning 不受支持
    MODBUS_FunctionCode_SerialLineOnly_ReportServerID = 0x11,
    /// @brief 读文件记录
    /// @warning 不受支持
    MODBUS_FunctionCode_ReadFileRecord = 0x14,
    /// @brief 写文件记录
    /// @warning 不受支持
    MODBUS_FunctionCode_WriteFileRecord = 0x15,
    /// @brief 带屏蔽字写入寄存器
    /// @warning 不受支持
    MODBUS_FunctionCode_MaskWriteRegister = 0x16,
    /// @brief 读/写多个寄存器
    /// @warning 不受支持
    MODBUS_FunctionCode_ReadWriteMultipleRegisters = 0x17,
    /// @brief 读取先进先出 (FIFO) 队列
    /// @warning 不受支持
    MODBUS_FunctionCode_ReadFIFOQueue = 0x18,
    /// @brief 封装接口传输
    /// @warning 不受支持
    MODBUS_FunctionCode_EncapsulatedInterfaceTransport = 0x2B,
} MODBUS_RTU_FunctionCodes;

/// @brief 功能码的属性
// Reserved | IsSingle (0) or Multiple(1) |  Is 1Bit (0) or 16Bits (1) | Is Read (0) or Writa (1)
typedef enum
{
    MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_UNKNOWN = 0,
    MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_WRITE = 0b1,
    MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_16BITS = 0b10,
    MODBUS_RTU_FUNCTIONCODE_ATTRIBUTE_MULTIPLE = 0b100,
} MODBUS_RTU_FunctionCodeAttribute;

/// @brief 错误码
typedef enum
{
    /// @brief 功能码不受支持
    MODBUS_RTU_EXCEPTIONCODE_INVALID_FUNCTIONCODE = 1,
    /// @brief 寄存器不存在
    MODBUS_RTU_EXCEPTIONCODE_INVALID_REGISTER_ADDRESS,
    /// @brief 请求帧中值不正确
    MODBUS_RTU_EXCEPTIONCODE_INVALID_REQUEST_VALUE,
    MODBUS_RTU_EXCEPTIONCODE_SERVER_FAILED,
    MODBUS_RTU_EXCEPTIONCODE_PROCESSING,

} MODBUS_RTU_ExceptionCode;

/// @brief 操作结果
typedef enum
{
    /// @brief 无异常
    MODBUS_RTU_RESULT_OK = 0,
    /// @brief 目标设备不是本设备
    MODBUS_RTU_RESULT_NOT_THIS_DEVICE,
    /// @brief CRC 不匹配
    MODBUS_RTU_RESULT_CRC_WRONG,
    /// @brief 功能码不受支持
    MODBUS_RTU_RESULT_FUNCTIONCODE_IS_NOT_SUPPORT,
    /// @brief 数据长度不符合预期
    MODBUS_RTU_RESULT_DATA_LENGTH_IS_INVALID,
    /// @brief 寄存器类型不正确
    MODBUS_RTU_RESULT_REGISTER_TYPE_IS_INCORRECT,
    /// @brief 目标寄存器不存在
    MODBUS_RTU_RESULT_REGISTER_NOT_EXIST,
    /// @brief 目标寄存器超出区域
    MODBUS_RTU_RESULT_REGISTER_OUT_OF_RANGE,
    /// @brief 要读取的寄存器不可读
    MODBUS_RTU_RESULT_REGISTER_NOT_READABLE,
    /// @brief 要写入的急寄存器不可写
    MODBUS_RTU_RESULT_REGISTER_NOT_WRITEABLE,
    /// @brief 广播要读取数据
    MODBUS_RTU_RESULT_BROADCAST_READ,
    /// @brief 写入次数太多或太少
    MODBUS_RTU_RESULT_WRITE_TIME_NOT_MATCH,
    /// @brief 写入单个线圈时传入的值不正确
    MODBUS_RTU_RESULT_WRITE_SINGLE_COIL_DATA_IS_INCORRECT,
    /// @brief 未知
    MODBUS_RTU_RESULT_UNKNOWN,
} MODBUS_RTU_ResultCode;

/// @brief 寄存器属性
typedef enum
{
    /// @brief 可读
    MODBUS_RTU_REGISTER_ATTRIBUTE_READABLE = 0b01,
    /// @brief 可写
    MODBUS_RTU_REGISTER_ATTRIBUTE_WRITABLE = 0b10,
} MODBUS_RTU_Register_Attribute;

/// @brief 寄存器属性
typedef enum
{
    /// @brief 线圈或离散输入
    MODBUS_RTU_REGISTER_TYPE_BIT = 1,
    /// @brief 16 字节寄存器
    MODBUS_RTU_REGISTER_TYPE_UINT16,
} MODBUS_RTU_Register_Type;

/// @brief 地址连续的若干寄存器
typedef struct
{
    /// @brief 寄存器 Array
    const void *Registers;
    /// @brief 寄存器属性 Array
    const uint8_t *RegistersAttribute;
    /// @brief 寄存器起始的地址
    uint16_t FirstRegisterAddress;
    /// @brief Sizeof (寄存器 Array)
    uint16_t Bytes;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
    /// @brief 寄存器类型
    MODBUS_RTU_Register_Type RegisterType;
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
    /// @brief 需要操作的次数
    uint8_t WriteTimes;
#endif
} MODBUS_RTU_ContiguousRegistersGroup;

#define MODBUS_RTU_MAX_LENGTH 256
#define MODBUS_RTU_DEVICE_ADDRESS_BROADCAST 0

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
#define MODBUS_RTU_SLAVE_THISPTR this
namespace _Boolean::Communication
{
    class MODBUS_RTU_Slave
    {
#ifdef MODBUS_RTU_SLAVE_DEBUG
    public:
#else
    private:
#endif
#else
typedef struct
{
#endif

        /// @brief 设备地址
        uint8_t _DeviceAddress;
        /// @brief 连续寄存器区
        MODBUS_RTU_ContiguousRegistersGroup *_ContiguousRegistersGroupArray;
        /// @brief 连续寄存器区的数量
        uint8_t _ContiguousRegistersGroupArrayLength;
        /// @brief 返回数据
        uint8_t _ReturnFrame[MODBUS_RTU_MAX_LENGTH];
        /// @brief 返回帧的长度
        uint16_t _ReturnFrameLength;
        /// @brief 上次操作的结果
        MODBUS_RTU_ResultCode _LastResult;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
        /// @brief 上次收到的数据
        uint8_t _LastRecvFrame[MODBUS_RTU_MAX_LENGTH - 4]; // 存储的数据不包括 设备地址(00) 操作码(01) CRC(-2 -1)
        /// @brief 上次收到的数据的长度
        uint16_t _LastRecvFrameLength;
        /// @brief 连续收到相同数据的次数, 如果不是写入操作则是 0, 不相同则是 1, 相同则是 2, 3, ...
        uint8_t _Counter_RecvDataIsSame;
#endif

        /// @brief 解析_设备地址 (FromFrame)
        uint8_t _Prase_RequestFrame_DeviceAddress;
        /// @brief 解析_功能码 (FromFrame)
        uint8_t _Prase_RequestFrame_FunctionCode;
        /// @brief 解析_寄存器地址 (FromFrame)
        uint16_t _Prase_RequestFrame_RegisterAddress;
        /// @brief 解析_请求的寄存器数 (FromFrame)
        uint16_t _Prase_RequestFrame_RequestAmount;
        /// @brief 解析_接收帧的预期长度 (字节数)
        uint16_t _Prase_Calculate_RequestFrameBytes;
        /// @brief 解析_目标寄存器区
        int _Prase_Calculate_TargetCoiledRegistersAreaIndex;
        /// @brief 解析_第一个要操作的寄存器的相对于所在区域首个寄存器的偏移地址
        uint16_t _Prase_Calculate_TargetRegistersMemoryAddressOffset;
        /// @brief 解析_要操作的寄存器的字节数
        uint16_t _Prase_Calculate_TargetRegistersBytes;
        /// @brief 解析_操作_写入
        bool _Prase_Calculate_FunctionCodeAttribute_Write;
        /// @brief 解析_操作_多个
        bool _Prase_Calculate_FunctionCodeAttribute_Multiple;
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
        /// @brief 解析_位偏移
        uint16_t _Prase_Calculate_TargetRegistersBitOffset;
        /// @brief 解析_操作_16位寄存器
        bool _Prase_Calculate_FunctionCodeAttribute_16Bits;
#endif

#ifndef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    } MODBUS_RTU_Slave;
#endif

#ifndef MODBUS_RTU_SLAVE_BIG_ENDIAN
    /// @brief 大小端转换
    /// @param 要转换的 16bit 值
    /// @return 转换后的值
    inline uint16_t _ToBigEndian(uint16_t value) { return (value >> 8) | (value << 8); }
#endif

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    /// @brief 根据功能码获得其特性
    /// @param code 功能码
    /// @return 特性码
    uint8_t _GetFunctionCodeAttribute(const uint8_t code);
    /// @brief 生成异常返回帧
    void _GenerateExceptionFrame();

public:
#else
    /// @brief 根据功能码获得其特性
    /// @param code 功能码
    /// @return 特性码
    uint8_t _MODBUS_RTU_Slave_GetFunctionCodeAttribute(const uint8_t code);
    /// @brief 生成异常返回帧
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    void _MODBUS_RTU_Slave_GenerateExceptionFrame(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR);
#endif

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
    /// @brief 构造函数
    /// @param deviceAddress 本机地址
    /// @param coiledRegisters 寄存器区 Array
    /// @param coiledRegistersAmount 寄存器区的数量
    MODBUS_RTU_Slave(uint8_t deviceAddress, MODBUS_RTU_ContiguousRegistersGroup *coiledRegisters, uint8_t coiledRegistersAmount);
    /// @brief 检查 MODBUS RTU Request Frame Head,
    /// @details 校验地址是否是本设备;
    /// @details 校验功能码是否受支持
    /// @details 校验功能码对应的功能是否能执行
    /// @details 校验目标寄存器是否都在正确范围内
    /// @details 计算预期的请求帧长度;
    /// @details 计算读取或写入的数据的长度;
    /// @details 计算返回帧的长度
    /// @param dataPtr 接收到的数据
    /// @param length 数据长度, 至少 6 Bytes
    /// @return 解析通过
    bool PraseHeader(const uint8_t *dataPtr, const uint8_t length);
    /// @brief 执行操作
    /// @param dataPtr 接收到的数据
    /// @param length 数据长度
    bool ProcessData(const uint8_t *dataPtr, const uint16_t length);
    /// @brief 执行操作
    /// @param dataPtr 接收到的数据
    /// @param length 数据长度
    static void InitializeContiguousRegistersGroup(MODBUS_RTU_ContiguousRegistersGroup *group, const void *registerArray, const uint16_t registerArrayLength, const uint8_t *attributeArray, const uint16_t firstRegisterAddress
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                                                   ,
                                                   const uint8_t writeTimes
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                   ,
                                                   const MODBUS_RTU_Register_Type type
#endif
    );
    /// @brief 计算 CRC
    /// @param data 数据源
    /// @param length 数据源长度
    /// @return CRC
    static uint16_t CalculateCRC(const uint8_t *data, const uint16_t length);
    /// @brief 清空返回帧
    void ClearReturnFrame();
    /// @brief 获取最后操作的结果
    /// @return 操作结果
    inline MODBUS_RTU_ResultCode GetLastResult() { return this->_LastResult; }
    /// @brief 获取请求帧的期望长度 在解析帧头之后调用
    /// @return 预期长度 (Bytes)
    inline uint16_t GetRequestFrameLength() { return this->_Prase_Calculate_RequestFrameBytes; }
    /// @brief 获取返回帧的长度 在解析帧头失败或处理数据结束后调用
    /// @return 长度 (Bytes)
    inline uint16_t GetReturnFrameLength() { return this->_ReturnFrameLength; }
    /// @brief 获取返回帧的内存地址
    /// @return 返回帧的内存首地址
    inline const unsigned char *GetReturnData() { return this->_ReturnFrame; }
    /// @brief 获取设备 MODBUS 地址
    /// @return 设备地址
    inline uint8_t GetDeviceAddress() { return this->_DeviceAddress; }
#else
    /// @brief 初始化
    /// @param MODBUS_RTU_SLAVE_THISPTR 实体
    /// @param deviceAddress 本机地址
    /// @param coiledRegisters 寄存器区 Array
    /// @param coiledRegistersAmount 寄存器区的数量
    void MODBUS_RTU_Slave_Initialize(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR, uint8_t deviceAddress, MODBUS_RTU_ContiguousRegistersGroup *coiledRegisters, uint8_t coiledRegistersAmount);
    /// @brief 解析帧头
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    /// @param dataPtr 接收帧
    /// @param length 接收帧长度
    /// @return 解析成功
    bool MODBUS_RTU_Slave_PraseHeader(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR, const uint8_t *dataPtr, const uint8_t length);
    /// @brief 处理数据
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    /// @param dataPtr 接收帧
    /// @param length 接收帧长度
    /// @return 操作成功
    bool MODBUS_RTU_Slave_ProcessData(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR, const uint8_t *dataPtr, const uint16_t length);
    /// @brief 初始化一个连续寄存器组
    /// @param group 要初始化的组
    /// @param registerArray 寄存器数组
    /// @param registerArrayLength 寄存器数组中元素的数量
    /// @param attributeArray 寄存器属性数组
    /// @param firstRegisterAddress 寄存器组第一个寄存器的 MODBUS 地址
    /// @param writeTimes 写入时要连续写入的次数
    /// @param type 寄存器组的类型
    void MODBUS_RTU_Slave_InitializeContiguousRegistersGroup(MODBUS_RTU_ContiguousRegistersGroup *group, const void *registerArray, const uint16_t registerArrayLength, const uint8_t *attributeArray, const uint16_t firstRegisterAddress
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES
                                                             ,
                                                             const uint8_t writeTimes
#endif
#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL
                                                             ,
                                                             const MODBUS_RTU_Register_Type type
#endif
    );
    /// @brief 计算 CRC
    /// @param data 数据源
    /// @param length 数据源长度
    /// @return CRC
    uint16_t MODBUS_RTU_Slave_CalculateCRC(const uint8_t *data, const uint16_t length);
    /// @brief 清空返回帧
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    void MODBUS_RTU_Slave_ClearReturnFrame(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR);
    /// @brief 获取最后操作的结果
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    /// @return 操作结果
    inline MODBUS_RTU_ResultCode MODBUS_RTU_Slave_GetLastResult(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR) { return MODBUS_RTU_SLAVE_THISPTR->_LastResult; }
    /// @brief 获取请求帧的期望长度 在解析帧头之后调用
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    /// @return 预期长度 (Bytes)
    inline uint16_t MODBUS_RTU_Slave_GetRequestFrameLength(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR) { return MODBUS_RTU_SLAVE_THISPTR->_Prase_Calculate_RequestFrameBytes; }
    /// @brief 获取返回帧的长度 在解析帧头失败或处理数据结束后调用
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的寄存器
    /// @return 长度 (Bytes)
    inline uint16_t MODBUS_RTU_Slave_GetReturnFrameLength(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR) { return MODBUS_RTU_SLAVE_THISPTR->_ReturnFrameLength; }
    /// @brief 获取返回帧的内存地址
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    /// @return 返回帧的内存首地址
    inline const unsigned char *MODBUS_RTU_Slave_GetReturnData(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR) { return MODBUS_RTU_SLAVE_THISPTR->_ReturnFrame; }
    /// @brief 获取设备 MODBUS 地址
    /// @param MODBUS_RTU_SLAVE_THISPTR 要操作的结构体
    /// @return 设备地址
    inline uint8_t MODBUS_RTU_Slave_GetDeviceAddress(MODBUS_RTU_Slave *MODBUS_RTU_SLAVE_THISPTR) { return MODBUS_RTU_SLAVE_THISPTR->_DeviceAddress; }
#endif

#ifdef MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS
};
}
;
#endif
#endif // __MODBUS_RTU_SLAVE_H__

// MODBUS RTU 帧结构
// 0x01 | 读取多线圈:
// 0x02 | 读取多离散输入寄存器:
//      Request:
//          Byte: |---- 00 ----|----- 01 -----|----- 02 03 -----|-------------------- 04 05 --------------------|- 06 07 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress                  RegistersAmount                |   CRC   |
//          Data: |     01            01      |      00 01                            00 04                     |  6C 09  |
//      Response: |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 01 -----|----- 02 03 -----|------ 04 ------|------------- 05 -------------|- 06 07 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress     BytesCount                 Data             |   CRC   |
//          Data: |     00            01      |      00 01              01                      06              |  ED 89  |
// 0x03 | 读取多保持寄存器:
// 0x04 | 读取多输入寄存器:
//      Request:  |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 05 -----|----- 02 03 -----|-------------------- 04 05 --------------------|- 06 07 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress                  RegistersAmount                |   CRC   |
//          Data: |     01            03      |      00 01                            00 02                     |  95 CB  |
//      Response: |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 01 -----|------  02 ------|----------------- 03 04 05 06 -----------------|- 07 08 -|
//          Desc: | DeviceAddr   FunctionCode |    BytesCount                          Data                     |   CRC   |
//          Data: |     01            03      |        04                          01 01 02 02                  |  2A AE  |
// 0x05 | 写入单线圈:
// 0x06 | 写入单保持寄存器:
//      Request:  |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 05 -----|----- 02 03 -----|-------------------- 04 05 --------------------|- 06 07 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress                       Data                      |   CRC   |
//          Data: |     01            05      |      00 01                            FF 00                     |  DD FA  |
//      Response: |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 05 -----|----- 02 03 -----|-------------------- 04 05 --------------------|- 06 07 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress                       Data                      |   CRC   |
//          Data: |     01            05      |      00 01                            FF 00                     |  DD FA  |
// 0x0F | 写入多线圈:
//      Request:  |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 01 -----|----- 02 03 -----|----- 04 05 -----|---- 06 ----|---- 07  08 ----|- 09 10 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress   RegistersAmount   BytesCount        Data      |   CRC   |
//          Data: |     01            0F      |      00 01              00 02           01           00  10     |  17 45  |
//      Response: |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 01 -----|----- 02 03 -----|----------- 04  05 -----------|------ 06 ------|- 07 08 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress         Registers Amount           BytesCount   |   CRC   |
//          Data: |     01            0F      |      00 01                   00  02                    01       |  CA 63  |
// 0x10 | 写入多寄存器:
//      Request:  |                           |                                                                 |         |
//          Byte: |---- 00 ----|----- 01 -----|----- 02 03 -----|----- 04 05 -----|---- 06 ----|- 07 08  09 10 -|- 11 12 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress   RegistersAmount   BytesCount        Data      |   CRC   |
//          Data: |     01            10      |      00 01             00 02            04        01 01  02 02  |  E2 FE  |
//      Response:
//          Byte: |---- 00 ----|----- 01 -----|----- 02 03 -----|-------------------- 04 05 --------------------|- 06 07 -|
//          Desc: | DeviceAddr   FunctionCode | RegisterAddress                  RegistersAmount                |   CRC   |
//          Data: |     01            10      |      00 01                            00 02                     |  10 08  |
// 校验字段低 8 位在前, 高 8 位在后
