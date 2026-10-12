######################################
# target
######################################
TARGET = OV_Watch

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Core/Src/main.c \
Core/Src/gpio.c \
Core/Src/adc.c \
Core/Src/dma.c \
Core/Src/rtc.c \
Core/Src/spi.c \
Core/Src/tim.c \
Core/Src/usart.c \
Core/Src/stm32f4xx_it.c \
Core/Src/stm32f4xx_hal_msp.c \
Core/Src/stm32f4xx_hal_timebase_tim.c \
Core/Src/system_stm32f4xx.c \
Core/Src/freertos.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_adc.c \
Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
Middlewares/Third_Party/FreeRTOS/Source/list.c \
Middlewares/Third_Party/FreeRTOS/Source/queue.c \
Middlewares/Third_Party/FreeRTOS/Source/stream_buffer.c \
Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
Middlewares/Third_Party/FreeRTOS/Source/timers.c \
Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
BSP/LCD/lcd.c \
BSP/LCD/lcd_init.c \
BSP/AHT21/AHT21.c \
BSP/BL24C02/BL24C02.c \
BSP/BL24C02/DataSave.c \
BSP/IIC/iic_hal.c \
BSP/MPU6050/mpu6050.c \
BSP/MPU6050/eMPL/inv_mpu.c \
BSP/MPU6050/eMPL/inv_mpu_dmp_motion_driver.c \
BSP/OWDG/WDOG.c \
BSP/POWER/power.c \
BSP/SPL06_001/SPL06_001.c \
BSP/EM7028/em70x8.c \
BSP/EM7028/HrAlgorythm.c \
BSP/EM7028/user_Queue.c \
BSP/TOUCH/CST816.c \
BSP/KEY/key.c \
BSP/KT6328/KT6328.c \
BSP/LSM303DLH/LSM303.c \
SYSTEM/delay.c \
User/Tasks/Src/user_TasksInit.c \
User/Tasks/Src/user_HardwareInitTask.c \
User/Tasks/Src/user_SensUpdateTask.c \
User/Tasks/Src/user_KeyTask.c \
User/Tasks/Src/user_ScrRenewTask.c \
User/Tasks/Src/user_RunModeTasks.c \
User/Tasks/Src/user_DataSaveTask.c \
User/Tasks/Src/user_MessageSendTask.c \
User/Tasks/Src/user_ChargCheckTask.c \
User/Func/Src/PageManager.c \
User/Func/Src/HWDataAccess.c \
User/Func/Src/StrCalculate.c \
User/GUI_App/ui.c \
User/GUI_App/ui_helpers.c \
2/screens/ui_HomePage.c \
2/screens/ui_MenuPage.c \
2/screens/ui_uiAboutPage.c \
User/GUI_App/Screens/Src/ui_HomeCompat.c \
User/GUI_App/Screens/Src/ui_HRPage.c \
User/GUI_App/Screens/Src/ui_SetPage.c \
User/GUI_App/Screens/Src/ui_CalendarPage.c \
User/GUI_App/Screens/Src/ui_ComputerPage.c \
User/GUI_App/Screens/Src/ui_TimerPage.c \
User/GUI_App/Screens/Src/ui_AboutPage.c \
User/GUI_App/Screens/Src/ui_ChargPage.c \
User/GUI_App/Screens/Src/ui_CompassPage.c \
User/GUI_App/Screens/Src/ui_DateTimeSetPage.c \
User/GUI_App/Screens/Src/ui_EnvPage.c \
User/GUI_App/Screens/Src/ui_Game2048Page.c \
User/GUI_App/Screens/Src/ui_GameMemPage.c \
User/GUI_App/Screens/Src/ui_GameSelectPage.c \
User/GUI_App/Screens/Src/ui_Megboxes.c \
User/GUI_App/Screens/Src/ui_NFCCardPage.c \
User/GUI_App/Screens/Src/ui_OffTimePage.c \
User/GUI_App/Screens/Src/ui_SPO2Page.c \
User/GUI_App/Screens/Src/ui_WatchFacePage.c \
User/GUI_App/Screens/Src/ui_QuickAppsPage.c \
User/GUI_App/Screens/Src/ui_FitnessPage.c \
User/GUI_App/Screens/Src/ui_WatchFacePicker.c \
User/GUI_App/Fonts/ui_font_Cuyuan18.c \
User/GUI_App/Fonts/ui_font_Cuyuan20.c \
User/GUI_App/Fonts/ui_font_Cuyuan24.c \
User/GUI_App/Fonts/ui_font_Cuyuan30.c \
User/GUI_App/Fonts/ui_font_Cuyuan38.c \
User/GUI_App/Fonts/ui_font_Cuyuan48.c \
User/GUI_App/Fonts/ui_font_Cuyuan80.c \
User/GUI_App/Fonts/ui_font_Cuyuan100.c \
User/GUI_App/Fonts/nihao.c \
User/GUI_App/Fonts/ui_font_iconfont16.c \
User/GUI_App/Fonts/ui_font_iconfont24.c \
User/GUI_App/Fonts/ui_font_iconfont28.c \
User/GUI_App/Fonts/ui_font_iconfont30.c \
User/GUI_App/Fonts/ui_font_iconfont32.c \
User/GUI_App/Fonts/ui_font_iconfont34.c \
User/GUI_App/Fonts/ui_font_iconfont45.c \
User/GUI_App/Fonts/ui_font_tubiao1.c \
User/GUI_App/Fonts/ui_font_Font2.c \
2/fonts/ui_font_about.c \
2/fonts/ui_font_Font1.c \
User/GUI_App/IMGs/ui_img_compass_needle_png.c \
User/GUI_App/IMGs/ui_img_sb_png.c \
User/GUI_App/IMGs/ui_img_sz_png.c \
User/GUI_App/IMGs/ui_img_zz_png.c \
User/GUI_App/IMGs/ui_img_1337270863.c \
User/GUI_App/IMGs/ui_img_ink_watch_bg_240x280.c \
User/GUI_App/IMGs/ui_img_ink_battery_80_28x14.c \
User/GUI_App/IMGs/ui_img_ink_footsteps_24x24.c \
User/GUI_App/IMGs/ui_img_ink_heart_rate_24x24.c \
User/GUI_App/IMGs/ui_img_ink_center_mountain_circle_44x44.c \
User/GUI_App/IMGs/ui_img_ink_week_bg_54x26.c \
User/GUI_App/IMGs/ui_img_about_product_28x28.c \
User/GUI_App/IMGs/ui_img_about_author_28x28.c \
User/GUI_App/IMGs/ui_img_about_os_28x28.c \
User/GUI_App/IMGs/ui_img_about_ui_28x28.c \
User/GUI_App/IMGs/ui_img_about_chip_28x28.c \
User/GUI_App/IMGs/ui_img_menu_calendar_28x28.c \
User/GUI_App/IMGs/ui_img_menu_computer_28x28.c \
User/GUI_App/IMGs/ui_img_menu_timer_28x28.c \
User/GUI_App/IMGs/ui_img_menu_card_28x28.c \
User/GUI_App/IMGs/ui_img_menu_hr_28x28.c \
User/GUI_App/IMGs/ui_img_menu_spo2_28x28.c \
User/GUI_App/IMGs/ui_img_menu_env_28x28.c \
User/GUI_App/IMGs/ui_img_menu_compass_28x28.c \
User/GUI_App/IMGs/ui_img_menu_game_28x28.c \
User/GUI_App/IMGs/ui_img_menu_setting_28x28.c \
User/GUI_App/IMGs/ui_img_menu_about_28x28.c \
User/GUI_App/IMGs/ui_img_menu_title_72x34.c \
User/GUI_App/IMGs/ui_img_page_calendar_36x36.c \
User/GUI_App/IMGs/ui_img_page_temp_36x36.c \
User/GUI_App/IMGs/ui_img_page_humi_36x36.c \
Middlewares/LVGL/GUI/porting/lv_port_disp.c \
Middlewares/LVGL/GUI/porting/lv_port_indev.c

