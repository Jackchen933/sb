// Ink style date/time setting pages for OV_Watch.

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_HomePage.h"
#include "../Inc/ui_DateTimeSetPage.h"

#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_setting_28x28);

#define TXT_DATETIME "\xE6\x97\xB6\xE9\x97\xB4\xE6\x97\xA5\xE6\x9C\x9F"
#define TXT_SYNC     "\xE5\x90\x8C\xE6\xAD\xA5" "APP"
#define TXT_DATE     "\xE6\x97\xA5\xE6\x9C\x9F\xE8\xAE\xBE\xE7\xBD\xAE"
#define TXT_TIME     "\xE6\x97\xB6\xE9\x97\xB4\xE8\xAE\xBE\xE7\xBD\xAE"

///////////////////// Page Manager //////////////////
Page_t Page_DateTimeSet = {ui_DateTimeSetPage_screen_init, ui_DateTimeSetPage_screen_deinit, &ui_DateTimeSetPage};
Page_t Page_DateSet = {ui_DateSetPage_screen_init, ui_DateSetPage_screen_deinit, &ui_DateSetPage};
Page_t Page_TimeSet = {ui_TimeSetPage_screen_init, ui_TimeSetPage_screen_deinit, &ui_TimeSetPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_DateTimeSetPage = NULL;
lv_obj_t * ui_APPSyPanel = NULL;
lv_obj_t * ui_APPSySwitch = NULL;
lv_obj_t * ui_APPSyLabel = NULL;
lv_obj_t * ui_DateSetPanel = NULL;
lv_obj_t * ui_DateSetLabel = NULL;
lv_obj_t * ui_DateSetLabel1 = NULL;
lv_obj_t * ui_DateSetLabel2 = NULL;
lv_obj_t * ui_DateSetLabel3 = NULL;
lv_obj_t * ui_TimeSetPanel = NULL;
lv_obj_t * ui_TimeSetLabel = NULL;
lv_obj_t * ui_TimeSetLabel1 = NULL;
lv_obj_t * ui_TimeSetLabel2 = NULL;
lv_obj_t * ui_TimeSetLabel3 = NULL;
lv_obj_t * ui_DateSetPage = NULL;
lv_obj_t * ui_YearSetRoller = NULL;
lv_obj_t * ui_MonthSetRoller = NULL;
lv_obj_t * ui_DaySetRoller = NULL;
lv_obj_t * ui_DateSetOKButton = NULL;
lv_obj_t * ui_DateSetOKicon = NULL;
lv_obj_t * ui_TimeSetPage = NULL;
lv_obj_t * ui_HourSetRoller = NULL;
lv_obj_t * ui_TimePoint = NULL;
lv_obj_t * ui_MinSetRoller = NULL;
lv_obj_t * ui_TimePoint1 = NULL;
lv_obj_t * ui_SecSetRoller = NULL;
lv_obj_t * ui_TimeSetOKButton = NULL;
lv_obj_t * ui_TimeSetOKicon = NULL;

uint8_t ui_APPSy_EN = 0;

const char yearoptions[] = "2022\n2023\n2024\n2025\n2026\n2027\n2028\n2029\n2030\n2031\n2032\n2033\n2034\n2035";
const char monthoptions[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12";
const char day31options[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31";
const char day30options[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30";
const char day29options[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29";
const char day28options[] = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28";
const char houroptions[] = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23";
const char minoptions[] = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";

///////////////////// FUNCTIONS ////////////////////
static void page_back_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Back();
    }
}

static void page_gesture_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_GESTURE &&
       lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

void ui_event_DateTimeSetPage(lv_event_t * e)
{
    page_gesture_event_cb(e);
}

void ui_event_DateSetPanel(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_DateSet);
    }
}

void ui_event_TimeSetPanel(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_TimeSet);
    }
}

