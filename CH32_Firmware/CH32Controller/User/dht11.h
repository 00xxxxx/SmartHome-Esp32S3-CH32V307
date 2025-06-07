/*********************************************************************
 * @file      dht11.h
 * @author    Gemini
 * @brief     DHT11温湿度传感器驱动模块的头文件
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __DHT11_H
#define __DHT11_H

#include "ch32v30x_gpio.h"
#include "debug.h"

/**
 * @brief  初始化DHT11传感器所使用的GPIO引脚.
 *
 * @return none.
 */
void DHT11_Init(void);

/**
 * @brief  从DHT11传感器读取温湿度数据.
 *
 * @param  temp 指向用于存储温度值的指针 (u8).
 * @param  humi 指向用于存储湿度值的指针 (u8).
 *
 * @return u8 0: 成功, 1: 失败 (校验和错误或无响应).
 */
u8 DHT11_Read_Data(u8 *temp, u8 *humi);

#endif 