// Ink style compass page for OV_Watch.

#include <stdio.h>

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_CompassPage.h"

#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_compass_28x28);

LV_FONT_DECLARE(ui_font_Cuyuan20);

#define TXT_COMPASS "\xE6\x8C\x87\xE5\x8D\x97\xE9\x92\x88"

///////////////////// Page Manager //////////////////
Page_t Page_Compass = {ui_CompassPage_screen_init, ui_CompassPage_screen_deinit, &ui_CompassPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_CompassPage = NULL;
lv_obj_t * ui_CompassMeter = NULL;
lv_obj_t * ui_Compassneedle = NULL;

lv_obj_t * ui_CompassDirBtn = NULL;
lv_obj_t * ui_CompassDirLabel = NULL;
lv_obj_t * ui_CompassDirIcon = NULL;

lv_obj_t * ui_EnvAltitudeBtn = NULL;
lv_obj_t * ui_EnvAltitudeLabel = NULL;
lv_obj_t * ui_EnvAltitudeIcon = NULL;

lv_timer_t * ui_EcompassPageTimer = NULL;

///////////////////// FUNCTIONS ////////////////////
static void ui_event_CompassPage(lv_event_t * e);

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

    lv_obj_add_event_cb(btn, ui_event_CompassPage, LV_EVENT_ALL, NULL);
}

static lv_obj_t * make_data_card(lv_obj_t * parent,
                                 int16_t x,
                                 const char * tag,
                                 lv_obj_t ** tag_out,
                                 lv_obj_t ** value_out)
{
    lv_obj_t * card = lv_obj_create(parent);
    lv_obj_remove_style_all(card);
    lv_obj_set_size(card, 98, 36);
    lv_obj_set_pos(card, x, 235);
    lv_obj_set_style_radius(card, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(card, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(card, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(card, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(card, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * tag_label = lv_label_create(card);
    lv_label_set_text(tag_label, tag);
    lv_obj_set_style_text_font(tag_label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(tag_label, lv_color_hex(0x8D2D25), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(tag_label, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(tag_label, 10, 11);

    lv_obj_t * value = lv_label_create(card);
    lv_obj_set_style_text_font(value, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(value, lv_color_hex(0x111511), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(value, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_pos(value, 42, 8);

    if(tag_out) *tag_out = tag_label;
    if(value_out) *value_out = value;
    return card;
}

static void CompassPage_refresh_values(void)
{
    char value_strbuf[10];

    if(ui_Compassneedle != NULL) {
        lv_img_set_angle(ui_Compassneedle, HWInterface.Ecompass.direction * 10);
    }

    if(ui_CompassDirLabel != NULL) {
        snprintf(value_strbuf, sizeof(value_strbuf), "%d", HWInterface.Ecompass.direction);
        lv_label_set_text(ui_CompassDirLabel, value_strbuf);
    }

    if(ui_EnvAltitudeLabel != NULL) {
        snprintf(value_strbuf, sizeof(value_strbuf), "%dm", HWInterface.Barometer.altitude);
        lv_label_set_text(ui_EnvAltitudeLabel, value_strbuf);
    }
}

// need to be destroyed when the page is destroyed
static void EcompassPage_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    CompassPage_refresh_values();
}

static void ui_event_CompassPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && lv_event_get_target(e) != ui_CompassPage) {
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_CompassPage_screen_init(void)
{
    lv_obj_t * layer;
    lv_meter_scale_t * scale_360;
    lv_meter_scale_t * scale_12;

    ui_CompassPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_CompassPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_CompassPage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CompassPage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_CompassPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    layer = lv_obj_create(ui_CompassPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_SCROLLABLE);

    page_make_back_button(layer);

    lv_obj_t * icon = lv_img_create(layer);
    lv_img_set_src(icon, &ui_img_menu_compass_28x28);
    lv_obj_set_pos(icon, 62, 8);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, TXT_COMPASS);
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

    ui_CompassMeter = lv_meter_create(layer);
    lv_obj_set_size(ui_CompassMeter, 190, 190);
    lv_obj_set_pos(ui_CompassMeter, 25, 46);
    lv_obj_add_flag(ui_CompassMeter, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(ui_CompassMeter, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_bg_color(ui_CompassMeter, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CompassMeter, LV_OPA_50, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_CompassMeter, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_CompassMeter, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_CompassMeter, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_CompassMeter, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_CompassMeter, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_remove_style(ui_CompassMeter, NULL, LV_PART_INDICATOR);

    scale_360 = lv_meter_add_scale(ui_CompassMeter);
    lv_meter_set_scale_ticks(ui_CompassMeter, scale_360, 61, 1, 9, lv_color_hex(0x30362F));
    lv_meter_set_scale_range(ui_CompassMeter, scale_360, 0, 360, 360, 0);

    scale_12 = lv_meter_add_scale(ui_CompassMeter);
    lv_meter_set_scale_ticks(ui_CompassMeter, scale_12, 12, 0, 0, lv_color_hex(0x30362F));
    lv_meter_set_scale_major_ticks(ui_CompassMeter, scale_12, 1, 2, 12,
                                   lv_color_hex(0x982B24), 12);
    lv_meter_set_scale_range(ui_CompassMeter, scale_12, 0, 330, 330, 270);

    ui_Compassneedle = lv_img_create(ui_CompassMeter);
    lv_img_set_src(ui_Compassneedle, &ui_img_compass_needle_png);
    lv_obj_set_align(ui_Compassneedle, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Compassneedle, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_Compassneedle, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * n_label = lv_label_create(ui_CompassMeter);
    lv_label_set_text(n_label, "N");
    lv_obj_set_style_text_font(n_label, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(n_label, lv_color_hex(0x8D2D25), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(n_label, LV_ALIGN_TOP_MID, 0, 12);

    lv_obj_t * s_label = lv_label_create(ui_CompassMeter);
    lv_label_set_text(s_label, "S");
    lv_obj_set_style_text_font(s_label, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(s_label, lv_color_hex(0x30362F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(s_label, LV_ALIGN_BOTTOM_MID, 0, -12);

    lv_obj_t * e_label = lv_label_create(ui_CompassMeter);
    lv_label_set_text(e_label, "E");
    lv_obj_set_style_text_font(e_label, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(e_label, lv_color_hex(0x30362F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(e_label, LV_ALIGN_RIGHT_MID, -12, 0);

    lv_obj_t * w_label = lv_label_create(ui_CompassMeter);
    lv_label_set_text(w_label, "W");
    lv_obj_set_style_text_font(w_label, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(w_label, lv_color_hex(0x30362F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(w_label, LV_ALIGN_LEFT_MID, 12, 0);

    ui_CompassDirBtn = make_data_card(layer, 17, "DIR", &ui_CompassDirIcon, &ui_CompassDirLabel);
    ui_EnvAltitudeBtn = make_data_card(layer, 125, "ALT", &ui_EnvAltitudeIcon, &ui_EnvAltitudeLabel);

    CompassPage_refresh_values();

    lv_obj_add_event_cb(ui_CompassPage, ui_event_CompassPage, LV_EVENT_ALL, NULL);
    enable_gesture_bubble_recursive(ui_CompassPage);

    ui_EcompassPageTimer = lv_timer_create(EcompassPage_timer_cb, 500, NULL);
}

/////////////////// SCREEN deinit ////////////////////
void ui_CompassPage_screen_deinit(void)
{
    if(ui_EcompassPageTimer != NULL) {
        lv_timer_del(ui_EcompassPageTimer);
        ui_EcompassPageTimer = NULL;
    }
}
