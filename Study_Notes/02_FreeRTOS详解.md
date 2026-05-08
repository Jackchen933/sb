# 第二章：FreeRTOS 详解（零基础入门版）

> 本文档面向嵌入式基础一般的同学，从最基础的概念讲起，结合本项目代码

---

## 2.0 什么是 FreeRTOS？

### 现实生活中的类比

想象一下你一个人同时要做好几件事：
- 做饭
- 接电话
- 看孩子

**单线程（裸机）**：你只能一件事做完再做下一件
- 先做饭 → 饭糊了
- 先接电话 → 孩子哭了

**多线程（FreeRTOS）**：你可以在做饭间隙接电话
- 做饭时看锅 → 腾出手接电话 → 继续做饭
- 虽然是"同时"做多件事，但其实是快速切换

---

## 2.1 一些基本概念

### 2.1.1 任务（Task）

**任务**就像一个小程序，有自己的"栈"（存放临时变量）和"优先级"。

```c
// 这就是一个任务函数
void MyTask(void *argument)
{
    // 初始化代码（只执行一次）

    while(1)  // 无限循环，任务会一直运行
    {
        // 任务的主要工作
        printf("Hello!\n");

        // 延时，释放CPU给其他任务
        osDelay(1000);  // 延时1000ms
    }
}
```

**重要**：每个任务都必须有 `while(1)` 无限循环，因为任务不能"结束"。

---

### 2.1.2 调度器（Scheduler）

调度器就像一个"主持人"，负责决定：
- 现在让哪个任务运行？
- 每个任务运行多长时间？

FreeRTOS 使用**抢占式调度**：
- 高优先级任务可以"抢断"低优先级任务
- 相同优先级任务轮流执行（时间片）

```
优先级: 高 ←─────────────────→ 低
       TaskA (优先级3) ████████
       TaskB (优先级2)         ████████
       TaskC (优先级1)                 ████████████████
       
时间:   ───────────────────────────────────────────▶
```

---

### 2.1.3 队列（Queue）

队列是任务之间"传递消息"的管道。

```
TaskA (发消息) ──队列──▶ TaskB (收消息)
   osMessageQueuePut()    osMessageQueueGet()
```

特点：
- **先进先出**（FIFO）
- 可以设置队列长度
- 发送和接收可以是不同任务

---

### 2.1.4 延时函数

```c
osDelay(100);  // 延时100ms，当前任务会"睡觉"，CPU让给其他任务
```

**为什么需要延时？**
- 如果任务没有延时，会一直占用CPU，其他任务无法运行
- 延时让CPU有时间处理其他任务

---

## 2.2 本项目的 FreeRTOS 配置

文件位置：`Core/Inc/FreeRTOSConfig.h`

### 2.2.1 关键配置项

```c
// 系统时钟：1000Hz，即每1ms发生一次时钟中断
#define configTICK_RATE_HZ ((TickType_t)1000)

// 最大任务优先级：56（数字越大优先级越高）
#define configMAX_PRIORITIES ( 56 )

// 最小任务栈大小：128字 = 512字节（1字=4字节）
#define configMINIMAL_STACK_SIZE ((uint16_t)128)

// FreeRTOS堆内存：16KB（所有任务共享的内存池）
#define configTOTAL_HEAP_SIZE ((size_t)16*1024)

// 使用抢占式调度
#define configUSE_PREEMPTION 1
```

### 2.2.2 堆内存

**什么是堆？** 就像一个"内存池"，用于动态分配内存。

```
RAM 128KB 分配：
┌─────────────────┐
│  任务栈区域      │  ~12KB
├─────────────────┤
│  FreeRTOS堆     │  16KB (configTOTAL_HEAP_SIZE)
├─────────────────┤
│  LVGL内存池     │  48KB (LV_MEM_SIZE)
├─────────────────┤
│  全局变量/BSS   │  ~50KB
└─────────────────┘
```

---

## 2.3 任务创建（结合项目代码）

### 2.3.1 任务属性结构体

在 `User/Tasks/Src/user_TasksInit.c` 中：