# LVGL sources - core
C_SOURCES += \
Middlewares/LVGL/GUI/lvgl/src/core/lv_disp.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_event.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_group.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_indev.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_indev_scroll.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj_class.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj_draw.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj_pos.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj_scroll.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj_style.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj_style_gen.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_obj_tree.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_refr.c \
Middlewares/LVGL/GUI/lvgl/src/core/lv_theme.c

# LVGL sources - draw
C_SOURCES += \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw_arc.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw_img.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw_label.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw_line.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw_mask.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw_rect.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_draw_triangle.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_img_buf.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_img_cache.c \
Middlewares/LVGL/GUI/lvgl/src/draw/lv_img_decoder.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_blend.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_dither.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_gradient.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_img.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_line.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_polygon.c \
Middlewares/LVGL/GUI/lvgl/src/draw/sw/lv_draw_sw_rect.c

# LVGL sources - font
C_SOURCES += \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_fmt_txt.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_loader.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_8.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_10.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_12.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_12_subpx.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_14.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_16.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_18.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_20.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_22.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_24.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_26.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_28.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_28_compressed.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_30.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_32.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_34.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_36.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_38.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_40.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_42.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_44.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_46.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_montserrat_48.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_dejavu_16_persian_hebrew.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_simsun_16_cjk.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_unscii_8.c \
Middlewares/LVGL/GUI/lvgl/src/font/lv_font_unscii_16.c

