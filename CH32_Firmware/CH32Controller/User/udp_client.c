/*********************************************************************
 * @file      udp_client.c
 * @author    Gemini
 * @brief     UDP客户端模块的实现文件 (兼容旧版API).
 * @version   2.4
 * @date      2025-06-08
 *
 * @copyright Copyright (c) 2025
 *
 *********************************************************************/
#include "udp_client.h"
#include "WCHNET.h"
#include <string.h>
#include <stdio.h>
#include "debug.h"

/*
 *********************************************************************************
 *                                  宏定义与全局变量
 *********************************************************************************
 */
#define UDP_CLIENT_PORT     1000
#define UDP_SERVER_PORT     2000

u8 DestIP[4] = {192, 168, 1, 10};
static u8 SocketId;
static void (*p_app_socket_callback)(u8 sockeid, u8 intstat) = NULL;

/*
 *********************************************************************************
 *                                  内部函数
 *********************************************************************************
 */

/**
 * @brief  内部Socket中断处理函数.
 * @note   此函数会调用上层应用注册的回调函数.
 * @param  socketid - 发生中断的socket id.
 * @param  intstat - 中断状态.
 * @return none.
 */
static void WCHNET_HandleSockInt_Private(u8 socketid, u8 intstat)
{
    if(p_app_socket_callback)
    {
        p_app_socket_callback(socketid, intstat);
    }
}

/*
 *********************************************************************************
 *                                  公共函数
 *********************************************************************************
 */

/**
 * @brief  初始化UDP客户端, 并注册socket中断回调函数.
 * @param  socket_callback - 指向socket中断回调函数的指针.
 * @return u8 - WCHNET_ERR_SUCCESS (0) 表示成功, 其他表示失败.
 */
u8 UDP_Client_Init(void (*socket_callback)(u8 sockeid, u8 intstat))
{
    u8 i;
    SOCK_INF TmpSocketInf;

    p_app_socket_callback = socket_callback;

    // 清空socket信息结构
    memset(&TmpSocketInf, 0, sizeof(SOCK_INF));

    // 配置Socket信息
    memcpy(TmpSocketInf.IPAddr, DestIP, 4);
    TmpSocketInf.DesPort  = UDP_SERVER_PORT;
    TmpSocketInf.SourPort = UDP_CLIENT_PORT;
    TmpSocketInf.ProtoType = PROTO_TYPE_UDP; // 使用 ProtoType
    TmpSocketInf.RecvBufLen = RECE_BUF_LEN;

    // 创建Socket
    i = WCHNET_SocketCreat(&SocketId, &TmpSocketInf);
    if (i != WCHNET_ERR_SUCCESS) {
        printf("UDP Socket Create Failed: %02X\r\n", i);
        return i;
    }

    return WCHNET_ERR_SUCCESS;
}


/**
 * @brief  WCHNET全局中断处理函数, 应在主循环中定期调用.
 * @return none.
 */
void UDP_Client_Handle_GlobalInt(void)
{
    u8 intstat = WCHNET_GetGlobalInt();
    if(intstat)
    {
        if (intstat & GINT_STAT_SOCKET)
        {
            for (u8 i = 0; i < WCHNET_MAX_SOCKET_NUM; i++)
            {
                u8 socketint = WCHNET_GetSocketInt(i);
                if (socketint)
                {
                    WCHNET_HandleSockInt_Private(i, socketint);
                }
            }
        }
    }
}


/**
 * @brief  向目标IP和端口发送数据.
 * @param  p_data - 指向要发送数据的指针.
 * @param  len    - 要发送的数据长度.
 * @return u8     - WCHNET库的发送结果.
 */
u8 UDP_Client_Send(const u8 *p_data, u16 len)
{
    u32 send_len = len;
    return WCHNET_SocketSend(SocketId, (u8 *)p_data, &send_len);
}


/**
 * @brief  检查是否可以发送UDP数据 (2秒间隔).
 * @return u8 - 1: 可以发送, 0: 时间未到.
 */
u8 UDP_Client_Can_Send(void)
{
    static u32 last_send_time = 0;

    if (SysTick_Get_Ms() - last_send_time >= 2000)
    {
        last_send_time = SysTick_Get_Ms();
        return 1;
    }
    return 0;
} 