/*********************************************************************
 * @file      bsp_sensors.c
 * @author    Gemini
 * @brief     板载传感器统一管理模块的实现文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "bsp_sensors.h"

// 引脚定义 (根据PIN.txt)
#define PHOTORES_PORT   GPIOA           // 光敏电阻
#define PHOTORES_PIN    GPIO_Pin_2
#define PHOTORES_ADC_CH ADC_Channel_2

#define PIR_PORT        GPIOA           // 人体红外
#define PIR_PIN         GPIO_Pin_13

#define SMOKE_PORT      GPIOC           // 烟雾
#define SMOKE_PIN       GPIO_Pin_0

/**
 * @brief  初始化传感器所连接的GPIO和ADC.
 * @return none.
 */
void Sensors_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    ADC_InitTypeDef ADC_InitStructure = {0};

    // 1. 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1, ENABLE);

    // 2. 配置数字输入引脚 (PIR, Smoke)
    GPIO_InitStructure.GPIO_Pin = PIR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 根据传感器模块特性，可能需要上拉或浮空
    GPIO_Init(PIR_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SMOKE_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(SMOKE_PORT, &GPIO_InitStructure);

    // 3. 配置模拟输入引脚 (Photoresistor)
    GPIO_InitStructure.GPIO_Pin = PHOTORES_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(PHOTORES_PORT, &GPIO_InitStructure);

    // 4. 配置ADC1
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    // 5. 使能ADC1并校准
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
}

/**
 * @brief  获取光敏传感器的ADC原始读数.
 * @return u16 - ADC转换值 (0-4095).
 */
u16 Photoresistor_Get_Val(void)
{
    // 配置规则通道
    ADC_RegularChannelConfig(ADC1, PHOTORES_ADC_CH, 1, ADC_SampleTime_239Cycles5);
    // 启动软件转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    // 等待转换完成
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    // 返回转换结果
    return ADC_GetConversionValue(ADC1);
}

/**
 * @brief  检测人体红外传感器是否触发.
 * @return u8 - 1: 检测到人, 0: 未检测到人.
 */
u8 PIR_Is_Triggered(void)
{
    // 假设高电平触发
    if (GPIO_ReadInputDataBit(PIR_PORT, PIR_PIN) == Bit_SET)
    {
        return 1;
    }
    return 0;
}

/**
 * @brief  检测烟雾传感器是否触发.
 * @return u8 - 1: 检测到烟雾, 0: 未检测到烟雾.
 */
u8 Smoke_Is_Triggered(void)
{
    // 假设高电平触发
    if (GPIO_ReadInputDataBit(SMOKE_PORT, SMOKE_PIN) == Bit_SET)
    {
        return 1;
    }
    return 0;
} 