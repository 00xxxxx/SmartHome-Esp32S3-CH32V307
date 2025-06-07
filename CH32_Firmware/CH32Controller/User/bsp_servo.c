/*********************************************************************
 * @file      bsp_servo.c
 * @author    Gemini
 * @brief     舵机(SG90)驱动模块的实现文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 * @note      使用TIM3的通道2 (PC5) 输出PWM信号控制舵机.
 *            舵机控制周期为20ms (50Hz), 脉宽0.5ms-2.5ms对应0-180度.
 *
 *********************************************************************/
#include "bsp_servo.h"

// 舵机引脚定义 (根据PIN.txt)
#define SERVO_PORT   GPIOC
#define SERVO_PIN    GPIO_Pin_5

/**
 * @brief  初始化舵机所连接的GPIO和TIM3_CH2 PWM.
 * @return none.
 */
void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // 2. 配置GPIO为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = SERVO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);

    // 3. 配置定时器基础设置 (TIM3)
    // 周期为20ms (50Hz), ARR=19999, PSC=71
    // F_pwm = F_clk / ((ARR+1)*(PSC+1)) = 72MHz / (20000 * 72) = 50Hz
    TIM_TimeBaseStructure.TIM_Period = 19999;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 4. 配置PWM输出模式 (TIM3_CH2)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始脉宽为0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);

    // 5. 使能TIM3的CH2预装载寄存器
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    // 6. 使能TIM3
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief  控制舵机旋转到指定角度.
 * @param  angle - 目标角度 (0 - 180).
 * @return none.
 */
void Servo_SetAngle(u8 angle)
{
    // 将角度(0-180)映射到脉宽(500-2500)
    // 脉宽 = 500 + angle * (2000 / 180)
    float pulse = (500.0f + angle * (2000.0f / 180.0f));
    TIM_SetCompare2(TIM3, (u16)pulse);
} 