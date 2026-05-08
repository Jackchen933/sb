# 第三章：LVGL 详解

## 3.1 LVGL 简介

**LVGL (Light and Versatile Graphics Library)** 是一个轻量级、开源的图形库，专为嵌入式设备设计。

本项目使用的是 **LVGL v8.2.0**。

### LVGL 特点

- **轻量级**：资源占用小，适合 MCU
- **功能丰富**：按钮、标签、滑动条、图表等控件
- **可定制**：支持主题、动画、字体
- **跨平台**：支持多种显示和输入设备

---

## 3.2 配置文件解析

文件位置：`Middlewares/LVGL/GUI/lv_conf.h`

### 显示配置

```c
// 颜色深度：1/8/16/32 位
// 本项目使用 RGB565 (16位)
#define LV_COLOR_DEPTH 16

// 屏幕刷新周期：10ms
#define LV_DISP_DEF_REFR_PERIOD 10      /*[ms]*/

// 输入设备读取周期：10ms
#define LV_INDEV_DEF_READ_PERIOD 10     /*[ms]*/
```

### 内存配置

```c
// LVGL 内部内存池大小：48KB
// 这是 LVGL 动态内存分配的主要来源
#define LV_MEM_SIZE (48U * 1024U)

// 内存缓冲区最大数量
#define LV_MEM_BUF_MAX_NUM 16
```

### 知识点：RGB565 格式

16位颜色格式：
- R: 5位 (0-31)
- G: 6位 (0-63)
- B: 5位 (0-31)

**为什么用 RGB565？**
- 节省内存（2字节/像素）
- 足够满足普通显示需求

---

## 3.3 显示驱动接口

文件位置：`Middlewares/LVGL/GUI/porting/lv_port_disp.c`

### 显示初始化流程

```c
void lv_port_disp_init(void)
{
    // 1. 初始化屏幕硬件
    disp_init();
    
    // 2. 创建显示驱动
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    
    // 3. 设置分辨率
    disp_drv.hor_res = LCD_W;  // 屏幕宽度
    disp_drv.ver_res = LCD_H;  // 屏幕高度
    
    // 4. 设置刷新回调
    disp_drv.flush_cb = disp_flush;
    
    // 5. 创建绘制缓冲区
    static lv_disp_draw_buf_t draw_buf_dsc_2;
    static lv_color_t buf_2_1[LCD_W * LCD_H / 10];
    static lv_color_t buf_2_2[LCD_W * LCD_H / 10];
    lv_disp_draw_buf_init(&draw_buf_dsc_2, buf_2_1, buf_2_2, LCD_W * LCD_H / 10);
    
    // 6. 注册驱动
    lv_disp_drv_register(&disp_drv);
}
```

### 刷新回调函数

```c
// 这是最核心的函数！
// 当 LVGL 渲染完一部分画面时，会调用这个函数
static void disp_flush(lv_disp_drv_t * disp_drv, 
                      const lv_area_t * area, 
                      lv_color_t * color_p)
{
    // area: 要刷新的区域 (x1, y1) 到 (x2, y2)
    // color_p: 颜色数据指针
    
    // 调用 LCD 驱动将颜色数据写入屏幕
    LCD_Color_Fill(area->x1, area->y1, area->x2, area->y2, (u16*)color_p);
    
    // 告诉 LVGL 刷新完成，可以继续渲染下一帧
    lv_disp_flush_ready(disp_drv);
}
```

---

## 3.4 缓冲区机制

LVGL 使用缓冲区来优化渲染性能，有三种模式：

### 模式 1：单缓冲区

```c
// 只创建一个缓冲区
static lv_color_t buf_1[LCD_W * LCD_H / 10];  // 1/10 屏幕大小
```

### 模式 2：双缓冲区（推荐）

```c
// 创建两个缓冲区
static lv_color_t buf_2_1[LCD_W * LCD_H / 10];
static lv_color_t buf_2_2[LCD_W * LCD_H / 10];

// LVGL 在一个缓冲区渲染，另一个可以同时发送给屏幕
// 本项目使用这种模式
```

### 模式 3：全屏双缓冲

```c
// 两个完整屏幕大小的缓冲区
static lv_color_t buf_3_1[LCD_W * LCD_H];
static lv_color_t buf_3_2[LCD_W * LCD_H];

disp_drv.full_refresh = 1;  // 需要设置全屏刷新模式
```

### 本项目的配置