void ui_event_DateSetOKButton(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        uint8_t setyear = lv_roller_get_selected(ui_YearSetRoller) + 22;
        uint8_t setmonth = lv_roller_get_selected(ui_MonthSetRoller) + 1;
        uint8_t setday = lv_roller_get_selected(ui_DaySetRoller) + 1;

        HWInterface.RealTimeClock.SetDate(setyear, setmonth, setday);
        ui_DateMonthValue = setmonth;
        ui_DateDayValue = setday;
        ui_DataWeekdayValue = HWInterface.RealTimeClock.CalculateWeekday(setyear, setmonth, setday, 20);
        Page_Back();
    }
}

void ui_event_TimeSetOKButton(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        uint8_t sethour = lv_roller_get_selected(ui_HourSetRoller);
        uint8_t setmin = lv_roller_get_selected(ui_MinSetRoller);
        uint8_t setsec = lv_roller_get_selected(ui_SecSetRoller);

        HWInterface.RealTimeClock.SetTime(sethour, setmin, setsec);
        ui_TimeHourValue = sethour;
        ui_TimeMinuteValue = setmin;
        Page_Back();
    }
}

void ui_event_APPSySwitch(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        ui_APPSy_EN = lv_obj_has_state(ui_APPSySwitch, LV_STATE_CHECKED) ? 1 : 0;
    }
}

static lv_obj_t * make_base_page(lv_obj_t ** page_out, const char * title_text)
{
    lv_obj_t * page = lv_obj_create(NULL);
    *page_out = page;
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(page, lv_color_hex(0xF4F1E6), 0);
    lv_obj_set_style_bg_opa(page, LV_OPA_COVER, 0);

    lv_obj_t * bg = lv_img_create(page);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * layer = lv_obj_create(page);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);

    lv_obj_t * back = lv_btn_create(layer);
    lv_obj_remove_style_all(back);
    lv_obj_set_size(back, 48, 42);
    lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(back, page_back_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * arrow = lv_label_create(back);
    lv_label_set_text(arrow, "<");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x1D211D), 0);
    lv_obj_set_style_text_opa(arrow, LV_OPA_80, 0);
    lv_obj_center(arrow);

    lv_obj_t * icon = lv_img_create(layer);
    lv_img_set_src(icon, &ui_img_menu_setting_28x28);
    lv_obj_set_pos(icon, 54, 8);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, title_text);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1B1E1B), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 16, 12);

    lv_obj_t * seal = lv_obj_create(layer);
    lv_obj_remove_style_all(seal);
    lv_obj_set_size(seal, 22, 22);
    lv_obj_set_pos(seal, 198, 11);
    lv_obj_set_style_radius(seal, 4, 0);
    lv_obj_set_style_bg_color(seal, lv_color_hex(0x982B24), 0);
    lv_obj_set_style_bg_opa(seal, LV_OPA_70, 0);

    lv_obj_add_event_cb(page, page_gesture_event_cb, LV_EVENT_ALL, NULL);
    return layer;
}

static lv_obj_t * make_list_item(lv_obj_t * parent, int16_t y, const char * text)
{
    lv_obj_t * item = lv_btn_create(parent);
    lv_obj_remove_style_all(item);
    lv_obj_set_size(item, 212, 48);
    lv_obj_set_pos(item, 14, y);
    lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_style_radius(item, 8, 0);
    lv_obj_set_style_bg_color(item, lv_color_hex(0xF8F6EE), 0);
    lv_obj_set_style_bg_opa(item, LV_OPA_70, 0);
    lv_obj_set_style_border_width(item, 1, 0);
    lv_obj_set_style_border_color(item, lv_color_hex(0x272B28), 0);
    lv_obj_set_style_border_opa(item, LV_OPA_40, 0);

    lv_obj_t * label = lv_label_create(item);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &ui_font_Cuyuan20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x1B1E1B), 0);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 16, 0);

    lv_obj_t * arrow = lv_label_create(item);
    lv_label_set_text(arrow, ">");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x56615A), 0);
    lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, -14, 0);
    return item;
}

