#include "../../ui.h"
#include "../../ui_helpers.h"
#include "../Inc/ui_FitnessPage.h"
#include "../Inc/ui_WatchFacePage.h"
#include "../../../Func/Inc/HWDataAccess.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_FitnessPage;
lv_obj_t * ui_Fit_ProgressArc;
lv_obj_t * ui_Fit_ProgressLabel;
lv_obj_t * ui_Fit_StepValueLabel;
lv_obj_t * ui_Fit_StepGoalLabel;
lv_obj_t * ui_Fit_CalorieValueLabel;
lv_obj_t * ui_Fit_CalorieGoalLabel;
lv_obj_t * ui_Fit_StandValueLabel;
lv_obj_t * ui_Fit_StandGoalLabel;
lv_obj_t * ui_Fit_ExerciseValueLabel;
lv_obj_t * ui_Fit_ExerciseGoalLabel;

lv_timer_t * ui_FitnessPageTimer;

uint16_t ui_Fit_StepValue = 0;
uint16_t ui_Fit_CalorieValue = 0;
uint8_t ui_Fit_StandValue = 0;
uint16_t ui_Fit_ExerciseValue = 0;

#define STEP_GOAL 10000
#define CALORIE_GOAL 500
#define STAND_GOAL 12
#define EXERCISE_GOAL 60  // 分钟

///////////////////// Page Manager //////////////////
Page_t Page_Fitness = {ui_FitnessPage_screen_init, ui_FitnessPage_screen_deinit, &ui_FitnessPage};

///////////////////// TIMER CALLBACK //////////////////
static void FitnessPage_timer_cb(lv_timer_t * timer)
{
    if(Page_Get_NowPage()->page_obj == &ui_FitnessPage)
    {
        char value_strbuf[16];

        // 更新步数
        if(ui_Fit_StepValue != HWInterface.IMU.Steps)
        {
            ui_Fit_StepValue = HWInterface.IMU.Steps;
            sprintf(value_strbuf, "%d", ui_Fit_StepValue);
            lv_label_set_text(ui_Fit_StepValueLabel, value_strbuf);

            // 更新卡路里
            ui_Fit_CalorieValue = ui_Fit_StepValue * 4 / 100;
            sprintf(value_strbuf, "%d", ui_Fit_CalorieValue);
            lv_label_set_text(ui_Fit_CalorieValueLabel, value_strbuf);

            // 计算总完成度
            uint16_t step_progress = (ui_Fit_StepValue > STEP_GOAL) ? 100 : (ui_Fit_StepValue * 100 / STEP_GOAL);
            uint16_t calorie_progress = (ui_Fit_CalorieValue > CALORIE_GOAL) ? 100 : (ui_Fit_CalorieValue * 100 / CALORIE_GOAL);
            uint16_t stand_progress = ui_Fit_StandValue * 100 / STAND_GOAL;
            if(stand_progress > 100) stand_progress = 100;

            uint16_t total_progress = (step_progress + calorie_progress + stand_progress) / 3;
            lv_arc_set_value(ui_Fit_ProgressArc, total_progress);
            sprintf(value_strbuf, "%d%%", total_progress);
            lv_label_set_text(ui_Fit_ProgressLabel, value_strbuf);
        }

        // 更新站立小时
        HW_DateTimeTypeDef DateTime;
        HWInterface.RealTimeClock.GetTimeDate(&DateTime);
        uint8_t stand_hours = (DateTime.Hours >= 6) ? ((DateTime.Hours - 6) % 24) : 0;
        if(stand_hours > 12) stand_hours = 12;
        if(ui_Fit_StandValue != stand_hours)
        {
            ui_Fit_StandValue = stand_hours;
            sprintf(value_strbuf, "%d", ui_Fit_StandValue);
            lv_label_set_text(ui_Fit_StandValueLabel, value_strbuf);

            uint16_t stand_progress = ui_Fit_StandValue * 100 / STAND_GOAL;
            if(stand_progress > 100) stand_progress = 100;

            uint16_t step_progress = (ui_Fit_StepValue > STEP_GOAL) ? 100 : (ui_Fit_StepValue * 100 / STEP_GOAL);
            uint16_t calorie_progress = (ui_Fit_CalorieValue > CALORIE_GOAL) ? 100 : (ui_Fit_CalorieValue * 100 / CALORIE_GOAL);

            uint16_t total_progress = (step_progress + calorie_progress + stand_progress) / 3;
            lv_arc_set_value(ui_Fit_ProgressArc, total_progress);
            sprintf(value_strbuf, "%d%%", total_progress);
            lv_label_set_text(ui_Fit_ProgressLabel, value_strbuf);
        }
    }
}

