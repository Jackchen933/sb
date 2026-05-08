@echo off
chcp 65001 >nul
echo ==============================================
echo   OV_Watch PC 模拟器构建脚本
echo ==============================================
echo.

:: 检查 SDL2
where sdl2-config >nul 2>&1
if %errorlevel% neq 0 (
    echo [错误] 未找到 SDL2!
    echo.
    echo 请先安装 SDL2 开发库:
    echo   1. 访问 https://github.com/libsdl-org/SDL/releases
    echo   2. 下载 SDL2-devel-2.x.x-mingw.tar.gz
    echo   3. 解压到 C:\SDL2 或其他位置
    echo   4. 将 SDL2 的 bin 目录添加到 PATH
    echo.
    echo 或使用 vcpkg:
    echo   vcpkg install sdl2:x64-windows
    echo.
    pause
    exit /b 1
)

echo [1/2] 正在编译...
gcc main_pc.c -o OV_Watch_Simulator.exe -Wall -O2 -I../Middlewares/LVGL/GUI -I../Middlewares/LVGL/GUI/lvgl/src -I../Middlewares/LVGL/GUI/lvgl/examples -I../Middlewares/LVGL/GUI/lvgl/demos -I../User -I../User/GUI_App -I../User/GUI_App/Screens/Inc -I../User/Func/Inc -I../User/GUI_App/Fonts -DLV_CONF_INCLUDE_SIMPLE -lm %SDL2_CFLAGS% %SDL2_LIBS%

if %errorlevel% neq 0 (
    echo [错误] 编译失败!
    pause
    exit /b 1
)

echo [2/2] 编译成功!
echo.
echo 生成文件: OV_Watch_Simulator.exe
echo.
echo 运行模拟器请双击 OV_Watch_Simulator.exe
echo.
pause
