#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_QuickAppsPage.h"
#include "../Inc/ui_WatchFacePage.h"
#include "../Inc/ui_HRPage.h"
#include "../Inc/ui_SPO2Page.h"
#include "../Inc/ui_EnvPage.h"
#include "../Inc/ui_CalendarPage.h"
#include "../Inc/ui_TimerPage.h"
#include "../Inc/ui_ComputerPage.h"
#include "../Inc/ui_CompassPage.h"
#include "../Inc/ui_NFCCardPage.h"
#include "../Inc/ui_GameSelectPage.h"
#include "../../../Func/Inc/HWDataAccess.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_QuickAppsPage;

lv_timer_t * ui_QuickAppsPageTimer;

// 应用数据
uint8_t ui_QA_HRValue = 72;
uint8_t ui_QA_SPO2Value = 98;
int8_t ui_QA_TempValue = 25;
int8_t ui_QA_HumiValue = 60;

///////////////////// Page Manager //////////////////
Page_t Page_QuickApps = {ui_QuickAppsPage_screen_init, ui_QuickAppsPage_screen_deinit, &ui_QuickAppsPage};

///////////////////// TIMER CALLBACK //////////////////
static void QuickAppsPage_timer_cb(lv_timer_t * timer)
{
    if(Page_Get_NowPage()->page_obj == &ui_QuickAppsPage)
    {
        char value_strbuf[16];
        HW_DateTimeTypeDef DateTime;
        HWInterface.RealTimeClock.GetTimeDate(&DateTime);

        // 更新心率
        if(ui_QA_HRValue != HWInterface.HR_meter.HrRate)
        {
            ui_QA_HRValue = HWInterface.HR_meter.HrRate;
            sprintf(value_strbuf, "%d", ui_QA_HRValue);
            // 查找心率标签并更新
        }

        // 更新血氧
        if(ui_QA_SPO2Value != HWInterface.HR_meter.SPO2)
        {
            ui_QA_SPO2Value = HWInterface.HR_meter.SPO2;
        }

        // 更新温湿度
        if(ui_QA_TempValue != (int8_t)HWInterface.AHT21.temperature)
        {
            ui_QA_TempValue = (int8_t)HWInterface.AHT21.temperature;
        }

        if(ui_QA_HumiValue != (int8_t)HWInterface.AHT21.humidity)
        {
            ui_QA_HumiValue = (int8_t)HWInterface.AHT21.humidity;
        }
    }
}

///////////////////// EVENTS ////////////////////
void ui_event_QA_BackBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Back();
    }
}

void ui_event_QA_HRBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_HR);
    }
}

void ui_event_QA_SPO2Btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_SPO2);
    }
}

void ui_event_QA_EnvBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_Env);
    }
}

void ui_event_QA_CalendarBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_Calender);
    }
}

void ui_event_QA_TimerBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_Timer);
    }
}

void ui_event_QA_ComputerBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_Computer);
    }
}

void ui_event_QA_CompassBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_Compass);
    }
}

void ui_event_QA_NFCCardBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_NFCCard);
    }
}

void ui_event_QA_GameBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_GameSelect);
    }
}

