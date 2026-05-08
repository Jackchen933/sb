// Ink style home page for OV_Watch.

#include "../ui.h"
#include "PageManager.h"
#include "HWDataAccess.h"
#include <stdio.h>

extern Page_t Page_Menu;
extern Page_t Page_SquareAbout;

extern uint8_t ui_TimeHourValue;
extern uint8_t ui_TimeMinuteValue;
extern uint8_t ui_DateMonthValue;
extern uint8_t ui_DateDayValue;
extern uint8_t ui_DataWeekdayValue;
extern uint8_t ui_BatArcValue;
extern uint16_t ui_StepNumValue;
extern uint8_t ui_LightSliderValue;

extern void drop_down_pressed(lv_event_t * e);
extern void drop_down_pressing(lv_event_t * e);
extern void drop_down_released(lv_event_t * e);

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_ink_battery_80_28x14);
LV_IMG_DECLARE(ui_img_ink_footsteps_24x24);
LV_IMG_DECLARE(ui_img_ink_heart_rate_24x24);
LV_IMG_DECLARE(ui_img_ink_center_mountain_circle_44x44);
LV_IMG_DECLARE(ui_img_ink_week_bg_54x26);

LV_FONT_DECLARE(ui_font_Cuyuan18);
LV_FONT_DECLARE(ui_font_Cuyuan20);
LV_FONT_DECLARE(ui_font_Cuyuan24);
LV_FONT_DECLARE(ui_font_Cuyuan30);
LV_FONT_DECLARE(ui_font_Cuyuan80);

#define TXT_MONTH      "\xE6\x9C\x88"
#define TXT_DAY        "\xE6\x97\xA5"
#define TXT_STEPS      "\xE6\xAD\xA5\xE6\x95\xB0"
#define TXT_BPM        "\xE6\xAC\xA1/\xE5\x88\x86"
#define TXT_LUNAR      "\xE5\x9B\x9B\xE6\x9C\x88\xE5\xBB\xBF\xE4\xB8\x89"

static const char * s_week_names[7] = {
    "\xE5\x91\xA8\xE4\xB8\x80",
    "\xE5\x91\xA8\xE4\xBA\x8C",
    "\xE5\x91\xA8\xE4\xB8\x89",
    "\xE5\x91\xA8\xE5\x9B\x9B",
    "\xE5\x91\xA8\xE4\xBA\x94",
    "\xE5\x91\xA8\xE5\x85\xAD",
    "\xE5\x91\xA8\xE6\x97\xA5"
};

lv_obj_t * ui_HomePage = NULL;
lv_obj_t * ui_uipullzone = NULL;
lv_obj_t * ui_uidroppanel = NULL;
lv_obj_t * ui_Button1 = NULL;
lv_obj_t * ui_uiBLELabel = NULL;
lv_obj_t * ui_Button2 = NULL;
lv_obj_t * ui_uiSetLabel = NULL;
lv_obj_t * ui_Button4 = NULL;
lv_obj_t * ui_Button5 = NULL;
lv_obj_t * ui_Slider1 = NULL;
lv_obj_t * ui_Label11 = NULL;
lv_obj_t * ui_uiHRiconLabel = NULL;
lv_obj_t * ui_uiHumiiconLabel = NULL;
lv_obj_t * ui_uiTempiconLabel = NULL;
lv_obj_t * ui_uiStepiconLabel = NULL;

static lv_timer_t * s_home_timer = NULL;
static lv_obj_t * s_time_hour_label = NULL;
static lv_obj_t * s_time_colon_top = NULL;
static lv_obj_t * s_time_colon_bottom = NULL;
static lv_obj_t * s_time_minute_label = NULL;
static lv_obj_t * s_date_label = NULL;
static lv_obj_t * s_week_label = NULL;
static lv_obj_t * s_bat_label = NULL;
static lv_obj_t * s_step_value_label = NULL;
static lv_obj_t * s_hr_value_label = NULL;

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

