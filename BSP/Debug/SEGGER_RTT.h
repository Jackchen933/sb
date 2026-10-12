/*
 * SEGGER_RTT.h — 精简版 SEGGER RTT 驱动（协议兼容官方 J-Link）
 *
 * 特性：
 *   - 仅实现 1 个上行通道（MCU -> J-Link RTT Viewer），无阻塞写入
 *   - 数据结构布局与官方 SEGGER RTT 协议一致，J-Link 可直接识别
 *   - 写入非阻塞：缓冲区满时整条丢弃（NO_BLOCK_SKIP 模式），不影响任务实时性
 */
#ifndef SEGGER_RTT_H
#define SEGGER_RTT_H

#include <stdint.h>

#define SEGGER_RTT_UP_BUFFER_SIZE   (1024u)   /* 上行缓冲区大小，可按 RAM 调整 */

void    SEGGER_RTT_Init(void);
int     SEGGER_RTT_WriteString(const char *s);
int     SEGGER_RTT_Write(const void *buf, unsigned len);
int     SEGGER_RTT_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

#endif /* SEGGER_RTT_H */