///////////////////// HELPER FUNCTIONS ////////////////////
static lv_obj_t* create_app_button(lv_obj_t * parent, int32_t x, int32_t y,
                                    const char * icon, const char * label,
                                    uint32_t icon_color, lv_event_cb_t event_cb)
{
    // 按钮
    lv_obj_t * btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 70, 70);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_style_radius(btn, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(btn, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(btn, lv_color_hex(0xE0E0E0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(btn, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_ofs_y(btn, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 图标
    lv_obj_t * icon_label = lv_label_create(btn);
    lv_obj_set_align(icon_label, LV_ALIGN_TOP_MID);
    lv_obj_set_y(icon_label, 8);
    lv_label_set_text(icon_label, icon);
    lv_obj_set_style_text_font(icon_label, &ui_font_iconfont28, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(icon_label, lv_color_hex(icon_color), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 标签
    lv_obj_t * text_label = lv_label_create(btn);
    lv_obj_set_align(text_label, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(text_label, -10);
    lv_label_set_text(text_label, label);
    lv_obj_set_style_text_font(text_label, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(text_label, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 事件
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_ALL, NULL);

    return btn;
}

///////////////////// SCREEN init ////////////////////
void ui_QuickAppsPage_screen_init(void)
{
    char value_strbuf[16];

    // 创建页面
    ui_QuickAppsPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_QuickAppsPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_QuickAppsPage, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 顶部导航栏 ==========
    lv_obj_t * ui_QA_TopBar = lv_obj_create(ui_QuickAppsPage);
    lv_obj_set_size(ui_QA_TopBar, 240, 40);
    lv_obj_set_align(ui_QA_TopBar, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_QA_TopBar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_QA_TopBar, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 返回按钮
    lv_obj_t * ui_QA_BackBtn = lv_btn_create(ui_QA_TopBar);
    lv_obj_set_size(ui_QA_BackBtn, 30, 30);
    lv_obj_set_align(ui_QA_BackBtn, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_QA_BackBtn, 5);
    lv_obj_set_style_radius(ui_QA_BackBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_QA_BackBtn, lv_color_hex(0xF0F0F0), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_QA_BackIcon = lv_label_create(ui_QA_BackBtn);
    lv_obj_set_align(ui_QA_BackIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_QA_BackIcon, "<");
    lv_obj_set_style_text_font(ui_QA_BackIcon, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_QA_BackIcon, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 标题
    lv_obj_t * ui_QA_Title = lv_label_create(ui_QA_TopBar);
    lv_obj_set_align(ui_QA_Title, LV_ALIGN_CENTER);
    lv_label_set_text(ui_QA_Title, "Quick Apps");
    lv_obj_set_style_text_font(ui_QA_Title, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_QA_Title, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 应用网格面板 ==========
    lv_obj_t * ui_QA_GridPanel = lv_obj_create(ui_QuickAppsPage);
    lv_obj_set_size(ui_QA_GridPanel, 240, 280);
    lv_obj_set_align(ui_QA_GridPanel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_QA_GridPanel, 10);
    lv_obj_clear_flag(ui_QA_GridPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_QA_GridPanel, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 第一行 ==========
    // 心率
    lv_obj_t * ui_QA_HRBtn = create_app_button(ui_QA_GridPanel, 10, 10, "...", "Heart",
                                                 0xE11432, ui_event_QA_HRBtn);
    lv_obj_t * ui_QA_HRValueLabel = lv_label_create(ui_QA_HRBtn);
    lv_obj_set_align(ui_QA_HRValueLabel, LV_ALIGN_CENTER);
    sprintf(value_strbuf, "%d", ui_QA_HRValue);
    lv_label_set_text(ui_QA_HRValueLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_QA_HRValueLabel, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_QA_HRValueLabel, lv_color_hex(0xE11432), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 血氧
    lv_obj_t * ui_QA_SPO2Btn = create_app_button(ui_QA_GridPanel, 85, 10, "...", "SpO2",
                                                  0x00D4FF, ui_event_QA_SPO2Btn);
    lv_obj_t * ui_QA_SPO2ValueLabel = lv_label_create(ui_QA_SPO2Btn);
    lv_obj_set_align(ui_QA_SPO2ValueLabel, LV_ALIGN_CENTER);
    sprintf(value_strbuf, "%d%%", ui_QA_SPO2Value);
    lv_label_set_text(ui_QA_SPO2ValueLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_QA_SPO2ValueLabel, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_QA_SPO2ValueLabel, lv_color_hex(0x00D4FF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 环境
    lv_obj_t * ui_QA_EnvBtn = create_app_button(ui_QA_GridPanel, 160, 10, "...", "Env",
                                                  0x19C819, ui_event_QA_EnvBtn);
    lv_obj_t * ui_QA_EnvValueLabel = lv_label_create(ui_QA_EnvBtn);
    lv_obj_set_align(ui_QA_EnvValueLabel, LV_ALIGN_CENTER);
    sprintf(value_strbuf, "%d...", ui_QA_TempValue);
    lv_label_set_text(ui_QA_EnvValueLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_QA_EnvValueLabel, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_QA_EnvValueLabel, lv_color_hex(0xF5A73A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 第二行 ==========
    // 日历
    lv_obj_t * ui_QA_CalendarBtn = create_app_button(ui_QA_GridPanel, 10, 95, "...", "Calendar",
                                                      0xFF8080, ui_event_QA_CalendarBtn);

    // 秒表
    lv_obj_t * ui_QA_TimerBtn = create_app_button(ui_QA_GridPanel, 85, 95, "...", "Timer",
                                                   0xDC80E6, ui_event_QA_TimerBtn);

    // 计算器
    lv_obj_t * ui_QA_ComputerBtn = create_app_button(ui_QA_GridPanel, 160, 95, "...", "Calc",
                                                      0xC86400, ui_event_QA_ComputerBtn);

    // ========== 第三行 ==========
    // 指南针
    lv_obj_t * ui_QA_CompassBtn = create_app_button(ui_QA_GridPanel, 10, 180, "...", "Compass",
                                                     0x9966FF, ui_event_QA_CompassBtn);

    // 卡包
    lv_obj_t * ui_QA_NFCCardBtn = create_app_button(ui_QA_GridPanel, 85, 180, "...", "NFC",
                                                     0x80641E, ui_event_QA_NFCCardBtn);

    // 游戏
    lv_obj_t * ui_QA_GameBtn = create_app_button(ui_QA_GridPanel, 160, 180, "...", "Game",
                                                  0xC07010, ui_event_QA_GameBtn);

    // ========== 事件注册 ==========
    lv_obj_add_event_cb(ui_QA_BackBtn, ui_event_QA_BackBtn, LV_EVENT_ALL, NULL);

    // ========== 定时器 ==========
    ui_QuickAppsPageTimer = lv_timer_create(QuickAppsPage_timer_cb, 1000, NULL);
}

///////////////////// SCREEN deinit ////////////////////
void ui_QuickAppsPage_screen_deinit(void)
{
    lv_timer_del(ui_QuickAppsPageTimer);
}
