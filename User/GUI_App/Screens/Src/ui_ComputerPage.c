// Ink style calculator page for OV_Watch.

#include <stdio.h>

#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_ComputerPage.h"

#include "../../../Func/Inc/PageManager.h"
#include "../../../Func/Inc/StrCalculate.h"

LV_IMG_DECLARE(ui_img_ink_watch_bg_240x280);
LV_IMG_DECLARE(ui_img_menu_computer_28x28);

LV_FONT_DECLARE(ui_font_Cuyuan20);
LV_FONT_DECLARE(ui_font_Cuyuan24);

#define TEXT_FULL 9
#define TXT_COMPUTER "\xE8\xAE\xA1\xE7\xAE\x97\xE5\x99\xA8"

///////////////////// Page Manager //////////////////
Page_t Page_Computer = {ui_ComputerPage_screen_init, ui_ComputerPage_screen_deinit, &ui_Computerpage};

///////////////////// VARIABLES ////////////////////
StrStack_t CalStr;
NumStack_t NumStack;
SymStack_t SymStack;
lv_obj_t * ui_Computerpage = NULL;
lv_obj_t * ui_CompageBtnM = NULL;
lv_obj_t * ui_CompageTextarea = NULL;
lv_obj_t * ui_CompageBackBtn = NULL;

static const char * ui_ComPageBtnmap[] = {
    "7", "8", "9", "\xC3\xB7", "\n",
    "4", "5", "6", "\xC3\x97", "\n",
    "1", "2", "3", "\xEF\xBC\x8D", "\n",
    ".", "0", "\xEF\xBC\x9D", "\xEF\xBC\x8B", ""
};

///////////////////// FUNCTIONS ////////////////////
static void ui_event_ComputerPage(lv_event_t * e);

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

