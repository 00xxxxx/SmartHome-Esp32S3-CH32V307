# 基于ESP32-S3与CH32V307的人脸识别智能家居系统

## 项目描述

本项目是一套采用双MCU架构的低功耗、智能化家居系统。系统以 ESP32-S3 为主控制器，负责算力密集型任务，如人脸识别和语音处理；以 CH32V307 (RISC-V) 为从控制器，专注于高实时性的外设驱动与环境数据采集。

该系统不仅支持通过人脸识别控制门锁、管理用户权限及处理异常开锁情况，还集成了语音指令控制、定时环境数据上报、火灾与入侵联合报警、以及基于环境光感的自动灯光调控。整体架构设计旨在实现优秀的稳定性、实时性和未来功能扩展性。

---

## 系统核心功能

- 人脸识别门禁
    - 使用 `OV2640` 摄像头实时采集图像。
    - 利用乐鑫 `ESP-WHO` AI框架在 `ESP32-S3` 上实现人脸录入、检测与识别。
    - 根据识别结果向下级控制器发送开锁或拒绝指令，并支持权限管理。

- 语音指令控制
    - 集成 `INMP441` 高信噪比麦克风阵列进行语音采集。
    - 通过 `ESP-SR` 语音识别框架处理语音信号，解析为控制指令。
    - 将解析后的指令通过串口下发，实现对家居设备的语音控制。

- 环境监测与上报
    - `CH32V307` 连接 `DHT11` 传感器，周期性采集温湿度数据。
    - 通过板载以太网模块，以 `UDP` 协议将环境数据定时上报至云端或本地服务器，便于远程监控。

- 自动化安防与调控
    - 集成光敏电阻、`MQ`系列烟雾传感器和 `HC-SR501` 人体红外传感器。
    - 智能照明: 根据环境光照强度自动开关灯光。
    - 复合报警: 当烟雾传感器与人体红外传感器同时触发时，判定为火灾或入侵，驱动蜂鸣器进行高优先级报警。

- 多任务实时管理
    - `ESP32-S3` 侧采用 `FreeRTOS` 操作系统，将摄像头采集、人脸识别、语音处理、数据通信等功能模块作为独立任务进行并行管理，确保系统在高负载下的稳定性和响应速度。

---

## 系统架构与技术栈

### 双MCU架构
- 主控制器 (Master): `ESP32-S3`
    - 职责: 运行FreeRTOS、执行人脸/语音识别算法、处理复杂逻辑、并通过串口向从机下发指令。
- 从控制器 (Slave): `CH32V307`
    - 职责: 接收并校验主机指令、直接驱动所有外设（LED, 舵机, 蜂鸣器）、采集各类传感器数据并执行本地的自动化逻辑、通过以太网上报数据。
- 通信协议:
    - 两者之间采用 `UART` 串口通信。数据包包含帧头、命令字、数据负载和校验和，确保了指令传输的稳定与可靠。

### 技术栈清单
| 类别 | 技术点 |
| :--- | :--- |
| **硬件平台** | `ESP32-S3`, `CH32V307 (RISC-V)`, `OV2640`, `INMP441`, `DHT11` |
| **核心框架** | `ESP-IDF`, `ESP-WHO` (人脸识别), `ESP-SR` (语音识别), `FreeRTOS` |
| **通信接口** | `Ethernet` (UDP), `UART`, `I2C`, `SPI`, `单总线` |
| **开发工具** | `MounRiver Studio` (for CH32), `VS Code` with `ESP-IDF Plugin` |
| **开发语言** | `C/C++` |
| **操作系统** | `Ubuntu` (for ESP-IDF development) |

---

## 固件详情

### 1. CH32V307 从机固件

这部分是项目的外设控制和数据采集核心。

- 代码位置: 所有源码及MounRiver工程文件均位于 `CH32_Firmware/` 目录下。
- 功能:
    1.  初始化所有连接的传感器和执行器。
    2.  监听来自 `ESP32-S3` 的串口指令，并执行相应动作 (如 `LED2ON`, `ReeSuccess`)。
    3.  运行一个本地任务循环 (`Sensor_Task`)，处理自动光控和报警逻辑。
    4.  通过WCH-NET协议栈，以DHCP方式连接以太网，并定时发送UDP数据包。
- 如何编译和下载:
    1.  使用 MounRiver Studio 导入 `CH32_Firmware/CH32Controller/` 工程。
    2.  编译并使用 WCH-LinkE 下载。

### 2. ESP32-S3 主机固件

这部分是项目的智能处理核心，负责运行AI算法和系统主逻辑。

- 代码位置: 所有源码及ESP-IDF工程文件均位于 `ESP32_Firmware/` 目录下。 (请根据您的实际目录结构调整)
- 开发环境: `ESP-IDF` (推荐 V5.0及以上)
- 核心任务:
    - Camera Task: 使用 `esp_camera` 组件驱动 `OV2640` 进行图像采集。
    - Face-Rec Task: 将图像帧送入 `ESP-WHO` 框架进行人脸检测与识别。
    - Voice-Rec Task: 初始化 `INMP441` 麦克风，并使用 `ESP-SR` 进行语音识别。
    - Control Task: 根据识别结果，或接收到的其他指令，组装命令并通过串口发送给 `CH32V307`。
- 如何编译和下载:
    1.  参考[官方文档](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html)安装 ESP-IDF 并配置好开发环境。
    2.  在终端中进入 ESP32 的固件目录 (例如 `ESP32_Firmware/`)。
    3.  运行 `idf.py build` 进行编译。
    4.  运行 `idf.py -p (您的串口号) flash monitor` 来下载固件并查看日志。

---

## 未来展望

- Zigbee 网关: 考虑在 `ESP32-S3` 上扩展Zigbee支持，通过集成Zigbee模块将系统接入更广泛的Zigbee生态智能家居设备，实现更全面的联动控制。
- 云平台对接: 将数据上报协议从UDP升级为MQTT，对接主流公有云平台（如阿里云、腾讯云），实现真正的远程设备管理和数据可视化。

---

## CH32V307 硬件引脚分配

```
=========================================================
           CH32V307EVT Pinout Configuration
=========================================================

--- Network (Ethernet RMII Interface) ---
- PA1: RMII_REF_CLK
- PA2: RMII_MDIO
- PA7: RMII_CRS_DV
- PC1: RMII_MDC
- PC4: RMII_RXD0
- PC5: RMII_RXD1
- PG11: RMII_TX_EN
- PG13: RMII_TXD0
- PG14: RMII_TXD1

--- User LEDs ---
- LED1 (Auto Light & Manual Control): PB0
- LED2 (UART Command Control): PB1

--- User KEY ---
- KEY1 (Manual LED1 Toggle): PA0 -> EXTI0

--- Actuators ---
- Buzzer: PB2
- Servo (Door Lock): PB4 -> TIM3_CH1

--- Sensors ---
- DHT11 (Temperature & Humidity): PA5
- Photoresistor (Light Sensor): PA6 -> ADC1_IN6
- PIR (Human Body Infrared): PA3
- Smoke Sensor: PA4

--- Debug & Control ---
- USART1_TX: PA9
- USART1_RX: PA10
=========================================================
``` 