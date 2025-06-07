/*********************************************************************
 * @file      udp_client.h
 * @author    Gemini
 * @brief     UDP客户端应用模块的头文件.
 * @version   1.0
 * @date      2025-06-07
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#ifndef __UDP_CLIENT_H
#define __UDP_CLIENT_H

#include "wchnet.h"
#include "eth_driver.h"

/**
 * @brief  初始化UDP客户端, 并注册socket中断回调函数.
 * @param  p_socket_callback - 指向socket中断回调函数的指针.
 * @return u8 - ERR_SUCCESS (0) 表示成功, 其他表示失败.
 */
u8 UDP_Client_Init(void (*p_socket_callback)(u8 sockeid, u8 intstat));

/**
 * @brief  向目标IP和端口发送数据.
 * @param  p_data - 指向要发送数据的指针.
 * @param  len    - 要发送的数据长度.
 * @return u8     - WCHNET库的发送结果.
 */
u8 UDP_Client_Send(const u8 *p_data, u16 len);

/**
 * @brief  WCHNET全局中断处理函数, 应在主循环中定期调用.
 * @return none.
 */
void UDP_Client_Handle_GlobalInt(void);

/**
 * @brief  检查是否可以发送UDP数据 (2秒间隔).
 * @return u8 - 1: 可以发送, 0: 时间未到.
 */
u8 UDP_Client_Can_Send(void);

#endif 