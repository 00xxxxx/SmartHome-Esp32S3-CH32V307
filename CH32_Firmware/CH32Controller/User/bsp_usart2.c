/*********************************************************************
 * @file      bsp_usart2.c
 * @author    Gemini
 * @brief     USART2 (Zigbee) 模块的实现文件.
 * @version   1.0
 * @date      2025-06-08
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "bsp_usart2.h"

// 环形缓冲区大小
#define RX_BUF_SIZE 16
static u8 RxBuffer[RX_BUF_SIZE];
static u8 RxWrite = 0;
static u8 RxRead = 0;

/**
 * @brief  初始化UART2及其中断.
 * @return none.
 */
void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};
    NVIC_InitTypeDef NVIC_InitStructure = {0};

    // 1. 使能时钟 (USART2, GPIOA)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. 配置GPIO (PA2-Tx, PA3-Rx)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; // Tx
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; // Rx
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. 配置USART2
    USART_InitStructure.USART_BaudRate = ZIGBEE_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    // 4. 配置并使能接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. 使能USART2
    USART_Cmd(USART2, ENABLE);
}

/**
 * @brief  从接收缓冲区获取一个字节.
 * @return 1: 成功获取数据, 0: 缓冲区为空.
 */
u8 USART2_GetData(u8 *data)
{
    if (RxRead != RxWrite)
    {
        *data = RxBuffer[RxRead];
        RxRead = (RxRead + 1) % RX_BUF_SIZE;
        return 1;
    }
    return 0;
}


/**
 * @brief  USART2中断服务函数的回调.
 * @note   此函数需要在 ch32v30x_it.c 的 USART2_IRQHandler 中被调用.
 * @return none.
 */
void USART2_IRQHandler_Callback(void)
{
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        u8 res = USART_ReceiveData(USART2);
        
        // 存入环形缓冲区
        if ((RxWrite + 1) % RX_BUF_SIZE != RxRead)
        {
            RxBuffer[RxWrite] = res;
            RxWrite = (RxWrite + 1) % RX_BUF_SIZE;
        }
        // 如果缓冲区满了，则放弃该数据 (或在此添加错误处理)
    }
} 