```c
// 这是一个任务属性结构体，定义任务的"属性"
const osThreadAttr_t HardwareInitTask_attributes = {
    .name = "HardwareInitTask",      // 任务名字（调试用）
    .stack_size = 128 * 10,         // 栈大小
                                      // 128字 × 10 = 5120字节 = 5KB
    .priority = (osPriority_t) osPriorityHigh3,  // 优先级
};
```

**栈大小的作用**：
- 每个任务都有自己的"临时仓库"（栈）
- 栈越大，能存放的临时变量越多
- 栈太小，可能导致"栈溢出"（程序崩溃）

```
任务栈示意图：
┌─────────────────────┐
│  局部变量           │  <- 栈顶
├─────────────────────┤
│  函数调用返回地址    │
├─────────────────────┤
│  上一级函数的局部变量 │
├─────────────────────┤
│  ...                │  <- 栈底
└─────────────────────┘
```

---

### 2.3.2 创建任务函数

```c
// 创建任务
// 参数1：任务函数名
// 参数2：传递给任务的参数（通常填NULL）
// 参数3：任务属性
HardwareInitTaskHandle = osThreadNew(
    HardwareInitTask,         // ← 任务函数
    NULL,                     // ← 参数
    &HardwareInitTask_attributes  // ← 属性
);
```

---

### 2.3.3 任务函数模板

```c
// 所有任务函数都是这种格式
void TaskName(void *argument)
{
    // ========== 初始化部分 ==========
    // 只执行一次，做准备工作

    // ========== 主循环部分 ==========
    while(1)  // 死循环，任务永远不会结束
    {
        // 任务要做的事情

        osDelay(xxx);  // 必须延时！否则其他任务无法运行
    }
}
```

---

## 2.4 本项目 13 个任务详解

### 任务总览表

| 序号 | 任务名 | 优先级 | 栈大小 | 功能 |
|------|--------|--------|--------|------|
| 1 | HardwareInitTask | High3 (19) | 5KB | 硬件初始化 |
| 2 | LvHandlerTask | Low (1) | 12KB | LVGL UI处理 |
| 3 | WDOGFeedTask | High2 (18) | 0.5KB | 看门狗喂狗 |
| 4 | IdleEnterTask | High (17) | 0.5KB | 进入空闲模式 |
| 5 | StopEnterTask | High1 (16) | 8KB | 进入停止模式 |
| 6 | KeyTask | Normal (8) | 0.5KB | 按键处理 |
| 7 | ScrRenewTask | Low1 (2) | 5KB | 屏幕刷新 |
| 8 | SensorDataTask | Low1 (2) | 2.5KB | 传感器数据更新 |
| 9 | HRDataTask | Low1 (2) | 2.5KB | 心率数据更新 |
| 10 | ChargPageEnterTask | Low1 (2) | 5KB | 充电页面进入 |
| 11 | MessageSendTask | Low1 (2) | 2.5KB | 消息发送 |
| 12 | MPUCheckTask | Low2 (3) | 1.5KB | 计步检测 |
| 13 | DataSaveTask | Low2 (3) | 2.5KB | 数据保存 |

---

### 2.4.1 HardwareInitTask（硬件初始化任务系统启动时初始化所有硬件

）

**作用**：```c
// 任务属性
const osThreadAttr_t HardwareInitTask_attributes = {
    .name = "HardwareInitTask",
    .stack_size = 128 * 10,     // 5KB
    .priority = osPriorityHigh3, // 最高优先级！
};

// 任务函数
void HardwareInitTask(void *argument)
{
    // 初始化屏幕
    LCD_Init();

    // 初始化触摸
    Touch_Init();

    // 初始化传感器
    MPU_Init();
    AHT21_Init();

    // 初始化LVGL
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    ui_init();

    // 删除自己（初始化任务只需运行一次）
    osThreadExit();
}
```

**为什么优先级最高？**
- 必须先完成硬件初始化，其他任务才能正常工作
- 如果初始化失败，系统无法运行

---

### 2.4.2 LvHandlerTask（UI 处理任务）

