#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_WatchFacePage.h"
#include "../Inc/ui_QuickAppsPage.h"
#include "../Inc/ui_FitnessPage.h"
#include "../Inc/ui_MenuPage.h"
#include "../../../Func/Inc/HWDataAccess.h"

///////////////////// VARIABLES ////////////////////
// 页面对象
lv_obj_t * ui_WatchFacePage;

// 时间显示
lv_obj_t * ui_WF_TimeHourLabel;
lv_obj_t * ui_WF_TimeMinuteLabel;
lv_obj_t * ui_WF_DateLabel;
lv_obj_t * ui_WF_DayLabel;

// 电池
lv_obj_t * ui_WF_BatArc;
lv_obj_t * ui_WF_BatLabel;

// 三环 - 步数(外环,蓝色)
lv_obj_t * ui_WF_StepArc;
lv_obj_t * ui_WF_StepLabel;

// 三环 - 卡路里(中环,橙色)
lv_obj_t * ui_WF_CalorieArc;
lv_obj_t * ui_WF_CalorieLabel;

// 三环 - 站立(内环,绿色)
lv_obj_t * ui_WF_StandArc;
lv_obj_t * ui_WF_StandLabel;

// 底部快捷入口
lv_obj_t * ui_WF_QuickAppsBtn;
lv_obj_t * ui_WF_FitnessBtn;
lv_obj_t * ui_WF_MenuBtn;
lv_obj_t * ui_WF_HRLabel;
lv_obj_t * ui_WF_TempLabel;
lv_obj_t * ui_WF_HumiLabel;

// 定时器
lv_timer_t * ui_WatchFacePageTimer;

// 变量缓存
uint8_t ui_WF_TimeHourValue = 12;
uint8_t ui_WF_TimeMinuteValue = 0;
uint8_t ui_WF_BatValue = 85;
uint16_t ui_WF_StepValue = 0;
uint16_t ui_WF_CalorieValue = 0;
uint8_t ui_WF_StandValue = 0;
int8_t ui_WF_TempValue = 25;
int8_t ui_WF_HumiValue = 60;
uint8_t ui_WF_HRValue = 72;

