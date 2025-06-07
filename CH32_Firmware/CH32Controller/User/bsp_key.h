/*********************************************************************
 * @file      bsp_key.h
 * @author    Gemini
 * @brief     按键及外部中断驱动模块的头文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "ch32v30x.h"

/**
 * @brief  初始化按键所连接的GPIO引脚及对应的外部中断.
 * @return none.
 */
void Key_Init(void);


#endif 