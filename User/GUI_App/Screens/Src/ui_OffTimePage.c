// Ink style screen-time setting pages for OV_Watch.

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_OffTimePage.h"

#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_setting_28x28);

#define TXT_LIGHT "\xE5\xB8\xB8\xE4\xBA\xAE\xE6\x97\xB6\xE9\x97\xB4"
#define TXT_SLEEP "\xE7\x86\x84\xE5\xB1\x8F\xE6\x97\xB6\xE9\x97\xB4"
#define TXT_SEC   "\xE7\xA7\x92"

///////////////////// Page Manager //////////////////
Page_t Page_LOffTimeSet = {ui_LOffTimeSetPage_screen_init, ui_LOffTimeSetPage_screen_deinit, &ui_LOffTimeSetPage};
Page_t Page_TOffTimeSet = {ui_TOffTimeSetPage_screen_init, ui_TOffTimeSetPage_screen_deinit, &ui_TOffTimeSetPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_LOffTimeSetPage = NULL;
lv_obj_t * ui_LTimeSetRoller = NULL;
lv_obj_t * ui_LTimeSetUnitLabel = NULL;
lv_obj_t * ui_LTimeSetOKButton = NULL;
lv_obj_t * ui_LTimeSetOKLabel = NULL;
lv_obj_t * ui_LTimeSeticon = NULL;

lv_obj_t * ui_TOffTimeSetPage = NULL;
lv_obj_t * ui_TTimeSetRoller = NULL;
lv_obj_t * ui_TTimeSetUnitLabel = NULL;
lv_obj_t * ui_TTimeSetOKButton = NULL;
lv_obj_t * ui_TTimeSetOKLabel = NULL;
lv_obj_t * ui_TTimeSeticon = NULL;

const uint8_t ui_LTimeOptions[6] = {10, 15, 20, 30, 45, 60};
const uint8_t ui_TTimeOptions[6] = {15, 20, 25, 35, 50, 65};
uint8_t ui_LTimeValue = 10;
uint8_t ui_TTimeValue = 15;

uint8_t ui_LTimeSelected = 0;
uint8_t ui_TTimeSelected = 0;

///////////////////// FUNCTIONS ////////////////////
static void page_gesture_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_GESTURE &&
       lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

static void page_back_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Back();
    }
}

void ui_event_LTimeSetOKButton(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_LTimeSelected = lv_roller_get_selected(ui_LTimeSetRoller);
        ui_LTimeValue = ui_LTimeOptions[ui_LTimeSelected];
        Page_Back();
    }
}

void ui_event_TTimeSetOKButton(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_TTimeSelected = lv_roller_get_selected(ui_TTimeSetRoller);
        ui_TTimeValue = ui_TTimeOptions[ui_TTimeSelected];
        Page_Back();
    }
}

static void make_header(lv_obj_t * parent, const char * title_text)
{
    lv_obj_t * back = lv_btn_create(parent);
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

    lv_obj_t * icon = lv_img_create(parent);
    lv_img_set_src(icon, &ui_img_menu_setting_28x28);
    lv_obj_set_pos(icon, 54, 8);

    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, title_text);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1B1E1B), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 16, 12);

    lv_obj_t * seal = lv_obj_create(parent);
    lv_obj_remove_style_all(seal);
    lv_obj_set_size(seal, 22, 22);
    lv_obj_set_pos(seal, 198, 11);
    lv_obj_set_style_radius(seal, 4, 0);
    lv_obj_set_style_bg_color(seal, lv_color_hex(0x982B24), 0);
    lv_obj_set_style_bg_opa(seal, LV_OPA_70, 0);
}

