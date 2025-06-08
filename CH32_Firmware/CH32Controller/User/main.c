/********************************************************************************
 * @file      main.c
 * @author    Gemini
 * @brief     智能家居外设控制器主程序.
 * @version   3.0 (Modular Zigbee)
 * @date      2025-06-08
 *
 * @copyright Copyright (c) 2025
 *
 * @par       项目架构 (Project Architecture):
 *            本程序采用模块化设计，将硬件驱动、传感器管理和应用逻辑分离。
 *            - BSP层 (bsp_*.c/.h): 板级支持包，提供对LED、蜂鸣器、按键、舵机
 *              等基础外设的原子操作接口。
 *            - Sensor层 (bsp_sensors.c/.h): 统一管理光敏、红外、烟雾等传感器，
 *              负责其初始化和数据读取。
 *            - Protocol层 (udp_client.c/.h): 封装网络通信协议，实现UDP数据的
 *              打包和发送。
 *            - Handler层 (uart_handler.c/.h, zigbee_handler.c/.h): 负责解析和处理
 *              来自特定接口（如串口1、Zigbee模块）的指令和数据。
 *            - App层 (main.c): 作为顶层应用，负责初始化所有模块，并在主循环中
 *              调度各个任务，实现核心业务逻辑。
 *
 * @par       核心逻辑 (Core Logic):
 *            1. 系统初始化: 调用 System_Init() 初始化所有硬件和模块。
 *            2. 网络初始化: 调用 UDP_Client_Init() 初始化以太网和UDP协议栈。
 *            3. 主循环 (while(1)):
 *               - WCHNET_MainTask(): WCH-NET协议栈的核心轮询任务。
 *               - UDP数据上报: 定期读取DHT11温湿度，并通过UDP发送。
 *               - 本地光感任务: 定期巡检光敏传感器，并根据阈值自动控制LED1。
 *               - Zigbee报警任务: 监听并处理来自远程Zigbee节点的报警信息，
 *                 实现持续鸣叫报警及按键消警功能。
 *
 * @par       中断服务 (Interrupt Services in ch32v30x_it.c):
 *            - EXTI0_IRQHandler: 按键(KEY)中断，用于手动翻转LED1及清除Zigbee报警。
 *            - USART1_IRQHandler: 串口1接收中断，用于处理来自ESP32的远程控制指令。
 *            - USART2_IRQHandler: 串口2接收中断，用于接收来自Zigbee协调器的数据。
 *            - SysTick_Handler: 系统滴答定时器，为非阻塞延时提供时基。
 *            - TIM2_IRQHandler: 通用定时器2，为WCH-NET协议栈提供时基。
 *
 ********************************************************************************/
#include "debug.h"
#include "WCHNET.h"
#include "string.h"
#include "eth_driver.h"
#include "dht11.h"
#include "udp_client.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_buzzer.h"
#include "bsp_servo.h"
#include "bsp_sensors.h"
#include "uart_handler.h"
#include "zigbee_handler.h"

/* 为WCHNET库中定义的全局变量提供外部声明 */
extern u8 IPAddr[4];

// 光敏电阻阈值，低于此值认为天黑
#define PHOTORES_THRESHOLD  1000

/**
 * @brief  Socket事件回调函数 (当前未使用).
 * @param  sockeid - socket id.
 * @param  intstat - 中断状态.
 * @return none
 */
static void App_Socket_Callback(u8 sockeid, u8 intstat)
{
    // 可在此处添加socket事件处理逻辑，如数据接收
}

/**
 * @brief  系统外设初始化.
 * @return none
 */
static void System_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    LED_Init();
    Buzzer_Init();
    Key_Init();
    Sensors_Init();
    UART_Handler_Init();
    Zigbee_Handler_Init();
    Servo_Init();
    DHT11_Init();

    /* 初始化舵机到关锁位置(0度) */
    Servo_SetAngle(0);
}

/**
 * @brief  本地传感器处理任务.
 * @return none
 */
static void Sensor_Task(void)
{
    static u32 last_run_time = 0;

    // 每200ms检测一次
    if(SysTick_Get_Ms() - last_run_time < 200)
    {
        return;
    }
    last_run_time = SysTick_Get_Ms();

    // 1. 光敏传感器 -> LED1
    if(Photoresistor_Get_Val() < PHOTORES_THRESHOLD)
    {
        LED_On(LED1); // 天黑开灯
    }
    else
    {
        LED_Off(LED1); // 天亮关灯
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   主函数
 *
 * @return  none
 */
int main(void)
{
    u8 dht_temp, dht_humi;
    char udp_buf[50];

    System_Init();
    printf("Welcome to CH32Controller V3.0\r\n");

    /* 初始化网络并等待IP分配 */
    if (UDP_Client_Init(App_Socket_Callback) != 0)
    {
        printf("WCHNET init failed.\r\n");
        while(1);
    }
    printf("WCHNET init success. IP: %d.%d.%d.%d\r\n", IPAddr[0], IPAddr[1], IPAddr[2], IPAddr[3]);


    while(1)
    {
        /* 网络核心任务 */
        WCHNET_MainTask();
        UDP_Client_Handle_GlobalInt();

        /* 传感器数据上报任务 (UDP) */
        if(UDP_Client_Can_Send())
        {
            if(DHT11_Read_Data(&dht_temp, &dht_humi) == 0)
            {
                sprintf(udp_buf, "Hello PC, Temp: %dC, Humi: %d%%", dht_temp, dht_humi);
                UDP_Client_Send((u8*)udp_buf, strlen(udp_buf));
            }
        }

        /* 本地传感器逻辑任务 */
        Sensor_Task();

        /* Zigbee报警应用逻辑任务 */
        Zigbee_Handler_Task();
    }
}

