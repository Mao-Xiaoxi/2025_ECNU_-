@echo off
setlocal enabledelayedexpansion

set "PROGRAM=build\bin\main.exe"

echo ============================================
echo   批量测试工具 v3.0 (修复路径问题)
echo ============================================
echo.

echo 检查data目录下的文件...
dir data
echo.

echo 测试模式1: 词法分析
for %%i in (1 2 3 4) do (
    echo [测试 %%i] Test%%i.txt
    REM 去掉路径的引号
    (
        echo 1
        echo n
        echo data\Test%%i.txt
    ) | "%PROGRAM%"
    echo ======================
    echo.
)

echo 测试模式2: LL1语法分析
for %%i in (5 6 7 8) do (
    echo [测试 %%i] Test%%i.txt
    (
        echo 2
        echo n
        echo data\Test%%i.txt
    ) | "%PROGRAM%"
    echo ======================
    echo.
)

echo 测试模式3: LR语法分析
for %%i in (9 10 11 12) do (
    echo [测试 %%i] Test%%i.txt
    (
        echo 3
        echo n
        echo data\Test%%i.txt
    ) | "%PROGRAM%"
    echo ======================
    echo.
)

echo 测试模式4: 语义分析
for %%i in (13 14 15 16) do (
    echo [测试 %%i] Test%%i.txt
    (
        echo 4
        echo n
        echo data\Test%%i.txt
    ) | "%PROGRAM%"
    echo ======================
    echo.
)

echo ============================================
echo   所有测试完成
echo ============================================
pause