static void style_roller(lv_obj_t * roller)
{
    lv_obj_set_size(roller, 74, 140);
    lv_obj_set_style_text_font(roller, &ui_font_Cuyuan38, LV_PART_MAIN | LV_STATE_DEFAULT);
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

static void init_time_page(lv_obj_t ** page_out,
                           lv_obj_t ** roller_out,
                           lv_obj_t ** unit_out,
                           lv_obj_t ** ok_btn_out,
                           lv_obj_t ** ok_label_out,
                           lv_obj_t ** icon_out,
                           const char * title_text,
                           const char * options,
                           uint8_t selected,
                           lv_event_cb_t ok_cb)
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
    make_header(layer, title_text);

    lv_obj_t * panel = lv_obj_create(layer);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, 176, 168);
    lv_obj_set_pos(panel, 32, 62);
    lv_obj_set_style_radius(panel, 8, 0);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xF8F6EE), 0);
    lv_obj_set_style_bg_opa(panel, LV_OPA_70, 0);
    lv_obj_set_style_border_width(panel, 1, 0);
    lv_obj_set_style_border_color(panel, lv_color_hex(0x272B28), 0);
    lv_obj_set_style_border_opa(panel, LV_OPA_40, 0);

    *roller_out = lv_roller_create(panel);
    lv_roller_set_options(*roller_out, options, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(*roller_out, selected, LV_ANIM_OFF);
    lv_obj_align(*roller_out, LV_ALIGN_CENTER, -12, 0);
    style_roller(*roller_out);

    *unit_out = lv_label_create(panel);
    lv_label_set_text(*unit_out, TXT_SEC);
    lv_obj_set_style_text_font(*unit_out, &ui_font_Cuyuan20, 0);
    lv_obj_set_style_text_color(*unit_out, lv_color_hex(0x56615A), 0);
    lv_obj_align(*unit_out, LV_ALIGN_CENTER, 52, 0);

    *ok_btn_out = lv_btn_create(layer);
    lv_obj_remove_style_all(*ok_btn_out);
    lv_obj_set_size(*ok_btn_out, 128, 36);
    lv_obj_align(*ok_btn_out, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_flag(*ok_btn_out, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_style_radius(*ok_btn_out, 8, 0);
    lv_obj_set_style_bg_color(*ok_btn_out, lv_color_hex(0x982B24), 0);
    lv_obj_set_style_bg_opa(*ok_btn_out, LV_OPA_80, 0);
    lv_obj_add_event_cb(*ok_btn_out, ok_cb, LV_EVENT_ALL, NULL);

    *ok_label_out = lv_label_create(*ok_btn_out);
    lv_label_set_text(*ok_label_out, "OK");
    lv_obj_set_style_text_font(*ok_label_out, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(*ok_label_out, lv_color_hex(0xF8F4EA), 0);
    lv_obj_center(*ok_label_out);

    *icon_out = lv_img_create(layer);
    lv_img_set_src(*icon_out, &ui_img_menu_setting_28x28);
    lv_obj_align(*icon_out, LV_ALIGN_TOP_MID, 0, 42);

    lv_obj_add_event_cb(page, page_gesture_event_cb, LV_EVENT_ALL, NULL);
}

///////////////////// SCREEN init ////////////////////
void ui_LOffTimeSetPage_screen_init(void)
{
    init_time_page(&ui_LOffTimeSetPage, &ui_LTimeSetRoller, &ui_LTimeSetUnitLabel,
                   &ui_LTimeSetOKButton, &ui_LTimeSetOKLabel, &ui_LTimeSeticon,
                   TXT_LIGHT, "10\n15\n20\n30\n45\n60", ui_LTimeSelected,
                   ui_event_LTimeSetOKButton);
}

void ui_TOffTimeSetPage_screen_init(void)
{
    init_time_page(&ui_TOffTimeSetPage, &ui_TTimeSetRoller, &ui_TTimeSetUnitLabel,
                   &ui_TTimeSetOKButton, &ui_TTimeSetOKLabel, &ui_TTimeSeticon,
                   TXT_SLEEP, "15\n20\n25\n35\n50\n65", ui_TTimeSelected,
                   ui_event_TTimeSetOKButton);
}

///////////////////// SCREEN deinit ////////////////////
void ui_LOffTimeSetPage_screen_deinit(void)
{
}

void ui_TOffTimeSetPage_screen_deinit(void)
{
}
