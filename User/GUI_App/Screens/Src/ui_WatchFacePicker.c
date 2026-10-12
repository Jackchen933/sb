#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_WatchFacePicker.h"
#include "../Inc/ui_WatchFacePage.h"
#include "../../../Func/Inc/HWDataAccess.h"
#include "../../../BSP/BL24C02/DataSave.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_WatchFacePicker;
WatchFaceStyle_t ui_WFP_CurrentFace = WATCH_FACE_MODERN;
WatchFaceStyle_t ui_WFP_TempFace = WATCH_FACE_MODERN;

lv_timer_t * ui_WatchFacePickerTimer;
lv_obj_t * ui_WFP_PreviewArc1;
lv_obj_t * ui_WFP_PreviewArc2;
lv_obj_t * ui_WFP_PreviewArc3;
lv_obj_t * ui_WFP_FaceNameLabel;

lv_obj_t * ui_WFP_PrevBtn;
lv_obj_t * ui_WFP_NextBtn;

// 表盘名称
static const char * face_names[WATCH_FACE_COUNT] = {
    "Modern",
    "Sport",
    "Minimal",
    "Analog",
    "Digital"
};

///////////////////// Page Manager //////////////////
Page_t Page_WatchFacePicker = {ui_WatchFacePicker_screen_init, ui_WatchFacePicker_screen_deinit, &ui_WatchFacePicker};

///////////////////// HELPER FUNCTIONS ////////////////////
const char* ui_WFP_GetFaceName(WatchFaceStyle_t style)
{
    if(style >= WATCH_FACE_COUNT)
        return "Unknown";
    return face_names[style];
}

