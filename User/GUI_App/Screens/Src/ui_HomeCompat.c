#include "../../ui.h"
#include "../Inc/ui_HomePage.h"

extern lv_obj_t * ui_uidroppanel;

uint8_t ui_TimeHourValue = 11;
uint8_t ui_TimeMinuteValue = 59;
uint8_t ui_DateMonthValue = 11;
uint8_t ui_DateDayValue = 8;
uint8_t ui_DataWeekdayValue = 2;
const char * ui_Days[7] = {"Mon.", "Tue.", "Wed.", "Thu.", "Fri.", "Sat.", "Sun."};

uint8_t ui_BatArcValue = 70;
uint16_t ui_StepNumValue = 0;
uint8_t ui_LightSliderValue = 50;

void ui_HomePage_screen_deinit(void)
{
    /*
     * PageManager loads the next screen with lv_scr_load_anim(..., true),
     * so the previous active screen is deleted by LVGL after the transition.
     * Deleting it here can free the active screen too early and cause a fault.
     */
}

Page_t Page_Home = {ui_HomePage_screen_init, ui_HomePage_screen_deinit, &ui_HomePage};

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

void drop_down_pressed(lv_event_t * e)
{
    LV_UNUSED(e);
    s_drop_dragging = 1;
    if(ui_uidroppanel != NULL) {
        lv_anim_del(ui_uidroppanel, (lv_anim_exec_xcb_t)lv_obj_set_y);
    }
}

void drop_down_pressing(lv_event_t * e)
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

void drop_down_released(lv_event_t * e)
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
