// Ink style full menu page for OV_Watch.

#include "../ui.h"
#include "PageManager.h"

extern Page_t Page_Calender;
extern Page_t Page_Computer;
extern Page_t Page_Timer;
extern Page_t Page_NFCCard;
extern Page_t Page_HR;
extern Page_t Page_SPO2;
extern Page_t Page_Env;
extern Page_t Page_Compass;
extern Page_t Page_GameSelect;
extern Page_t Page_Set;
extern Page_t Page_SquareAbout;

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_calendar_28x28);
LV_IMG_DECLARE(ui_img_menu_computer_28x28);
LV_IMG_DECLARE(ui_img_menu_timer_28x28);
LV_IMG_DECLARE(ui_img_menu_card_28x28);
LV_IMG_DECLARE(ui_img_menu_hr_28x28);
LV_IMG_DECLARE(ui_img_menu_spo2_28x28);
LV_IMG_DECLARE(ui_img_menu_env_28x28);
LV_IMG_DECLARE(ui_img_menu_compass_28x28);
LV_IMG_DECLARE(ui_img_menu_game_28x28);
LV_IMG_DECLARE(ui_img_menu_setting_28x28);
LV_IMG_DECLARE(ui_img_menu_about_28x28);
LV_IMG_DECLARE(ui_img_menu_title_72x34);

LV_FONT_DECLARE(ui_font_Cuyuan20);

#define TXT_CALENDAR "\xE6\x97\xA5\xE5\x8E\x86"
#define TXT_COMPUTER "\xE8\xAE\xA1\xE7\xAE\x97\xE5\x99\xA8"
#define TXT_TIMER    "\xE7\xA7\x92\xE8\xA1\xA8"
#define TXT_CARD     "\xE5\x8D\xA1\xE5\x8C\x85"
#define TXT_HR       "\xE5\xBF\x83\xE7\x8E\x87"
#define TXT_SPO2     "\xE8\xA1\x80\xE6\xB0\xA7"
#define TXT_ENV      "\xE7\x8E\xAF\xE5\xA2\x83"
#define TXT_COMPASS  "\xE6\x8C\x87\xE5\x8D\x97\xE9\x92\x88"
#define TXT_GAME     "\xE6\xB8\xB8\xE6\x88\x8F"
#define TXT_SET      "\xE8\xAE\xBE\xE7\xBD\xAE"
#define TXT_ABOUT    "\xE5\x85\xB3\xE4\xBA\x8E"

typedef struct {
    const void * icon_src;
    const char * title_text;
    const char * sub_text;
    Page_t * target_page;
    lv_obj_t ** panel_out;
    lv_obj_t ** button_out;
    lv_obj_t ** icon_out;
    lv_obj_t ** label_out;
} menu_item_desc_t;

lv_obj_t * ui_MenuPage = NULL;
lv_obj_t * ui_Container3 = NULL;
lv_obj_t * ui_MenuList = NULL;

lv_obj_t * ui_MenuCalPanel = NULL;
lv_obj_t * ui_MenuCalButton = NULL;
lv_obj_t * ui_MenuCalicon = NULL;
lv_obj_t * ui_MenuCalLabel = NULL;

lv_obj_t * ui_MenuComPanel = NULL;
lv_obj_t * ui_MenuComButton = NULL;
lv_obj_t * ui_MenuComicon = NULL;
lv_obj_t * ui_MenuComLabel = NULL;

lv_obj_t * ui_MenuTimPanel = NULL;
lv_obj_t * ui_MenuTimButton = NULL;
lv_obj_t * ui_MenuTimicon = NULL;
lv_obj_t * ui_MenuTimLabel = NULL;

lv_obj_t * ui_MenuCardPanel = NULL;
lv_obj_t * ui_MenuCardButton = NULL;
lv_obj_t * ui_MenuCardicon = NULL;
lv_obj_t * ui_MenuCardLabel = NULL;

lv_obj_t * ui_MenuSprPanel = NULL;
lv_obj_t * ui_MenuSprButton = NULL;
lv_obj_t * ui_MenuSpricon = NULL;
lv_obj_t * ui_MenuSprLabel = NULL;

lv_obj_t * ui_MenuHRPanel = NULL;
lv_obj_t * ui_MenuHRButton = NULL;
lv_obj_t * ui_MenuHRicon = NULL;
lv_obj_t * ui_MenuHRLabel = NULL;

