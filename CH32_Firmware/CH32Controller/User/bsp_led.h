/*********************************************************************
 * @file      bsp_led.h
 * @author    Gemini
 * @brief     板载LED驱动模块的头文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "ch32v30x.h"

// 定义LED编号
typedef enum
{
    LED1 = 0,
    LED2 = 1
} LED_ID;

/**
 * @brief  初始化LED所连接的GPIO引脚.
 * @return none.
 */
void LED_Init(void);

/**
 * @brief  点亮指定的LED.
 * @param  led_id - 要操作的LED编号 (LED_ID).
 * @return none.
 */
void LED_On(LED_ID led_id);

/**
 * @brief  熄灭指定的LED.
 * @param  led_id - 要操作的LED编号 (LED_ID).
 * @return none.
 */
void LED_Off(LED_ID led_id);

/**
 * @brief  翻转指定LED的状态.
 * @param  led_id - 要操作的LED编号 (LED_ID).
 * @return none.
 */
void LED_Toggle(LED_ID led_id);


#endif 