**作用**：处理 LVGL 界面的渲染和刷新

```c
const osThreadAttr_t LvHandlerTask_attributes = {
    .name = "LvHandlerTask",
    .stack_size = 128 * 24,   // 12KB（最大！）
    .priority = osPriorityLow, // 最低
};

void LvHandlerTask(void *argument)
{
    while(1)
    {
        // 检测屏幕是否活跃（1秒内有操作=活跃）
        if(lv_disp_get_inactive_time(NULL) < 1000)
        {
            // 发送"活跃"信号
            uint8_t flag = 0;
            osMessageQueuePut(IdleBreak_MessageQueue, &flag, 0, 0);
        }

        // 处理LVGL任务（渲染界面）
        lv_task_handler();

        osDelay(1);  // 1ms刷新一次
    }
}
```

**问题**：为什么栈要 12KB（最大）？

**回答**：LVGL 渲染需要较大的临时内存来存储界面元素、颜色数据等。

---

### 2.4.3 WDOGFeedTask（看门狗任务）

**作用**：防止系统死机

```c
const osThreadAttr_t WDOGFeedTask_attributes = {
    .name = "WDOGFeedTask",
    .stack_size = 128 * 1,      // 0.5KB
    .priority = osPriorityHigh2, // 次高优先级
};

void WDOGFeedTask(void *argument)
{
    // 初始化看门狗
    WDOG_Port_Init();

    while(1)
    {
        WDOG_Feed();   // "喂狗"
        WDOG_Enable(); // 重新使能

        osDelay(100); // 每100ms喂一次
    }
}
```

**什么是看门狗？**
- 就像一条狗，必须定期"喂"它
- 如果忘记"喂"（程序卡死），狗就会"叫"（复位芯片）
- 这样系统就能自动恢复

---

### 2.4.4 KeyTask（按键处理任务）

**作用**：处理用户按键输入

```c
const osThreadAttr_t KeyTask_attributes = {
    .name = "KeyTask",
    .stack_size = 128 * 1,    // 0.5KB
    .priority = osPriorityNormal, // 普通优先级
};

void KeyTask(void *argument)
{
    uint8_t key_value;

    while(1)
    {
        // 从队列等待按键消息（阻塞状态）
        // 队列有消息时才醒来
        osMessageQueueGet(
            Key_MessageQueue,      // 队列ID
            &key_value,            // 存放位置
            NULL,                  // 优先级（暂不使用）
            osWaitForever         // 永久等待
        );

        // 处理按键
        Handle_Key(key_value);

        osDelay(10);
    }
}
```

**按键流程**：
```
按键按下 → 中断检测 → 发送到Key_MessageQueue 
→ KeyTask收到消息 → Handle_Key()处理
```

---

### 2.4.5 SensorDataTask（传感器数据更新任务）

**作用**：周期性读取各传感器数据

```c
const osThreadAttr_t SensorDataTask_attributes = {
    .name = "SensorDataTask",
    .stack_size = 128 * 5,   // 2.5KB
    .priority = osPriorityLow1,
};

void SensorDataUpdateTask(void *argument)
{
    while(1)
    {
        // 1. 读取加速度计
        MPU_Get_Accelerometer(&ax, &ay, &az);
        MPU_Get_Gyroscope(&gx, &gy, &gz);

        // 2. 读取温湿度
        AHT21_Read_Data(&temperature, &humidity);

        // 3. 读取气压
        SPL06_Read_Pressure(&pressure);

        // 4. 发送更新信号到显示队列
        uint8_t update_flag = 1;
        osMessageQueuePut(HomeUpdata_MessageQueue, &update_flag, 0, 0);

        osDelay(1000);  // 每秒更新一次
    }
}
```

**关键点**：
- 使用 `osDelay(1000)` 控制更新频率
- 读取完成后，通过队列通知显示任务更新界面

---

### 2.4.6 HRDataTask（心率数据任务）

**作用**：读取心率传感器数据

