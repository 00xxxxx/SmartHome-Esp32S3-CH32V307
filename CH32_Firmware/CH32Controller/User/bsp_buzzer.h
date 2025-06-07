/*********************************************************************
 * @file      bsp_buzzer.h
 * @author    Gemini
 * @brief     有源蜂鸣器驱动模块的头文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __BSP_BUZZER_H
#define __BSP_BUZZER_H

#include "ch32v30x.h"

/**
 * @brief  初始化蜂鸣器所连接的GPIO引脚.
 * @return none.
 */
void Buzzer_Init(void);

/**
 * @brief  打开蜂鸣器.
 * @return none.
 */
void Buzzer_On(void);

/**
 * @brief  关闭蜂鸣器.
 * @return none.
 */
void Buzzer_Off(void);


#endif 