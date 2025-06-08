/*********************************************************************
 * @file      bsp_usart2.h
 * @author    Gemini
 * @brief     USART2 (Zigbee) 模块的头文件.
 * @version   1.0
 * @date      2025-06-08
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __BSP_USART2_H
#define __BSP_USART2_H

#include "ch32v30x.h"

#define ZIGBEE_BAUDRATE     115200  // 请确保此波特率与CC2530协调器模块一致

// Zigbee 报警协议定义
#define INFRARED_ALARM_CMD  0xA1    // 红外传感器报警
#define SMOKE_ALARM_CMD     0xB1    // 烟雾传感器报警

/**
 * @brief  初始化UART2及其中断，用于Zigbee通信.
 * @return none.
 */
void USART2_Init(void);

/**
 * @brief  从接收缓冲区获取一个字节.
 * @param  data - 用于存储数据的指针.
 * @return u8 - 1: 成功获取数据, 0: 缓冲区为空.
 */
u8 USART2_GetData(u8 *data);

/**
 * @brief  USART2中断服务函数的回调.
 * @note   此函数应在 ch32v30x_it.c 的 USART2_IRQHandler 中被调用.
 * @return none.
 */
void USART2_IRQHandler_Callback(void);


#endif // __BSP_USART2_H 