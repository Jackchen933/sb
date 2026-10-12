// Ink style settings page for OV_Watch.

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_SetPage.h"
#include "../Inc/ui_OffTimePage.h"
#include "../Inc/ui_DateTimeSetPage.h"

#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_setting_28x28);

#define TXT_SET       "\xE8\xAE\xBE\xE7\xBD\xAE"
#define TXT_LIGHT     "\xE5\xB8\xB8\xE4\xBA\xAE\xE6\x97\xB6\xE9\x97\xB4"
#define TXT_OFF       "\xE7\x86\x84\xE5\xB1\x8F\xE6\x97\xB6\xE9\x97\xB4"
#define TXT_DATETIME  "\xE6\x97\xB6\xE9\x97\xB4\xE6\x97\xA5\xE6\x9C\x9F"
#define TXT_WRIST     "\xE6\x8A\xAC\xE8\x85\x95\xE4\xBA\xAE\xE5\xB1\x8F"
#define TXT_PASSWORD  "\xE5\xAF\x86\xE7\xA0\x81\xE8\xAE\xBE\xE7\xBD\xAE"

///////////////////// Page Manager //////////////////
Page_t Page_Set = {ui_SetPage_screen_init, ui_SetPage_screen_deinit, &ui_SetPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_SetPage = NULL;
lv_obj_t * ui_LightTimePanel = NULL;
lv_obj_t * ui_LightTimeButton = NULL;
lv_obj_t * ui_LightTimeicon = NULL;
lv_obj_t * ui_LightTimeLabel = NULL;
lv_obj_t * ui_TurnOffTimePanel = NULL;
lv_obj_t * ui_TOffTimeButton = NULL;
lv_obj_t * ui_TOffTimeicon = NULL;
lv_obj_t * ui_TOffTimeLabel = NULL;
lv_obj_t * ui_DateTimeSetPanel = NULL;
lv_obj_t * ui_DateTimeSetButton = NULL;
lv_obj_t * ui_LOffTimeicon = NULL;
lv_obj_t * ui_DateTimeSetLabel = NULL;
lv_obj_t * ui_WristPanel = NULL;
lv_obj_t * ui_WristButton = NULL;
lv_obj_t * ui_Wristicon = NULL;
lv_obj_t * ui_WristLabel = NULL;
lv_obj_t * ui_WristSwitch = NULL;
lv_obj_t * ui_PasswordPanel = NULL;
lv_obj_t * ui_PasswordButton = NULL;
lv_obj_t * ui_Passwordicon = NULL;
lv_obj_t * ui_PasswordLabel = NULL;

///////////////////// FUNCTIONS ////////////////////
static void page_back_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Back();
    }
}

void ui_event_SetPage(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_GESTURE &&
       lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

void ui_event_LightTimePanel(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_LOffTimeSet);
    }
}

void ui_event_TOffTimePanel(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_TOffTimeSet);
    }
}

void ui_event_WristSwitch(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        if(lv_obj_has_state(ui_WristSwitch, LV_STATE_CHECKED)) {
            HWInterface.IMU.WristEnable();
        } else {
            HWInterface.IMU.WristDisable();
        }
    }
}

void ui_event_DateTimeSetPanel(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_DateTimeSet);
    }
}

void ui_event_PasswordPanel(lv_event_t * e)
{
    (void)e;
}

static void make_header(lv_obj_t * parent)
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
    lv_obj_set_pos(icon, 75, 8);

    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, TXT_SET);
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
    lv_obj_set_style_border_width(seal, 1, 0);
    lv_obj_set_style_border_color(seal, lv_color_hex(0x6B211B), 0);
    lv_obj_set_style_border_opa(seal, LV_OPA_40, 0);
}

