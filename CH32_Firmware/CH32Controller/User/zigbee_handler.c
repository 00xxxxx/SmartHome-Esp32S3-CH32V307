/*********************************************************************
 * @file      zigbee_handler.c
 * @author    Gemini
 * @brief     Zigbee应用逻辑处理模块的实现文件.
 * @version   1.0
 * @date      2025-06-08
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "zigbee_handler.h"
#include "bsp_usart2.h"
#include "bsp_buzzer.h"

// 模块私有的全局报警状态标志
static volatile u8 g_alarm_active = 0;

/**
 * @brief  初始化Zigbee处理模块.
 */
void Zigbee_Handler_Init(void)
{
    USART2_Init(); // 初始化底层串口驱动
}

/**
 * @brief  Zigbee处理模块的任务函数.
 */
void Zigbee_Handler_Task(void)
{
    u8 zigbee_cmd;

    // 1. 检查是否有新的报警命令
    if(USART2_GetData(&zigbee_cmd))
    {
        if(zigbee_cmd == INFRARED_ALARM_CMD || zigbee_cmd == SMOKE_ALARM_CMD)
        {
            g_alarm_active = 1; // 收到报警命令，设置报警标志
        }
    }

    // 2. 根据报警标志控制蜂鸣器
    if(g_alarm_active)
    {
        Buzzer_On();
    }
    else
    {
        Buzzer_Off();
    }
}

/**
 * @brief  清除报警状态.
 */
void Zigbee_Clear_Alarm(void)
{
    g_alarm_active = 0;
} 