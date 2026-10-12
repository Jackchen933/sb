#ifndef _UI_WATCHFACEPAGE_H
#define _UI_WATCHFACEPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

// 外部变量声明
extern lv_obj_t * ui_WatchFacePage;
extern lv_obj_t * ui_WF_TimeHourLabel;
extern lv_obj_t * ui_WF_TimeMinuteLabel;
extern lv_obj_t * ui_WF_DateLabel;
extern lv_obj_t * ui_WF_DayLabel;
extern lv_obj_t * ui_WF_BatArc;
extern lv_obj_t * ui_WF_BatLabel;
extern lv_obj_t * ui_WF_StepArc;
extern lv_obj_t * ui_WF_StepLabel;
extern lv_obj_t * ui_WF_CalorieArc;
extern lv_obj_t * ui_WF_CalorieLabel;
extern lv_obj_t * ui_WF_StandArc;
extern lv_obj_t * ui_WF_StandLabel;
extern lv_obj_t * ui_WF_HRLabel;
extern lv_obj_t * ui_WF_TempLabel;
extern lv_obj_t * ui_WF_HumiLabel;
extern lv_obj_t * ui_WF_QuickAppsBtn;
extern lv_obj_t * ui_WF_FitnessBtn;

// Page结构
extern Page_t Page_WatchFace;

void ui_WatchFacePage_screen_init(void);
void ui_WatchFacePage_screen_deinit(void);
void ui_event_WatchFacePage(lv_event_t * e);
void ui_event_WF_QuickAppsBtn(lv_event_t * e);
void ui_event_WF_FitnessBtn(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
