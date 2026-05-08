# OV_Watch Screen Adaptation Notes

这份文件记录当前工程在 `2/screens` 新界面接入时的真实约束和已踩过的坑。
以后如果继续改界面，或者新开对话让助手接手，先让它读这份文件。

## 1. 当前工程的实际页面框架

当前运行时不是纯 SquareLine 工程，而是“混合工程”：

- 页面栈和切页逻辑用的是 `User/Func/Src/PageManager.c`
- 初始化入口用的是 `User/GUI_App/ui.c`
- 这次接入的新页面主要在：
  - `2/screens/ui_HomePage.c`
  - `2/screens/ui_Screen2.c`
  - `2/screens/ui_uiAboutPage.c`

结论：

- 新页面不要再用旧的 `_ui_screen_change(...)`
- 新页面必须接入 `PageManager`
- 进入页面用 `Page_Load(&Page_xxx)`
- 返回页面用 `Page_Back()`

## 2. 新页面接入的标准做法

如果你在 `2/screens` 里新增或更新一个页面，按这个方式接：

1. 在页面 `.c` 里包含：

```c
#include "PageManager.h"
```

2. 给页面定义一个 `Page_t`：

```c
static void ui_xxx_screen_deinit(void)
{
    /* 不要在这里手动 lv_obj_del 当前页 */
}

Page_t Page_xxx = {ui_xxx_screen_init, ui_xxx_screen_deinit, &ui_xxx};
```

3. 进入页面：

```c
Page_Load(&Page_xxx);
```

4. 返回页面：

```c
Page_Back();
```

## 3. 最重要的稳定性经验：deinit 不要手动删当前页

这次“点按钮就重启”的根因之一，就是在页面栈切换时，旧页面在 `deinit` 里提前 `lv_obj_del()` 了。

当前 `PageManager` 的切页过程本身会走：

```c
lv_scr_load_anim(..., true)
```

这里的 `true` 会让 LVGL 在切页完成后删除旧页。

所以：

- `deinit` 里不要手动 `lv_obj_del(当前页面)`
- 否则很容易在切页动画或事件回调里把活跃对象提前释放，导致硬件异常重启
- `deinit` 最多只做“停 timer / 清状态 / 释放非页面对象”

这条规则对下面这些页面已经验证过：

- `Page_Home`
- `Page_Folder2`
- `Page_SquareAbout`

## 4. 手势页面的经验

如果页面需要左右滑动切页：

- 只给父页面绑 `LV_EVENT_GESTURE` 不够
- 子控件会吞手势
- 需要把 `LV_OBJ_FLAG_GESTURE_BUBBLE` 递归加到子对象上

可复用做法：

```c
static void enable_gesture_bubble_recursive(lv_obj_t * obj)
{
    uint32_t i;
    uint32_t child_cnt;

    if(obj == NULL) return;

    child_cnt = lv_obj_get_child_cnt(obj);
    for(i = 0; i < child_cnt; i++)
    {
        lv_obj_t * child = lv_obj_get_child(obj, i);
        lv_obj_add_flag(child, LV_OBJ_FLAG_GESTURE_BUBBLE);
        enable_gesture_bubble_recursive(child);
    }
}
```

然后在页面初始化末尾调用：

```c
enable_gesture_bubble_recursive(ui_xxx);
```

## 5. 下拉菜单的经验

首页顶部下拉菜单这块，不是靠默认滚动做的，而是靠手动拖动 Y 坐标做的。

稳定做法已经在：

- `User/GUI_App/Screens/Src/ui_HomeCompat.c`

注意：

- 不要把 `2/ui_events.c` 重新塞回当前 `Makefile`
- 这不是功能问题，是构建结构问题

## 6. 滚动列表页面的经验

如果页面内容超过一屏，比如“关于页 5 个条目，但屏幕只能先看到 4 个”，要这样做：

### 6.1 滚动容器必须是实际触摸目标