```c
const osThreadAttr_t HRDataTask_attributes = {
    .name = "HRDataTask",
    .stack_size = 128 * 5,   // 2.5KB
    .priority = osPriorityLow1,
};

void HRDataUpdateTask(void *argument)
{
    while(1)
    {
        // 读取心率和血氧
        EM7028_Read_Data(&hr_value, &spo2_value);

        // 更新UI变量
        HR_UI.hr_value = hr_value;
        HR_UI.spo2 = spo2_value;

        osDelay(200);  // 每200ms更新（比传感器快）
    }
}
```

**为什么心率更新更快？**
- 心率是动态变化的，需要更频繁采样
- 传感器任务1秒更新一次就够了

---

### 2.4.7 MPUCheckTask（计步检测任务）

**作用**：通过加速度数据计算步数

```c
const osThreadAttr_t MPUCheckTask_attributes = {
    .name = "MPUCheckTask",
    .stack_size = 128 * 3,   // 1.5KB
    .priority = osPriorityLow2,
};

void MPUCheckTask(void *argument)
{
    while(1)
    {
        // 读取加速度
        MPU_Get_Accelerometer(&ax, &ay, &az);

        // 计步算法处理
        steps = Pedometer_Calculate(ax, ay, az);

        // 每10步保存一次
        if(steps % 10 == 0)
        {
            Save_Steps_To_EEPROM(steps);
        }

        osDelay(50);  // 20Hz采样（20次/秒）
    }
}
```

---

## 2.5 消息队列（结合项目代码）

### 2.5.1 什么是消息队列？

队列就像**邮箱**：
- 发送任务把消息投递到邮箱
- 接收任务从邮箱取出消息
- 邮箱满时发送要等待，邮箱空时接收也要等待

```
TaskA ──投递──▶ 队列邮箱 ◀──取出── TaskB
   (生产者)              (消费者)
```

---

### 2.5.2 本项目的队列

在 `User_Tasks_Init()` 中创建：

```c
void User_Tasks_Init(void)
{
    // 创建队列
    // osMessageQueueNew(队列长度, 消息大小, 属性)

    // 1. 按键消息队列：1条消息，每条1字节
    Key_MessageQueue = osMessageQueueNew(1, 1, NULL);

    // 2. 主界面更新队列
    HomeUpdata_MessageQueue = osMessageQueueNew(1, 1, NULL);

    // 3. 空闲检测队列
    IdleBreak_MessageQueue = osMessageQueueNew(1, 1, NULL);

    // 4. 数据保存队列：2条消息
    DataSave_MessageQueue = osMessageQueueNew(2, 1, NULL);
}
```

---

### 2.5.3 发送消息

```c
// 发送消息到队列
// 参数1：队列ID
// 参数2：发送的数据指针
// 参数3：优先级（暂未使用）
// 参数4：超时时间（0=立即返回）

uint8_t data = 10;
osMessageQueuePut(Key_MessageQueue, &data, 0, 0);
```

---

### 2.5.4 接收消息

```c
// 从队列接收消息
// 参数1：队列ID
// 参数2：存放接收数据的变量地址
// 参数3：优先级
// 参数4：超时时间（osWaitForever=永久等待）

uint8_t received_data;
osMessageQueueGet(
    Key_MessageQueue,
    &received_data,
    NULL,
    osWaitForever  // 队列为空时会阻塞
);
```

---

### 2.5.5 队列使用示例：按键处理

```c
// ===== 按键按下时（可能在中断或定时器中）=====
void OnKeyPressed(uint8_t key)
{
    // 发送按键值到队列
    osMessageQueuePut(Key_MessageQueue, &key, 0, 0);
}

// ===== 按键任务中 =====
void KeyTask(void *argument)
{
    uint8_t key;
    while(1)
    {
        // 等待按键消息（阻塞）
        osMessageQueueGet(Key_MessageQueue, &key, NULL, osWaitForever);

        // 处理按键
        switch(key)
        {
            case KEY_UP:    /*...*/ break;
            case KEY_DOWN:  /*...*/ break;
            case KEY_OK:    /*...*/ break;
        }
    }
}
```

---

## 2.6 任务优先级详解