static void obj_clear_box(lv_obj_t * obj)
{
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static lv_obj_t * create_label(lv_obj_t * parent, const lv_font_t * font, uint32_t color,
                               lv_align_t align, lv_coord_t x, lv_coord_t y, const char * text)
{
    lv_obj_t * label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_align(label, align);
    lv_obj_set_x(label, x);
    lv_obj_set_y(label, y);
    lv_obj_set_style_text_font(label, font, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(label, lv_color_hex(color), LV_PART_MAIN | LV_STATE_DEFAULT);
    return label;
}

static lv_obj_t * create_colon_dot(lv_obj_t * parent, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t * dot = lv_obj_create(parent);
    lv_obj_set_size(dot, 8, 8);
    lv_obj_set_align(dot, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(dot, x, y);
    obj_clear_box(dot);
    lv_obj_set_style_radius(dot, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dot, lv_color_hex(0x080808), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    return dot;
}

static void style_card(lv_obj_t * card)
{
    obj_clear_box(card);
    lv_obj_set_style_radius(card, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(card, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(card, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void style_round_btn(lv_obj_t * btn)
{
    obj_clear_box(btn);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_size(btn, 50, 34);
    lv_obj_set_style_radius(btn, 17, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x2B2A27), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(btn, 210, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static const char * week_name(uint8_t weekday)
{
    if(weekday < 1 || weekday > 7) return s_week_names[0];
    return s_week_names[weekday - 1];
}

static void home_update_now(void)
{
    char buf[32];
    uint8_t bat;
    uint8_t hr;
    HW_DateTimeTypeDef now;
    Page_t * page = Page_Get_NowPage();

    if(page != NULL && page->page_obj != &ui_HomePage) return;
    if(s_time_hour_label == NULL || s_time_minute_label == NULL) return;

    HWInterface.RealTimeClock.GetTimeDate(&now);

    ui_TimeHourValue = now.Hours;
    ui_TimeMinuteValue = now.Minutes;
    ui_DateMonthValue = now.Month;
    ui_DateDayValue = now.Date;
    ui_DataWeekdayValue = now.WeekDay;

    sprintf(buf, "%02d", ui_TimeHourValue);
    lv_label_set_text(s_time_hour_label, buf);
    sprintf(buf, "%02d", ui_TimeMinuteValue);
    lv_label_set_text(s_time_minute_label, buf);

    sprintf(buf, "%d.%d", ui_DateMonthValue, ui_DateDayValue);
    lv_label_set_text(s_date_label, buf);
    lv_label_set_text(s_week_label, week_name(ui_DataWeekdayValue));

    bat = HWInterface.Power.power_remain;
    if(bat > 100) bat = 100;
    ui_BatArcValue = bat;
    sprintf(buf, "%d%%", ui_BatArcValue);
    lv_label_set_text(s_bat_label, buf);

    ui_StepNumValue = HWInterface.IMU.Steps;
    sprintf(buf, "%u", ui_StepNumValue);
    lv_label_set_text(s_step_value_label, buf);

    hr = HWInterface.HR_meter.HrRate;
    if(hr == 0) {
        lv_label_set_text(s_hr_value_label, "--");
    }
    else {
        sprintf(buf, "%d", hr);
        lv_label_set_text(s_hr_value_label, buf);
    }
}

static void home_timer_cb(lv_timer_t * timer)
{
    LV_UNUSED(timer);
    home_update_now();
}

static void brightness_slider_event(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED && ui_Slider1 != NULL) {
        ui_LightSliderValue = (uint8_t)lv_slider_get_value(ui_Slider1);
    }
}

void ui_event_HomePage(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_GESTURE &&
       lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Load(&Page_Menu);
    }
}

void ui_event_uipullzone(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_PRESSED) {
        drop_down_pressed(e);
    }
    if(event_code == LV_EVENT_PRESSING) {
        drop_down_pressing(e);
    }
    if(event_code == LV_EVENT_RELEASED) {
        drop_down_released(e);
    }
}

void ui_event_Button2(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_SquareAbout);
    }
}

static void create_dropdown(void)
{
    ui_uidroppanel = lv_obj_create(ui_HomePage);
    lv_obj_set_size(ui_uidroppanel, 240, 122);
    lv_obj_set_align(ui_uidroppanel, LV_ALIGN_TOP_MID);
    obj_clear_box(ui_uidroppanel);
    lv_obj_set_style_radius(ui_uidroppanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_uidroppanel, lv_color_hex(0xF3EFE4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_uidroppanel, 235, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_uidroppanel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_uidroppanel, lv_color_hex(0xD2C9B8), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button5 = lv_btn_create(ui_uidroppanel);
    style_round_btn(ui_Button5);
    lv_obj_set_pos(ui_Button5, 16, 18);
    create_label(ui_Button5, &lv_font_montserrat_14, 0xFFFFFF, LV_ALIGN_CENTER, 0, 0, "PWR");

    ui_Button1 = lv_btn_create(ui_uidroppanel);
    style_round_btn(ui_Button1);
    lv_obj_set_pos(ui_Button1, 74, 18);
    ui_uiBLELabel = create_label(ui_Button1, &lv_font_montserrat_14, 0xFFFFFF, LV_ALIGN_CENTER, 0, 0, "BLE");

    ui_Button4 = lv_btn_create(ui_uidroppanel);
    style_round_btn(ui_Button4);
    lv_obj_set_pos(ui_Button4, 16, 72);
    create_label(ui_Button4, &lv_font_montserrat_14, 0xFFFFFF, LV_ALIGN_CENTER, 0, 0, "NFC");

    ui_Button2 = lv_btn_create(ui_uidroppanel);
    style_round_btn(ui_Button2);
    lv_obj_set_pos(ui_Button2, 74, 72);
    ui_uiSetLabel = create_label(ui_Button2, &lv_font_montserrat_14, 0xFFFFFF, LV_ALIGN_CENTER, 0, 0, "INFO");

    ui_Slider1 = lv_slider_create(ui_uidroppanel);
    lv_obj_set_size(ui_Slider1, 82, 8);
    lv_obj_set_pos(ui_Slider1, 146, 56);
    lv_slider_set_range(ui_Slider1, 5, 100);
    lv_slider_set_value(ui_Slider1, ui_LightSliderValue, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0xD8D0C0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0x2B2A27), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Slider1, lv_color_hex(0x2B2A27), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_Slider1, brightness_slider_event, LV_EVENT_ALL, NULL);

    ui_Label11 = create_label(ui_uidroppanel, &lv_font_montserrat_14, 0x2B2A27,
                              LV_ALIGN_TOP_RIGHT, -18, 30, "LIGHT");
}

void ui_HomePage_screen_init(void)
{
    lv_obj_t * bg;
    lv_obj_t * bat_img;
    lv_obj_t * week_box;
    lv_obj_t * week_bg;
    lv_obj_t * deco_line_l;
    lv_obj_t * deco_line_r;
    lv_obj_t * left_card;
    lv_obj_t * right_card;
    lv_obj_t * center_circle;

    ui_HomePage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_HomePage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_HomePage, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_HomePage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    bg = lv_img_create(ui_HomePage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_set_align(bg, LV_ALIGN_CENTER);

    bat_img = lv_img_create(ui_HomePage);
    lv_img_set_src(bat_img, &ui_img_ink_battery_80_28x14);
    lv_obj_set_align(bat_img, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_pos(bat_img, -74, 18);

    s_bat_label = create_label(ui_HomePage, &lv_font_montserrat_24, 0x111111,
                               LV_ALIGN_TOP_RIGHT, -16, 11, "0%");

    s_date_label = create_label(ui_HomePage, &lv_font_montserrat_24, 0x111111,
                                LV_ALIGN_TOP_MID, -28, 58, "5.20");

    week_box = lv_obj_create(ui_HomePage);
    lv_obj_set_size(week_box, 54, 26);
    lv_obj_set_align(week_box, LV_ALIGN_TOP_MID);
    lv_obj_set_pos(week_box, 70, 61);
    obj_clear_box(week_box);
    lv_obj_set_style_radius(week_box, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(week_box, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    week_bg = lv_img_create(week_box);
    lv_img_set_src(week_bg, &ui_img_ink_week_bg_54x26);
    lv_obj_align(week_bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(week_bg, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    s_week_label = create_label(week_box, &ui_font_Cuyuan20, 0xFFFFFF, LV_ALIGN_CENTER, 0, 0,
                                s_week_names[1]);

    s_time_hour_label = create_label(ui_HomePage, &ui_font_Cuyuan80, 0x080808,
                                     LV_ALIGN_TOP_MID, -64, 87, "10");
    s_time_colon_top = create_colon_dot(ui_HomePage, 0, 111);
    s_time_colon_bottom = create_colon_dot(ui_HomePage, 0, 135);
    s_time_minute_label = create_label(ui_HomePage, &ui_font_Cuyuan80, 0x080808,
                                       LV_ALIGN_TOP_MID, 64, 87, "28");

    deco_line_l = lv_obj_create(ui_HomePage);
    lv_obj_set_size(deco_line_l, 36, 1);
    lv_obj_set_pos(deco_line_l, 52, 168);
    obj_clear_box(deco_line_l);
    lv_obj_set_style_bg_color(deco_line_l, lv_color_hex(0xA9A39A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(deco_line_l, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    deco_line_r = lv_obj_create(ui_HomePage);
    lv_obj_set_size(deco_line_r, 36, 1);
    lv_obj_set_pos(deco_line_r, 152, 168);
    obj_clear_box(deco_line_r);
    lv_obj_set_style_bg_color(deco_line_r, lv_color_hex(0xA9A39A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(deco_line_r, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

    create_label(ui_HomePage, &ui_font_Cuyuan24, 0x111111, LV_ALIGN_TOP_MID, 0, 153, TXT_LUNAR);

    left_card = lv_obj_create(ui_HomePage);
    lv_obj_set_size(left_card, 92, 43);
    lv_obj_set_pos(left_card, 8, 228);
    style_card(left_card);
    ui_uiStepiconLabel = lv_img_create(left_card);
    lv_img_set_src(ui_uiStepiconLabel, &ui_img_ink_footsteps_24x24);
    lv_obj_set_pos(ui_uiStepiconLabel, 9, 9);
    s_step_value_label = create_label(left_card, &lv_font_montserrat_24, 0x070707,
                                      LV_ALIGN_TOP_LEFT, 41, 6, "0");
    create_label(left_card, &ui_font_Cuyuan18, 0x66625C, LV_ALIGN_TOP_LEFT, 43, 24, TXT_STEPS);

    right_card = lv_obj_create(ui_HomePage);
    lv_obj_set_size(right_card, 92, 43);
    lv_obj_set_pos(right_card, 140, 228);
    style_card(right_card);
    ui_uiHRiconLabel = lv_img_create(right_card);
    lv_img_set_src(ui_uiHRiconLabel, &ui_img_ink_heart_rate_24x24);
    lv_obj_set_pos(ui_uiHRiconLabel, 11, 9);
    s_hr_value_label = create_label(right_card, &lv_font_montserrat_24, 0x070707,
                                    LV_ALIGN_TOP_LEFT, 43, 6, "--");
    create_label(right_card, &ui_font_Cuyuan18, 0x66625C, LV_ALIGN_TOP_LEFT, 43, 28, TXT_BPM);

    center_circle = lv_img_create(ui_HomePage);
    lv_img_set_src(center_circle, &ui_img_ink_center_mountain_circle_44x44);
    lv_obj_set_align(center_circle, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(center_circle, -12);

    ui_uipullzone = lv_obj_create(ui_HomePage);
    lv_obj_set_size(ui_uipullzone, 240, 28);
    lv_obj_set_align(ui_uipullzone, LV_ALIGN_TOP_MID);
    obj_clear_box(ui_uipullzone);
    lv_obj_add_flag(ui_uipullzone, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_bg_opa(ui_uipullzone, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_uiHumiiconLabel = NULL;
    ui_uiTempiconLabel = NULL;

    create_dropdown();
    lv_obj_update_layout(ui_HomePage);
    lv_obj_set_y(ui_uidroppanel, -lv_obj_get_height(ui_uidroppanel));

    lv_obj_add_event_cb(ui_uipullzone, ui_event_uipullzone, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_HomePage, ui_event_HomePage, LV_EVENT_ALL, NULL);

    enable_gesture_bubble_recursive(ui_HomePage);
    home_update_now();

    if(s_home_timer == NULL) {
        s_home_timer = lv_timer_create(home_timer_cb, 500, NULL);
    }

    LV_UNUSED(center_circle);
}

void ui_HomePage_screen_destroy(void)
{
    if(s_home_timer) {
        lv_timer_del(s_home_timer);
        s_home_timer = NULL;
    }

    if(ui_HomePage) lv_obj_del(ui_HomePage);

    ui_HomePage = NULL;
    ui_uipullzone = NULL;
    ui_uidroppanel = NULL;
    ui_Button1 = NULL;
    ui_uiBLELabel = NULL;
    ui_Button2 = NULL;
    ui_uiSetLabel = NULL;
    ui_Button4 = NULL;
    ui_Button5 = NULL;
    ui_Slider1 = NULL;
    ui_Label11 = NULL;
    ui_uiHRiconLabel = NULL;
    ui_uiHumiiconLabel = NULL;
    ui_uiTempiconLabel = NULL;
    ui_uiStepiconLabel = NULL;
    s_time_hour_label = NULL;
    s_time_colon_top = NULL;
    s_time_colon_bottom = NULL;
    s_time_minute_label = NULL;
    s_date_label = NULL;
    s_week_label = NULL;
    s_bat_label = NULL;
    s_step_value_label = NULL;
    s_hr_value_label = NULL;
}