const char * ui_WF_Days[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

// 目标值
#define STEP_GOAL 10000
#define CALORIE_GOAL 500
#define STAND_GOAL 12

///////////////////// Page Manager //////////////////
Page_t Page_WatchFace = {ui_WatchFacePage_screen_init, ui_WatchFacePage_screen_deinit, &ui_WatchFacePage};

///////////////////// TIMER CALLBACK //////////////////
static void WatchFacePage_timer_cb(lv_timer_t * timer)
{
    if(Page_Get_NowPage()->page_obj == &ui_WatchFacePage)
    {
        char value_strbuf[16];
        HW_DateTimeTypeDef DateTime;

        // 获取时间
        HWInterface.RealTimeClock.GetTimeDate(&DateTime);

        // 更新时间
        if(ui_WF_TimeMinuteValue != DateTime.Minutes)
        {
            ui_WF_TimeMinuteValue = DateTime.Minutes;
            sprintf(value_strbuf, "%02d", ui_WF_TimeMinuteValue);
            lv_label_set_text(ui_WF_TimeMinuteLabel, value_strbuf);
        }

        if(ui_WF_TimeHourValue != DateTime.Hours)
        {
            ui_WF_TimeHourValue = DateTime.Hours;
            sprintf(value_strbuf, "%02d", ui_WF_TimeHourValue);
            lv_label_set_text(ui_WF_TimeHourLabel, value_strbuf);
        }

        // 更新日期
        sprintf(value_strbuf, "%d/%d", DateTime.Month, DateTime.Date);
        lv_label_set_text(ui_WF_DateLabel, value_strbuf);
        lv_label_set_text(ui_WF_DayLabel, ui_WF_Days[DateTime.WeekDay - 1]);

        // 更新电池
        if(ui_WF_BatValue != HWInterface.Power.power_remain)
        {
            ui_WF_BatValue = HWInterface.Power.power_remain;
            lv_arc_set_value(ui_WF_BatArc, ui_WF_BatValue);
            sprintf(value_strbuf, "%d%%", ui_WF_BatValue);
            lv_label_set_text(ui_WF_BatLabel, value_strbuf);
        }

        // 更新步数环 (外环, 蓝色)
        if(ui_WF_StepValue != HWInterface.IMU.Steps)
        {
            ui_WF_StepValue = HWInterface.IMU.Steps;
            uint8_t step_percent = (ui_WF_StepValue > STEP_GOAL) ? 100 : (ui_WF_StepValue * 100 / STEP_GOAL);
            lv_arc_set_value(ui_WF_StepArc, step_percent);
            sprintf(value_strbuf, "%d", ui_WF_StepValue);
            lv_label_set_text(ui_WF_StepLabel, value_strbuf);
        }

        // 更新卡路里环 (中环, 橙色) - 根据步数估算
        uint16_t calorie = ui_WF_StepValue * 4 / 100;  // 简单估算
        if(ui_WF_CalorieValue != calorie)
        {
            ui_WF_CalorieValue = calorie;
            uint8_t calorie_percent = (ui_WF_CalorieValue > CALORIE_GOAL) ? 100 : (ui_WF_CalorieValue * 100 / CALORIE_GOAL);
            lv_arc_set_value(ui_WF_CalorieArc, calorie_percent);
            sprintf(value_strbuf, "%d", ui_WF_CalorieValue);
            lv_label_set_text(ui_WF_CalorieLabel, value_strbuf);
        }

        // 更新站立环 (内环, 绿色)
        uint8_t stand_hours = (DateTime.Hours >= 6) ? ((DateTime.Hours - 6) % 24) : 0; // 简化计算
        if(stand_hours > 12) stand_hours = 12;
        if(ui_WF_StandValue != stand_hours)
        {
            ui_WF_StandValue = stand_hours;
            uint8_t stand_percent = ui_WF_StandValue * 100 / STAND_GOAL;
            if(stand_percent > 100) stand_percent = 100;
            lv_arc_set_value(ui_WF_StandArc, stand_percent);
            sprintf(value_strbuf, "%d", ui_WF_StandValue);
            lv_label_set_text(ui_WF_StandLabel, value_strbuf);
        }

        // 更新心率
        if(ui_WF_HRValue != HWInterface.HR_meter.HrRate)
        {
            ui_WF_HRValue = HWInterface.HR_meter.HrRate;
            sprintf(value_strbuf, "%d", ui_WF_HRValue);
            lv_label_set_text(ui_WF_HRLabel, value_strbuf);
        }

        // 更新温湿度
        if(ui_WF_TempValue != (int8_t)HWInterface.AHT21.temperature)
        {
            ui_WF_TempValue = (int8_t)HWInterface.AHT21.temperature;
            sprintf(value_strbuf, "%d", ui_WF_TempValue);
            lv_label_set_text(ui_WF_TempLabel, value_strbuf);
        }

        if(ui_WF_HumiValue != (int8_t)HWInterface.AHT21.humidity)
        {
            ui_WF_HumiValue = (int8_t)HWInterface.AHT21.humidity;
            sprintf(value_strbuf, "%d%%", ui_WF_HumiValue);
            lv_label_set_text(ui_WF_HumiLabel, value_strbuf);
        }
    }
}

///////////////////// EVENTS ////////////////////
void ui_event_WatchFacePage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_GESTURE)
    {
        if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
        {
            Page_Load(&Page_Menu);
        }
    }
}

void ui_event_WF_QuickAppsBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_QuickApps);
    }
}

void ui_event_WF_FitnessBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Load(&Page_Fitness);
    }
}

