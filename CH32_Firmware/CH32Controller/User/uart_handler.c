/*********************************************************************
 * @file      uart_handler.c
 * @author    Gemini
 * @brief     串口命令处理模块的实现文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "uart_handler.h"
#include "string.h"
#include "bsp_led.h"
#include "bsp_buzzer.h"
#include "bsp_servo.h"

// 串口接收缓冲区
#define RX_BUF_SIZE 64
u8 RxBuffer[RX_BUF_SIZE];
u8 RxCounter = 0;

/**
 * @brief  解析并执行接收到的命令.
 * @param  cmd - 指向命令字符串的指针.
 * @return none.
 */
static void Parse_Command(const char *cmd)
{
    if (strcmp(cmd, "LED2ON") == 0)
    {
        LED_On(LED2);
    }
    else if (strcmp(cmd, "LED2OFF") == 0)
    {
        LED_Off(LED2);
    }
    else if (strcmp(cmd, "RecSuccess") == 0) // 根据您的描述，这里假设是 RecSuccess
    {
        Servo_SetAngle(90); // 开锁
    }
    else if (strcmp(cmd, "ReFail") == 0)
    {
        Buzzer_On();
        Delay_Ms(1000); // 鸣叫1秒
        Buzzer_Off();
    }
}

/**
 * @brief  初始化UART1及其中断.
 * @return none.
 */
void UART_Handler_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 2. 配置GPIO (PA9-Tx, PA10-Rx)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置USART1
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // 4. 配置并使能接收中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. 使能USART1
    USART_Cmd(USART1, ENABLE);
}


/**
 * @brief  USART1中断服务函数.
 * @note   此函数需要在 ch32v30x_it.c 中被调用.
 * @return none.
 */
void USART1_IRQHandler_Callback(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        u8 res = USART_ReceiveData(USART1);

        if(RxCounter < RX_BUF_SIZE)
        {
            // 使用\r\n作为结束符
            if(res == '\n' || res == '\r')
            {
                if(RxCounter > 0)
                {
                    RxBuffer[RxCounter] = '\0'; // 添加字符串结束符
                    Parse_Command((const char*)RxBuffer);
                    RxCounter = 0; // 重置计数器
                }
            }
            else
            {
                RxBuffer[RxCounter++] = res;
            }
        }
        else
        {
            RxCounter = 0; // 缓冲区溢出，重置
        }
    }
} 