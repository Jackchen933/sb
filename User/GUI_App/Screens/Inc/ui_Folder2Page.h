#ifndef _UI_FOLDER2PAGE_H
#define _UI_FOLDER2PAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../../ui.h"

extern lv_obj_t * ui_Screen1;
extern lv_obj_t * ui_uipullzone;
extern lv_obj_t * ui_uidroppanel;
extern lv_obj_t * ui_Button1;
extern lv_obj_t * ui_Label2;
extern lv_obj_t * ui_Button2;
extern lv_obj_t * ui_Label6;
extern lv_obj_t * ui_Button4;
extern lv_obj_t * ui_Button5;
extern lv_obj_t * ui_Slider1;
extern lv_obj_t * ui_Label11;
extern lv_obj_t * ui_Label3;
extern lv_obj_t * ui_Label4;
extern lv_obj_t * ui_Label5;
extern lv_obj_t * ui_Label7;
extern lv_obj_t * ui_F2TimeHourLabel;
extern lv_obj_t * ui_F2TimeColonLabel;
extern lv_obj_t * ui_F2TimeMinuteLabel;
extern lv_obj_t * ui_F2DateLabel;
extern lv_obj_t * ui_F2DayLabel;

void ui_Screen1_screen_init(void);
void ui_Screen1_screen_destroy(void);

#ifdef __cplusplus
}
#endif

#endif
