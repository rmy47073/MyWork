@echo off
echo 正在编译计算器程序...
g++ calculator_gui.cpp -o calculator_gui.exe -lgdi32 -luser32
if %errorlevel% == 0 (
    echo 编译成功！
    echo 正在运行程序...
    start calculator_gui.exe
) else (
    echo 编译失败，请检查错误信息。
    pause
)


