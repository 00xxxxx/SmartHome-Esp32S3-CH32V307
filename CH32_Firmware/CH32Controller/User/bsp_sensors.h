/*********************************************************************
 * @file      bsp_sensors.h
 * @author    Gemini
 * @brief     板载传感器统一管理模块的头文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __BSP_SENSORS_H
#define __BSP_SENSORS_H

#include "ch32v30x.h"

/**
 * @brief  初始化所有传感器所需的GPIO和ADC.
 * @return none.
 */
void Sensors_Init(void);

/**
 * @brief  获取光敏传感器的ADC原始读数.
 * @return u16 - ADC转换值.
 */
u16 Photoresistor_Get_Val(void);

/**
 * @brief  检测人体红外传感器是否触发.
 * @return u8 - 1: 检测到人, 0: 未检测到人.
 */
u8 PIR_Is_Triggered(void);

/**
 * @brief  检测烟雾传感器是否触发.
 * @return u8 - 1: 检测到烟雾, 0: 未检测到烟雾.
 */
u8 Smoke_Is_Triggered(void);


#endif 