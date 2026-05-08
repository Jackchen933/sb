#include "../Inc/ui_Folder1Page.h"

lv_obj_t * ui_Screen1 = NULL;
lv_obj_t * ui_uipullzone = NULL;
lv_obj_t * ui_uidroppanel = NULL;

static uint8_t s_drop_dragging = 0;

static lv_coord_t drop_get_collapsed_y(void)
{
    if(ui_uidroppanel == NULL) return 0;
    return -lv_obj_get_height(ui_uidroppanel);
}

static lv_coord_t drop_get_threshold_y(void)
{
    return drop_get_collapsed_y() / 2;
}

static lv_coord_t drop_clamp_y(lv_coord_t y)
{
    lv_coord_t collapsed_y = drop_get_collapsed_y();
    if(y > 0) return 0;
    if(y < collapsed_y) return collapsed_y;
    return y;
}

static void drop_anim_to(lv_coord_t target_y)
{
    lv_anim_t a;
    lv_coord_t now_y;

    if(ui_uidroppanel == NULL) return;

    target_y = drop_clamp_y(target_y);
    now_y = lv_obj_get_y(ui_uidroppanel);

    lv_anim_del(ui_uidroppanel, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_init(&a);
    lv_anim_set_var(&a, ui_uidroppanel);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&a, now_y, target_y);
    lv_anim_set_time(&a, 180);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
}

static void drop_down_pressed(lv_event_t * e)
{
    LV_UNUSED(e);
    s_drop_dragging = 1;
    if(ui_uidroppanel != NULL) {
        lv_anim_del(ui_uidroppanel, (lv_anim_exec_xcb_t)lv_obj_set_y);
    }
}

static void drop_down_pressing(lv_event_t * e)
{
    lv_indev_t * indev;
    lv_point_t vect = {0};
    lv_coord_t next_y;

    LV_UNUSED(e);
    if(!s_drop_dragging || ui_uidroppanel == NULL) return;

    indev = lv_indev_get_act();
    if(indev == NULL) indev = lv_event_get_indev(e);
    if(indev == NULL) return;

    lv_indev_get_vect(indev, &vect);
    next_y = drop_clamp_y(lv_obj_get_y(ui_uidroppanel) + vect.y);
    lv_obj_set_y(ui_uidroppanel, next_y);
}

static void drop_down_released(lv_event_t * e)
{
    lv_coord_t y_now;

    LV_UNUSED(e);
    if(!s_drop_dragging || ui_uidroppanel == NULL) return;
    s_drop_dragging = 0;

    y_now = lv_obj_get_y(ui_uidroppanel);
    if(y_now > drop_get_threshold_y()) {
        drop_anim_to(0);
    } else {
        drop_anim_to(drop_get_collapsed_y());
    }
}

static void ui_event_uipullzone(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_PRESSED) drop_down_pressed(e);
    if(event_code == LV_EVENT_PRESSING) drop_down_pressing(e);
    if(event_code == LV_EVENT_RELEASED || event_code == LV_EVENT_PRESS_LOST) drop_down_released(e);
}

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);

    ui_uipullzone = lv_obj_create(ui_Screen1);
    lv_obj_set_height(ui_uipullzone, 30);
    lv_obj_set_width(ui_uipullzone, lv_pct(100));
    lv_obj_set_align(ui_uipullzone, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_uipullzone, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(ui_uipullzone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_img_opa(ui_uipullzone, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_uidroppanel = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_uidroppanel, lv_pct(100));
    lv_obj_set_height(ui_uidroppanel, lv_pct(60));
    lv_obj_set_x(ui_uidroppanel, -1);
    lv_obj_set_y(ui_uidroppanel, 0);
    lv_obj_set_align(ui_uidroppanel, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_uidroppanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_uidroppanel, lv_color_hex(0x4795E9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_uidroppanel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_y(ui_uidroppanel, -lv_obj_get_height(ui_uidroppanel));

    lv_obj_add_event_cb(ui_uipullzone, ui_event_uipullzone, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_uidroppanel, ui_event_uipullzone, LV_EVENT_ALL, NULL);
}

void ui_Screen1_screen_destroy(void)
{
    if(ui_Screen1) lv_obj_del(ui_Screen1);
    ui_Screen1 = NULL;
    ui_uipullzone = NULL;
    ui_uidroppanel = NULL;
}
