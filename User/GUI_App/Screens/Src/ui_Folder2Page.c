#include "../Inc/ui_Folder2Page.h"
#include "../Inc/ui_SetPage.h"

lv_obj_t * ui_Screen1 = NULL;
lv_obj_t * ui_uipullzone = NULL;
lv_obj_t * ui_uidroppanel = NULL;
lv_obj_t * ui_Button1 = NULL;
lv_obj_t * ui_Label2 = NULL;
lv_obj_t * ui_Button2 = NULL;
lv_obj_t * ui_Label6 = NULL;
lv_obj_t * ui_Button4 = NULL;
lv_obj_t * ui_Button5 = NULL;
lv_obj_t * ui_Slider1 = NULL;
lv_obj_t * ui_Label11 = NULL;
lv_obj_t * ui_Label3 = NULL;
lv_obj_t * ui_Label4 = NULL;
lv_obj_t * ui_Label5 = NULL;
lv_obj_t * ui_Label7 = NULL;
lv_obj_t * ui_F2TimeHourLabel = NULL;
lv_obj_t * ui_F2TimeColonLabel = NULL;
lv_obj_t * ui_F2TimeMinuteLabel = NULL;
lv_obj_t * ui_F2DateLabel = NULL;
lv_obj_t * ui_F2DayLabel = NULL;

static uint8_t s_set_page_loading = 0;

static void folder2_open_setpage_async(void * user_data)
{
    LV_UNUSED(user_data);
    s_set_page_loading = 0;

    if(Page_Get_NowPage() && Page_Get_NowPage()->page_obj == &ui_Screen1)
    {
        Page_Load(&Page_Set);
    }
}

static void ui_event_Screen1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
    {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

static void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if((code == LV_EVENT_CLICKED || code == LV_EVENT_SHORT_CLICKED) && !s_set_page_loading)
    {
        s_set_page_loading = 1;
        lv_async_call(folder2_open_setpage_async, NULL);
    }
}

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0x9C999C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_uipullzone = lv_obj_create(ui_Screen1);
    lv_obj_remove_style_all(ui_uipullzone);
    lv_obj_set_width(ui_uipullzone, lv_pct(100));
    lv_obj_set_height(ui_uipullzone, lv_pct(100));
    lv_obj_set_x(ui_uipullzone, 0);
    lv_obj_set_y(ui_uipullzone, -1);
    lv_obj_set_align(ui_uipullzone, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_uipullzone, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_uipullzone, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_uipullzone, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(ui_uipullzone, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_uipullzone, 5, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_uidroppanel = lv_obj_create(ui_uipullzone);
    lv_obj_remove_style_all(ui_uidroppanel);
    lv_obj_set_width(ui_uidroppanel, 100);
    lv_obj_set_height(ui_uidroppanel, 50);
    lv_obj_set_x(ui_uidroppanel, -51);
    lv_obj_set_y(ui_uidroppanel, -96);
    lv_obj_set_align(ui_uidroppanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_uidroppanel, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_Label2 = lv_label_create(ui_uidroppanel);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_Label2, -29);
    lv_obj_set_y(ui_Label2, -3);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "\xEE\x99\xA0");
    lv_obj_set_style_text_font(ui_Label2, &ui_font_Font2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button1 = lv_obj_create(ui_uipullzone);
    lv_obj_remove_style_all(ui_Button1);
    lv_obj_set_width(ui_Button1, 100);
    lv_obj_set_height(ui_Button1, 50);
    lv_obj_set_x(ui_Button1, -51);
    lv_obj_set_y(ui_Button1, -50);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    ui_Label6 = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_Label6, -27);
    lv_obj_set_y(ui_Label6, 3);
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, "\xEE\x99\xA3");
    lv_obj_set_style_text_font(ui_Label6, &ui_font_Font2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button2 = lv_obj_create(ui_uipullzone);
    lv_obj_remove_style_all(ui_Button2);
    lv_obj_set_width(ui_Button2, 100);
    lv_obj_set_height(ui_Button2, 50);
    lv_obj_set_x(ui_Button2, -63);
    lv_obj_set_y(ui_Button2, 3);
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);

    ui_Label11 = lv_label_create(ui_Button2);
    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT);
    lv_obj_set_x(ui_Label11, -22);
    lv_obj_set_y(ui_Label11, 2);
    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label11, "\xEE\x98\x90");
    lv_obj_set_style_text_font(ui_Label11, &ui_font_Font2, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Screen1, ui_event_Screen1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button2, ui_event_Button2, LV_EVENT_CLICKED, NULL);
}

void ui_Screen1_screen_destroy(void)
{
    if(ui_Screen1) lv_obj_del(ui_Screen1);

    ui_Screen1 = NULL;
    ui_uipullzone = NULL;
    ui_uidroppanel = NULL;
    ui_Button1 = NULL;
    ui_Label2 = NULL;
    ui_Button2 = NULL;
    ui_Label6 = NULL;
    ui_Button4 = NULL;
    ui_Button5 = NULL;
    ui_Slider1 = NULL;
    ui_Label11 = NULL;
    ui_Label3 = NULL;
    ui_Label4 = NULL;
    ui_Label5 = NULL;
    ui_Label7 = NULL;
    ui_F2TimeHourLabel = NULL;
    ui_F2TimeColonLabel = NULL;
    ui_F2TimeMinuteLabel = NULL;
    ui_F2DateLabel = NULL;
    ui_F2DayLabel = NULL;
}
