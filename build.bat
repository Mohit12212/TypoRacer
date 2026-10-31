@echo off
setlocal
cd /d "%~dp0"
title Building TypoRacer

echo ==========================================
echo          Building TypoRacer (C++ GUI)
echo ==========================================

where g++ >nul 2>nul
if errorlevel 1 (
    echo [ERROR] g++ was not found. Install MinGW-w64 and add its bin folder to PATH.
    pause
    exit /b 1
)

g++ -std=c++14 -O2 -mwindows -o "TypoRacer.exe" "src/main.cpp" "src/TypingTest.cpp" -lgdi32 -lcomctl32 -luser32 -lwinmm
if errorlevel 1 (
    echo.
    echo [ERROR] Build failed. Review the compiler messages above.
    pause
    exit /b 1
)

echo.
echo [SUCCESS] TypoRacer.exe built successfully.
start "" "%~dp0TypoRacer.exe"
endlocal
