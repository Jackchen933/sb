/*
 * SEGGER_RTT.c — 精简版 SEGGER RTT 驱动（协议兼容官方 J-Link）
 *
 * 控制块（Control Block）内存布局与 SEGGER 官方定义一致：
 *   J-Link 调试器通过搜索 "SEGGER RTT" 魔数定位控制块，
 *   后台读取上行缓冲区的 RdOff/WrOff 搬运数据，MCU 侧零开销。
 */
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "SEGGER_RTT.h"
#include "stm32f4xx_hal.h"

/* ---------------- 与官方协议一致的数据结构 ---------------- */

typedef struct {
    const char     *sBufferPointer;   /* 缓冲区首地址      */
    volatile int    WrOff;            /* 写偏移（MCU 更新）*/
    volatile int    RdOff;            /* 读偏移（J-Link 更新）*/
    int             SizeOfBuffer;     /* 缓冲区大小        */
    volatile int    Flags;            /* 模式：0 = NO_BLOCK_SKIP */
} SEGGER_RTT_BUFFER_UP;

typedef struct {
    char                   acID[16];              /* "SEGGER RTT" 魔数 */
    int                    MaxNumUpBuffers;       /* 上行通道数 */
    int                    MaxNumDownBuffers;     /* 下行通道数 */
    SEGGER_RTT_BUFFER_UP   aUp[1];                /* 上行（MCU->PC）*/
    SEGGER_RTT_BUFFER_UP   aDown[1];              /* 下行（PC->MCU，占位）*/
} SEGGER_RTT_CB;

/* ---------------- 静态分配 ---------------- */

static char               _upBuf[SEGGER_RTT_UP_BUFFER_SIZE];
static SEGGER_RTT_CB      _cb;
static uint8_t            _initDone = 0;

/* 短临界区：保护 WrOff 更新（任务/中断都可能调用） */
static inline void _lock(void)   { __disable_irq(); }
static inline void _unlock(void) { __enable_irq(); }

void SEGGER_RTT_Init(void)
{
    memset(&_cb, 0, sizeof(_cb));
    strcpy(_cb.acID, "SEGGER RTT");
    _cb.MaxNumUpBuffers   = 1;
    _cb.MaxNumDownBuffers = 1;

    _cb.aUp[0].sBufferPointer = _upBuf;
    _cb.aUp[0].SizeOfBuffer   = sizeof(_upBuf);
    _cb.aUp[0].WrOff          = 0;
    _cb.aUp[0].RdOff          = 0;
    _cb.aUp[0].Flags          = 0;    /* NO_BLOCK_SKIP */

    _cb.aDown[0].sBufferPointer = 0;
    _cb.aDown[0].SizeOfBuffer   = 0;

    _initDone = 1;
}

int SEGGER_RTT_Write(const void *buf, unsigned len)
{
    if (!_initDone) SEGGER_RTT_Init();
    if (len == 0) return 0;

    SEGGER_RTT_BUFFER_UP *p = &_cb.aUp[0];
    const unsigned Size = (unsigned)p->SizeOfBuffer;
    unsigned WrOff = (unsigned)p->WrOff;
    unsigned RdOff = (unsigned)p->RdOff;

    unsigned free = Size - 1u - ((WrOff - RdOff + Size) % Size);
    if (len > free) return 0;               /* 满则整条丢弃，绝不阻塞 */

    _lock();
    {
        unsigned rem = Size - WrOff;        /* 尾部剩余空间 */
        const char *src = (const char *)buf;
        if (rem >= len) {
            memcpy(&_upBuf[WrOff], src, len);
            WrOff += len;
        } else {                            /* 环形回绕，分两段拷贝 */
            memcpy(&_upBuf[WrOff], src, rem);
            memcpy(&_upBuf[0], src + rem, len - rem);
            WrOff = len - rem;
        }
        p->WrOff = (int)WrOff;              /* 一次性发布，J-Link 才可见 */
    }
    _unlock();
    return (int)len;
}

int SEGGER_RTT_WriteString(const char *s)
{
    return SEGGER_RTT_Write(s, strlen(s));
}

int SEGGER_RTT_printf(const char *fmt, ...)
{
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (n <= 0) return 0;
    if ((unsigned)n >= sizeof(buf)) n = sizeof(buf) - 1;
    return SEGGER_RTT_Write(buf, (unsigned)n);
}
