/*********************************************************************
 * @file      bsp_buzzer.c
 * @author    Gemini
 * @brief     有源蜂鸣器驱动模块的实现文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "bsp_buzzer.h"

// 蜂鸣器引脚定义 (根据PIN.txt)
#define BUZZER_PORT   GPIOA
#define BUZZER_PIN    GPIO_Pin_5

/**
 * @brief  初始化蜂鸣器所连接的GPIO引脚.
 * @return none.
 */
void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA5为推挽输出
    GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_PORT, &GPIO_InitStructure);

    // 初始化后默认关闭蜂鸣器
    Buzzer_Off();
}

/**
 * @brief  打开蜂鸣器.
 * @return none.
 */
void Buzzer_On(void)
{
    // 假设高电平鸣叫
    GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}

/**
 * @brief  关闭蜂鸣器.
 * @return none.
 */
void Buzzer_Off(void)
{
    GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
} 