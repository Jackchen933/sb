# OV-Watch 智能手表（STM32F411 + FreeRTOS + LVGL）

基于 STM32F411CEU6 的智能手表固件，跑 FreeRTOS 实时操作系统 + LVGL v8 图形库，集成心率血氧（EM7028）、六轴姿态（MPU6050）、温湿度（AHT21）、气压（SPL06）、磁力计（LSM303）等多传感器，支持抬腕亮屏、分级低功耗、蓝牙 BLE 透传。

> 本仓库只保留手写的业务代码与板级驱动（LVGL / HAL 等第三方库未收录），工程骨架见 `OV_Watch.ioc`（CubeMX）与 `Makefile`。

## 核心技术点导航

### 1. 刷屏链路异步化（SPI DMA + 双缓冲 + 二值信号量 + 零拷贝）

定位 LVGL 刷屏时 CPU 忙等空转的性能问题，重构为异步流水线：

- `LVGL_porting/lv_port_disp.c` —— `disp_flush` 启动 DMA 后用二值信号量阻塞休眠（不再忙等）；`HAL_SPI_TxCpltCallback` 中断里释放信号量唤醒任务；LVGL 渲染缓冲区指针直接交给 DMA，零拷贝
- `BSP/LCD/lcd.c` —— LCD 驱动与异步填充接口

### 2. FreeRTOS 任务间通信四件套（信号量 / 互斥量 / 消息队列 / 邮箱）

- 信号量：DMA 刷屏完成通知（`LVGL_porting/lv_port_disp.c`）
- 互斥量：保护 `HWInterface` 全局数据结构，带优先级继承（`User/Func/Src/HWDataAccess.c`）
- 消息队列：按键 / 息屏 / 界面刷新等事件流水（`User/Tasks/Src/` 各任务）
- 邮箱（深度 1 队列）：抬腕亮屏请求，只保留最新状态，天然不积压（`User/Tasks/Src/user_SensUpdateTask.c`）

### 3. 分级日志系统（EasyLogger 思想 + SEGGER RTT）

六级日志（A/E/W/I/D/V），带时间戳 / 线程名 / 文件行号 / 标签，编译期裁剪 + 运行期过滤双闸：

- `BSP/Debug/elog.c` `BSP/Debug/elog.h` —— 日志核心模块
- `BSP/Debug/SEGGER_RTT.c/.h` —— RTT 非阻塞输出（写内存环形缓冲，J-Link 后台搬运，不占用串口）
- 埋点按三条链路覆盖：初始化链路（成功失败成对输出）、低功耗状态机（I 级状态迁移）、高频数据链路（D/V 级默认过滤）

### 4. 低功耗状态机

- `User/Tasks/Src/user_RunModeTasks.c` —— IdleEnterTask（无操作降亮 5%）→ StopEnterTask（熄屏 + 外设反初始化 + 进 STOP 模式，唤醒后重配时钟并喂看门狗）

### 5. 传感器应用层架构

驱动层（`BSP/` 各传感器）→ 算法层（厂商算法库）→ 应用层（调度 / 按页面按需使能 / 合理性校验）：

- `BSP/EM7028/` 心率血氧、`BSP/MPU6050/` 六轴与抬腕检测、`BSP/AHT21/` 温湿度、`BSP/SPL06_001/` 气压、`BSP/LSM303DLH/` 磁力计
- 传感器按页面使能 / 休眠，降低整机功耗

## 目录结构

```
├── BSP/                 # 板级驱动：LCD、触摸、按键、传感器、电源、看门狗、elog/RTT
├── Core/                # CubeMX 生成的内核代码（main、时钟、中断，含 USART1 IDLE 蓝牙收帧改造）
├── User/
│   ├── Tasks/           # FreeRTOS 任务：硬件初始化、喂狗、低功耗状态机、传感器更新、按键、LVGL 处理
│   ├── Func/            # 业务功能：硬件数据访问层（互斥量保护）
│   └── GUI_App/         # LVGL 界面：21 个页面（表盘 / 菜单 / 心率 / 计算器 / 2048 / 设置等）
├── SYSTEM/              # 系统延时 / UART 基础函数
├── LVGL_porting/        # LVGL 显示与触摸移植层（刷屏异步化改造所在）
├── OV_Watch.ioc         # CubeMX 工程配置
├── Makefile             # GCC 构建
└── STM32F411CEUx_FLASH.ld
```

## 构建

LVGL v8.2 与 STM32 HAL 库为第三方依赖未收录，放回对应目录后可用 Keil MDK 或 `make` 构建。硬件平台：STM32F411CEU6，屏幕 ST7789（240×280 SPI），触摸 CST816，蓝牙为串口透传模块。
