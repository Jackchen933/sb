// Ink style NFC card page for OV_Watch.

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_Megboxes.h"
#include "../Inc/ui_NFCCardPage.h"

#include "../../../Func/Inc/PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_card_28x28);

#define TXT_CARD      "\xE5\x8D\xA1\xE5\x8C\x85"
#define TXT_CAMPUS    "\xE6\xA0\xA1\xE5\x9B\xAD\xE5\x8D\xA1"
#define TXT_DOOR      "\xE9\x97\xA8\xE7\xA6\x81\xE5\x8D\xA1"
#define TXT_TRANSIT   "\xE4\xBA\xA4\xE9\x80\x9A\xE5\x8D\xA1"
#define TXT_WRITE     "\xE6\x94\xB9\xE5\x86\x99"
#define TXT_CHOOSE    "\xE9\x80\x89\xE6\x8B\xA9"
#define TXT_DEV       "\xE5\xBC\x80\xE5\x8F\x91\xE4\xB8\xAD"

///////////////////// Page Manager //////////////////
Page_t Page_NFCCard = {ui_NFCCardPage_screen_init, ui_NFCCardPage_screen_deinit, &ui_NFCCardPage};

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_NFCCardPage = NULL;
lv_obj_t * ui_Card1Panel = NULL;
lv_obj_t * ui_Card1Btn = NULL;
lv_obj_t * ui_Card1icon = NULL;
lv_obj_t * ui_Card1Label = NULL;
lv_obj_t * ui_WriteBtn1 = NULL;
lv_obj_t * ui_WriteLabel1 = NULL;
lv_obj_t * ui_ChooseBtn1 = NULL;
lv_obj_t * ui_ChooseLabel1 = NULL;

lv_obj_t * ui_Card2Panel = NULL;
lv_obj_t * ui_Card2Btn = NULL;
lv_obj_t * ui_Card2icon = NULL;
lv_obj_t * ui_Card2Label = NULL;
lv_obj_t * ui_WriteBtn2 = NULL;
lv_obj_t * ui_WriteLabel2 = NULL;
lv_obj_t * ui_ChooseBtn2 = NULL;
lv_obj_t * ui_ChooseLabel2 = NULL;

lv_obj_t * ui_Card3Panel = NULL;
lv_obj_t * ui_Card3Btn = NULL;
lv_obj_t * ui_Card3icon = NULL;
lv_obj_t * ui_Card3Label = NULL;
lv_obj_t * ui_WriteBtn3 = NULL;
lv_obj_t * ui_WriteLabel3 = NULL;
lv_obj_t * ui_ChooseBtn3 = NULL;
lv_obj_t * ui_ChooseLabel3 = NULL;

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

static void write_btn_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        ui_mbox_create((uint8_t *)TXT_DEV);
    }
}

void ui_event_WriteBtn1(lv_event_t * e)
{
    write_btn_event_cb(e);
}

void ui_event_ChooseBtn1(lv_event_t * e)
{
    (void)e;
}

static void make_header(lv_obj_t * parent)
{
    lv_obj_t * back = lv_btn_create(parent);
    lv_obj_remove_style_all(back);
    lv_obj_set_size(back, 48, 42);
    lv_obj_set_pos(back, 0, 0);
    lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(back, page_back_event_cb, LV_EVENT_ALL, NULL);

    lv_obj_t * arrow = lv_label_create(back);
    lv_label_set_text(arrow, "<");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x1D211D), 0);
    lv_obj_set_style_text_opa(arrow, LV_OPA_80, 0);
    lv_obj_center(arrow);

    lv_obj_t * icon = lv_img_create(parent);
    lv_img_set_src(icon, &ui_img_menu_card_28x28);
    lv_obj_set_pos(icon, 75, 8);

    lv_obj_t * title = lv_label_create(parent);
    lv_label_set_text(title, TXT_CARD);
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

static lv_obj_t * make_action_btn(lv_obj_t * parent, int16_t x, const char * text)
{
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, 58, 24);
    lv_obj_set_pos(btn, x, 35);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_style_radius(btn, 8, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1F251F), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_60, 0);
    lv_obj_set_style_border_width(btn, 1, 0);
    lv_obj_set_style_border_color(btn, lv_color_hex(0x111611), 0);
    lv_obj_set_style_border_opa(btn, LV_OPA_30, 0);

    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &ui_font_Cuyuan18, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xF8F4EA), 0);
    lv_obj_center(label);
    return btn;
}

