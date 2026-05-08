// Ink style About page for OV_Watch.

#include "../ui.h"
#include "PageManager.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_about_product_28x28);
LV_IMG_DECLARE(ui_img_about_author_28x28);
LV_IMG_DECLARE(ui_img_about_os_28x28);
LV_IMG_DECLARE(ui_img_about_ui_28x28);
LV_IMG_DECLARE(ui_img_about_chip_28x28);

LV_FONT_DECLARE(ui_font_Cuyuan20);

#define TXT_ABOUT      "\xE5\x85\xB3\xE4\xBA\x8E"
#define TXT_PRODUCT    "\xE4\xBA\xA7\xE5\x93\x81"
#define TXT_AUTHOR     "\xE4\xBD\x9C\xE8\x80\x85"
#define TXT_OS         "\xE6\x93\x8D\xE4\xBD\x9C\xE7\xB3\xBB\xE7\xBB\x9F"
#define TXT_UI         "\xE5\x9B\xBE\xE5\xBD\xA2\xE7\x95\x8C\xE9\x9D\xA2"
#define TXT_CHIP       "\xE8\x8A\xAF\xE7\x89\x87\xE5\x9E\x8B\xE5\x8F\xB7"
#define TXT_XIAOCHEN   "\xE5\xB0\x8F\xE9\x99\x88"

lv_obj_t * uic_Image2;
lv_obj_t * ui_uiAboutPage = NULL;
lv_obj_t * ui_Container1 = NULL;
lv_obj_t * ui_Container2 = NULL;
lv_obj_t * ui_Image2 = NULL;
lv_obj_t * ui_Label2 = NULL;
lv_obj_t * ui_Label3 = NULL;
lv_obj_t * ui_Button3 = NULL;
lv_obj_t * ui_Container4 = NULL;
lv_obj_t * ui_Image3 = NULL;
lv_obj_t * ui_Label4 = NULL;
lv_obj_t * ui_Label5 = NULL;
lv_obj_t * ui_Container5 = NULL;
lv_obj_t * ui_Image4 = NULL;
lv_obj_t * ui_Label6 = NULL;
lv_obj_t * ui_Label7 = NULL;
lv_obj_t * ui_Container6 = NULL;
lv_obj_t * ui_Image5 = NULL;
lv_obj_t * ui_Label10 = NULL;
lv_obj_t * ui_Label12 = NULL;
lv_obj_t * ui_Container7 = NULL;
lv_obj_t * ui_Label13 = NULL;
lv_obj_t * ui_Label14 = NULL;
lv_obj_t * ui_Label15 = NULL;

void ui_uiAboutPage_screen_init(void);
void ui_uiAboutPage_screen_destroy(void);
static void ui_uiAboutPage_screen_deinit(void);

Page_t Page_SquareAbout = {ui_uiAboutPage_screen_init, ui_uiAboutPage_screen_deinit, &ui_uiAboutPage};

