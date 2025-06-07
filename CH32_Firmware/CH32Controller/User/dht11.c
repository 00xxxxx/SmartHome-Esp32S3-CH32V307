/*********************************************************************
 * @file      dht11.c
 * @author    Gemini
 * @brief     DHT11温湿度传感器驱动模块的实现文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "dht11.h"

// DHT11 数据线连接的GPIO引脚 (PC1)
#define DHT11_DATA_PORT     GPIOC
#define DHT11_DATA_PIN      GPIO_Pin_1

// 模块内部静态函数声明
static void DH11_GPIO_Init_OUT(void);
static void DH11_GPIO_Init_IN(void);
static void DHT11_Start(void);
static u8 DHT11_Rec_Byte(void);

/**
 * @brief  初始化DHT11传感器所使用的GPIO引脚.
 *
 * @return none.
 */
void DHT11_Init(void)
{
    // 使能GPIOC时钟，虽然上层main中可能已开启，但为确保模块独立性此处再次调用
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 初始状态设置为输入模式，以检测总线空闲状态
    DH11_GPIO_Init_IN();
}

/**
 * @brief  将DHT11数据引脚配置为推挽输出模式.
 * @note   此为模块内部函数.
 * @return none.
 */
static void DH11_GPIO_Init_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_DATA_PORT, &GPIO_InitStructure);
}

/**
 * @brief  将DHT11数据引脚配置为浮空输入模式.
 * @note   此为模块内部函数.
 * @return none.
 */
static void DH11_GPIO_Init_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin   = DHT11_DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_DATA_PORT, &GPIO_InitStructure);
}

/**
 * @brief  主机向DHT11发送起始信号.
 * @note   此为模块内部函数.
 * @return none.
 */
static void DHT11_Start(void)
{
    DH11_GPIO_Init_OUT();                           // 切换为输出模式
    GPIO_ResetBits(DHT11_DATA_PORT, DHT11_DATA_PIN);// 主机拉低总线
    Delay_Ms(20);                                   // 延时至少18ms
    GPIO_SetBits(DHT11_DATA_PORT, DHT11_DATA_PIN);  // 主机拉高总线
    Delay_Us(30);                                   // 延时20-40us
    DH11_GPIO_Init_IN();                            // 切换为输入模式，准备接收DHT11的响应
}

/**
 * @brief  从DHT11接收一个字节的数据.
 * @note   此为模块内部函数.
 * @return u8 - 接收到的8位数据.
 */
static u8 DHT11_Rec_Byte(void)
{
    u8 i, data = 0;
    for (i = 0; i < 8; i++)
    {
        // 等待数据位的50us低电平结束
        while (GPIO_ReadInputDataBit(DHT11_DATA_PORT, DHT11_DATA_PIN) == 0);
        // 延时30us，用于判断是数据'0'还是数据'1'
        Delay_Us(30);
        data <<= 1;
        // 如果延时后引脚仍为高电平，则表示接收到的是'1'
        if (GPIO_ReadInputDataBit(DHT11_DATA_PORT, DHT11_DATA_PIN) == 1)
        {
            data |= 1;
        }
        // 等待当前数据位的高电平结束
        while (GPIO_ReadInputDataBit(DHT11_DATA_PORT, DHT11_DATA_PIN) == 1);
    }
    return data;
}

/**
 * @brief  从DHT11传感器读取温湿度数据.
 *
 * @param  temp 指向用于存储温度值的指针 (u8).
 * @param  humi 指向用于存储湿度值的指针 (u8).
 *
 * @return u8 0: 成功, 1: 失败 (校验和错误或无响应).
 */
u8 DHT11_Read_Data(u8 *temp, u8 *humi)
{
    u8 buf[5];
    u8 i;

    DHT11_Start();
    // 等待并检测DHT11的响应信号（拉低总线80us）
    if (GPIO_ReadInputDataBit(DHT11_DATA_PORT, DHT11_DATA_PIN) == 0)
    {
        // 等待80us的低电平响应结束
        while (GPIO_ReadInputDataBit(DHT11_DATA_PORT, DHT11_DATA_PIN) == 0);
        // 等待80us的高电平准备信号结束
        while (GPIO_ReadInputDataBit(DHT11_DATA_PORT, DHT11_DATA_PIN) == 1);

        // 循环接收5个字节的数据（湿度整数、湿度小数、温度整数、温度小数、校验和）
        for(i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Rec_Byte();
        }

        // 校验数据和
        if ((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0]; // 湿度整数部分
            *temp = buf[2]; // 温度整数部分
            return 0;       // 读取成功
        }
    }
    return 1; // 读取失败
} 