static lv_obj_t * make_card(lv_obj_t * parent,
                            int16_t y,
                            const char * index,
                            const char * title,
                            lv_obj_t ** badge_out,
                            lv_obj_t ** badge_label_out,
                            lv_obj_t ** title_out,
                            lv_obj_t ** write_out,
                            lv_obj_t ** write_label_out,
                            lv_obj_t ** choose_out,
                            lv_obj_t ** choose_label_out)
{
    lv_obj_t * card = lv_obj_create(parent);
    lv_obj_remove_style_all(card);
    lv_obj_set_size(card, 212, 66);
    lv_obj_set_pos(card, 14, y);
    lv_obj_add_flag(card, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_style_radius(card, 8, 0);
    lv_obj_set_style_bg_color(card, lv_color_hex(0xF8F6EE), 0);
    lv_obj_set_style_bg_opa(card, LV_OPA_70, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0x272B28), 0);
    lv_obj_set_style_border_opa(card, LV_OPA_40, 0);

    lv_obj_t * badge = lv_obj_create(card);
    lv_obj_remove_style_all(badge);
    lv_obj_set_size(badge, 34, 34);
    lv_obj_set_pos(badge, 12, 10);
    lv_obj_set_style_radius(badge, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(badge, lv_color_hex(0x982B24), 0);
    lv_obj_set_style_bg_opa(badge, LV_OPA_70, 0);

    lv_obj_t * badge_label = lv_label_create(badge);
    lv_label_set_text(badge_label, index);
    lv_obj_set_style_text_font(badge_label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(badge_label, lv_color_hex(0xF8F4EA), 0);
    lv_obj_center(badge_label);

    lv_obj_t * title_label = lv_label_create(card);
    lv_label_set_text(title_label, title);
    lv_obj_set_style_text_font(title_label, &ui_font_Cuyuan20, 0);
    lv_obj_set_style_text_color(title_label, lv_color_hex(0x1B1E1B), 0);
    lv_obj_set_pos(title_label, 58, 10);

    lv_obj_t * write = make_action_btn(card, 58, TXT_WRITE);
    lv_obj_t * choose = make_action_btn(card, 126, TXT_CHOOSE);

    if(badge_out) *badge_out = badge;
    if(badge_label_out) *badge_label_out = badge_label;
    if(title_out) *title_out = title_label;
    if(write_out) *write_out = write;
    if(write_label_out) *write_label_out = lv_obj_get_child(write, 0);
    if(choose_out) *choose_out = choose;
    if(choose_label_out) *choose_label_out = lv_obj_get_child(choose, 0);
    return card;
}

///////////////////// SCREEN init ////////////////////
void ui_NFCCardPage_screen_init(void)
{
    ui_NFCCardPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_NFCCardPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_NFCCardPage, lv_color_hex(0xF4F1E6), 0);
    lv_obj_set_style_bg_opa(ui_NFCCardPage, LV_OPA_COVER, 0);

    lv_obj_t * bg = lv_img_create(ui_NFCCardPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t * layer = lv_obj_create(ui_NFCCardPage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_add_event_cb(ui_NFCCardPage, page_gesture_event_cb, LV_EVENT_ALL, NULL);

    make_header(layer);

    ui_Card1Panel = make_card(layer, 54, "1", TXT_CAMPUS,
                              &ui_Card1Btn, &ui_Card1icon, &ui_Card1Label,
                              &ui_WriteBtn1, &ui_WriteLabel1, &ui_ChooseBtn1, &ui_ChooseLabel1);
    ui_Card2Panel = make_card(layer, 124, "2", TXT_DOOR,
                              &ui_Card2Btn, &ui_Card2icon, &ui_Card2Label,
                              &ui_WriteBtn2, &ui_WriteLabel2, &ui_ChooseBtn2, &ui_ChooseLabel2);
    ui_Card3Panel = make_card(layer, 194, "3", TXT_TRANSIT,
                              &ui_Card3Btn, &ui_Card3icon, &ui_Card3Label,
                              &ui_WriteBtn3, &ui_WriteLabel3, &ui_ChooseBtn3, &ui_ChooseLabel3);

    lv_obj_add_event_cb(ui_WriteBtn1, ui_event_WriteBtn1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WriteBtn2, write_btn_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WriteBtn3, write_btn_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_ChooseBtn1, ui_event_ChooseBtn1, LV_EVENT_ALL, NULL);
}

///////////////////// SCREEN deinit ////////////////////
void ui_NFCCardPage_screen_deinit(void)
{
}