static void page_make_back_button(lv_obj_t * parent)
{
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_remove_style_all(btn);
    lv_obj_set_size(btn, 48, 42);
    lv_obj_set_pos(btn, 0, 0);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);

    lv_obj_t * arrow = lv_label_create(btn);
    lv_label_set_text(arrow, "<");
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(arrow, lv_color_hex(0x1D211D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(arrow, LV_OPA_80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(arrow);

    lv_obj_add_event_cb(btn, ui_event_ComputerPage, LV_EVENT_ALL, NULL);
}

static lv_obj_t * make_paper_panel(lv_obj_t * parent, int16_t x, int16_t y, int16_t w, int16_t h)
{
    lv_obj_t * panel = lv_obj_create(parent);
    lv_obj_remove_style_all(panel);
    lv_obj_set_size(panel, w, h);
    lv_obj_set_pos(panel, x, y);
    lv_obj_set_style_radius(panel, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(panel, lv_color_hex(0xF8F6EE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(panel, LV_OPA_70, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(panel, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(panel, lv_color_hex(0x272B28), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(panel, LV_OPA_40, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(panel, LV_OBJ_FLAG_SCROLLABLE);
    return panel;
}

static char btn_id_to_stack_char(uint16_t btn_id)
{
    static const char btn_chars[] = {
        '7', '8', '9', '/',
        '4', '5', '6', '*',
        '1', '2', '3', '-',
        '.', '0', '=', '+'
    };

    if(btn_id >= sizeof(btn_chars)) return 0;
    return btn_chars[btn_id];
}

static uint8_t is_operator_btn(uint16_t btn_id)
{
    return (btn_id == 3 || btn_id == 7 || btn_id == 11 || btn_id == 15);
}

static void calculator_clear_text(uint8_t enable_input)
{
    uint8_t i;

    strclear(&CalStr);
    NumStackClear(&NumStack);
    SymStackClear(&SymStack);

    if(ui_CompageTextarea != NULL) {
        for(i = 0; i < (TEXT_FULL * 2); i++) {
            lv_textarea_del_char(ui_CompageTextarea);
        }
    }

    if(enable_input && ui_CompageBtnM != NULL) {
        lv_obj_add_flag(ui_CompageBtnM, LV_OBJ_FLAG_CLICKABLE);
    }
}

static void calculator_show_result(void)
{
    char strout[14];

    lv_textarea_add_text(ui_CompageTextarea, "\n");
    strput(&CalStr, '\n');

    if(StrCalculate(CalStr.strque, &NumStack, &SymStack)) {
        lv_textarea_add_text(ui_CompageTextarea, "erro");
    } else {
        if(isIntNumber(NumStack.data[NumStack.Top_Point - 1])) {
            snprintf(strout, sizeof(strout), "%.0f", NumStack.data[NumStack.Top_Point - 1]);
        } else {
            snprintf(strout, sizeof(strout), "%.4f", NumStack.data[NumStack.Top_Point - 1]);
        }
        lv_textarea_add_text(ui_CompageTextarea, strout);
    }

    strclear(&CalStr);
    if(ui_CompageBtnM != NULL) {
        lv_obj_clear_flag(ui_CompageBtnM, LV_OBJ_FLAG_CLICKABLE);
    }
}

void ui_CompageBtnM_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_DRAW_PART_BEGIN) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_param(e);

        if(dsc == NULL || dsc->part != LV_PART_ITEMS || dsc->rect_dsc == NULL || dsc->label_dsc == NULL) {
            return;
        }

        dsc->rect_dsc->radius = 8;
        dsc->rect_dsc->border_width = 1;
        dsc->rect_dsc->border_color = lv_color_hex(0x242A25);
        dsc->rect_dsc->border_opa = LV_OPA_40;
        dsc->rect_dsc->bg_color = lv_color_hex(0xF8F6EE);
        dsc->rect_dsc->bg_opa = LV_OPA_70;
        dsc->label_dsc->color = lv_color_hex(0x1D211D);

        if(is_operator_btn(dsc->id)) {
            dsc->rect_dsc->bg_color = lv_color_hex(0xE9E1D2);
            dsc->label_dsc->color = lv_color_hex(0x8D2D25);
        }

        if(dsc->id == 14) {
            dsc->rect_dsc->bg_color = lv_color_hex(0x9C2B22);
            dsc->rect_dsc->bg_opa = LV_OPA_80;
            dsc->rect_dsc->border_color = lv_color_hex(0x6B211B);
            dsc->label_dsc->color = lv_color_hex(0xF7F4EA);
        }

        if(lv_btnmatrix_get_selected_btn(obj) == dsc->id) {
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        return;
    }

    if(code == LV_EVENT_VALUE_CHANGED) {
        uint16_t btn_id = lv_btnmatrix_get_selected_btn(obj);
        const char * txt = lv_btnmatrix_get_btn_text(obj, btn_id);
        char stack_char = btn_id_to_stack_char(btn_id);

        if(txt == NULL || ui_CompageTextarea == NULL || stack_char == 0) {
            return;
        }

        if(stack_char == '=') {
            if(!strstack_isEmpty(&CalStr)) {
                calculator_show_result();
            }
            return;
        }

        if(CalStr.Top_Point < TEXT_FULL) {
            lv_textarea_add_text(ui_CompageTextarea, txt);
            strput(&CalStr, stack_char);
        }
    }
}

void ui_CompageBackBtn_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(ui_CompageTextarea == NULL) {
        return;
    }

    if(code == LV_EVENT_CLICKED) {
        if(!strstack_isEmpty(&CalStr)) {
            lv_textarea_del_char(ui_CompageTextarea);
            strdel(&CalStr);
        } else {
            calculator_clear_text(1);
        }
    }

    if(code == LV_EVENT_LONG_PRESSED) {
        calculator_clear_text(1);
    }
}

static void ui_event_ComputerPage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED && lv_event_get_target(e) != ui_Computerpage) {
        Page_Back();
        return;
    }

    if(event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_ComputerPage_screen_init(void)
{
    int i;
    lv_obj_t * layer;
    lv_obj_t * display_panel;
    lv_obj_t * keypad_panel;

    strclear(&CalStr);
    NumStackClear(&NumStack);
    SymStackClear(&SymStack);

    ui_Computerpage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Computerpage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Computerpage, lv_color_hex(0xF4F1E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Computerpage, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * bg = lv_img_create(ui_Computerpage);
    lv_img_set_src(bg, &ui_img_ink_watch_bg_240x280);
    lv_obj_align(bg, LV_ALIGN_CENTER, 0, 0);
    lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);

    layer = lv_obj_create(ui_Computerpage);
    lv_obj_remove_style_all(layer);
    lv_obj_set_size(layer, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_pos(layer, 0, 0);
    lv_obj_add_flag(layer, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(layer, LV_OBJ_FLAG_SCROLLABLE);

    page_make_back_button(layer);

    lv_obj_t * icon = lv_img_create(layer);
    lv_img_set_src(icon, &ui_img_menu_computer_28x28);
    lv_obj_set_pos(icon, 65, 8);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * title = lv_label_create(layer);
    lv_label_set_text(title, TXT_COMPUTER);
    lv_obj_set_style_text_color(title, lv_color_hex(0x1B1E1B), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(title, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(title, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 19, 12);

    lv_obj_t * seal = lv_obj_create(layer);
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

    display_panel = make_paper_panel(layer, 12, 50, 216, 55);
    keypad_panel = make_paper_panel(layer, 12, 115, 216, 150);

    ui_CompageTextarea = lv_textarea_create(display_panel);
    lv_textarea_set_one_line(ui_CompageTextarea, false);
    lv_textarea_set_max_length(ui_CompageTextarea, TEXT_FULL * 2);
    lv_textarea_set_align(ui_CompageTextarea, LV_TEXT_ALIGN_RIGHT);
    lv_obj_add_state(ui_CompageTextarea, LV_STATE_FOCUSED);
    lv_obj_remove_style_all(ui_CompageTextarea);
    lv_obj_set_size(ui_CompageTextarea, 166, 48);
    lv_obj_set_pos(ui_CompageTextarea, 8, 4);
    lv_obj_clear_flag(ui_CompageTextarea, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_text_font(ui_CompageTextarea, &ui_font_Cuyuan24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_CompageTextarea, lv_color_hex(0x111511), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_CompageTextarea, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CompageTextarea, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_CompageBackBtn = lv_btn_create(display_panel);
    lv_obj_remove_style_all(ui_CompageBackBtn);
    lv_obj_set_size(ui_CompageBackBtn, 36, 36);
    lv_obj_set_pos(ui_CompageBackBtn, 174, 9);
    lv_obj_add_flag(ui_CompageBackBtn, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_clear_flag(ui_CompageBackBtn, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_style_radius(ui_CompageBackBtn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_CompageBackBtn, lv_color_hex(0x1E241F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CompageBackBtn, LV_OPA_60, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_CompageBackBtn, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_CompageBackBtn, lv_color_hex(0x101410), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_CompageBackBtn, LV_OPA_30, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * btnlabel = lv_label_create(ui_CompageBackBtn);
    lv_label_set_text(btnlabel, LV_SYMBOL_BACKSPACE);
    lv_obj_set_style_text_font(btnlabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(btnlabel, lv_color_hex(0xF8F4EA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_center(btnlabel);

    ui_CompageBtnM = lv_btnmatrix_create(keypad_panel);
    lv_btnmatrix_set_map(ui_CompageBtnM, ui_ComPageBtnmap);
    lv_btnmatrix_set_one_checked(ui_CompageBtnM, true);
    for(i = 0; i < 16; i++) {
        lv_btnmatrix_set_btn_ctrl(ui_CompageBtnM, i, LV_BTNMATRIX_CTRL_NO_REPEAT);
    }
    lv_obj_clear_flag(ui_CompageBtnM, LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(ui_CompageBtnM, LV_OBJ_FLAG_GESTURE_BUBBLE);
    lv_obj_set_size(ui_CompageBtnM, 204, 138);
    lv_obj_set_pos(ui_CompageBtnM, 6, 6);
    lv_obj_set_style_border_width(ui_CompageBtnM, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_CompageBtnM, LV_OPA_0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_CompageBtnM, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_gap(ui_CompageBtnM, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_CompageBtnM, &ui_font_Cuyuan24, LV_PART_ITEMS | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Computerpage, ui_event_ComputerPage, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_CompageBtnM, ui_CompageBtnM_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_CompageBackBtn, ui_CompageBackBtn_event_cb, LV_EVENT_ALL, NULL);
    enable_gesture_bubble_recursive(ui_Computerpage);
}

/////////////////// SCREEN deinit ////////////////////
void ui_ComputerPage_screen_deinit(void)
{
}
