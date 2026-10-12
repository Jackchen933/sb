#include "../../ui.h"
#include "../Inc/ui_AboutPage.h"
#include "../../../version.h"

extern const lv_img_dsc_t ui_img_sb_png;
extern const lv_img_dsc_t ui_img_zz_png;
extern const lv_img_dsc_t ui_img_sz_png;
extern const lv_img_dsc_t ui_img_1337270863;

lv_obj_t * ui_AboutPage;

static lv_obj_t * ui_Container1;
static lv_obj_t * ui_Container2;
static lv_obj_t * ui_Container4;
static lv_obj_t * ui_Container5;
static lv_obj_t * ui_Container6;
static lv_obj_t * ui_Container7;
static lv_obj_t * ui_Button3;

Page_t Page_About = {ui_AboutPage_screen_init, ui_AboutPage_screen_deinit, &ui_AboutPage};

static void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_PRESSED)
    {
        Page_Back();
    }
}

static lv_obj_t * about_row_create(lv_obj_t * parent)
{
    lv_obj_t * row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_size(row, 220, 70);
    lv_obj_set_align(row, LV_ALIGN_CENTER);
    lv_obj_clear_flag(row, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    return row;
}

void ui_AboutPage_screen_init(void)
{
    char ver[24];

    ui_AboutPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_AboutPage, LV_OBJ_FLAG_SCROLLABLE);

    ui_Container1 = lv_obj_create(ui_AboutPage);
    lv_obj_remove_style_all(ui_Container1);
    lv_obj_set_width(ui_Container1, lv_pct(99));
    lv_obj_set_height(ui_Container1, lv_pct(125));
    lv_obj_set_y(ui_Container1, 36);
    lv_obj_set_align(ui_Container1, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Container1, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Container1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_Container1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_scroll_dir(ui_Container1, LV_DIR_VER);

    ui_Container2 = about_row_create(ui_Container1);
    lv_obj_t * img2 = lv_img_create(ui_Container2);
    lv_img_set_src(img2, &ui_img_sb_png);
    lv_obj_set_size(img2, 30, 28);
    lv_obj_set_pos(img2, -83, -7);
    lv_obj_set_align(img2, LV_ALIGN_CENTER);
    lv_obj_t * t2 = lv_label_create(ui_Container2);
    lv_obj_set_pos(t2, -39, -16);
    lv_obj_set_align(t2, LV_ALIGN_CENTER);
    lv_label_set_text(t2, "Watch Model");
    lv_obj_t * v2 = lv_label_create(ui_Container2);
    lv_obj_set_pos(v2, -39, 1);
    lv_obj_set_align(v2, LV_ALIGN_CENTER);
    lv_label_set_text(v2, "OV-Watch");
    ui_Button3 = lv_btn_create(ui_Container2);
    lv_obj_set_size(ui_Button3, 46, 50);
    lv_obj_set_pos(ui_Button3, 81, -6);
    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t * back = lv_label_create(ui_Button3);
    lv_obj_set_align(back, LV_ALIGN_CENTER);
    lv_label_set_text(back, "<");

    ui_Container4 = about_row_create(ui_Container1);
    lv_obj_t * img3 = lv_img_create(ui_Container4);
    lv_img_set_src(img3, &ui_img_zz_png);
    lv_obj_set_size(img3, 44, 34);
    lv_obj_set_pos(img3, -87, -16);
    lv_obj_set_align(img3, LV_ALIGN_CENTER);
    lv_obj_t * t4 = lv_label_create(ui_Container4);
    lv_obj_set_pos(t4, -34, -22);
    lv_obj_set_align(t4, LV_ALIGN_CENTER);
    lv_label_set_text(t4, "Firmware");
    lv_obj_t * v4 = lv_label_create(ui_Container4);
    lv_obj_set_pos(v4, -34, -5);
    lv_obj_set_align(v4, LV_ALIGN_CENTER);
    snprintf(ver, sizeof(ver), "V%d.%d.%d", watch_version_major(), watch_version_minor(), watch_version_patch());
    lv_label_set_text(v4, ver);

    ui_Container5 = about_row_create(ui_Container1);
    lv_obj_t * img4 = lv_img_create(ui_Container5);
    lv_img_set_src(img4, &ui_img_sz_png);
    lv_obj_set_size(img4, 24, 29);
    lv_obj_set_pos(img4, -82, -15);
    lv_obj_set_align(img4, LV_ALIGN_CENTER);
    lv_obj_t * t5 = lv_label_create(ui_Container5);
    lv_obj_set_pos(t5, -35, -19);
    lv_obj_set_align(t5, LV_ALIGN_CENTER);
    lv_label_set_text(t5, "MCU");
    lv_obj_t * v5 = lv_label_create(ui_Container5);
    lv_obj_set_pos(v5, -35, -3);
    lv_obj_set_align(v5, LV_ALIGN_CENTER);
    lv_label_set_text(v5, "STM32F411CEU6");

    ui_Container6 = about_row_create(ui_Container1);
    lv_obj_t * img5 = lv_img_create(ui_Container6);
    lv_img_set_src(img5, &ui_img_1337270863);
    lv_obj_set_size(img5, 31, 38);
    lv_obj_set_pos(img5, -85, -16);
    lv_obj_set_align(img5, LV_ALIGN_CENTER);
    lv_obj_t * t6 = lv_label_create(ui_Container6);
    lv_obj_set_pos(t6, -30, -26);
    lv_obj_set_align(t6, LV_ALIGN_CENTER);
    lv_label_set_text(t6, "System");
    lv_obj_t * v6 = lv_label_create(ui_Container6);
    lv_obj_set_pos(v6, -30, -5);
    lv_obj_set_align(v6, LV_ALIGN_CENTER);
    lv_label_set_text(v6, "FreeRTOS + LVGL");

    ui_Container7 = about_row_create(ui_Container1);
    lv_obj_t * t7 = lv_label_create(ui_Container7);
    lv_obj_set_align(t7, LV_ALIGN_CENTER);
    lv_obj_set_y(t7, -10);
    lv_label_set_text(t7, "Developer");
    lv_obj_t * v7 = lv_label_create(ui_Container7);
    lv_obj_set_align(v7, LV_ALIGN_CENTER);
    lv_obj_set_y(v7, 10);
    lv_label_set_text(v7, "chen");

    lv_obj_add_event_cb(ui_Button3, ui_event_Button3, LV_EVENT_ALL, NULL);
}

void ui_AboutPage_screen_deinit(void)
{
    if(ui_AboutPage) lv_obj_del(ui_AboutPage);

    ui_AboutPage = NULL;
    ui_Container1 = NULL;
    ui_Container2 = NULL;
    ui_Container4 = NULL;
    ui_Container5 = NULL;
    ui_Container6 = NULL;
    ui_Container7 = NULL;
    ui_Button3 = NULL;
}