void ui_WFP_UpdatePreview(void)
{
    uint8_t value_strbuf[16];

    // 更新预览环
    switch(ui_WFP_TempFace)
    {
        case WATCH_FACE_MODERN:
            // 现代简约 - 三环
            lv_arc_set_value(ui_WFP_PreviewArc1, 75);
            lv_arc_set_value(ui_WFP_PreviewArc2, 60);
            lv_arc_set_value(ui_WFP_PreviewArc3, 50);
            lv_obj_set_style_arc_color(ui_WFP_PreviewArc1, lv_color_hex(0x3278FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(ui_WFP_PreviewArc2, lv_color_hex(0xF5A73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(ui_WFP_PreviewArc3, lv_color_hex(0x19C819), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            break;

        case WATCH_FACE_SPORT:
            // 运动风格 - 单一大环
            lv_arc_set_value(ui_WFP_PreviewArc1, 85);
            lv_arc_set_value(ui_WFP_PreviewArc2, 0);
            lv_arc_set_value(ui_WFP_PreviewArc3, 0);
            lv_obj_set_style_arc_color(ui_WFP_PreviewArc1, lv_color_hex(0x19C819), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            break;

        case WATCH_FACE_MINIMALIST:
            // 极简风格 - 无环
            lv_arc_set_value(ui_WFP_PreviewArc1, 0);
            lv_arc_set_value(ui_WFP_PreviewArc2, 0);
            lv_arc_set_value(ui_WFP_PreviewArc3, 0);
            break;

        case WATCH_FACE_ANALOG:
            // 模拟时钟 - 环形
            lv_arc_set_value(ui_WFP_PreviewArc1, 50);
            lv_arc_set_value(ui_WFP_PreviewArc2, 0);
            lv_arc_set_value(ui_WFP_PreviewArc3, 0);
            lv_obj_set_style_arc_color(ui_WFP_PreviewArc1, lv_color_hex(0xFFD700), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            break;

        case WATCH_FACE_DIGITAL:
            // 纯数字 - 条形进度
            lv_arc_set_value(ui_WFP_PreviewArc1, 65);
            lv_arc_set_value(ui_WFP_PreviewArc2, 0);
            lv_arc_set_value(ui_WFP_PreviewArc3, 0);
            lv_obj_set_style_arc_color(ui_WFP_PreviewArc1, lv_color_hex(0x00D4FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            break;

        default:
            break;
    }

    // 更新名称标签
    lv_label_set_text(ui_WFP_FaceNameLabel, ui_WFP_GetFaceName(ui_WFP_TempFace));
}

///////////////////// TIMER CALLBACK //////////////////
static void WatchFacePicker_timer_cb(lv_timer_t * timer)
{
    if(Page_Get_NowPage()->page_obj == &ui_WatchFacePicker)
    {
        // 可以在这里添加预览动画等
    }
}

///////////////////// EVENTS ////////////////////
void ui_event_WFP_BackBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Back();
    }
}

void ui_event_WFP_PrevBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        if(ui_WFP_TempFace > 0)
            ui_WFP_TempFace--;
        else
            ui_WFP_TempFace = WATCH_FACE_COUNT - 1;
        ui_WFP_UpdatePreview();
    }
}

void ui_event_WFP_NextBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        if(ui_WFP_TempFace < WATCH_FACE_COUNT - 1)
            ui_WFP_TempFace++;
        else
            ui_WFP_TempFace = 0;
        ui_WFP_UpdatePreview();
    }
}

void ui_event_WFP_ConfirmBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        // 保存选择到 EEPROM
        ui_WFP_CurrentFace = ui_WFP_TempFace;
        DataSave_SetWatchFace(ui_WFP_CurrentFace);

        // 返回表盘页面
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_WatchFacePicker_screen_init(void)
{
    // 创建页面
    ui_WatchFacePicker = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_WatchFacePicker, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WatchFacePicker, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 初始化临时选择为当前选择
    ui_WFP_TempFace = ui_WFP_CurrentFace;

    // ========== 顶部导航栏 ==========
    lv_obj_t * ui_WFP_TopBar = lv_obj_create(ui_WatchFacePicker);
    lv_obj_set_size(ui_WFP_TopBar, 240, 40);
    lv_obj_set_align(ui_WFP_TopBar, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_WFP_TopBar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WFP_TopBar, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 返回按钮
    lv_obj_t * ui_WFP_BackBtn = lv_btn_create(ui_WFP_TopBar);
    lv_obj_set_size(ui_WFP_BackBtn, 30, 30);
    lv_obj_set_align(ui_WFP_BackBtn, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_WFP_BackBtn, 5);
    lv_obj_set_style_radius(ui_WFP_BackBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WFP_BackBtn, lv_color_hex(0xF0F0F0), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_WFP_BackIcon = lv_label_create(ui_WFP_BackBtn);
    lv_obj_set_align(ui_WFP_BackIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WFP_BackIcon, "<");
    lv_obj_set_style_text_font(ui_WFP_BackIcon, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WFP_BackIcon, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 标题
    lv_obj_t * ui_WFP_Title = lv_label_create(ui_WFP_TopBar);
    lv_obj_set_align(ui_WFP_Title, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WFP_Title, "Watch Face");
    lv_obj_set_style_text_font(ui_WFP_Title, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WFP_Title, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 预览区域 ==========
    lv_obj_t * ui_WFP_PreviewPanel = lv_obj_create(ui_WatchFacePicker);
    lv_obj_set_size(ui_WFP_PreviewPanel, 180, 180);
    lv_obj_set_align(ui_WFP_PreviewPanel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WFP_PreviewPanel, -10);
    lv_obj_clear_flag(ui_WFP_PreviewPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WFP_PreviewPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_WFP_PreviewPanel, 20, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 预览环1
    ui_WFP_PreviewArc1 = lv_arc_create(ui_WFP_PreviewPanel);
    lv_obj_set_size(ui_WFP_PreviewArc1, 160, 160);
    lv_obj_set_align(ui_WFP_PreviewArc1, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_WFP_PreviewArc1, 75);
    lv_arc_set_bg_angles(ui_WFP_PreviewArc1, 135, 405);
    lv_obj_set_style_arc_width(ui_WFP_PreviewArc1, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WFP_PreviewArc1, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WFP_PreviewArc1, lv_color_hex(0x3278FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WFP_PreviewArc1, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WFP_PreviewArc1, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_WFP_PreviewArc1, LV_OBJ_FLAG_CLICKABLE);

    // 预览环2
    ui_WFP_PreviewArc2 = lv_arc_create(ui_WFP_PreviewPanel);
    lv_obj_set_size(ui_WFP_PreviewArc2, 130, 130);
    lv_obj_set_align(ui_WFP_PreviewArc2, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_WFP_PreviewArc2, 60);
    lv_arc_set_bg_angles(ui_WFP_PreviewArc2, 135, 405);
    lv_obj_set_style_arc_width(ui_WFP_PreviewArc2, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WFP_PreviewArc2, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WFP_PreviewArc2, lv_color_hex(0xF5A73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WFP_PreviewArc2, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WFP_PreviewArc2, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_WFP_PreviewArc2, LV_OBJ_FLAG_CLICKABLE);

    // 预览环3
    ui_WFP_PreviewArc3 = lv_arc_create(ui_WFP_PreviewPanel);
    lv_obj_set_size(ui_WFP_PreviewArc3, 100, 100);
    lv_obj_set_align(ui_WFP_PreviewArc3, LV_ALIGN_CENTER);
    lv_arc_set_value(ui_WFP_PreviewArc3, 50);
    lv_arc_set_bg_angles(ui_WFP_PreviewArc3, 135, 405);
    lv_obj_set_style_arc_width(ui_WFP_PreviewArc3, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WFP_PreviewArc3, 10, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WFP_PreviewArc3, lv_color_hex(0x19C819), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WFP_PreviewArc3, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WFP_PreviewArc3, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_WFP_PreviewArc3, LV_OBJ_FLAG_CLICKABLE);

    // 表盘名称
    ui_WFP_FaceNameLabel = lv_label_create(ui_WFP_PreviewPanel);
    lv_obj_set_align(ui_WFP_FaceNameLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WFP_FaceNameLabel, ui_WFP_GetFaceName(ui_WFP_TempFace));
    lv_obj_set_style_text_font(ui_WFP_FaceNameLabel, &ui_font_Cuyuan24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WFP_FaceNameLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 选择按钮 ==========
    // 上一页按钮
    ui_WFP_PrevBtn = lv_btn_create(ui_WatchFacePicker);
    lv_obj_set_size(ui_WFP_PrevBtn, 50, 50);
    lv_obj_set_align(ui_WFP_PrevBtn, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_WFP_PrevBtn, 5);
    lv_obj_set_y(ui_WFP_PrevBtn, -20);
    lv_obj_set_style_radius(ui_WFP_PrevBtn, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WFP_PrevBtn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_WFP_PrevBtn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_WFP_PrevBtn, lv_color_hex(0xE0E0E0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_WFP_PrevBtn, 80, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_WFP_PrevIcon = lv_label_create(ui_WFP_PrevBtn);
    lv_obj_set_align(ui_WFP_PrevIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WFP_PrevIcon, "<");
    lv_obj_set_style_text_font(ui_WFP_PrevIcon, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WFP_PrevIcon, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 下一页按钮
    ui_WFP_NextBtn = lv_btn_create(ui_WatchFacePicker);
    lv_obj_set_size(ui_WFP_NextBtn, 50, 50);
    lv_obj_set_align(ui_WFP_NextBtn, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_WFP_NextBtn, -5);
    lv_obj_set_y(ui_WFP_NextBtn, -20);
    lv_obj_set_style_radius(ui_WFP_NextBtn, 25, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WFP_NextBtn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_WFP_NextBtn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_WFP_NextBtn, lv_color_hex(0xE0E0E0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_WFP_NextBtn, 80, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_WFP_NextIcon = lv_label_create(ui_WFP_NextBtn);
    lv_obj_set_align(ui_WFP_NextIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WFP_NextIcon, ">");
    lv_obj_set_style_text_font(ui_WFP_NextIcon, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WFP_NextIcon, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 确认按钮
    lv_obj_t * ui_WFP_ConfirmBtn = lv_btn_create(ui_WatchFacePicker);
    lv_obj_set_size(ui_WFP_ConfirmBtn, 120, 45);
    lv_obj_set_align(ui_WFP_ConfirmBtn, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WFP_ConfirmBtn, 100);
    lv_obj_set_style_radius(ui_WFP_ConfirmBtn, 22, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WFP_ConfirmBtn, lv_color_hex(0x3278FF), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_WFP_ConfirmLabel = lv_label_create(ui_WFP_ConfirmBtn);
    lv_obj_set_align(ui_WFP_ConfirmLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WFP_ConfirmLabel, "Confirm");
    lv_obj_set_style_text_font(ui_WFP_ConfirmLabel, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WFP_ConfirmLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 页码指示器
    lv_obj_t * ui_WFP_PageIndicator = lv_label_create(ui_WatchFacePicker);
    lv_obj_set_align(ui_WFP_PageIndicator, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WFP_PageIndicator, 140);
    lv_label_set_text(ui_WFP_PageIndicator, "...");
    lv_obj_set_style_text_font(ui_WFP_PageIndicator, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WFP_PageIndicator, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 事件注册 ==========
    lv_obj_add_event_cb(ui_WFP_BackBtn, ui_event_WFP_BackBtn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WFP_PrevBtn, ui_event_WFP_PrevBtn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WFP_NextBtn, ui_event_WFP_NextBtn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WFP_ConfirmBtn, ui_event_WFP_ConfirmBtn, LV_EVENT_ALL, NULL);

    // ========== 定时器 ==========
    ui_WatchFacePickerTimer = lv_timer_create(WatchFacePicker_timer_cb, 100, NULL);
}

///////////////////// SCREEN deinit ////////////////////
void ui_WatchFacePicker_screen_deinit(void)
{
    lv_timer_del(ui_WatchFacePickerTimer);
}
