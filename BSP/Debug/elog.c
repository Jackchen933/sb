/*
 * elog.c — 轻量分级日志模块实现
 *
 * 输出格式（与 EasyLogger 对齐）：
 *   I/HR.data [00:01:23.456][LvHandler] (em70x8.c:88) HR rate = 72
 *   ─┬── ─┬──── ─────┬───── ────┬──── ─────┬──────
 *    级别  标签     时间戳     线程名     位置 + 正文
 *
 * 彩色方案（ANSI 转义码，RTT Viewer / 终端均支持）：
 *   A=反显红  E=红  W=黄  I=绿  D=默认  V=青
 */
#include "elog.h"
#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/* ---------------- 内部状态 ---------------- */
static volatile uint8_t s_minLvl = ELOG_LVL_DEBUG;   /* 运行期过滤，默认 D 及以上 */
static uint32_t s_tickBase = 0;                      /* 上电时刻 tick，用于换算时间戳 */

/* RTC 时间可被 elog_set_time() 注入，-1 表示未同步，用 tick 换算 */
static volatile int8_t s_rtcH = -1;
static volatile uint8_t s_rtcM, s_rtcS;
static volatile uint16_t s_rtcMs;

/* ---------------- ANSI 颜色 ---------------- */
#if ELOG_COLOR_ENABLE
static const char *const s_lvlColor[] = {
    "\033[0;41m",   /* A: 反显红底 */
    "\033[0;31m",   /* E: 红 */
    "\033[0;33m",   /* W: 黄 */
    "\033[0;32m",   /* I: 绿 */
    "\033[0m",      /* D: 默认 */
    "\033[0;36m",   /* V: 青 */
};
#define COLOR_RESET "\033[0m"
#else
#define COLOR_RESET ""
#endif

/* ---------------- 对外接口 ---------------- */

void elog_init(void)
{
    SEGGER_RTT_Init();
    s_tickBase = HAL_GetTick();
    elog_set_filter(ELOG_LVL_DEBUG);
}

void elog_set_filter(uint8_t min_lvl)
{
    if (min_lvl <= ELOG_LVL_VERBOSE) s_minLvl = min_lvl;
}

/* 允许在拿到 RTC 时间后注入真实时间（如手表已对时） */
void elog_set_time(uint8_t h, uint8_t m, uint8_t s, uint16_t ms)
{
    s_rtcH = (int8_t)h;
    s_rtcM = m; s_rtcS = s; s_rtcMs = ms;
}

/* 取时间戳：优先用注入的真实时间，否则用 tick 换算成 hh:mm:ss.mmm */
static void _get_time(uint8_t *h, uint8_t *m, uint8_t *s, uint16_t *ms)
{
    if (s_rtcH >= 0) {
        *h = (uint8_t)s_rtcH; *m = s_rtcM; *s = s_rtcS; *ms = s_rtcMs;
    } else {
        uint32_t t = HAL_GetTick() - s_tickBase;
        *ms = (uint16_t)(t % 1000);
        uint32_t sec = t / 1000;
        *s = (uint8_t)(sec % 60);
        *m = (uint8_t)((sec / 60) % 60);
        *h = (uint8_t)((sec / 3600) % 24);
    }
}

/* 去掉路径前缀，只留文件名（armcc __FILE__ 可能带相对路径） */
static const char *_base_name(const char *path)
{
    const char *p = strrchr(path, '/');
    const char *q = strrchr(path, '\\');
    if (q > p) p = q;
    return p ? p + 1 : path;
}

void elog_output(uint8_t lvl, const char *tag,
                 const char *file, const char *func, int line,
                 const char *fmt, ...)
{
    if (lvl > s_minLvl || lvl > ELOG_COMPILE_LVL) return;

    char lineBuf[224];
    int  pos = 0;
    const char *lvlChar = "AEWIDV";
    const char *fn = _base_name(file);

    /* 颜色前缀 */
#if ELOG_COLOR_ENABLE
    pos += snprintf(lineBuf + pos, sizeof(lineBuf) - pos, "%s", s_lvlColor[lvl]);
#endif

    /* 级别/标签 + 时间戳 + 线程名 */
    uint8_t h, m, s; uint16_t ms;
    _get_time(&h, &m, &s, &ms);

    const char *taskName = "?";
    osThreadId_t tid = osThreadGetId();
    if (tid != NULL) {
        const char *n = osThreadGetName(tid);
        if (n != NULL) taskName = n;
    }

    pos += snprintf(lineBuf + pos, sizeof(lineBuf) - pos,
                    "%c/%s [%02u:%02u:%02u.%03u][%s] ",
                    lvlChar[lvl], tag, h, m, s, ms, taskName);

    /* 位置 + 正文 */
    pos += snprintf(lineBuf + pos, sizeof(lineBuf) - pos,
                    "(%s:%d) ", fn, line);

    va_list ap;
    va_start(ap, fmt);
    pos += vsnprintf(lineBuf + pos, sizeof(lineBuf) - pos, fmt, ap);
    va_end(ap);

    /* 换行 + 颜色复位 */
    pos += snprintf(lineBuf + pos, sizeof(lineBuf) - pos, "%s\n", COLOR_RESET);

    /* 统一出口：RTT 非阻塞写入 */
#if ELOG_OUT_RTT
    SEGGER_RTT_Write(lineBuf, (unsigned)pos);
#else
    HAL_UART_Transmit(&huart1, (uint8_t *)lineBuf, (uint16_t)pos, 10);
#endif
}
