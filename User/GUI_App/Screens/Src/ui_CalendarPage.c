// Ink style calendar page for OV_Watch.

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_CalendarPage.h"
#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_page_calendar_36x36);

LV_FONT_DECLARE(ui_font_Cuyuan20);

#define TXT_CALENDAR "\xE6\x97\xA5\xE5\x8E\x86"

///////////////////// Page Manager //////////////////
Page_t Page_Calender = {ui_CalendarPage_screen_init, ui_CalendarPage_screen_deinit, &ui_CalendarPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_CalendarPage = NULL;
lv_obj_t * ui_CalendarPageCalendar = NULL;

///////////////////// FUNCTIONS ////////////////////
static void ui_event_CalendarPage(lv_event_t * e);

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

    lv_obj_add_event_cb(btn, ui_event_CalendarPage, LV_EVENT_ALL, NULL);
}

static void ui_event_CalendarPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && lv_event_get_target(e) != ui_CalendarPage) {
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_CalendarPage_screen_init(void)
{
    HW_DateTimeTypeDef DateTime;
    HWInterface.RealTimeClock.GetTimeDate(&DateTime);

    ui_CalendarPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_CalendarPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_CalendarPage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CalendarPage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_CalendarPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * layer = lv_obj_create(ui_CalendarPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_SCROLLABLE);

    page_make_back_button(layer);

    lv_obj_t * icon = lv_img_create(layer);
    lv_img_set_src(icon, &ui_img_page_calendar_36x36);
    lv_obj_set_pos(icon, 62, 4);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, TXT_CALENDAR);
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

    ui_CalendarPageCalendar = lv_calendar_create(layer);
    lv_calendar_set_today_date(ui_CalendarPageCalendar, 2000 + DateTime.Year, DateTime.Month, DateTime.Date);
    lv_calendar_set_showed_date(ui_CalendarPageCalendar, 2000 + DateTime.Year, DateTime.Month);
    (void)lv_calendar_header_arrow_create(ui_CalendarPageCalendar);
    lv_obj_set_size(ui_CalendarPageCalendar, 218, 210);
    lv_obj_set_pos(ui_CalendarPageCalendar, 11, 56);
    lv_obj_add_flag(ui_CalendarPageCalendar, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(ui_CalendarPageCalendar, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_radius(ui_CalendarPageCalendar, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_CalendarPageCalendar, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CalendarPageCalendar, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_CalendarPageCalendar, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_CalendarPageCalendar, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_CalendarPageCalendar, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_CalendarPageCalendar, lv_color_hex(0x1F241F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_CalendarPageCalendar, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_CalendarPageCalendar, lv_color_hex(0x9C2B22), LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui_CalendarPageCalendar, LV_OPA_80, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui_CalendarPageCalendar, lv_color_hex(0xF7F4EA), LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_obj_add_event_cb(ui_CalendarPage, ui_event_CalendarPage, LV_EVENT_ALL, NULL);
    enable_gesture_bubble_recursive(ui_CalendarPage);
}

///////////////////// SCREEN deinit ////////////////////
void ui_CalendarPage_screen_deinit(void)
{
    /*
     * PageManager uses lv_scr_load_anim(..., true), so LVGL deletes the
     * previous active screen after the transition completes.
     */
}
