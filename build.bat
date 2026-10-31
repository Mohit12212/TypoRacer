@echo off
title Building TypoRacer Pro...
echo ======================================================
echo           Building TypoRacer Pro (C++ GUI)
echo ======================================================

g++ -o "TypoRacer.exe" src/main.cpp src/TypingTest.cpp -mwindows -lgdi32 -lcomctl32 -luser32 -lwinmm -std=c++14 -O2

if %errorlevel% == 0 (
    echo.
    echo [SUCCESS] TypoRacer.exe built successfully!
    echo Launching TypoRacer Pro...
    start TypoRacer.exe
) else (
    echo.
    echo [ERROR] Build failed! Ensure MinGW g++ is installed and on your PATH.
    pause
)
