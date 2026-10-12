/* Private includes -----------------------------------------------------------*/
//includes
#include "user_TasksInit.h"

#include "ui.h"
#include "ui_HomePage.h"
#include "ui_OffTimePage.h"

#include "main.h"
#include "stm32f4xx_it.h"
#include "usart.h"
#include "lcd_init.h"
#include "power.h"
#include "CST816.h"
#include "MPU6050.h"
#include "key.h"
#include "WDOG.h"

#include "HWDataAccess.h"
#include "elog.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint16_t IdleTimerCount = 0;

/* Private function prototypes -----------------------------------------------*/

/* Tasks ---------------------------------------------------------------------*/

/**
	* @brief  Enter Idle state
  * @param  argument: Not used
  * @retval None
  */
void IdleEnterTask(void *argument)
{
	uint8_t Idlestr=0;
	uint8_t IdleBreakstr=0;
#if UI_DISPLAY_ALWAYS_ON
	LCD_Set_Light(ui_LightSliderValue);
#endif
	while(1)
	{
		//light get dark
		if(osMessageQueueGet(Idle_MessageQueue,&Idlestr,NULL,1)==osOK)
		{
#if UI_DISPLAY_ALWAYS_ON
			LCD_Set_Light(ui_LightSliderValue);
#else
			LCD_Set_Light(5);
			log_i("PWR", "idle: backlight -> 5%% (no interaction)");
#endif
		}
		//resume light if light got dark and idle state breaked by key pressing or screen touching
		if(osMessageQueueGet(IdleBreak_MessageQueue,&IdleBreakstr,NULL,1)==osOK)
		{
			IdleTimerCount = 0;
			LCD_Set_Light(ui_LightSliderValue);
			log_i("PWR", "idle break by key/touch: backlight restore");
		}
		//resume light by wrist up (mailbox keeps only the latest wake request)
		uint8_t WristUpstr=0;
		if(osMessageQueueGet(WristUp_Mailbox,&WristUpstr,NULL,0)==osOK)
		{
			IdleTimerCount = 0;
			LCD_Set_Light(ui_LightSliderValue);
			log_i("PWR", "idle break by wrist up: backlight restore");
		}
		osDelay(10);
	}
}

/**
  * @brief  enter the stop mode and resume
  * @param  argument: Not used
  * @retval None
  */
void StopEnterTask(void *argument)
{
	uint8_t Stopstr;
	uint8_t HomeUpdataStr;
	uint8_t Wrist_Flag=0;
	while(1)
	{
		if(osMessageQueueGet(Stop_MessageQueue,&Stopstr,NULL,0)==osOK)
		{
#if UI_DISPLAY_ALWAYS_ON
			IdleTimerCount = 0;
			LCD_Set_Light(ui_LightSliderValue);
			continue;
#endif

			/*************************** your operations before sleep***************************/
			sleep:
			IdleTimerCount = 0;
			log_i("PWR", "enter stop mode (screen off, peripherals deinit)");

			//sensors

			//usart
			HAL_UART_MspDeInit(&huart1);

			//lcd
			LCD_RES_Clr();
			LCD_Close_Light();
			//touch
			CST816_Sleep();

			/***********************************************************************************/

			/****************************** enter wakeup operations *****************************/

			vTaskSuspendAll();
			//Disnable Watch Dog
			WDOG_Disnable();
			//systick int
			CLEAR_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
			//enter stop mode
			HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_STOPENTRY_WFI);

			//here is the sleep period

			/***********************************************************************************/

			/****************************** quit wakeup operations *****************************/

			//resume run mode and reset the sysclk
			SET_BIT(SysTick->CTRL, SysTick_CTRL_TICKINT_Msk);
			HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq));
			SystemClock_Config();
			WDOG_Feed();
			xTaskResumeAll();
			log_i("PWR", "wake up from stop mode: reconfig clock, feed WDG");

			/***********************************************************************************/

			/****************************** your wakeup operations *****************************/

			//MPU Check
			if(HWInterface.IMU.wrist_is_enabled)
			{
				uint8_t hor;
				hor = MPU_isHorizontal();
				if(hor && HWInterface.IMU.wrist_state == WRIST_DOWN)
				{
					HWInterface.IMU.wrist_state = WRIST_UP;
					Wrist_Flag = 1;
					//resume, go on
				}
				else if(!hor && HWInterface.IMU.wrist_state == WRIST_UP)
				{
					HWInterface.IMU.wrist_state = WRIST_DOWN;
					IdleTimerCount  = 0;
					goto sleep;
				}
			}

			//
			if(!KEY1 || KEY2 || HardInt_Charg_flag || Wrist_Flag)
			{
				Wrist_Flag = 0;
				//resume, go on
			}
			else
			{
				IdleTimerCount  = 0;
				goto sleep;
			}

			//usart
			HAL_UART_MspInit(&huart1);
			//lcd
			LCD_Init();
			LCD_Set_Light(ui_LightSliderValue);
			//touch
			CST816_Wakeup();
			//check if is Charging
			if(ChargeCheck())
			{HardInt_Charg_flag = 1;}
			//send the Home Updata message
			osMessageQueuePut(HomeUpdata_MessageQueue, &HomeUpdataStr, 0, 1);

			/**************************************************************************************/

		}
		osDelay(100);
	}
}

void IdleTimerCallback(void *argument)
{
#if UI_DISPLAY_ALWAYS_ON
	IdleTimerCount = 0;
	return;
#endif
	IdleTimerCount+=1;
	//make sure the LightOffTime<TurnOffTime
	if(IdleTimerCount == (ui_LTimeValue*10))
	{
		uint8_t Idlestr=0;
		//send the Light off message
		osMessageQueuePut(Idle_MessageQueue, &Idlestr, 0, 1);

	}
	if(IdleTimerCount == (ui_TTimeValue*10))
	{
		uint8_t Stopstr = 1;
		IdleTimerCount  = 0;
		//send the Stop message
		osMessageQueuePut(Stop_MessageQueue, &Stopstr, 0, 1);
	}
}

