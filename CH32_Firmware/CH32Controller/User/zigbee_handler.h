/*********************************************************************
 * @file      zigbee_handler.h
 * @author    Gemini
 * @brief     Zigbee应用逻辑处理模块的头文件.
 * @version   1.0
 * @date      2025-06-08
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __ZIGBEE_HANDLER_H
#define __ZIGBEE_HANDLER_H

#include "ch32v30x.h"

/**
 * @brief  初始化Zigbee处理模块 (包括底层USART2).
 * @return none.
 */
void Zigbee_Handler_Init(void);

/**
 * @brief  Zigbee处理模块的任务函数，应在主循环中周期性调用.
 * @return none.
 */
void Zigbee_Handler_Task(void);

/**
 * @brief  清除报警状态.
 * @note   通常由外部事件调用，如按键中断.
 * @return none.
 */
void Zigbee_Clear_Alarm(void);


#endif // __ZIGBEE_HANDLER_H 