static lv_obj_t * make_setting_item(lv_obj_t * parent,
                                    int16_t y,
                                    const char * badge_text,
                                    const char * label_text,
                                    lv_obj_t ** badge_out,
                                    lv_obj_t ** badge_label_out,
                                    lv_obj_t ** label_out)
{
    lv_obj_t * item = lv_btn_create(parent);
    lv_obj_remove_style_all(item);
    lv_obj_set_size(item, 212, 38);
    lv_obj_set_pos(item, 14, y);
    lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_style_radius(item, 8, 0);
    lv_obj_set_style_bg_color(item, lv_color_hex(0xF8F6EE), 0);
    lv_obj_set_style_bg_opa(item, LV_OPA_70, 0);
    lv_obj_set_style_border_width(item, 1, 0);
    lv_obj_set_style_border_color(item, lv_color_hex(0x272B28), 0);
    lv_obj_set_style_border_opa(item, LV_OPA_40, 0);

    lv_obj_t * badge = lv_obj_create(item);
    lv_obj_remove_style_all(badge);
    lv_obj_set_size(badge, 26, 26);
    lv_obj_align(badge, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_set_style_radius(badge, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(badge, lv_color_hex(0x982B24), 0);
    lv_obj_set_style_bg_opa(badge, LV_OPA_70, 0);

    lv_obj_t * badge_label = lv_label_create(badge);
    lv_label_set_text(badge_label, badge_text);
    lv_obj_set_style_text_font(badge_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(badge_label, lv_color_hex(0xF8F4EA), 0);
    lv_obj_center(badge_label);

    lv_obj_t * label = lv_label_create(item);
    lv_label_set_text(label, label_text);
    lv_obj_set_style_text_font(label, &ui_font_Cuyuan20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0x1B1E1B), 0);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 48, 0);

    lv_obj_t * arrow = lv_label_create(item);
    lv_label_set_text(arrow, ">");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x56615A), 0);
    lv_obj_set_style_text_opa(arrow, LV_OPA_70, 0);
    lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, -12, 0);

    if(badge_out) *badge_out = badge;
    if(badge_label_out) *badge_label_out = badge_label;
    if(label_out) *label_out = label;
    return item;
}

///////////////////// SCREEN init ////////////////////
void ui_SetPage_screen_init(void)
{
    ui_SetPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_SetPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_SetPage, lv_color_hex(0xF4F1E6), 0);
    lv_obj_set_style_bg_opa(ui_SetPage, LV_OPA_COVER, 0);

    lv_obj_t * bg = lv_img_create(ui_SetPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * layer = lv_obj_create(ui_SetPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    make_header(layer);

    ui_LightTimePanel = make_setting_item(layer, 50, "L", TXT_LIGHT,
                                          &ui_LightTimeButton, &ui_LightTimeicon, &ui_LightTimeLabel);
    ui_TurnOffTimePanel = make_setting_item(layer, 94, "S", TXT_OFF,
                                            &ui_TOffTimeButton, &ui_TOffTimeicon, &ui_TOffTimeLabel);
    ui_DateTimeSetPanel = make_setting_item(layer, 138, "D", TXT_DATETIME,
                                            &ui_DateTimeSetButton, &ui_LOffTimeicon, &ui_DateTimeSetLabel);
    ui_WristPanel = make_setting_item(layer, 182, "W", TXT_WRIST,
                                      &ui_WristButton, &ui_Wristicon, &ui_WristLabel);
    ui_PasswordPanel = make_setting_item(layer, 226, "P", TXT_PASSWORD,
                                         &ui_PasswordButton, &ui_Passwordicon, &ui_PasswordLabel);

    ui_WristSwitch = lv_switch_create(ui_WristPanel);
    lv_obj_set_size(ui_WristSwitch, 46, 24);
    lv_obj_align(ui_WristSwitch, LV_ALIGN_RIGHT_MID, -12, 0);
    lv_obj_add_flag(ui_WristSwitch, LV_OBJ_FLAG_GESTURE_BUBBLE);
    if(HWInterface.IMU.wrist_is_enabled) {
        lv_obj_add_state(ui_WristSwitch, LV_STATE_CHECKED);
    }

    lv_obj_add_event_cb(ui_SetPage, ui_event_SetPage, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_LightTimePanel, ui_event_LightTimePanel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_TurnOffTimePanel, ui_event_TOffTimePanel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WristSwitch, ui_event_WristSwitch, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_DateTimeSetPanel, ui_event_DateTimeSetPanel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_PasswordPanel, ui_event_PasswordPanel, LV_EVENT_ALL, NULL);
}

/////////////////// SCREEN deinit ////////////////////
void ui_SetPage_screen_deinit(void)
{
}
