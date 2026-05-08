// Ink style SpO2 page for OV_Watch.

#include <stdio.h>

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_SPO2Page.h"

#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_spo2_28x28);

LV_FONT_DECLARE(ui_font_Cuyuan20);
LV_FONT_DECLARE(ui_font_Cuyuan80);

#define TXT_SPO2 "\xE8\xA1\x80\xE6\xB0\xA7"
#define TXT_MEASURING "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xB5\x8B\xE9\x87\x8F..."

///////////////////// Page Manager //////////////////
Page_t Page_SPO2 = {ui_SPO2Page_screen_init, ui_SPO2Page_screen_deinit, &ui_SPO2Page};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_SPO2Page = NULL;
lv_obj_t * ui_SPO2NumLabel = NULL;
lv_obj_t * ui_SPO2UnitLabel = NULL;
lv_obj_t * ui_SPO2NoticeLabel = NULL;
lv_obj_t * ui_SPO2Icon = NULL;

lv_timer_t * ui_SPO2PageTimer = NULL;

/////////////////// private functions ///////////////////
static void ui_event_SPO2Page(lv_event_t * e);

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

    lv_obj_add_event_cb(btn, ui_event_SPO2Page, LV_EVENT_ALL, NULL);
}

static lv_obj_t * make_paper_panel(lv_obj_t * parent, int16_t x, int16_t y, int16_t w, int16_t h)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, w, h);
    lv_obj_set_pos(panel, x, y);
    lv_obj_set_style_radius(panel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(panel, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(panel, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(panel, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    return panel;
}

static void SPO2Page_refresh_value(void)
{
    char value_strbuf[6];

    if(ui_SPO2NumLabel == NULL) return;

    snprintf(value_strbuf, sizeof(value_strbuf), "%d", HWInterface.HR_meter.SPO2);
    lv_label_set_text(ui_SPO2NumLabel, value_strbuf);
}

// need to be destroyed when the page is destroyed
static void SPO2Page_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    SPO2Page_refresh_value();
}

static void ui_event_SPO2Page(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && lv_event_get_target(e) != ui_SPO2Page) {
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_SPO2Page_screen_init(void)
{
    lv_obj_t * layer;
    lv_obj_t * card;
    lv_obj_t * ring;

    ui_SPO2Page = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_SPO2Page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_SPO2Page, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_SPO2Page, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_SPO2Page);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    layer = lv_obj_create(ui_SPO2Page);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_SCROLLABLE);

    page_make_back_button(layer);

    ui_SPO2Icon = lv_img_create(layer);
    lv_img_set_src(ui_SPO2Icon, &ui_img_menu_spo2_28x28);
    lv_obj_set_pos(ui_SPO2Icon, 75, 8);
    lv_obj_clear_flag(ui_SPO2Icon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, TXT_SPO2);
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

    card = make_paper_panel(layer, 20, 58, 200, 188);

    ring = lv_obj_create(card);
    lv_obj_remove_style_all(ring);
    lv_obj_set_size(ring, 140, 140);
    lv_obj_align(ring, LV_ALIGN_TOP_MID, 0, 14);
    lv_obj_set_style_radius(ring, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ring, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ring, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ring, lv_color_hex(0x385B6E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ring, LV_OPA_60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ring, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_SPO2NumLabel = lv_label_create(card);
    lv_obj_set_style_text_font(ui_SPO2NumLabel, &ui_font_Cuyuan80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_SPO2NumLabel, lv_color_hex(0x111511), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SPO2NumLabel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_SPO2NumLabel, LV_ALIGN_TOP_MID, -12, 42);

    ui_SPO2UnitLabel = lv_label_create(card);
    lv_label_set_text(ui_SPO2UnitLabel, "%");
    lv_obj_set_style_text_font(ui_SPO2UnitLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_SPO2UnitLabel, lv_color_hex(0x385B6E), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SPO2UnitLabel, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_SPO2UnitLabel, LV_ALIGN_TOP_MID, 50, 95);

    ui_SPO2NoticeLabel = lv_label_create(card);
    lv_label_set_text(ui_SPO2NoticeLabel, TXT_MEASURING);
    lv_obj_set_style_text_color(ui_SPO2NoticeLabel, lv_color_hex(0x56615A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_SPO2NoticeLabel, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_SPO2NoticeLabel, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_SPO2NoticeLabel, LV_ALIGN_BOTTOM_MID, 0, -14);

    SPO2Page_refresh_value();

    lv_obj_add_event_cb(ui_SPO2Page, ui_event_SPO2Page, LV_EVENT_ALL, NULL);
    enable_gesture_bubble_recursive(ui_SPO2Page);

    ui_SPO2PageTimer = lv_timer_create(SPO2Page_timer_cb, 500, NULL);
}

/////////////////// SCREEN deinit ////////////////////
void ui_SPO2Page_screen_deinit(void)
{
    if(ui_SPO2PageTimer != NULL) {
        lv_timer_del(ui_SPO2PageTimer);
        ui_SPO2PageTimer = NULL;
    }
}