lv_obj_t * ui_MenuO2Panel = NULL;
lv_obj_t * ui_MenuO2Button = NULL;
lv_obj_t * ui_MenuO2icon = NULL;
lv_obj_t * ui_MenuO2Label = NULL;

lv_obj_t * ui_MenuEnvPanel = NULL;
lv_obj_t * ui_MenuEnvButton = NULL;
lv_obj_t * ui_MenuEnvicon = NULL;
lv_obj_t * ui_MenuEnvLabel = NULL;

lv_obj_t * ui_MenuCPPanel = NULL;
lv_obj_t * ui_MenuCPButton = NULL;
lv_obj_t * ui_MenuCPicon = NULL;
lv_obj_t * ui_MenuCPLabel = NULL;

lv_obj_t * ui_MenuGamePanel = NULL;
lv_obj_t * ui_MenuGameButton = NULL;
lv_obj_t * ui_MenuGameicon = NULL;
lv_obj_t * ui_MenuGameLabel = NULL;

lv_obj_t * ui_MenuSetPanel = NULL;
lv_obj_t * ui_MenuSetButton = NULL;
lv_obj_t * ui_MenuSeticon = NULL;
lv_obj_t * ui_MenuSetLabel = NULL;

lv_obj_t * ui_MenuAbPanel = NULL;
lv_obj_t * ui_MenuAbButton = NULL;
lv_obj_t * ui_MenuAbicon = NULL;
lv_obj_t * ui_MenuAbLabel = NULL;

int16_t ui_MenuScrollY = 0;

void ui_MenuPage_screen_init(void);
void ui_MenuPage_screen_destroy(void);
void ui_MenuPage_screen_deinit(void);

Page_t Page_Menu = {ui_MenuPage_screen_init, ui_MenuPage_screen_deinit, &ui_MenuPage};

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