///////////////////// SCREEN init ////////////////////
void ui_WatchFacePage_screen_init(void)
{
    char value_strbuf[16];

    // 创建页面
    ui_WatchFacePage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_WatchFacePage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WatchFacePage, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);  // 浅灰背景

    // ========== 顶部状态栏 ==========
    // 顶部面板
    lv_obj_t * ui_WF_TopBar = lv_obj_create(ui_WatchFacePage);
    lv_obj_set_size(ui_WF_TopBar, 240, 30);
    lv_obj_set_align(ui_WF_TopBar, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_WF_TopBar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WF_TopBar, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_WF_TopBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 日期标签 (左上)
    ui_WF_DateLabel = lv_label_create(ui_WF_TopBar);
    lv_obj_set_align(ui_WF_DateLabel, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_WF_DateLabel, 10);
    sprintf(value_strbuf, "%d/%d", ui_WF_BatValue, ui_WF_BatValue);  // 临时
    lv_label_set_text(ui_WF_DateLabel, "3/23");
    lv_obj_set_style_text_font(ui_WF_DateLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_DateLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 电池弧形 (右上)
    ui_WF_BatArc = lv_arc_create(ui_WF_TopBar);
    lv_obj_set_size(ui_WF_BatArc, 24, 24);
    lv_obj_set_align(ui_WF_BatArc, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_WF_BatArc, -10);
    lv_arc_set_value(ui_WF_BatArc, ui_WF_BatValue);
    lv_arc_set_bg_angles(ui_WF_BatArc, 135, 405);
    lv_obj_set_style_arc_width(ui_WF_BatArc, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WF_BatArc, 3, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_BatArc, lv_color_hex(0x19C819), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_BatArc, lv_color_hex(0xE0E0E0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_WF_BatArc, LV_OBJ_FLAG_CLICKABLE);

    // 电池标签
    ui_WF_BatLabel = lv_label_create(ui_WF_TopBar);
    lv_obj_set_align(ui_WF_BatLabel, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_WF_BatLabel, -38);
    sprintf(value_strbuf, "%d%%", ui_WF_BatValue);
    lv_label_set_text(ui_WF_BatLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_BatLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_BatLabel, lv_color_hex(0x19C819), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 三环区域 (居中) ==========
    // 外环 - 步数 (蓝色)
    ui_WF_StepArc = lv_arc_create(ui_WatchFacePage);
    lv_obj_set_size(ui_WF_StepArc, 200, 200);
    lv_obj_set_align(ui_WF_StepArc, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WF_StepArc, -10);
    uint8_t step_percent = (ui_WF_StepValue > STEP_GOAL) ? 100 : (ui_WF_StepValue * 100 / STEP_GOAL);
    lv_arc_set_value(ui_WF_StepArc, step_percent);
    lv_arc_set_bg_angles(ui_WF_StepArc, 135, 405);
    lv_obj_set_style_arc_width(ui_WF_StepArc, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WF_StepArc, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_StepArc, lv_color_hex(0x3278FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_StepArc, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WF_StepArc, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_WF_StepArc, LV_OBJ_FLAG_CLICKABLE);

    // 步数标签
    ui_WF_StepLabel = lv_label_create(ui_WF_StepArc);
    lv_obj_set_align(ui_WF_StepLabel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(ui_WF_StepLabel, -2);
    sprintf(value_strbuf, "%d", ui_WF_StepValue);
    lv_label_set_text(ui_WF_StepLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_StepLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_StepLabel, lv_color_hex(0x3278FF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 中环 - 卡路里 (橙色)
    ui_WF_CalorieArc = lv_arc_create(ui_WatchFacePage);
    lv_obj_set_size(ui_WF_CalorieArc, 160, 160);
    lv_obj_set_align(ui_WF_CalorieArc, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WF_CalorieArc, -10);
    uint8_t calorie_percent = (ui_WF_CalorieValue > CALORIE_GOAL) ? 100 : (ui_WF_CalorieValue * 100 / CALORIE_GOAL);
    lv_arc_set_value(ui_WF_CalorieArc, calorie_percent);
    lv_arc_set_bg_angles(ui_WF_CalorieArc, 135, 405);
    lv_obj_set_style_arc_width(ui_WF_CalorieArc, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WF_CalorieArc, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_CalorieArc, lv_color_hex(0xF5A73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_CalorieArc, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WF_CalorieArc, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_WF_CalorieArc, LV_OBJ_FLAG_CLICKABLE);

    // 卡路里标签
    ui_WF_CalorieLabel = lv_label_create(ui_WF_CalorieArc);
    lv_obj_set_align(ui_WF_CalorieLabel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(ui_WF_CalorieLabel, -2);
    sprintf(value_strbuf, "%d", ui_WF_CalorieValue);
    lv_label_set_text(ui_WF_CalorieLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_CalorieLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_CalorieLabel, lv_color_hex(0xF5A73A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 内环 - 站立 (绿色)
    ui_WF_StandArc = lv_arc_create(ui_WatchFacePage);
    lv_obj_set_size(ui_WF_StandArc, 120, 120);
    lv_obj_set_align(ui_WF_StandArc, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WF_StandArc, -10);
    uint8_t stand_percent = ui_WF_StandValue * 100 / STAND_GOAL;
    if(stand_percent > 100) stand_percent = 100;
    lv_arc_set_value(ui_WF_StandArc, stand_percent);
    lv_arc_set_bg_angles(ui_WF_StandArc, 135, 405);
    lv_obj_set_style_arc_width(ui_WF_StandArc, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_WF_StandArc, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_StandArc, lv_color_hex(0x19C819), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_WF_StandArc, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_WF_StandArc, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_WF_StandArc, LV_OBJ_FLAG_CLICKABLE);

    // 站立标签
    ui_WF_StandLabel = lv_label_create(ui_WF_StandArc);
    lv_obj_set_align(ui_WF_StandLabel, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(ui_WF_StandLabel, -2);
    sprintf(value_strbuf, "%dh", ui_WF_StandValue);
    lv_label_set_text(ui_WF_StandLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_StandLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_StandLabel, lv_color_hex(0x19C819), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 中心时间显示 ==========
    // 小时
    ui_WF_TimeHourLabel = lv_label_create(ui_WatchFacePage);
    lv_obj_set_align(ui_WF_TimeHourLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WF_TimeHourLabel, -20);
    sprintf(value_strbuf, "%02d", ui_WF_TimeHourValue);
    lv_label_set_text(ui_WF_TimeHourLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_TimeHourLabel, &ui_font_Cuyuan48, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_TimeHourLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 分钟
    ui_WF_TimeMinuteLabel = lv_label_create(ui_WatchFacePage);
    lv_obj_set_align(ui_WF_TimeMinuteLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WF_TimeMinuteLabel, 25);
    sprintf(value_strbuf, "%02d", ui_WF_TimeMinuteValue);
    lv_label_set_text(ui_WF_TimeMinuteLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_TimeMinuteLabel, &ui_font_Cuyuan30, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_TimeMinuteLabel, lv_color_hex(0x666666), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 星期
    ui_WF_DayLabel = lv_label_create(ui_WatchFacePage);
    lv_obj_set_align(ui_WF_DayLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_WF_DayLabel, 55);
    lv_label_set_text(ui_WF_DayLabel, "Mon");
    lv_obj_set_style_text_font(ui_WF_DayLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_DayLabel, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 底部快捷入口 ==========
    // 底部面板
    lv_obj_t * ui_WF_BottomBar = lv_obj_create(ui_WatchFacePage);
    lv_obj_set_size(ui_WF_BottomBar, 240, 80);
    lv_obj_set_align(ui_WF_BottomBar, LV_ALIGN_BOTTOM_MID);
    lv_obj_clear_flag(ui_WF_BottomBar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WF_BottomBar, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_WF_BottomBar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 快捷应用按钮
    ui_WF_QuickAppsBtn = lv_btn_create(ui_WF_BottomBar);
    lv_obj_set_size(ui_WF_QuickAppsBtn, 60, 50);
    lv_obj_set_align(ui_WF_QuickAppsBtn, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_WF_QuickAppsBtn, 15);
    lv_obj_set_style_radius(ui_WF_QuickAppsBtn, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WF_QuickAppsBtn, lv_color_hex(0x3278FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_WF_QuickAppsBtn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_WF_QuickAppsBtn, lv_color_hex(0x3278FF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_WF_QuickAppsBtn, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_WF_QuickAppsIcon = lv_label_create(ui_WF_QuickAppsBtn);
    lv_obj_set_align(ui_WF_QuickAppsIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WF_QuickAppsIcon, "+");
    lv_obj_set_style_text_color(ui_WF_QuickAppsIcon, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_WF_QuickAppsIcon, &ui_font_Cuyuan30, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 健身按钮
    ui_WF_FitnessBtn = lv_btn_create(ui_WF_BottomBar);
    lv_obj_set_size(ui_WF_FitnessBtn, 60, 50);
    lv_obj_set_align(ui_WF_FitnessBtn, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_WF_FitnessBtn, -15);
    lv_obj_set_style_radius(ui_WF_FitnessBtn, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_WF_FitnessBtn, lv_color_hex(0x19C819), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui_WF_FitnessBtn, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_WF_FitnessBtn, lv_color_hex(0x19C819), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_WF_FitnessBtn, 50, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_WF_FitnessIcon = lv_label_create(ui_WF_FitnessBtn);
    lv_obj_set_align(ui_WF_FitnessIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_WF_FitnessIcon, "Run");
    lv_obj_set_style_text_color(ui_WF_FitnessIcon, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_WF_FitnessIcon, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 中间心率显示
    lv_obj_t * ui_WF_HRPanel = lv_obj_create(ui_WF_BottomBar);
    lv_obj_set_size(ui_WF_HRPanel, 70, 50);
    lv_obj_set_align(ui_WF_HRPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_WF_HRPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WF_HRPanel, lv_color_hex(0xFFF5F5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_WF_HRPanel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WF_HRLabel = lv_label_create(ui_WF_HRPanel);
    lv_obj_set_align(ui_WF_HRLabel, LV_ALIGN_CENTER);
    sprintf(value_strbuf, "%d", ui_WF_HRValue);
    lv_label_set_text(ui_WF_HRLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_HRLabel, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_HRLabel, lv_color_hex(0xE11432), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_WF_HRIcon = lv_label_create(ui_WF_HRPanel);
    lv_obj_set_align(ui_WF_HRIcon, LV_ALIGN_TOP_MID);
    lv_obj_set_y(ui_WF_HRIcon, 2);
    lv_label_set_text(ui_WF_HRIcon, "HR");
    lv_obj_set_style_text_font(ui_WF_HRIcon, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_HRIcon, lv_color_hex(0xE11432), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 温湿度显示
    lv_obj_t * ui_WF_EnvPanel = lv_obj_create(ui_WF_BottomBar);
    lv_obj_set_size(ui_WF_EnvPanel, 70, 50);
    lv_obj_set_align(ui_WF_EnvPanel, LV_ALIGN_CENTER);
    lv_obj_set_x(ui_WF_EnvPanel, -80);
    lv_obj_clear_flag(ui_WF_EnvPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_WF_EnvPanel, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_WF_EnvPanel, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WF_TempLabel = lv_label_create(ui_WF_EnvPanel);
    lv_obj_set_align(ui_WF_TempLabel, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_WF_TempLabel, 8);
    sprintf(value_strbuf, "%d...", ui_WF_TempValue);
    lv_label_set_text(ui_WF_TempLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_TempLabel, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_TempLabel, lv_color_hex(0xF5A73A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_WF_HumiLabel = lv_label_create(ui_WF_EnvPanel);
    lv_obj_set_align(ui_WF_HumiLabel, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_WF_HumiLabel, -8);
    sprintf(value_strbuf, "%d%%", ui_WF_HumiValue);
    lv_label_set_text(ui_WF_HumiLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_WF_HumiLabel, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_WF_HumiLabel, lv_color_hex(0x14C8E1), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 事件注册 ==========
    lv_obj_add_event_cb(ui_WatchFacePage, ui_event_WatchFacePage, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WF_QuickAppsBtn, ui_event_WF_QuickAppsBtn, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_WF_FitnessBtn, ui_event_WF_FitnessBtn, LV_EVENT_ALL, NULL);

    // ========== 定时器 ==========
    ui_WatchFacePageTimer = lv_timer_create(WatchFacePage_timer_cb, 500, NULL);
}

///////////////////// SCREEN deinit ////////////////////
void ui_WatchFacePage_screen_deinit(void)
{
    lv_timer_del(ui_WatchFacePageTimer);
}
