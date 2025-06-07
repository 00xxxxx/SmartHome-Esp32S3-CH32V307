/*********************************************************************
 * @file      bsp_led.c
 * @author    Gemini
 * @brief     板载LED驱动模块的实现文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "bsp_led.h"

// LED引脚定义 (根据PIN.txt)
#define LED1_PORT   GPIOC
#define LED1_PIN    GPIO_Pin_3
#define LED2_PORT   GPIOC
#define LED2_PIN    GPIO_Pin_4

// 存储每个LED的端口和引脚信息，方便函数调用
GPIO_TypeDef* LED_PORT[] = {LED1_PORT, LED2_PORT};
uint16_t LED_PIN[] = {LED1_PIN, LED2_PIN};


/**
 * @brief  初始化LED所连接的GPIO引脚.
 * @return none.
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 使能GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 配置PC3和PC4为推挽输出
    GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // 初始化后默认熄灭所有LED
    GPIO_SetBits(GPIOC, LED1_PIN | LED2_PIN); // 假设高电平熄灭
}

/**
 * @brief  点亮指定的LED.
 * @param  led_id - 要操作的LED编号 (LED_ID).
 * @return none.
 */
void LED_On(LED_ID led_id)
{
    // 假设低电平点亮
    GPIO_ResetBits(LED_PORT[led_id], LED_PIN[led_id]);
}

/**
 * @brief  熄灭指定的LED.
 * @param  led_id - 要操作的LED编号 (LED_ID).
 * @return none.
 */
void LED_Off(LED_ID led_id)
{
    // 假设高电平熄灭
    GPIO_SetBits(LED_PORT[led_id], LED_PIN[led_id]);
}

/**
 * @brief  翻转指定LED的状态.
 * @param  led_id - 要操作的LED编号 (LED_ID).
 * @return none.
 */
void LED_Toggle(LED_ID led_id)
{
    u8 old_state = GPIO_ReadOutputDataBit(LED_PORT[led_id], LED_PIN[led_id]);
    if (old_state == Bit_SET)
    {
        GPIO_ResetBits(LED_PORT[led_id], LED_PIN[led_id]);
    }
    else
    {
        GPIO_SetBits(LED_PORT[led_id], LED_PIN[led_id]);
    }
} 