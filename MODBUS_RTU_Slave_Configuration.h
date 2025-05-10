#ifndef __MODBUS_RTU_SLAVE_CONFIGURATION_H__
#define __MODBUS_RTU_SLAVE_CONFIGURATION_H__

/////////////////////////////////////////////////////////////////////////////////

/// @brief 启用 Feature 写入多次生效
#define MODBUS_RTU_SLAVE_ENABLE_FEATURE_WRITE_SEVEIAL_TIMES

/// @brief 启用 Feature: 1Bit 寄存器
#define MODBUS_RTU_SLAVE_ENABLE_FEATURE_COIL

/// @brief 启用 Feature: C++ Class
// #define MODBUS_RTU_SLAVE_ENABLE_FEATURE_CPLUSPLUS_CLASS

/// @brief 调试模式, C++ Class 模式下所有成员均可访问
#define MODBUS_RTU_SLAVE_DEBUG

/// @brief 不进行大小端转换
// #define MODBUS_RTU_SLAVE_BIG_ENDIAN
// MODBUS RTU 报文为大端模式, 如果运行设备也是大端模式, 则要保留此选项
// STM8s105 @ SDCC 为大端模式

// 确认大小端的简单方法 (伪代码):
// typedef struct
// {
// 	uint8_t Value0;
// 	uint8_t Value1;
// 	uint8_t Value2;
// 	uint8_t Value3;
// } UInt8_4;
// uint32_t value_UInt32 = 0x44332211;
// UInt8_4 uint8_4;
// uint8_4 = *(UInt8_4 *)&value_UInt32;
// UART_PrintHex_UInt32(value_UInt32);
// UART_PrintLine("");
// UART_PrintHex_UInt8(uint8_4.Value0);
// UART_PrintHex_UInt8(uint8_4.Value1);
// UART_PrintHex_UInt8(uint8_4.Value2);
// UART_PrintHex_UInt8(uint8_4.Value3);
//
// 在大端模式下会得到以下结果:
//  44332211
//  44332211
//
// 在小端模式下会得到以下结果:
//  44332211
//  11223344

/////////////////////////////////////////////////////////////////////////////////

// 选择计算 CRC 的方法
//      1 = 查短表法, 均衡速度与空间占用
//      2 = 查长表法, 速度最快, 占用空间最多
//      其他或不定义 = 直接计算法, 占用空间最小, 速度最慢
#define MODBUS_RTU_SLAVE_CRC_METHOD 2

/////////////////////////////////////////////////////////////////////////////////

#include "string.h"
#include <QDebug>

#endif
