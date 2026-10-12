// Ink style environment page for OV_Watch.

#include <stdio.h>

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_EnvPage.h"
#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_page_temp_36x36);
LV_IMG_DECLARE(ui_img_page_humi_36x36);

LV_FONT_DECLARE(ui_font_Cuyuan20);

#define TXT_ENV "\xE7\x8E\xAF\xE5\xA2\x83"

///////////////////// Page Manager //////////////////
Page_t Page_Env = {ui_EnvPage_screen_init, ui_EnvPage_screen_deinit, &ui_EnvPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_EnvPage = NULL;
lv_obj_t * ui_EnvTempBar = NULL;
lv_obj_t * ui_EnvHumiBar = NULL;
lv_obj_t * ui_EnvTempNumLabel = NULL;
lv_obj_t * ui_EnvHumiNumLabel = NULL;
lv_obj_t * ui_EnvTempUnitLabel = NULL;
lv_obj_t * ui_EnvHumiUnitLabel = NULL;
lv_obj_t * ui_EnvHumiIcon = NULL;
lv_obj_t * ui_EnvTempIcon = NULL;

lv_timer_t * ui_EnvPageTimer = NULL;

/////////////////// private functions ///////////////////
static void ui_event_EnvPage(lv_event_t * e);

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

    lv_obj_add_event_cb(btn, ui_event_EnvPage, LV_EVENT_ALL, NULL);
}

static lv_obj_t * env_make_metric_card(lv_obj_t * parent,
                                       int16_t y,
                                       const void * icon_src,
                                       const char * label_text,
                                       const char * unit_text,
                                       lv_color_t accent,
                                       int16_t range_min,
                                       int16_t range_max,
                                       int16_t initial_value,
                                       lv_obj_t ** icon_out,
                                       lv_obj_t ** value_out,
                                       lv_obj_t ** unit_out,
                                       lv_obj_t ** bar_out)
{
    lv_obj_t * card = lv_obj_create(parent);
    lv_obj_remove_style_all(card);
    lv_obj_set_size(card, 208, 84);
    lv_obj_set_pos(card, 16, y);
    lv_obj_set_style_radius(card, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(card, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(card, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(card, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * icon = lv_img_create(card);
    lv_img_set_src(icon, icon_src);
    lv_obj_set_pos(icon, 12, 24);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * name = lv_label_create(card);
    lv_label_set_text(name, label_text);
    lv_obj_set_style_text_font(name, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(name, lv_color_hex(0x586058), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(name, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(name, 64, 16);
    lv_obj_clear_flag(name, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * value = lv_label_create(card);
    lv_label_set_text(value, "0");
    lv_obj_set_style_text_font(value, &ui_font_Cuyuan30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(value, lv_color_hex(0x1A1D1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(value, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(value, 64, 34);
    lv_obj_clear_flag(value, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * unit = lv_label_create(card);
    lv_label_set_text(unit, unit_text);
    lv_obj_set_style_text_font(unit, &ui_font_Cuyuan30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(unit, lv_color_hex(0x414A43), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(unit, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(unit, 124, 35);
    lv_obj_clear_flag(unit, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * bar = lv_bar_create(card);
    lv_bar_set_range(bar, range_min, range_max);
    lv_bar_set_value(bar, initial_value, LV_ANIM_OFF);
    lv_obj_set_size(bar, 130, 8);
    lv_obj_set_pos(bar, 64, 68);
    lv_obj_clear_flag(bar, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(bar, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x1D241E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(bar, LV_OPA_30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(bar, 4, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(bar, accent, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(bar, LV_OPA_80, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    if(icon_out) *icon_out = icon;
    if(value_out) *value_out = value;
    if(unit_out) *unit_out = unit;
    if(bar_out) *bar_out = bar;

    return card;
}

static void EnvPage_update_values(void)
{
    char value_strbuf[8];
    int temp = HWInterface.AHT21.temperature;
    int humi = HWInterface.AHT21.humidity;

    if(ui_EnvTempBar == NULL || ui_EnvHumiBar == NULL ||
       ui_EnvTempNumLabel == NULL || ui_EnvHumiNumLabel == NULL) {
        return;
    }

    lv_bar_set_value(ui_EnvTempBar, temp, LV_ANIM_OFF);
    lv_bar_set_value(ui_EnvHumiBar, humi, LV_ANIM_OFF);

    snprintf(value_strbuf, sizeof(value_strbuf), "%d", temp);
    lv_label_set_text(ui_EnvTempNumLabel, value_strbuf);
    snprintf(value_strbuf, sizeof(value_strbuf), "%d", humi);
    lv_label_set_text(ui_EnvHumiNumLabel, value_strbuf);
}

// need to be destroyed when the page is destroyed
static void EnvPage_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    EnvPage_update_values();
}

static void ui_event_EnvPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && lv_event_get_target(e) != ui_EnvPage) {
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_EnvPage_screen_init(void)
{
    ui_EnvPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_EnvPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_EnvPage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_EnvPage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_EnvPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * layer = lv_obj_create(ui_EnvPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_SCROLLABLE);

    page_make_back_button(layer);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, TXT_ENV);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1B1E1B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(title, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

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

    (void)env_make_metric_card(layer, 56, &ui_img_page_temp_36x36,
                               "TEMP", "C", lv_color_hex(0x9C2B22), -10, 50,
                               HWInterface.AHT21.temperature,
                               &ui_EnvTempIcon, &ui_EnvTempNumLabel,
                               &ui_EnvTempUnitLabel, &ui_EnvTempBar);

    (void)env_make_metric_card(layer, 154, &ui_img_page_humi_36x36,
                               "HUMI", "%", lv_color_hex(0x385B6E), 0, 100,
                               HWInterface.AHT21.humidity,
                               &ui_EnvHumiIcon, &ui_EnvHumiNumLabel,
                               &ui_EnvHumiUnitLabel, &ui_EnvHumiBar);

    EnvPage_update_values();

    lv_obj_add_event_cb(ui_EnvPage, ui_event_EnvPage, LV_EVENT_ALL, NULL);
    enable_gesture_bubble_recursive(ui_EnvPage);

    ui_EnvPageTimer = lv_timer_create(EnvPage_timer_cb, 500, NULL);
}

/////////////////// SCREEN deinit ////////////////////
void ui_EnvPage_screen_deinit(void)
{
    if(ui_EnvPageTimer != NULL) {
        lv_timer_del(ui_EnvPageTimer);
        ui_EnvPageTimer = NULL;
    }
}
