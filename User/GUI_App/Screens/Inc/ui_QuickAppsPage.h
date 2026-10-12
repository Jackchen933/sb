#ifndef _UI_QUICKAPPSPAGE_H
#define _UI_QUICKAPPSPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

// 外部变量声明
extern lv_obj_t * ui_QuickAppsPage;

// Page结构
extern Page_t Page_QuickApps;

void ui_QuickAppsPage_screen_init(void);
void ui_QuickAppsPage_screen_deinit(void);
void ui_event_QA_BackBtn(lv_event_t * e);

// 应用按钮事件
void ui_event_QA_HRBtn(lv_event_t * e);
void ui_event_QA_SPO2Btn(lv_event_t * e);
void ui_event_QA_EnvBtn(lv_event_t * e);
void ui_event_QA_CalendarBtn(lv_event_t * e);
void ui_event_QA_TimerBtn(lv_event_t * e);
void ui_event_QA_ComputerBtn(lv_event_t * e);
void ui_event_QA_CompassBtn(lv_event_t * e);
void ui_event_QA_NFCCardBtn(lv_event_t * e);
void ui_event_QA_GameBtn(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
