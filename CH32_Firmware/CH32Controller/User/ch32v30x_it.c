/********************************** (C) COPYRIGHT *******************************
* File Name          : ch32v30x_it.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2024/03/06
* Description        : Main Interrupt Service Routines.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/
#include "ch32v30x_it.h"
#include "dht11.h"
#include "udp_client.h"
#include "bsp_led.h"
#include "bsp_usart2.h"
#include "zigbee_handler.h"

// 为中断处理函数声明外部回调
extern void USART1_IRQHandler_Callback(void);
extern void USART2_IRQHandler_Callback(void);
extern void SysTick_Handler_Callback(void);

void NMI_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

void SysTick_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void EXTI0_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART1_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void USART2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

/*********************************************************************
 * @fn      NMI_Handler
 *
 * @brief   This function handles NMI exception.
 *
 * @return  none
 */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      HardFault_Handler
 *
 * @brief   This function handles Hard Fault exception.
 *
 * @return  none
 */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/*********************************************************************
 * @fn      SysTick_Handler
 *
 * @brief   系统滴答定时器中断服务程序.
 * @note    由 `Delay_Init()` 启动，每1ms中断一次.
 *          主要用于为 `SysTick_Get_Ms()` 提供毫秒级时基.
 *
 * @return  none
 */
void SysTick_Handler(void)
{
    SysTick_Handler_Callback();
}

/*********************************************************************
 * @fn      EXTI0_IRQHandler
 *
 * @brief   外部中断0服务程序.
 * @note    连接到 KEY 按键 (PA0)，下降沿触发.
 *          主要功能:
 *          1. 手动清除由Zigbee模块触发的报警状态.
 *          2. 翻转LED1的亮灭状态作为按键反馈.
 *
 * @return  none
 */
void EXTI0_IRQHandler(void)
{
    // 按键消抖
    Delay_Ms(20);
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // 1. 调用接口清除报警
        Zigbee_Clear_Alarm();

        // 2. 翻转LED1状态 (提供按键反馈)
        LED_Toggle(LED1);

        // 3. 清除中断标志
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/*********************************************************************
 * @fn      TIM2_IRQHandler
 *
 * @brief   定时器2中断服务程序.
 * @note    此中断由WCH-NET协议栈库使用，为其内部超时机制提供时基.
 *
 * @return  none
 */
void TIM2_IRQHandler(void)
{
    WCHNET_TimeIsr(WCHNETTIMERPERIOD);
    TIM_ClearITPendingBit( TIM2, TIM_IT_Update );
}

/*********************************************************************
 * @fn      USART1_IRQHandler
 *
 * @brief   串口1中断服务程序.
 * @note    当通过串口1接收到数据时触发.
 *          具体的数据处理和命令解析在 `uart_handler.c` 的回调函数中完成.
 *
 * @return  none
 */
void USART1_IRQHandler(void)
{
    USART1_IRQHandler_Callback();
}

/*********************************************************************
 * @fn      USART2_IRQHandler
 *
 * @brief   串口2中断服务程序.
 * @note    当通过串口2接收到Zigbee数据时触发.
 *          具体的数据处理在 `bsp_usart2.c` 的回调函数中完成.
 *
 * @return  none
 */
void USART2_IRQHandler(void)
{
    USART2_IRQHandler_Callback();
}