# LVGL sources - hal
C_SOURCES += \
Middlewares/LVGL/GUI/lvgl/src/hal/lv_hal_disp.c \
Middlewares/LVGL/GUI/lvgl/src/hal/lv_hal_indev.c \
Middlewares/LVGL/GUI/lvgl/src/hal/lv_hal_tick.c

# LVGL sources - misc
C_SOURCES += \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_anim.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_anim_timeline.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_area.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_async.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_bidi.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_color.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_fs.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_gc.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_ll.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_log.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_lru.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_math.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_mem.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_printf.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_style.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_style_gen.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_templ.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_timer.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_tlsf.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_txt.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_txt_ap.c \
Middlewares/LVGL/GUI/lvgl/src/misc/lv_utils.c

# LVGL sources - widgets
C_SOURCES += \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_arc.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_bar.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_btn.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_btnmatrix.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_canvas.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_checkbox.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_dropdown.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_img.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_label.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_objx_templ.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_roller.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_slider.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_switch.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_table.c \
Middlewares/LVGL/GUI/lvgl/src/widgets/lv_textarea.c

# LVGL sources - extra
C_SOURCES += \
Middlewares/LVGL/GUI/lvgl/src/extra/lv_extra.c \
Middlewares/LVGL/GUI/lvgl/src/extra/layouts/flex/lv_flex.c \
Middlewares/LVGL/GUI/lvgl/src/extra/layouts/grid/lv_grid.c \
Middlewares/LVGL/GUI/lvgl/src/extra/themes/default/lv_theme_default.c \
Middlewares/LVGL/GUI/lvgl/src/extra/widgets/calendar/lv_calendar.c \
Middlewares/LVGL/GUI/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.c \
Middlewares/LVGL/GUI/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.c \
Middlewares/LVGL/GUI/lvgl/src/extra/widgets/meter/lv_meter.c \
Middlewares/LVGL/GUI/lvgl/src/extra/widgets/spinner/lv_spinner.c

# ASM sources
ASM_SOURCES =  \
MDK-ARM/startup_stm32f411xe_gcc.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F411xE


# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-ICore/Inc \
-IDrivers/STM32F4xx_HAL_Driver/Inc \
-IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy \
-IMiddlewares/Third_Party/FreeRTOS/Source/include \
-IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 \
-IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F \
-IDrivers/CMSIS/Device/ST/STM32F4xx/Include \
-IDrivers/CMSIS/Include \
-ISYSTEM \
-IBSP/LCD \
-IBSP/AHT21 \
-IBSP/BL24C02 \
-IBSP/IIC \
-IBSP/LSM303DLH \
-IBSP/MPU6050 \
-IBSP/OWDG \
-IBSP/POWER \
-IBSP/SPL06_001 \
-IBSP/EM7028 \
-IBSP/TOUCH \
-IBSP/KEY \
-IBSP/KT6328 \
-IMiddlewares/LVGL/GUI \
-IMiddlewares/LVGL/GUI/lvgl/src \
-IMiddlewares/LVGL/GUI/lvgl \
-IMiddlewares/LVGL/GUI/porting \
-IBSP/MPU6050/eMPL \
-IUser \
-IUser/GUI_App/Screens/Inc \
-IUser/Tasks/Inc \
-IUser/Func/Inc \
-IUser/GUI_App \
-IUser/GUI_App/Fonts \
-IUser/GUI_App/IMGs \
-I2 \
-I2/screens \
-I2/components \
-I2/fonts \
-I2/images


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F411CEUx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