///////////////////// EVENTS ////////////////////
void ui_event_Fit_BackBtn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if(event_code == LV_EVENT_CLICKED)
    {
        Page_Back();
    }
}

///////////////////// SCREEN init ////////////////////
void ui_FitnessPage_screen_init(void)
{
    char value_strbuf[16];

    // 创建页面
    ui_FitnessPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_FitnessPage, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_FitnessPage, lv_color_hex(0xF5F5F5), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 顶部导航栏 ==========
    lv_obj_t * ui_Fit_TopBar = lv_obj_create(ui_FitnessPage);
    lv_obj_set_size(ui_Fit_TopBar, 240, 40);
    lv_obj_set_align(ui_Fit_TopBar, LV_ALIGN_TOP_MID);
    lv_obj_clear_flag(ui_Fit_TopBar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Fit_TopBar, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 返回按钮
    lv_obj_t * ui_Fit_BackBtn = lv_btn_create(ui_Fit_TopBar);
    lv_obj_set_size(ui_Fit_BackBtn, 30, 30);
    lv_obj_set_align(ui_Fit_BackBtn, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Fit_BackBtn, 5);
    lv_obj_set_style_radius(ui_Fit_BackBtn, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Fit_BackBtn, lv_color_hex(0xF0F0F0), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_Fit_BackIcon = lv_label_create(ui_Fit_BackBtn);
    lv_obj_set_align(ui_Fit_BackIcon, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Fit_BackIcon, "<");
    lv_obj_set_style_text_font(ui_Fit_BackIcon, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_BackIcon, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 标题
    lv_obj_t * ui_Fit_Title = lv_label_create(ui_Fit_TopBar);
    lv_obj_set_align(ui_Fit_Title, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Fit_Title, "Fitness");
    lv_obj_set_style_text_font(ui_Fit_Title, &ui_font_Cuyuan18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_Title, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 今日完成度 ==========
    lv_obj_t * ui_Fit_ProgressPanel = lv_obj_create(ui_FitnessPage);
    lv_obj_set_size(ui_Fit_ProgressPanel, 180, 180);
    lv_obj_set_align(ui_Fit_ProgressPanel, LV_ALIGN_TOP_MID);
    lv_obj_set_y(ui_Fit_ProgressPanel, 50);
    lv_obj_clear_flag(ui_Fit_ProgressPanel, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Fit_ProgressPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Fit_ProgressPanel, 90, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 主进度环
    ui_Fit_ProgressArc = lv_arc_create(ui_Fit_ProgressPanel);
    lv_obj_set_size(ui_Fit_ProgressArc, 160, 160);
    lv_obj_set_align(ui_Fit_ProgressArc, LV_ALIGN_CENTER);
    uint16_t step_progress = (ui_Fit_StepValue > STEP_GOAL) ? 100 : (ui_Fit_StepValue * 100 / STEP_GOAL);
    uint16_t calorie_progress = (ui_Fit_CalorieValue > CALORIE_GOAL) ? 100 : (ui_Fit_CalorieValue * 100 / CALORIE_GOAL);
    uint16_t stand_progress = ui_Fit_StandValue * 100 / STAND_GOAL;
    if(stand_progress > 100) stand_progress = 100;
    uint16_t total_progress = (step_progress + calorie_progress + stand_progress) / 3;
    lv_arc_set_value(ui_Fit_ProgressArc, total_progress);
    lv_arc_set_bg_angles(ui_Fit_ProgressArc, 135, 405);
    lv_obj_set_style_arc_width(ui_Fit_ProgressArc, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_Fit_ProgressArc, 15, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_Fit_ProgressArc, lv_color_hex(0x3278FF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui_Fit_ProgressArc, lv_color_hex(0xE8E8E8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui_Fit_ProgressArc, true, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_Fit_ProgressArc, LV_OBJ_FLAG_CLICKABLE);

    // 完成度标签
    ui_Fit_ProgressLabel = lv_label_create(ui_Fit_ProgressPanel);
    lv_obj_set_align(ui_Fit_ProgressLabel, LV_ALIGN_CENTER);
    sprintf(value_strbuf, "%d%%", total_progress);
    lv_label_set_text(ui_Fit_ProgressLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_ProgressLabel, &ui_font_Cuyuan48, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_ProgressLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_Fit_ProgressSubLabel = lv_label_create(ui_Fit_ProgressPanel);
    lv_obj_set_align(ui_Fit_ProgressSubLabel, LV_ALIGN_CENTER);
    lv_obj_set_y(ui_Fit_ProgressSubLabel, 30);
    lv_label_set_text(ui_Fit_ProgressSubLabel, "Complete");
    lv_obj_set_style_text_font(ui_Fit_ProgressSubLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_ProgressSubLabel, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 数据卡片 ==========
    // 步数卡片
    lv_obj_t * ui_Fit_StepCard = lv_obj_create(ui_FitnessPage);
    lv_obj_set_size(ui_Fit_StepCard, 105, 70);
    lv_obj_set_align(ui_Fit_StepCard, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Fit_StepCard, 10);
    lv_obj_set_y(ui_Fit_StepCard, 55);
    lv_obj_clear_flag(ui_Fit_StepCard, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Fit_StepCard, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Fit_StepCard, 15, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_Fit_StepIcon = lv_label_create(ui_Fit_StepCard);
    lv_obj_set_align(ui_Fit_StepIcon, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(ui_Fit_StepIcon, 10);
    lv_obj_set_y(ui_Fit_StepIcon, 8);
    lv_label_set_text(ui_Fit_StepIcon, "...");
    lv_obj_set_style_text_font(ui_Fit_StepIcon, &ui_font_iconfont24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_StepIcon, lv_color_hex(0x3278FF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_StepValueLabel = lv_label_create(ui_Fit_StepCard);
    lv_obj_set_align(ui_Fit_StepValueLabel, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Fit_StepValueLabel, 10);
    sprintf(value_strbuf, "%d", ui_Fit_StepValue);
    lv_label_set_text(ui_Fit_StepValueLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_StepValueLabel, &ui_font_Cuyuan24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_StepValueLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_StepGoalLabel = lv_label_create(ui_Fit_StepCard);
    lv_obj_set_align(ui_Fit_StepGoalLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_x(ui_Fit_StepGoalLabel, 10);
    lv_obj_set_y(ui_Fit_StepGoalLabel, -8);
    sprintf(value_strbuf, "/%d", STEP_GOAL);
    lv_label_set_text(ui_Fit_StepGoalLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_StepGoalLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_StepGoalLabel, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 卡路里卡片
    lv_obj_t * ui_Fit_CalorieCard = lv_obj_create(ui_FitnessPage);
    lv_obj_set_size(ui_Fit_CalorieCard, 105, 70);
    lv_obj_set_align(ui_Fit_CalorieCard, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_Fit_CalorieCard, -10);
    lv_obj_set_y(ui_Fit_CalorieCard, 55);
    lv_obj_clear_flag(ui_Fit_CalorieCard, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Fit_CalorieCard, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Fit_CalorieCard, 15, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_Fit_CalorieIcon = lv_label_create(ui_Fit_CalorieCard);
    lv_obj_set_align(ui_Fit_CalorieIcon, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(ui_Fit_CalorieIcon, 10);
    lv_obj_set_y(ui_Fit_CalorieIcon, 8);
    lv_label_set_text(ui_Fit_CalorieIcon, "...");
    lv_obj_set_style_text_font(ui_Fit_CalorieIcon, &ui_font_iconfont24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_CalorieIcon, lv_color_hex(0xF5A73A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_CalorieValueLabel = lv_label_create(ui_Fit_CalorieCard);
    lv_obj_set_align(ui_Fit_CalorieValueLabel, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Fit_CalorieValueLabel, 10);
    sprintf(value_strbuf, "%d", ui_Fit_CalorieValue);
    lv_label_set_text(ui_Fit_CalorieValueLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_CalorieValueLabel, &ui_font_Cuyuan24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_CalorieValueLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_CalorieGoalLabel = lv_label_create(ui_Fit_CalorieCard);
    lv_obj_set_align(ui_Fit_CalorieGoalLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_x(ui_Fit_CalorieGoalLabel, 10);
    lv_obj_set_y(ui_Fit_CalorieGoalLabel, -8);
    sprintf(value_strbuf, "/%d", CALORIE_GOAL);
    lv_label_set_text(ui_Fit_CalorieGoalLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_CalorieGoalLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_CalorieGoalLabel, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 站立小时卡片
    lv_obj_t * ui_Fit_StandCard = lv_obj_create(ui_FitnessPage);
    lv_obj_set_size(ui_Fit_StandCard, 105, 70);
    lv_obj_set_align(ui_Fit_StandCard, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Fit_StandCard, 10);
    lv_obj_set_y(ui_Fit_StandCard, 135);
    lv_obj_clear_flag(ui_Fit_StandCard, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Fit_StandCard, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Fit_StandCard, 15, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_Fit_StandIcon = lv_label_create(ui_Fit_StandCard);
    lv_obj_set_align(ui_Fit_StandIcon, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(ui_Fit_StandIcon, 10);
    lv_obj_set_y(ui_Fit_StandIcon, 8);
    lv_label_set_text(ui_Fit_StandIcon, "...");
    lv_obj_set_style_text_font(ui_Fit_StandIcon, &ui_font_iconfont24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_StandIcon, lv_color_hex(0x19C819), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_StandValueLabel = lv_label_create(ui_Fit_StandCard);
    lv_obj_set_align(ui_Fit_StandValueLabel, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Fit_StandValueLabel, 10);
    sprintf(value_strbuf, "%d", ui_Fit_StandValue);
    lv_label_set_text(ui_Fit_StandValueLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_StandValueLabel, &ui_font_Cuyuan24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_StandValueLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_StandGoalLabel = lv_label_create(ui_Fit_StandCard);
    lv_obj_set_align(ui_Fit_StandGoalLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_x(ui_Fit_StandGoalLabel, 10);
    lv_obj_set_y(ui_Fit_StandGoalLabel, -8);
    sprintf(value_strbuf, "/%dh", STAND_GOAL);
    lv_label_set_text(ui_Fit_StandGoalLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_StandGoalLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_StandGoalLabel, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);

    // 运动时长卡片
    lv_obj_t * ui_Fit_ExerciseCard = lv_obj_create(ui_FitnessPage);
    lv_obj_set_size(ui_Fit_ExerciseCard, 105, 70);
    lv_obj_set_align(ui_Fit_ExerciseCard, LV_ALIGN_RIGHT_MID);
    lv_obj_set_x(ui_Fit_ExerciseCard, -10);
    lv_obj_set_y(ui_Fit_ExerciseCard, 135);
    lv_obj_clear_flag(ui_Fit_ExerciseCard, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(ui_Fit_ExerciseCard, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui_Fit_ExerciseCard, 15, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * ui_Fit_ExerciseIcon = lv_label_create(ui_Fit_ExerciseCard);
    lv_obj_set_align(ui_Fit_ExerciseIcon, LV_ALIGN_TOP_LEFT);
    lv_obj_set_x(ui_Fit_ExerciseIcon, 10);
    lv_obj_set_y(ui_Fit_ExerciseIcon, 8);
    lv_label_set_text(ui_Fit_ExerciseIcon, "...");
    lv_obj_set_style_text_font(ui_Fit_ExerciseIcon, &ui_font_iconfont24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_ExerciseIcon, lv_color_hex(0x9966FF), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_ExerciseValueLabel = lv_label_create(ui_Fit_ExerciseCard);
    lv_obj_set_align(ui_Fit_ExerciseValueLabel, LV_ALIGN_LEFT_MID);
    lv_obj_set_x(ui_Fit_ExerciseValueLabel, 10);
    sprintf(value_strbuf, "%d", ui_Fit_ExerciseValue);
    lv_label_set_text(ui_Fit_ExerciseValueLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_ExerciseValueLabel, &ui_font_Cuyuan24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_ExerciseValueLabel, lv_color_hex(0x1A1A1A), LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Fit_ExerciseGoalLabel = lv_label_create(ui_Fit_ExerciseCard);
    lv_obj_set_align(ui_Fit_ExerciseGoalLabel, LV_ALIGN_BOTTOM_LEFT);
    lv_obj_set_x(ui_Fit_ExerciseGoalLabel, 10);
    lv_obj_set_y(ui_Fit_ExerciseGoalLabel, -8);
    sprintf(value_strbuf, "/%dmin", EXERCISE_GOAL);
    lv_label_set_text(ui_Fit_ExerciseGoalLabel, value_strbuf);
    lv_obj_set_style_text_font(ui_Fit_ExerciseGoalLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Fit_ExerciseGoalLabel, lv_color_hex(0x999999), LV_PART_MAIN | LV_STATE_DEFAULT);

    // ========== 事件注册 ==========
    lv_obj_add_event_cb(ui_Fit_BackBtn, ui_event_Fit_BackBtn, LV_EVENT_ALL, NULL);

    // ========== 定时器 ==========
    ui_FitnessPageTimer = lv_timer_create(FitnessPage_timer_cb, 500, NULL);
}

///////////////////// SCREEN deinit ////////////////////
void ui_FitnessPage_screen_deinit(void)
{
    lv_timer_del(ui_FitnessPageTimer);
}