```c
#define BUFFER_METHOD 2  // 使用双缓冲模式

// 缓冲区大小：1/10 屏幕
// 屏幕 240x240 = 57600 像素
// 缓冲区 = 57600 / 10 = 5760 像素
// 每个像素 2 字节 = 11.52KB
// 两个缓冲区 = 23KB
```

**为什么用 1/10 而不是完整屏幕？**
- 节省内存（STM32F411 只有 128KB RAM）
- 速度足够快

---

## 3.5 输入设备驱动

文件位置：`Middlewares/LVGL/GUI/porting/lv_port_indev.c`

### 触摸初始化

```c
void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;
    
    // 1. 初始化触摸驱动
    touch_init();
    
    // 2. 创建输入设备驱动
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;  // 触摸/鼠标
    
    // 3. 设置读取回调
    indev_drv.read_cb = touch_read;
    
    // 4. 注册输入设备
    lv_indev_drv_register(&indev_drv);
}
```

### 触摸读取回调

```c
static bool touch_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    // 读取触摸状态
    static lv_coord_t last_x = 0;
    static lv_coord_t last_y = 0;
    
    // 获取触摸坐标
    if (touch_get_xy(&last_x, &last_y))
    {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_PRESSED;  // 按下
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;  // 释放
    }
    
    return false;  // false = 数据有效
}
```

---

## 3.6 界面刷新机制

### LVGL 任务处理

在 `LvHandlerTask` 中：

```c
void LvHandlerTask(void *argument)
{
    uint8_t IdleBreakstr = 0;
    while(1)
    {
        // 检测屏幕是否活跃
        if(lv_disp_get_inactive_time(NULL) < 1000)
        {
            // 1秒内有过操作，认为是活跃的
            osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 0);
        }
        
        // 处理 LVGL 任务（界面渲染）
        lv_task_handler();
        
        osDelay(1);  // 1ms 调用一次
    }
}
```

### lv_task_handler() 做了什么？

1. **检查是否有待渲染内容**
2. **调用各控件的绘制函数**
3. **将渲染结果写入缓冲区**
4. **调用 flush_cb 刷新屏幕**

### 时钟更新

```c
// 在时钟钩子中每 1ms 调用
void TaskTickHook(void)
{
    // LVGL 需要一个时间基准
    lv_tick_inc(1);  // 告诉 LVGL 过去了 1ms
}
```

---

## 3.7 UI 界面结构

### 本项目的 UI 文件

```
User/GUI_App/
├── ui.c / ui.h           # UI 初始化
├── Screens/
│   ├── ui_HomePage.c    # 主界面
│   ├── ui_HRPage.c      # 心率界面
│   ├── ui_SPO2Page.c    # 血氧界面
│   ├── ui_MenuPage.c    # 菜单
│   ├── ui_SettingPage.c # 设置
│   └── ...
└── Fonts/                # 字体文件
```

### 界面创建流程

```c
// ui.c 中的初始化
void ui_init(void)
{
    lv_disp_t * disp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(...);
    lv_disp_set_theme(disp, theme);
    
    // 创建主界面
    ui_HomePage_screen_init();
}
```

---

## 3.8 常用控件示例

### 创建按钮

```c
lv_obj_t * btn = lv_btn_create(lv_scr_act());
lv_obj_set_pos(btn, 10, 10);
lv_obj_set_size(btn, 100, 40);

// 添加标签
lv_obj_t * label = lv_label_create(btn);
lv_label_set_text(label, "Click");
```

### 创建文本

```c
lv_obj_t * label = lv_label_create(lv_scr_act());
lv_label_set_text(label, "Hello OV-Watch!");
lv_obj_set_pos(label, 50, 100);
```

### 创建进度条

```c
lv_obj_t * bar = lv_bar_create(lv_scr_act());
lv_bar_set_value(bar, 70, LV_ANIM_ON);
```

---

## 本章小结

1. **LVGL 作用**：嵌入式图形库，负责 UI 渲染
2. **配置要点**：颜色深度 16位，内存 48KB
3. **显示接口**：通过 `flush_cb` 将缓冲区数据发送到屏幕
4. **缓冲区**：双缓冲模式，1/10 屏幕大小
5. **输入设备**：触摸驱动通过 `read_cb` 上报坐标
6. **刷新机制**：`lv_task_handler()` 每 1ms 调用一次

---

## 思考题

1. 如果 `lv_disp_flush_ready()` 没有调用，会发生什么？
2. 缓冲区大小对性能有什么影响？
3. 如何实现按键控制代替触摸？
