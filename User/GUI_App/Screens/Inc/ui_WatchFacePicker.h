#ifndef _UI_WATCHFACEPICKER_H
#define _UI_WATCHFACEPICKER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

// 表盘样式枚举
typedef enum {
    WATCH_FACE_MODERN = 0,      // 现代简约
    WATCH_FACE_SPORT,          // 运动风格
    WATCH_FACE_MINIMALIST,     // 极简风格
    WATCH_FACE_ANALOG,         // 模拟时钟
    WATCH_FACE_DIGITAL,        // 纯数字
    WATCH_FACE_COUNT
} WatchFaceStyle_t;

// 外部变量声明
extern lv_obj_t * ui_WatchFacePicker;
extern WatchFaceStyle_t ui_WFP_CurrentFace;
extern WatchFaceStyle_t ui_WFP_TempFace;  // 临时选择(确认前)

// Page结构
extern Page_t Page_WatchFacePicker;

void ui_WatchFacePicker_screen_init(void);
void ui_WatchFacePicker_screen_deinit(void);
void ui_event_WFP_BackBtn(lv_event_t * e);
void ui_event_WFP_PrevBtn(lv_event_t * e);
void ui_event_WFP_NextBtn(lv_event_t * e);
void ui_event_WFP_ConfirmBtn(lv_event_t * e);

// 表盘名称获取
const char* ui_WFP_GetFaceName(WatchFaceStyle_t style);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