static lv_obj_t * about_make_dot(lv_obj_t * parent)
{
    lv_obj_t * dot = lv_obj_create(parent);
    lv_obj_remove_style_all(dot);
    lv_obj_set_size(dot, 4, 4);
    lv_obj_set_style_radius(dot, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(dot, lv_color_hex(0x9C2B22), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(dot, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(dot, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    return dot;
}

static lv_obj_t * about_make_row(lv_obj_t * parent,
                                 int16_t y,
                                 const void * icon_src,
                                 const char * label_text,
                                 const char * value_text,
                                 const lv_font_t * value_font,
                                 int16_t value_width,
                                 lv_obj_t ** icon_out,
                                 lv_obj_t ** label_out,
                                 lv_obj_t ** value_out)
{
    lv_obj_t * row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_size(row, 218, 38);
    lv_obj_set_pos(row, 11, y);
    lv_obj_set_style_radius(row, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(row, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(row, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(row, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(row, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(row, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * icon = lv_img_create(row);
    lv_img_set_src(icon, icon_src);
    lv_obj_set_size(icon, 28, 28);
    lv_obj_align(icon, LV_ALIGN_LEFT_MID, 8, 0);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * dot = about_make_dot(row);
    lv_obj_align(dot, LV_ALIGN_LEFT_MID, 42, 0);

    lv_obj_t * label = lv_label_create(row);
    lv_label_set_text(label, label_text);
    lv_obj_set_style_text_color(label, lv_color_hex(0x1B1E1B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(label, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &ui_font_about, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 55, -1);
    lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * value = lv_label_create(row);
    lv_label_set_text(value, value_text);
    lv_label_set_long_mode(value, LV_LABEL_LONG_CLIP);
    lv_obj_set_width(value, value_width);
    lv_obj_set_style_text_align(value, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(value, lv_color_hex(0x3F4742), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(value, LV_OPA_90, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(value, value_font, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(value, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_clear_flag(value, LV_OBJ_FLAG_SCROLLABLE);

    if(icon_out) *icon_out = icon;
    if(label_out) *label_out = label;
    if(value_out) *value_out = value;

    return row;
}

static void about_make_back_button(lv_obj_t * parent)
{
    ui_Button3 = lv_btn_create(parent);
    lv_obj_remove_style_all(ui_Button3);
    lv_obj_set_size(ui_Button3, 48, 42);
    lv_obj_set_pos(ui_Button3, 0, 0);
    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t * arrow = lv_label_create(ui_Button3);
    lv_label_set_text(arrow, "<");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x1D211D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(arrow, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(arrow);

    lv_obj_add_event_cb(ui_Button3, ui_event_Button3, LV_EVENT_ALL, NULL);
}

void ui_event_Button3(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        Page_Back();
    }
}

void ui_uiAboutPage_screen_init(void)
{
    ui_uiAboutPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_uiAboutPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_uiAboutPage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_uiAboutPage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_uiAboutPage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    ui_Container1 = lv_obj_create(ui_uiAboutPage);
    lv_obj_remove_style_all(ui_Container1);
    lv_obj_set_size(ui_Container1, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(ui_Container1, 0, 0);
    lv_obj_clear_flag(ui_Container1, LV_OBJ_FLAG_SCROLLABLE);

    about_make_back_button(ui_Container1);

    lv_obj_t * title = lv_label_create(ui_Container1);
    lv_label_set_text(title, TXT_ABOUT);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1B1E1B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(title, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

    lv_obj_t * seal = lv_obj_create(ui_Container1);
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

    ui_Container2 = about_make_row(ui_Container1, 47, &ui_img_about_product_28x28,
                                   TXT_PRODUCT, "OV-Watch", &lv_font_montserrat_14, 108,
                                   &ui_Image2, &ui_Label3, &ui_Label2);
    ui_Container4 = about_make_row(ui_Container1, 93, &ui_img_about_author_28x28,
                                   TXT_AUTHOR, TXT_XIAOCHEN, &ui_font_about, 82,
                                   &ui_Image3, &ui_Label5, &ui_Label4);
    ui_Container5 = about_make_row(ui_Container1, 139, &ui_img_about_os_28x28,
                                   TXT_OS, "FreeRTOS", &lv_font_montserrat_14, 104,
                                   &ui_Image4, &ui_Label7, &ui_Label6);
    ui_Container6 = about_make_row(ui_Container1, 185, &ui_img_about_ui_28x28,
                                   TXT_UI, "LVGL", &lv_font_montserrat_14, 90,
                                   &ui_Image5, &ui_Label12, &ui_Label10);
    ui_Container7 = about_make_row(ui_Container1, 231, &ui_img_about_chip_28x28,
                                   TXT_CHIP, "STM32F411CEU6", &lv_font_montserrat_14, 112,
                                   &ui_Label15, &ui_Label14, &ui_Label13);

    uic_Image2 = ui_Image2;
}

static void ui_uiAboutPage_screen_deinit(void)
{
    /*
     * PageManager uses lv_scr_load_anim(..., true), so LVGL deletes the
     * previous active screen after the transition completes.
     */
}

void ui_uiAboutPage_screen_destroy(void)
{
    if(ui_uiAboutPage) lv_obj_del(ui_uiAboutPage);

    ui_uiAboutPage = NULL;
    ui_Container1 = NULL;
    ui_Container2 = NULL;
    uic_Image2 = NULL;
    ui_Image2 = NULL;
    ui_Label2 = NULL;
    ui_Label3 = NULL;
    ui_Button3 = NULL;
    ui_Container4 = NULL;
    ui_Image3 = NULL;
    ui_Label4 = NULL;
    ui_Label5 = NULL;
    ui_Container5 = NULL;
    ui_Image4 = NULL;
    ui_Label6 = NULL;
    ui_Label7 = NULL;
    ui_Container6 = NULL;
    ui_Image5 = NULL;
    ui_Label10 = NULL;
    ui_Label12 = NULL;
    ui_Container7 = NULL;
    ui_Label13 = NULL;
    ui_Label14 = NULL;
    ui_Label15 = NULL;
}