static void style_roller(lv_obj_t * roller, const lv_font_t * font, lv_coord_t w)
{
    lv_obj_set_size(roller, w, 132);
    lv_obj_set_style_text_font(roller, font, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(roller, lv_color_hex(0x56615A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(roller, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(roller, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(roller, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(roller, lv_color_hex(0x111511), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(roller, lv_color_hex(0xF8F6EE), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(roller, LV_OPA_70, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(roller, lv_color_hex(0x982B24), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(roller, LV_OPA_60, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(roller, 2, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(roller, 1, LV_PART_SELECTED | LV_STATE_DEFAULT);
}

static void make_ok_button(lv_obj_t * parent, lv_obj_t ** btn_out, lv_obj_t ** label_out, lv_event_cb_t cb)
{
    *btn_out = lv_btn_create(parent);
    lv_obj_remove_style_all(*btn_out);
    lv_obj_set_size(*btn_out, 128, 36);
    lv_obj_align(*btn_out, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_flag(*btn_out, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_style_radius(*btn_out, 8, 0);
    lv_obj_set_style_bg_color(*btn_out, lv_color_hex(0x982B24), 0);
    lv_obj_set_style_bg_opa(*btn_out, LV_OPA_80, 0);
    lv_obj_add_event_cb(*btn_out, cb, LV_EVENT_ALL, NULL);

    *label_out = lv_label_create(*btn_out);
    lv_label_set_text(*label_out, "OK");
    lv_obj_set_style_text_font(*label_out, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(*label_out, lv_color_hex(0xF8F4EA), 0);
    lv_obj_center(*label_out);
}

///////////////////// SCREEN init ////////////////////
void ui_DateTimeSetPage_screen_init(void)
{
    lv_obj_t * layer = make_base_page(&ui_DateTimeSetPage, TXT_DATETIME);

    ui_APPSyPanel = make_list_item(layer, 62, TXT_SYNC);
    lv_obj_clear_flag(ui_APPSyPanel, LV_OBJ_FLAG_CLICKABLE);
    ui_APPSyLabel = lv_obj_get_child(ui_APPSyPanel, 0);
    ui_APPSySwitch = lv_switch_create(ui_APPSyPanel);
    lv_obj_set_size(ui_APPSySwitch, 46, 24);
    lv_obj_align(ui_APPSySwitch, LV_ALIGN_RIGHT_MID, -12, 0);
    if(ui_APPSy_EN) {
        lv_obj_add_state(ui_APPSySwitch, LV_STATE_CHECKED);
    }

    ui_DateSetPanel = make_list_item(layer, 118, TXT_DATE);
    ui_DateSetLabel = lv_obj_get_child(ui_DateSetPanel, 0);
    ui_DateSetLabel1 = lv_obj_get_child(ui_DateSetPanel, 1);
    ui_DateSetLabel2 = NULL;
    ui_DateSetLabel3 = NULL;

    ui_TimeSetPanel = make_list_item(layer, 174, TXT_TIME);
    ui_TimeSetLabel = lv_obj_get_child(ui_TimeSetPanel, 0);
    ui_TimeSetLabel1 = lv_obj_get_child(ui_TimeSetPanel, 1);
    ui_TimeSetLabel2 = NULL;
    ui_TimeSetLabel3 = NULL;

    lv_obj_add_event_cb(ui_DateSetPanel, ui_event_DateSetPanel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TimeSetPanel, ui_event_TimeSetPanel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_APPSySwitch, ui_event_APPSySwitch, LV_EVENT_ALL, NULL);
}

void ui_DateSetPage_screen_init(void)
{
    HW_DateTimeTypeDef datetime;
    HWInterface.RealTimeClock.GetTimeDate(&datetime);

    lv_obj_t * layer = make_base_page(&ui_DateSetPage, TXT_DATE);

    lv_obj_t * panel = lv_obj_create(layer);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, 212, 158);
    lv_obj_set_pos(panel, 14, 66);
    lv_obj_set_style_radius(panel, 8, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xF8F6EE), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_70, 0);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_color(panel, lv_color_hex(0x272B28), 0);
    lv_obj_set_style_border_opa(panel, LV_OPA_40, 0);

    ui_YearSetRoller = lv_roller_create(panel);
    lv_roller_set_options(ui_YearSetRoller, yearoptions, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_YearSetRoller, datetime.Year - 22, LV_ANIM_OFF);
    lv_obj_align(ui_YearSetRoller, LV_ALIGN_LEFT_MID, 4, 0);
    style_roller(ui_YearSetRoller, &ui_font_Cuyuan30, 76);

    ui_MonthSetRoller = lv_roller_create(panel);
    lv_roller_set_options(ui_MonthSetRoller, monthoptions, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_MonthSetRoller, datetime.Month - 1, LV_ANIM_OFF);
    lv_obj_align(ui_MonthSetRoller, LV_ALIGN_CENTER, 16, 0);
    style_roller(ui_MonthSetRoller, &ui_font_Cuyuan30, 48);

    ui_DaySetRoller = lv_roller_create(panel);
    lv_roller_set_options(ui_DaySetRoller, day31options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_DaySetRoller, datetime.Date - 1, LV_ANIM_OFF);
    lv_obj_align(ui_DaySetRoller, LV_ALIGN_RIGHT_MID, -4, 0);
    style_roller(ui_DaySetRoller, &ui_font_Cuyuan30, 48);

    make_ok_button(layer, &ui_DateSetOKButton, &ui_DateSetOKicon, ui_event_DateSetOKButton);
}

void ui_TimeSetPage_screen_init(void)
{
    HW_DateTimeTypeDef datetime;
    HWInterface.RealTimeClock.GetTimeDate(&datetime);

    lv_obj_t * layer = make_base_page(&ui_TimeSetPage, TXT_TIME);

    lv_obj_t * panel = lv_obj_create(layer);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, 212, 158);
    lv_obj_set_pos(panel, 14, 66);
    lv_obj_set_style_radius(panel, 8, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xF8F6EE), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_70, 0);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_color(panel, lv_color_hex(0x272B28), 0);
    lv_obj_set_style_border_opa(panel, LV_OPA_40, 0);

    ui_HourSetRoller = lv_roller_create(panel);
    lv_roller_set_options(ui_HourSetRoller, houroptions, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_HourSetRoller, datetime.Hours, LV_ANIM_OFF);
    lv_obj_align(ui_HourSetRoller, LV_ALIGN_LEFT_MID, 8, 0);
    style_roller(ui_HourSetRoller, &ui_font_Cuyuan38, 52);

    ui_TimePoint = lv_label_create(panel);
    lv_label_set_text(ui_TimePoint, ":");
    lv_obj_set_style_text_font(ui_TimePoint, &ui_font_Cuyuan38, 0);
    lv_obj_set_style_text_color(ui_TimePoint, lv_color_hex(0x982B24), 0);
    lv_obj_align(ui_TimePoint, LV_ALIGN_CENTER, -34, 0);

    ui_MinSetRoller = lv_roller_create(panel);
    lv_roller_set_options(ui_MinSetRoller, minoptions, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_MinSetRoller, datetime.Minutes, LV_ANIM_OFF);
    lv_obj_align(ui_MinSetRoller, LV_ALIGN_CENTER, 0, 0);
    style_roller(ui_MinSetRoller, &ui_font_Cuyuan38, 52);

    ui_TimePoint1 = lv_label_create(panel);
    lv_label_set_text(ui_TimePoint1, ":");
    lv_obj_set_style_text_font(ui_TimePoint1, &ui_font_Cuyuan38, 0);
    lv_obj_set_style_text_color(ui_TimePoint1, lv_color_hex(0x982B24), 0);
    lv_obj_align(ui_TimePoint1, LV_ALIGN_CENTER, 34, 0);

    ui_SecSetRoller = lv_roller_create(panel);
    lv_roller_set_options(ui_SecSetRoller, minoptions, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_SecSetRoller, datetime.Seconds, LV_ANIM_OFF);
    lv_obj_align(ui_SecSetRoller, LV_ALIGN_RIGHT_MID, -8, 0);
    style_roller(ui_SecSetRoller, &ui_font_Cuyuan38, 52);

    make_ok_button(layer, &ui_TimeSetOKButton, &ui_TimeSetOKicon, ui_event_TimeSetOKButton);
}

/////////////////// SCREEN deinit ////////////////////
void ui_DateTimeSetPage_screen_deinit(void)
{
}

void ui_DateSetPage_screen_deinit(void)
{
}

void ui_TimeSetPage_screen_deinit(void)
{
}
