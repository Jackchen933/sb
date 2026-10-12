// Ink style game select page for OV_Watch.

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_GameSelectPage.h"
#include "../Inc/ui_Game2048Page.h"
#include "../Inc/ui_GameMemPage.h"

#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_game_28x28);

#define TXT_GAME "\xE6\xB8\xB8\xE6\x88\x8F"
#define TXT_MEM  "\xE8\xAE\xB0\xE5\xBF\x86\xE6\x96\xB9\xE5\x9D\x97"
#define TXT_BALL "\xE5\x8F\x8D\xE5\xBC\xB9\xE7\x90\x83"

///////////////////// Page Manager //////////////////
Page_t Page_GameSelect = {ui_GameSelectPage_screen_init, ui_GameSelectPage_screen_deinit, &ui_GameSelectPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_GameSelectPage = NULL;
lv_obj_t * ui_Game2048Panel = NULL;
lv_obj_t * ui_GameMemPanel = NULL;
lv_obj_t * ui_GameBallPanel = NULL;

void ui_event_GameSelectPage(lv_event_t * e)
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

void ui_event_Game2048Panel(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_Game_2048);
    }
}

void ui_event_GameMemPanel(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Load(&Page_GameMem);
    }
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
    lv_img_set_src(icon, &ui_img_menu_game_28x28);
    lv_obj_set_pos(icon, 75, 8);

    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, TXT_GAME);
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

static lv_obj_t * make_game_item(lv_obj_t * parent, int16_t y, const char * badge_text, const char * title_text, const char * sub_text)
{
    lv_obj_t * item = lv_btn_create(parent);
    lv_obj_remove_style_all(item);
    lv_obj_set_size(item, 212, 58);
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
    lv_obj_set_size(badge, 34, 34);
    lv_obj_align(badge, LV_ALIGN_LEFT_MID, 14, 0);
    lv_obj_set_style_radius(badge, 8, 0);
    lv_obj_set_style_bg_color(badge, lv_color_hex(0x982B24), 0);
    lv_obj_set_style_bg_opa(badge, LV_OPA_70, 0);

    lv_obj_t * badge_label = lv_label_create(badge);
    lv_label_set_text(badge_label, badge_text);
    lv_obj_set_style_text_font(badge_label, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(badge_label, lv_color_hex(0xF8F4EA), 0);
    lv_obj_center(badge_label);

    lv_obj_t * title = lv_label_create(item);
    lv_label_set_text(title, title_text);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1B1E1B), 0);
    lv_obj_align(title, LV_ALIGN_LEFT_MID, 62, -10);

    lv_obj_t * sub = lv_label_create(item);
    lv_label_set_text(sub, sub_text);
    lv_obj_set_style_text_font(sub, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(sub, lv_color_hex(0x56615A), 0);
    lv_obj_set_style_text_opa(sub, LV_OPA_80, 0);
    lv_obj_align(sub, LV_ALIGN_LEFT_MID, 63, 15);

    lv_obj_t * arrow = lv_label_create(item);
    lv_label_set_text(arrow, ">");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x56615A), 0);
    lv_obj_align(arrow, LV_ALIGN_RIGHT_MID, -14, 0);
    return item;
}

///////////////////// SCREEN init ////////////////////
void ui_GameSelectPage_screen_init(void)
{
    ui_GameSelectPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_GameSelectPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_GameSelectPage, lv_color_hex(0xF4F1E6), 0);
    lv_obj_set_style_bg_opa(ui_GameSelectPage, LV_OPA_COVER, 0);

    lv_obj_t * bg = lv_img_create(ui_GameSelectPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * layer = lv_obj_create(ui_GameSelectPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    make_header(layer);

    ui_Game2048Panel = make_game_item(layer, 62, "48", "2048", "Puzzle");
    ui_GameMemPanel = make_game_item(layer, 128, "M", TXT_MEM, "Memory");
    ui_GameBallPanel = make_game_item(layer, 194, "B", TXT_BALL, "Soon");

    lv_obj_add_event_cb(ui_GameSelectPage, ui_event_GameSelectPage, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Game2048Panel, ui_event_Game2048Panel, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_GameMemPanel, ui_event_GameMemPanel, LV_EVENT_ALL, NULL);
}

///////////////////// SCREEN deinit ////////////////////
void ui_GameSelectPage_screen_deinit(void)
{
}