### 2.6.1 优先级数字

FreeRTOS 中，**数字越大优先级越高**。

```
osPriorityIdle     = 0      ← 最低（空闲任务）
osPriorityLow      = 1
osPriorityLow1     = 2
osPriorityLow2     = 3
...
osPriorityNormal   = 8      ← 普通
...
osPriorityHigh1    = 17
osPriorityHigh2    = 18
osPriorityHigh3    = 19      ← 最高（用户任务）
osPriorityRealtime = 31     ← 实时（系统保留）
```

### 2.6.2 本项目优先级分配

```
高 ──┬── HardwareInitTask (19)  ← 初始化最重要
     ├── WDOGFeedTask (18)     ← 喂狗不能耽误
     ├── IdleEnterTask (17)    ← 系统状态重要
     ├── StopEnterTask (16)
     │
     ├── KeyTask (8)          ← 用户交互
     │
     ├── ScrRenewTask (2)      ← UI相关
     ├── SensorDataTask (2)
     ├── HRDataTask (2)
     │
     ├── MPUCheckTask (3)
     ├── DataSaveTask (3)
     │
低 ──┴── LvHandlerTask (1)     ← UI渲染，不急
```

---

## 2.7 延时函数详解

### osDelay() vs osDelayUntil()

```c
// osDelay() - 相对延时
// 从调用时刻开始，延时指定时间
void TaskA(void *arg)
{
    while(1)
    {
        do_something();
        osDelay(1000);  // 延时1000ms
    }
}

// osDelayUntil() - 绝对延时
// 延时到指定时刻，保证精确周期
void TaskB(void *arg)
{
    uint32_t previousWakeTime;
    osDelayUntil(&previousWakeTime, 1000);  // 每1000ms执行一次
}
```

---

## 2.8 软件定时器

### 2.8.1 什么是软件定时器？

软件定时器就像一个"闹钟"，到时间了调用指定的回调函数。

### 2.8.2 本项目的定时器

```c
// 创建定时器
osTimerId_t IdleTimerHandle;

// 初始化
IdleTimerHandle = osTimerNew(
    IdleTimerCallback,     // 回调函数
    osTimerPeriodic,       // 周期性定时器
    NULL,                  // 参数
    NULL                   // 属性
);

// 启动定时器：100ms周期
osTimerStart(IdleTimerHandle, 100);
```

---

### 2.8.3 定时器回调

```c
void IdleTimerCallback(void *argument)
{
    // 每100ms执行一次
    // 这里做一些周期性检查
}
```

---

## 2.9 FreeRTOS 钩子函数

### 2.9.1 什么是钩子函数？

钩子函数（Hook）是一种"回调"机制：
- 你写一个函数，FreeRTOS 在特定时机帮你调用
- 就像"钩子"一样，挂在某个事件上

### 2.9.2 时钟钩子（Tick Hook）

在 `FreeRTOSConfig.h` 中启用：
```c
#define configUSE_TICK_HOOK 1
```

在 `user_TasksInit.c` 中实现：

```c
void TaskTickHook(void)
{
    // 每1ms（时钟中断）执行一次

    // 1. LVGL需要时钟基准
    lv_tick_inc(1);

    // 2. 计时器页面更新
    if(ui_TimerPageFlag)
    {
        ui_TimerPage_ms += 1;
        // ... 处理计时
    }

    // 3. 心率采样计时
    user_HR_timecount += 1;
}
```

---

## 2.10 总结与思考题

### 本章小结

1. **任务**：独立运行的小程序，有自己的栈和优先级
2. **调度器**：决定哪个任务先运行
3. **队列**：任务间通信的"邮箱"
4. **优先级**：高优先级任务可以抢断低优先级任务
5. **延时**：让CPU有时间处理其他任务
6. **看门狗**：防止系统死机

### 思考题

1. 如果某个任务里没有 `osDelay()`，会发生什么？

2. 任务A优先级比任务B高，那么A运行时B在做什么？

3. 队列满时再发送消息会怎样？

4. 看门狗超时时间设置太长或太短有什么影响？
