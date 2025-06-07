/*********************************************************************
 * @file      uart_handler.h
 * @author    Gemini
 * @brief     串口命令处理模块的头文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __UART_HANDLER_H
#define __UART_HANDLER_H

#include "ch32v30x.h"

/**
 * @brief  初始化UART1及其中断.
 * @return none.
 */
void UART_Handler_Init(void);

#endif 