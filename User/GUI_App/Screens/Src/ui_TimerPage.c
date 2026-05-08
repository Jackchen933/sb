// Ink style timer page for OV_Watch.

#include <stdio.h>

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_TimerPage.h"

#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_timer_28x28);

LV_FONT_DECLARE(ui_font_Cuyuan20);
LV_FONT_DECLARE(ui_font_Cuyuan38);

#define TXT_TIMER "\xE7\xA7\x92\xE8\xA1\xA8"

///////////////////// Page Manager //////////////////
Page_t Page_Timer = {ui_TimerPage_screen_init, ui_TimerPage_screen_deinit, &ui_TimerPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_TimerPage = NULL;
lv_obj_t * ui_TimerMeter = NULL;
lv_obj_t * ui_TimerStartBtn = NULL;
lv_obj_t * ui_TimerReBtn = NULL;
lv_obj_t * ui_TimerStartLabel = NULL;
lv_obj_t * ui_TimerReLabel = NULL;
lv_obj_t * ui_TimerMinLabel = NULL;
lv_obj_t * ui_TimerSecColonLabel = NULL;
lv_obj_t * ui_TimerSecLabel = NULL;
lv_obj_t * ui_TimerMilSecColonLabel = NULL;
lv_obj_t * ui_TimerMilSecLabel = NULL;

lv_timer_t * ui_TimerPageTimer = NULL;

uint8_t ui_TimerPageFlag = 0;
uint8_t ui_TimerPage_min = 0;
uint8_t ui_TimerPage_sec = 0;
uint8_t ui_TimerPage_10ms = 0;
uint8_t ui_TimerPage_ms = 0;

///////////////////// FUNCTIONS ////////////////////
static void ui_event_TimerPage(lv_event_t * e);

static void enable_gesture_bubble_recursive(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt;

    if(obj == NULL) return;

    child_cnt = lv_obj_get_child_cnt(obj);
    for(i = 0; i < child_cnt; i++) {
        lv_obj_t * child = lv_obj_get_child(obj, i);
        lv_obj_add_flag(child, LV_OBJ_FLAG_GESTURE_BUBBLE);
        enable_gesture_bubble_recursive(child);
    }
}

static void page_make_back_button(lv_obj_t * parent)
{
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, 48, 42);
    lv_obj_set_pos(btn, 0, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t * arrow = lv_label_create(btn);
    lv_label_set_text(arrow, "<");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x1D211D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(arrow, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(arrow);

    lv_obj_add_event_cb(btn, ui_event_TimerPage, LV_EVENT_ALL, NULL);
}

static void timer_make_button(lv_obj_t * btn, lv_color_t bg_color, lv_color_t text_color)
{
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, 58, 58);
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn, bg_color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(btn, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x202620), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(btn, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(btn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(btn, LV_OPA_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btn, text_color, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
}

///////////////////// Data Init ////////////////////
static void ui_TimerPage_Data_init(void)
{
    ui_TimerPageFlag = 0;
    ui_TimerPage_min = 99;
    ui_TimerPage_sec = 59;
    ui_TimerPage_10ms = 99;
    ui_TimerPage_ms = 0;
}

static void TimerPage_refresh_labels(void)
{
    char strbuf[4];

    if(ui_TimerMinLabel == NULL || ui_TimerSecLabel == NULL || ui_TimerMilSecLabel == NULL) {
        return;
    }

    snprintf(strbuf, sizeof(strbuf), "%02d", ui_TimerPage_min);
    lv_label_set_text(ui_TimerMinLabel, strbuf);
    snprintf(strbuf, sizeof(strbuf), "%02d", ui_TimerPage_sec);
    lv_label_set_text(ui_TimerSecLabel, strbuf);
    snprintf(strbuf, sizeof(strbuf), "%02d", ui_TimerPage_10ms);
    lv_label_set_text(ui_TimerMilSecLabel, strbuf);
}

/////////////////////// Timer //////////////////////
static void LabelRefresh_timer(lv_timer_t * timer)
{
    (void)timer;
    TimerPage_refresh_labels();

    if(ui_TimerPageFlag == 0 &&
       ui_TimerStartBtn != NULL &&
       ui_TimerStartLabel != NULL &&
       lv_obj_has_state(ui_TimerStartBtn, LV_STATE_CHECKED)) {
        lv_obj_clear_state(ui_TimerStartBtn, LV_STATE_CHECKED);
        lv_label_set_text(ui_TimerStartLabel, LV_SYMBOL_PLAY);
        if(ui_TimerPageTimer != NULL) {
            lv_timer_pause(ui_TimerPageTimer);
        }
    }
}

static void ui_event_TimerStartBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if(event_code == LV_EVENT_VALUE_CHANGED && lv_obj_has_state(target, LV_STATE_CHECKED)) {
        ui_TimerPageFlag = 1;
        lv_label_set_text(ui_TimerStartLabel, LV_SYMBOL_PAUSE);
        if(ui_TimerPageTimer != NULL) {
            lv_timer_resume(ui_TimerPageTimer);
        }
    }

    if(event_code == LV_EVENT_VALUE_CHANGED && !lv_obj_has_state(target, LV_STATE_CHECKED)) {
        ui_TimerPageFlag = 0;
        lv_label_set_text(ui_TimerStartLabel, LV_SYMBOL_PLAY);
        if(ui_TimerPageTimer != NULL) {
            lv_timer_pause(ui_TimerPageTimer);
        }
    }
}

static void ui_event_TimerReBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && !lv_obj_has_state(ui_TimerStartBtn, LV_STATE_CHECKED)) {
        if(ui_TimerPageTimer != NULL) {
            lv_timer_pause(ui_TimerPageTimer);
        }
        ui_TimerPage_Data_init();
        TimerPage_refresh_labels();
        lv_label_set_text(ui_TimerStartLabel, LV_SYMBOL_PLAY);
    }
}

static void ui_event_TimerPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && lv_event_get_target(e) != ui_TimerPage) {
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_TimerPage_screen_init(void)
{
    lv_obj_t * layer;
    lv_meter_scale_t * scale_min;

    ui_TimerPage_Data_init();

    ui_TimerPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_TimerPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_TimerPage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_TimerPage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_TimerPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    layer = lv_obj_create(ui_TimerPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_SCROLLABLE);

    page_make_back_button(layer);

    lv_obj_t * icon = lv_img_create(layer);
    lv_img_set_src(icon, &ui_img_menu_timer_28x28);
    lv_obj_set_pos(icon, 75, 8);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, TXT_TIMER);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1B1E1B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(title, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 16, 12);

    lv_obj_t * seal = lv_obj_create(layer);
    lv_obj_remove_style_all(seal);
    lv_obj_set_size(seal, 22, 22);
    lv_obj_set_pos(seal, 198, 11);
    lv_obj_set_style_radius(seal, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(seal, lv_color_hex(0x982B24), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(seal, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(seal, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(seal, lv_color_hex(0x6B211B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(seal, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(seal, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_TimerMeter = lv_meter_create(layer);
    lv_obj_set_size(ui_TimerMeter, 202, 202);
    lv_obj_set_pos(ui_TimerMeter, 19, 55);
    lv_obj_add_flag(ui_TimerMeter, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(ui_TimerMeter, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_bg_opa(ui_TimerMeter, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_TimerMeter, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_TimerMeter, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(ui_TimerMeter, NULL, LV_PART_INDICATOR);
    scale_min = lv_meter_add_scale(ui_TimerMeter);
    lv_meter_set_scale_ticks(ui_TimerMeter, scale_min, 61, 1, 9, lv_color_hex(0x2B302B));
    lv_meter_set_scale_range(ui_TimerMeter, scale_min, 0, 60, 360, 270);

    lv_obj_t * time_panel = lv_obj_create(ui_TimerMeter);
    lv_obj_remove_style_all(time_panel);
    lv_obj_set_size(time_panel, 164, 70);
    lv_obj_align(time_panel, LV_ALIGN_CENTER, 0, -18);
    lv_obj_set_style_radius(time_panel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(time_panel, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(time_panel, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(time_panel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(time_panel, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(time_panel, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(time_panel, LV_OBJ_FLAG_SCROLLABLE);

    ui_TimerMinLabel = lv_label_create(time_panel);
    lv_label_set_text(ui_TimerMinLabel, "99");
    lv_obj_set_style_text_font(ui_TimerMinLabel, &ui_font_Cuyuan38, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_TimerMinLabel, lv_color_hex(0x111511), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(ui_TimerMinLabel, 12, 14);

    ui_TimerSecColonLabel = lv_label_create(time_panel);
    lv_label_set_text(ui_TimerSecColonLabel, ":");
    lv_obj_set_style_text_font(ui_TimerSecColonLabel, &ui_font_Cuyuan38, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_TimerSecColonLabel, lv_color_hex(0x982B24), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(ui_TimerSecColonLabel, 54, 12);

    ui_TimerSecLabel = lv_label_create(time_panel);
    lv_label_set_text(ui_TimerSecLabel, "59");
    lv_obj_set_style_text_font(ui_TimerSecLabel, &ui_font_Cuyuan38, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_TimerSecLabel, lv_color_hex(0x111511), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(ui_TimerSecLabel, 72, 14);

    ui_TimerMilSecColonLabel = lv_label_create(time_panel);
    lv_label_set_text(ui_TimerMilSecColonLabel, ".");
    lv_obj_set_style_text_font(ui_TimerMilSecColonLabel, &ui_font_Cuyuan38, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_TimerMilSecColonLabel, lv_color_hex(0x982B24), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(ui_TimerMilSecColonLabel, 114, 12);

    ui_TimerMilSecLabel = lv_label_create(time_panel);
    lv_label_set_text(ui_TimerMilSecLabel, "99");
    lv_obj_set_style_text_font(ui_TimerMilSecLabel, &ui_font_Cuyuan38, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_TimerMilSecLabel, lv_color_hex(0x111511), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(ui_TimerMilSecLabel, 128, 14);

    ui_TimerReBtn = lv_btn_create(ui_TimerMeter);
    timer_make_button(ui_TimerReBtn, lv_color_hex(0xF8F6EE), lv_color_hex(0x1D211D));
    lv_obj_align(ui_TimerReBtn, LV_ALIGN_CENTER, -43, 62);

    ui_TimerStartBtn = lv_btn_create(ui_TimerMeter);
    timer_make_button(ui_TimerStartBtn, lv_color_hex(0x9C2B22), lv_color_hex(0xF7F4EA));
    lv_obj_align(ui_TimerStartBtn, LV_ALIGN_CENTER, 43, 62);
    lv_obj_add_flag(ui_TimerStartBtn, LV_OBJ_FLAG_CHECKABLE);

    ui_TimerReLabel = lv_label_create(ui_TimerReBtn);
    lv_label_set_text(ui_TimerReLabel, LV_SYMBOL_REFRESH);
    lv_obj_set_style_text_font(ui_TimerReLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_TimerReLabel, lv_color_hex(0x1D211D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_TimerReLabel);

    ui_TimerStartLabel = lv_label_create(ui_TimerStartBtn);
    lv_label_set_text(ui_TimerStartLabel, LV_SYMBOL_PLAY);
    lv_obj_set_style_text_font(ui_TimerStartLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_TimerStartLabel, lv_color_hex(0xF7F4EA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(ui_TimerStartLabel);

    TimerPage_refresh_labels();

    lv_obj_add_event_cb(ui_TimerPage, ui_event_TimerPage, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TimerStartBtn, ui_event_TimerStartBtn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TimerReBtn, ui_event_TimerReBtn, LV_EVENT_ALL, NULL);
    enable_gesture_bubble_recursive(ui_TimerPage);

    ui_TimerPageTimer = lv_timer_create(LabelRefresh_timer, 10, NULL);
    lv_timer_pause(ui_TimerPageTimer);
}

//////////////////// SCREEN deinit ////////////////////
void ui_TimerPage_screen_deinit(void)
{
    ui_TimerPageFlag = 0;
    if(ui_TimerPageTimer != NULL) {
        lv_timer_del(ui_TimerPageTimer);
        ui_TimerPageTimer = NULL;
    }
}
