// Ink style heart-rate page for OV_Watch.

#include <stdio.h>

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_HRPage.h"

#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_hr_28x28);

LV_FONT_DECLARE(ui_font_Cuyuan20);
LV_FONT_DECLARE(ui_font_Cuyuan80);

#define TXT_HR "\xE5\xBF\x83\xE7\x8E\x87"
#define TXT_MEASURING "\xE6\xAD\xA3\xE5\x9C\xA8\xE6\xB5\x8B\xE9\x87\x8F..."

///////////////////// Page Manager //////////////////
Page_t Page_HR = {ui_HRPage_screen_init, ui_HRPage_screen_deinit, &ui_HRPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_HRPage = NULL;
lv_obj_t * ui_HRPageNumLabel = NULL;
lv_obj_t * ui_HRPageUnitLabel = NULL;
lv_obj_t * ui_HRPaggiconLabel = NULL;
lv_obj_t * ui_HRPageNoticeLabel = NULL;

lv_timer_t * ui_HRPageTimer = NULL;

/////////////////// private functions ///////////////////
static void ui_event_HRPage(lv_event_t * e);

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

    lv_obj_add_event_cb(btn, ui_event_HRPage, LV_EVENT_ALL, NULL);
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

static void HRPage_refresh_value(void)
{
    char value_strbuf[6];

    if(ui_HRPageNumLabel == NULL) return;

    snprintf(value_strbuf, sizeof(value_strbuf), "%d", HWInterface.HR_meter.HrRate);
    lv_label_set_text(ui_HRPageNumLabel, value_strbuf);
}

// need to be destroyed when the page is destroyed
static void HRPage_timer_cb(lv_timer_t * timer)
{
    (void)timer;
    HRPage_refresh_value();
}

static void ui_event_HRPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && lv_event_get_target(e) != ui_HRPage) {
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_HRPage_screen_init(void)
{
    lv_obj_t * layer;
    lv_obj_t * card;
    lv_obj_t * ring;

    ui_HRPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_HRPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_HRPage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_HRPage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_HRPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    layer = lv_obj_create(ui_HRPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_SCROLLABLE);

    page_make_back_button(layer);

    ui_HRPaggiconLabel = lv_img_create(layer);
    lv_img_set_src(ui_HRPaggiconLabel, &ui_img_menu_hr_28x28);
    lv_obj_set_pos(ui_HRPaggiconLabel, 75, 8);
    lv_obj_clear_flag(ui_HRPaggiconLabel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, TXT_HR);
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
    lv_obj_set_style_border_color(ring, lv_color_hex(0x982B24), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ring, LV_OPA_60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ring, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_HRPageNumLabel = lv_label_create(card);
    lv_obj_set_style_text_font(ui_HRPageNumLabel, &ui_font_Cuyuan80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_HRPageNumLabel, lv_color_hex(0x111511), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_HRPageNumLabel, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_HRPageNumLabel, LV_ALIGN_TOP_MID, -12, 42);

    ui_HRPageUnitLabel = lv_label_create(card);
    lv_label_set_text(ui_HRPageUnitLabel, "BPM");
    lv_obj_set_style_text_font(ui_HRPageUnitLabel, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_HRPageUnitLabel, lv_color_hex(0x8D2D25), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_HRPageUnitLabel, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_HRPageUnitLabel, LV_ALIGN_TOP_MID, 50, 95);

    ui_HRPageNoticeLabel = lv_label_create(card);
    lv_label_set_text(ui_HRPageNoticeLabel, TXT_MEASURING);
    lv_obj_set_style_text_color(ui_HRPageNoticeLabel, lv_color_hex(0x56615A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_HRPageNoticeLabel, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HRPageNoticeLabel, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(ui_HRPageNoticeLabel, LV_ALIGN_BOTTOM_MID, 0, -14);

    HRPage_refresh_value();

    lv_obj_add_event_cb(ui_HRPage, ui_event_HRPage, LV_EVENT_ALL, NULL);
    enable_gesture_bubble_recursive(ui_HRPage);

    ui_HRPageTimer = lv_timer_create(HRPage_timer_cb, 50, NULL);
}

/////////////////// SCREEN deinit ////////////////////
void ui_HRPage_screen_deinit(void)
{
    if(ui_HRPageTimer != NULL) {
        lv_timer_del(ui_HRPageTimer);
        ui_HRPageTimer = NULL;
    }
}
