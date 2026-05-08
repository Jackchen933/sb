/**
 * LVGL PC Simulator - main_pc.c
 * 用于在 PC 上模拟显示 OV_Watch 手表界面
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

// LVGL
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"

// 你的 UI 头文件
#include "../User/GUI_App/ui.h"
#include "../User/Func/Inc/PageManager.h"

/*********************
 *      DEFINES
 *********************/
#define LCD_WIDTH   240
#define LCD_HEIGHT  280
#define WINDOW_SCALE 3   // 放大倍数

/*********************
 *      VARIABLES
 *********************/
static SDL_Window *window;
static SDL_Renderer *renderer;
static lv_disp_draw_buf_t draw_buf_dsc;
static lv_color_t draw_buf_1[LCD_WIDTH * LCD_HEIGHT];
static lv_disp_drv_t disp_drv;
static lv_indev_drv_t indev_drv;
static uint8_t framebuffer[LCD_WIDTH * LCD_HEIGHT * 3];  // RGB888 framebuffer

/*********************
 *   FUNCTION DECLARATIONS
 *********************/
static void timer_test(lv_timer_t *timer);

/*********************
 *      MACROS
 *********************/
#define px(r, g, b) (((r) << 16) | ((g) << 8) | (b))

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);
static void SDL_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
static void sdltimer(lv_timer_t *param);
static void mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static uint32_t keycode_to_lv_key(SDL_Keycode key);

/**
 * Convert ARGB8888 to RGB565
 */
static uint16_t argb8888_to_rgb565(uint32_t c)
{
    uint8_t r = (c >> 16) & 0xFF;
    uint8_t g = (c >> 8) & 0xFF;
    uint8_t b = c & 0xFF;
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

/**
 * Render LVGL buffer to SDL window
 */
static void render(lv_timer_t *timer)
{
    lv_disp_t *disp = lv_disp_get_default();
    if (!disp) return;

    lv_disp_flush_ready(&disp_drv);
}

/**
 * SDL flush callback
 */
static void SDL_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (area->x2 < 0 || area->y2 < 0 ||
        area->x1 > LCD_WIDTH - 1 || area->y1 > LCD_HEIGHT - 1) {
        lv_disp_flush_ready(disp_drv);
        return;
    }

    int32_t x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t x2 = area->x2 > LCD_WIDTH - 1 ? LCD_WIDTH - 1 : area->x2;
    int32_t y2 = area->y2 > LCD_HEIGHT - 1 ? LCD_HEIGHT - 1 : area->y2;

    for (int32_t y = y1; y <= y2; y++) {
        for (int32_t x = x1; x <= x2; x++) {
            uint16_t color = lv_color_to_u16(color_p[(y * LCD_WIDTH) + x]);
            uint8_t r = (color >> 11) << 3;
            uint8_t g = ((color >> 5) & 0x3F) << 2;
            uint8_t b = (color & 0x1F) << 3;

            int pixel_x = x * WINDOW_SCALE;
            int pixel_y = y * WINDOW_SCALE;

            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            SDL_Rect rect = {pixel_x, pixel_y, WINDOW_SCALE, WINDOW_SCALE};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    lv_disp_flush_ready(disp_drv);
}

/**
 * Timer to handle LVGL timing
 */
static void sdltimer(lv_timer_t *param)
{
    lv_tick_inc(5);  // 5ms per tick
}

/**
 * Mouse read callback
 */
static void mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    int x, y;
    SDL_GetMouseState(&x, &y);

    // Scale back from window coordinates to LCD coordinates
    data->point.x = x / WINDOW_SCALE;
    data->point.y = y / WINDOW_SCALE;
    data->state = (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1)) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
}

/**
 * Keyboard read callback
 */
static void keyboard_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    SDL_Event event;
    static uint32_t last_key = 0;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            last_key = keycode_to_lv_key(event.key.keysym.sym);
            data->state = LV_INDEV_STATE_PRESSED;
        } else if (event.type == SDL_KEYUP) {
            last_key = keycode_to_lv_key(event.key.keysym.sym);
            data->state = LV_INDEV_STATE_RELEASED;
        } else if (event.type == SDL_QUIT) {
            exit(0);
        }
    }

    data->key = last_key;
}

static uint32_t keycode_to_lv_key(SDL_Keycode key)
{
    switch (key) {
        case SDL_KEY_ESCAPE: return LV_KEY_ESC;
        case SDL_KEY_UP: return LV_KEY_UP;
        case SDL_KEY_DOWN: return LV_KEY_DOWN;
        case SDL_KEY_LEFT: return LV_KEY_LEFT;
        case SDL_KEY_RIGHT: return LV_KEY_RIGHT;
        case SDL_KEY_RETURN: return LV_KEY_ENTER;
        case SDL_KEY_BACKSPACE: return LV_KEY_BACKSPACE;
        default: return 0;
    }
}

/**
 * Initialize SDL and create window
 */
static void window_create(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init error: %s\n", SDL_GetError());
        exit(1);
    }

    window = SDL_CreateWindow(
        "OV_Watch Simulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LCD_WIDTH * WINDOW_SCALE, LCD_HEIGHT * WINDOW_SCALE,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (!window) {
        printf("Window create error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) {
        printf("Renderer create error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetRenderDrawColor(renderer, 245, 245, 245, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

/**
 * Initialize hardware abstraction layer
 */
static void hal_init(void)
{
    window_create();

    /*Initialize LVGL*/
    lv_init();

    /*Initialize display buffer*/
    lv_disp_draw_buf_init(&draw_buf_dsc, draw_buf_1, NULL, LCD_WIDTH * LCD_HEIGHT);

    /*Initialize display driver*/
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = SDL_flush;
    disp_drv.draw_buf = &draw_buf_dsc;
    lv_disp_drv_register(&disp_drv);

    /*Initialize input device driver*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = mouse_read;
    lv_indev_drv_register(&indev_drv);

    /*Initialize keyboard driver*/
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_KEYPAD;
    indev_drv.read_cb = keyboard_read;
    lv_indev_drv_register(&indev_drv);

    /*Create LVGL timer for timing*/
    lv_timer_create(sdltimer, 5, NULL);

    printf("HAL initialized successfully!\n");
}

/**
 * Main entry point
 */
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    printf("==============================================\n");
    printf("  OV_Watch PC Simulator\n");
    printf("  Screen: %dx%d (scaled x%d)\n", LCD_WIDTH, LCD_HEIGHT, WINDOW_SCALE);
    printf("==============================================\n");
    printf("Controls:\n");
    printf("  Mouse Click: Touch screen\n");
    printf("  Arrow Keys: Navigate\n");
    printf("  Enter: Confirm\n");
    printf("  ESC: Back\n");
    printf("==============================================\n");

    hal_init();

    /*Initialize UI - Load WatchFace Page*/
    ui_init();

    /*Main loop*/
    while (1) {
        lv_timer_handler();
        SDL_RenderPresent(renderer);
        SDL_Delay(5);
    }

    return 0;
}
