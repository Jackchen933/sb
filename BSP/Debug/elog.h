/*
 * elog.h — 轻量分级日志模块（EasyLogger 风格 API + SEGGER RTT 输出）
 *
 * 对齐 EasyLogger 核心特性：
 *   - 六级日志：A(断言) E(错误) W(警告) I(信息) D(调试) V(详细)
 *   - 标签（tag）定位日志来源
 *   - 时间戳 + 线程名 + (文件:行号) 上下文
 *   - 终端彩色分级显示
 *   - 运行期按最低级别过滤
 *
 * 使用方式：
 *   #include "elog.h"
 *   log_i("HW", "lcd init ok, id=0x%04X", id);
 *   log_e("EM7028", "i2c read failed!");
 */
#ifndef ELOG_H
#define ELOG_H

#include <stdint.h>

/* ---------------- 输出与颜色配置 ---------------- */
#define ELOG_OUT_RTT        1       /* 1=SEGGER RTT 输出（推荐，不占串口） */
#define ELOG_COLOR_ENABLE   1       /* 终端彩色显示 */

/* ---------------- 六个级别（数值越小越严重） ---------------- */
#define ELOG_LVL_ASSERT     0
#define ELOG_LVL_ERROR      1
#define ELOG_LVL_WARN       2
#define ELOG_LVL_INFO       3
#define ELOG_LVL_DEBUG      4
#define ELOG_LVL_VERBOSE    5

/* 编译期总开关：低于此级别的日志直接不编译进固件（省 Flash/RAM） */
#ifndef ELOG_COMPILE_LVL
#define ELOG_COMPILE_LVL    ELOG_LVL_VERBOSE
#endif

/* ---------------- 初始化与过滤 ---------------- */
void elog_init(void);
void elog_set_filter(uint8_t min_lvl);          /* 运行期最低输出级别 */
void elog_set_time(uint8_t h, uint8_t m, uint8_t s, uint16_t ms); /* 可选：同步真实时间 */

/* ---------------- 核心输出函数 ---------------- */
void elog_output(uint8_t lvl, const char *tag,
                 const char *file, const char *func, int line,
                 const char *fmt, ...);

/* ---------------- 用户接口宏（EasyLogger 风格） ---------------- */
#define log_a(tag, ...)  elog_output(ELOG_LVL_ASSERT,  tag, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_e(tag, ...)  elog_output(ELOG_LVL_ERROR,   tag, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_w(tag, ...)  elog_output(ELOG_LVL_WARN,    tag, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_i(tag, ...)  elog_output(ELOG_LVL_INFO,    tag, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_d(tag, ...)  elog_output(ELOG_LVL_DEBUG,   tag, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define log_v(tag, ...)  elog_output(ELOG_LVL_VERBOSE, tag, __FILE__, __func__, __LINE__, __VA_ARGS__)

#endif /* ELOG_H */