滚动容器不能只设置 `SCROLLABLE`，还必须保证能接到触摸：

```c
lv_obj_add_flag(container,
    LV_OBJ_FLAG_CLICKABLE |
    LV_OBJ_FLAG_SCROLLABLE |
    LV_OBJ_FLAG_SCROLL_ELASTIC |
    LV_OBJ_FLAG_SCROLL_MOMENTUM);
lv_obj_set_scroll_dir(container, LV_DIR_VER);
```

这次“上下拉完全没变化”的直接原因，就是滚动容器被清掉了 `LV_OBJ_FLAG_CLICKABLE`。

### 6.2 视口高度要按真实屏幕算，不要瞎估

当前屏幕分辨率是：

- `LCD_W = 240`
- `LCD_H = 280`

定义位置：

- `BSP/LCD/lcd_init.h`

所以如果一行高度是 `70`，那：

- `280 / 70 = 4`
- 默认应该正好看到 4 行
- 第 5 行要靠向上拖动列表出现

结论：

- 如果要“默认 4 个，第 5 个下拉出现”，滚动容器高度就应该接近 `LV_VER_RES`
- 不要随便改成 `210` 之类的试探值，否则会变成只显示 3 行

### 6.3 行项目本身通常不要再滚动

列表每一行条目建议：

```c
lv_obj_clear_flag(row, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
```

除非这一行本身真的需要点击。

## 7. 关于页这次的实际接法

当前关于页对应：

- 页面对象：`ui_uiAboutPage`
- 页面栈对象：`Page_SquareAbout`
- 进入路径：`ui_HomePage.c` 里的设置按钮
- 返回路径：`ui_uiAboutPage.c` 里的返回按钮 `Page_Back()`

如果后面 SquareLine 又重新生成了 `ui_uiAboutPage.c`，需要优先检查这几件事有没有被覆盖掉：

1. `#include "PageManager.h"`
2. `Page_SquareAbout`
3. 返回按钮是否还是 `Page_Back()`
4. 滚动容器是否还是：
   - `CLICKABLE`
   - `SCROLLABLE`
   - `LV_DIR_VER`

## 8. Makefile 的结构坑

当前 `Makefile` 有一个重要约束：

- 目标文件名用的是 `$(notdir ...)`

这意味着如果两个源文件同名，即使目录不同，也会编译成同一个 `.o` 名字，互相覆盖。

所以：

- 不要随便把重复 basename 的源文件一起加进 `Makefile`
- 例如 `ui.c`、`ui_helpers.c`、`ui_events.c` 这种名字，最容易出冲突

这类问题不是代码逻辑错误，而是构建系统覆盖了错误的源文件。

## 9. 以后重新适配页面的推荐流程

1. 先改 SquareLine 生成的 `2/screens/*.c`
2. 再手工补 `PageManager` 接口，不要直接信生成代码
3. 检查 `deinit` 是否安全
4. 如果有左右滑动，补 `GESTURE_BUBBLE`
5. 如果有纵向列表，检查滚动容器是否同时具备：
   - `CLICKABLE`
   - `SCROLLABLE`
   - `LV_DIR_VER`
6. 编译：

```powershell
make -j4
```

7. 烧录：

```powershell
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/OV_Watch.bin 0x08000000 verify reset exit"
```

## 10. 如果后面再次出现“点一下就重启”

优先按这个顺序查，不要先怀疑界面长相：

1. 有没有又用回 `_ui_screen_change(...)`
2. `Page_t` 的 `deinit` 是否又手动 `lv_obj_del()`
3. 事件回调里是否切页后还继续访问旧对象
4. `Makefile` 是否因为重名源文件编错了 `.o`
5. 如果以上都排除，再上 HardFault 定位

## 11. 给新对话的使用方式

如果以后新开一个对话，直接把这句话发给它：

```text
先读工程根目录的 SCREEN_ADAPTATION_NOTES.md，再帮我适配 2/screens 下的新页面。
```

这样效率最高。