static lv_obj_t * menu_make_dot(lv_obj_t * parent)
{
    lv_obj_t * dot = lv_obj_create(parent);
    lv_obj_remove_style_all(dot);
    lv_obj_set_size(dot, 5, 5);
    lv_obj_set_style_radius(dot, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dot, lv_color_hex(0x9C2B22), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dot, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    return dot;
}

static lv_obj_t * menu_make_item(lv_obj_t * parent,
                                 int16_t y,
                                 const void * icon_src,
                                 const char * title_text,
                                 const char * sub_text,
                                 lv_obj_t ** panel_out,
                                 lv_obj_t ** button_out,
                                 lv_obj_t ** icon_out,
                                 lv_obj_t ** label_out)
{
    lv_obj_t * item = lv_btn_create(parent);
    lv_obj_remove_style_all(item);
    lv_obj_set_size(item, 212, 56);
    lv_obj_set_pos(item, 14, y);
    lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(item, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_radius(item, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(item, lv_color_hex(0xF8F7EF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(item, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(item, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(item, lv_color_hex(0x282C28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(item, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * icon = lv_img_create(item);
    lv_img_set_src(icon, icon_src);
    lv_obj_set_size(icon, 28, 28);
    lv_obj_align(icon, LV_ALIGN_LEFT_MID, 16, 0);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * dot = menu_make_dot(item);
    lv_obj_align(dot, LV_ALIGN_LEFT_MID, 55, 0);

    lv_obj_t * title = lv_label_create(item);
    lv_label_set_text(title, title_text);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1A1D1A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(title, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, 68, -10);
    lv_obj_clear_flag(title, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * sub = lv_label_create(item);
    lv_label_set_text(sub, sub_text);
    lv_obj_set_style_text_font(sub, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(sub, lv_color_hex(0x56615A), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(sub, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(sub, LV_ALIGN_LEFT_MID, 69, 15);
    lv_obj_clear_flag(sub, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * arrow = lv_label_create(item);
    lv_label_set_text(arrow, ">");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x262B26), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(arrow, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, -14, 0);
    lv_obj_clear_flag(arrow, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    if(panel_out) *panel_out = item;
    if(button_out) *button_out = item;
    if(icon_out) *icon_out = icon;
    if(label_out) *label_out = title;

    return item;
}

static void menu_item_event_cb(lv_event_t * e)
{
    Page_t * target_page;

    if(lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    target_page = (Page_t *)lv_event_get_user_data(e);
    if(target_page != NULL) {
        Page_Load(target_page);
    }
}

static void menu_back_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Back();
    }
}

void ui_event_MenuPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_SCROLL_END && ui_MenuList != NULL) {
        ui_MenuScrollY = lv_obj_get_scroll_y(ui_MenuList);
    }
}

void ui_MenuPage_screen_init(void)
{
    uint32_t i;
    const menu_item_desc_t menu_items[] = {
        {&ui_img_menu_calendar_28x28, TXT_CALENDAR, "Calendar", &Page_Calender,
         &ui_MenuCalPanel, &ui_MenuCalButton, &ui_MenuCalicon, &ui_MenuCalLabel},
        {&ui_img_menu_computer_28x28, TXT_COMPUTER, "Calculator", &Page_Computer,
         &ui_MenuComPanel, &ui_MenuComButton, &ui_MenuComicon, &ui_MenuComLabel},
        {&ui_img_menu_timer_28x28, TXT_TIMER, "Stopwatch", &Page_Timer,
         &ui_MenuTimPanel, &ui_MenuTimButton, &ui_MenuTimicon, &ui_MenuTimLabel},
        {&ui_img_menu_card_28x28, TXT_CARD, "Cards", &Page_NFCCard,
         &ui_MenuCardPanel, &ui_MenuCardButton, &ui_MenuCardicon, &ui_MenuCardLabel},
        {&ui_img_menu_hr_28x28, TXT_HR, "Heart Rate", &Page_HR,
         &ui_MenuHRPanel, &ui_MenuHRButton, &ui_MenuHRicon, &ui_MenuHRLabel},
        {&ui_img_menu_spo2_28x28, TXT_SPO2, "SpO2", &Page_SPO2,
         &ui_MenuO2Panel, &ui_MenuO2Button, &ui_MenuO2icon, &ui_MenuO2Label},
        {&ui_img_menu_env_28x28, TXT_ENV, "Sensor", &Page_Env,
         &ui_MenuEnvPanel, &ui_MenuEnvButton, &ui_MenuEnvicon, &ui_MenuEnvLabel},
        {&ui_img_menu_compass_28x28, TXT_COMPASS, "Compass", &Page_Compass,
         &ui_MenuCPPanel, &ui_MenuCPButton, &ui_MenuCPicon, &ui_MenuCPLabel},
        {&ui_img_menu_game_28x28, TXT_GAME, "Games", &Page_GameSelect,
         &ui_MenuGamePanel, &ui_MenuGameButton, &ui_MenuGameicon, &ui_MenuGameLabel},
        {&ui_img_menu_setting_28x28, TXT_SET, "Settings", &Page_Set,
         &ui_MenuSetPanel, &ui_MenuSetButton, &ui_MenuSeticon, &ui_MenuSetLabel},
        {&ui_img_menu_about_28x28, TXT_ABOUT, "Info", &Page_SquareAbout,
         &ui_MenuAbPanel, &ui_MenuAbButton, &ui_MenuAbicon, &ui_MenuAbLabel}
    };

    ui_MenuPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_MenuPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_MenuPage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuPage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_MenuPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    ui_Container3 = lv_obj_create(ui_MenuPage);
    lv_obj_remove_style_all(ui_Container3);
    lv_obj_set_size(ui_Container3, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(ui_Container3, 0, 0);
    lv_obj_add_flag(ui_Container3, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(ui_Container3, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * back_btn = lv_btn_create(ui_Container3);
    lv_obj_remove_style_all(back_btn);
    lv_obj_set_size(back_btn, 48, 42);
    lv_obj_set_pos(back_btn, 0, 0);
    lv_obj_add_flag(back_btn, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(back_btn, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_add_event_cb(back_btn, menu_back_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * back_hint = lv_label_create(back_btn);
    lv_label_set_text(back_hint, "<");
    lv_obj_set_style_text_font(back_hint, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(back_hint, lv_color_hex(0x1D211D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(back_hint, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(back_hint);

    lv_obj_t * title = lv_img_create(ui_Container3);
    lv_img_set_src(title, &ui_img_menu_title_72x34);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 6);
    lv_obj_clear_flag(title, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * seal = lv_obj_create(ui_Container3);
    lv_obj_remove_style_all(seal);
    lv_obj_set_size(seal, 22, 22);
    lv_obj_set_pos(seal, 198, 12);
    lv_obj_set_style_radius(seal, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(seal, lv_color_hex(0x982B24), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(seal, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(seal, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(seal, lv_color_hex(0x6B211B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(seal, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(seal, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_MenuList = lv_obj_create(ui_Container3);
    lv_obj_remove_style_all(ui_MenuList);
    lv_obj_set_size(ui_MenuList, LV_HOR_RES, 232);
    lv_obj_set_pos(ui_MenuList, 0, 48);
    lv_obj_add_flag(ui_MenuList, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_scroll_dir(ui_MenuList, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(ui_MenuList, LV_SCROLLBAR_MODE_AUTO);
    lv_obj_set_style_bg_opa(ui_MenuList, LV_OPA_TRANSP, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_MenuList, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_MenuList, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_width(ui_MenuList, 3, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_MenuList, lv_color_hex(0x3A403A), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_MenuList, LV_OPA_40, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    for(i = 0; i < sizeof(menu_items) / sizeof(menu_items[0]); i++) {
        lv_obj_t * item = menu_make_item(ui_MenuList, (int16_t)(i * 62), menu_items[i].icon_src,
                                         menu_items[i].title_text, menu_items[i].sub_text,
                                         menu_items[i].panel_out, menu_items[i].button_out,
                                         menu_items[i].icon_out, menu_items[i].label_out);
        lv_obj_add_event_cb(item, menu_item_event_cb, LV_EVENT_CLICKED, menu_items[i].target_page);
    }

    lv_obj_add_event_cb(ui_MenuPage, ui_event_MenuPage, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_MenuList, ui_event_MenuPage, LV_EVENT_ALL, NULL);

    enable_gesture_bubble_recursive(ui_MenuPage);
    lv_obj_scroll_to(ui_MenuList, 0, ui_MenuScrollY, LV_ANIM_OFF);
}

void ui_MenuPage_screen_deinit(void)
{
    if(ui_MenuList != NULL) {
        ui_MenuScrollY = lv_obj_get_scroll_y(ui_MenuList);
    }
}

void ui_MenuPage_screen_destroy(void)
{
    if(ui_MenuPage) lv_obj_del(ui_MenuPage);

    ui_MenuPage = NULL;
    ui_Container3 = NULL;
    ui_MenuList = NULL;
    ui_MenuCalPanel = NULL;
    ui_MenuCalButton = NULL;
    ui_MenuCalicon = NULL;
    ui_MenuCalLabel = NULL;
    ui_MenuComPanel = NULL;
    ui_MenuComButton = NULL;
    ui_MenuComicon = NULL;
    ui_MenuComLabel = NULL;
    ui_MenuTimPanel = NULL;
    ui_MenuTimButton = NULL;
    ui_MenuTimicon = NULL;
    ui_MenuTimLabel = NULL;
    ui_MenuCardPanel = NULL;
    ui_MenuCardButton = NULL;
    ui_MenuCardicon = NULL;
    ui_MenuCardLabel = NULL;
    ui_MenuSprPanel = NULL;
    ui_MenuSprButton = NULL;
    ui_MenuSpricon = NULL;
    ui_MenuSprLabel = NULL;
    ui_MenuHRPanel = NULL;
    ui_MenuHRButton = NULL;
    ui_MenuHRicon = NULL;
    ui_MenuHRLabel = NULL;
    ui_MenuO2Panel = NULL;
    ui_MenuO2Button = NULL;
    ui_MenuO2icon = NULL;
    ui_MenuO2Label = NULL;
    ui_MenuEnvPanel = NULL;
    ui_MenuEnvButton = NULL;
    ui_MenuEnvicon = NULL;
    ui_MenuEnvLabel = NULL;
    ui_MenuCPPanel = NULL;
    ui_MenuCPButton = NULL;
    ui_MenuCPicon = NULL;
    ui_MenuCPLabel = NULL;
    ui_MenuGamePanel = NULL;
    ui_MenuGameButton = NULL;
    ui_MenuGameicon = NULL;
    ui_MenuGameLabel = NULL;
    ui_MenuSetPanel = NULL;
    ui_MenuSetButton = NULL;
    ui_MenuSeticon = NULL;
    ui_MenuSetLabel = NULL;
    ui_MenuAbPanel = NULL;
    ui_MenuAbButton = NULL;
    ui_MenuAbicon = NULL;
    ui_MenuAbLabel = NULL;
}
