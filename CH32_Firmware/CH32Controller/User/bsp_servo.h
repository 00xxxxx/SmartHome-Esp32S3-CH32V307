/*********************************************************************
 * @file      bsp_servo.h
 * @author    Gemini
 * @brief     舵机(SG90)驱动模块的头文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __BSP_SERVO_H
#define __BSP_SERVO_H

#include "ch32v30x.h"

/**
 * @brief  初始化舵机所连接的GPIO和TIM3_CH2 PWM.
 * @return none.
 */
void Servo_Init(void);

/**
 * @brief  控制舵机旋转到指定角度.
 * @param  angle - 目标角度 (0 - 180).
 * @return none.
 */
void Servo_SetAngle(u8 angle);


#endif 