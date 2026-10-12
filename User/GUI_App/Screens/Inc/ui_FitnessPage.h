#ifndef _UI_FITNESSPAGE_H
#define _UI_FITNESSPAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

// 外部变量声明
extern lv_obj_t * ui_FitnessPage;
extern lv_obj_t * ui_Fit_ProgressArc;
extern lv_obj_t * ui_Fit_ProgressLabel;
extern lv_obj_t * ui_Fit_StepValueLabel;
extern lv_obj_t * ui_Fit_StepGoalLabel;
extern lv_obj_t * ui_Fit_CalorieValueLabel;
extern lv_obj_t * ui_Fit_CalorieGoalLabel;
extern lv_obj_t * ui_Fit_StandValueLabel;
extern lv_obj_t * ui_Fit_StandGoalLabel;
extern lv_obj_t * ui_Fit_ExerciseValueLabel;
extern lv_obj_t * ui_Fit_ExerciseGoalLabel;

// Page结构
extern Page_t Page_Fitness;

void ui_FitnessPage_screen_init(void);
void ui_FitnessPage_screen_deinit(void);
void ui_event_Fit_BackBtn(lv_event_t * e);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
