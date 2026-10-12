#include "../Inc/PageManager.h"
#include "../../GUI_App/Screens/Inc/ui_HomePage.h"
#include "../../GUI_App/Screens/Inc/ui_MenuPage.h"
#include "../../GUI_App/Screens/Inc/ui_WatchFacePage.h"
#include "../../GUI_App/Screens/Inc/ui_QuickAppsPage.h"
#include "../../GUI_App/Screens/Inc/ui_FitnessPage.h"
#include "../../GUI_App/Screens/Inc/ui_WatchFacePicker.h"

PageStack_t PageStack;

static void page_stack_init(PageStack_t *stack)
{
    stack->top = 0;
}

static uint8_t page_stack_push(PageStack_t *stack, Page_t *page)
{
    if(stack->top >= MAX_DEPTH) return 1;
    stack->pages[stack->top++] = page;
    return 0;
}

static uint8_t page_stack_pop(PageStack_t *stack)
{
    if(stack->top == 0) return 1;
    stack->pages[--stack->top]->deinit();
    return 0;
}

static uint8_t page_stack_is_empty(const PageStack_t *stack)
{
    return stack->top == 0;
}

static Page_t *get_top_page(PageStack_t *stack)
{
    if(stack->top == 0) return NULL;
    return stack->pages[stack->top - 1];
}

Page_t *Page_Get_NowPage(void)
{
    return get_top_page(&PageStack);
}

void Page_Back(void)
{
    Page_t *previous_page;

    if(page_stack_is_empty(&PageStack)) return;

    page_stack_pop(&PageStack);

    if(page_stack_is_empty(&PageStack)) {
        page_stack_push(&PageStack, &Page_Home);
        page_stack_push(&PageStack, &Page_Menu);
        Page_Menu.init();
        lv_scr_load_anim(*Page_Menu.page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
        return;
    }

    previous_page = PageStack.pages[PageStack.top - 1];
    previous_page->init();
    lv_scr_load_anim(*previous_page->page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

void Page_Back_Bottom(void)
{
    if(page_stack_is_empty(&PageStack)) return;

    while(PageStack.top > 1) {
        page_stack_pop(&PageStack);
    }

    PageStack.pages[PageStack.top - 1]->init();
    lv_scr_load_anim(*PageStack.pages[PageStack.top - 1]->page_obj,
                     LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

void Page_Load(Page_t *newPage)
{
    if(PageStack.top >= MAX_DEPTH - 1) return;

    if(PageStack.top > 0) {
        PageStack.pages[PageStack.top - 1]->deinit();
    }

    page_stack_push(&PageStack, newPage);
    newPage->init();
    lv_scr_load_anim(*newPage->page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

void Pages_init(void)
{
    page_stack_init(&PageStack);
    page_stack_push(&PageStack, &Page_Home);
    Page_Home.init();
    lv_disp_load_scr(*Page_Home.page_obj);
}
