@echo off
REM Compiler Configuration Script
REM Set compiler environment variables for the test scripts

echo ========================================
echo Compiler Configuration
echo ========================================

REM Set default compiler path
if "%~1"=="" (
    set CXX=G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe
) else (
    set CXX=%~1
)

REM Set default compiler flags
if "%~2"=="" (
    set CXXFLAGS=-std=c++11 -Wall -Iinclude -Iir_lib/include
) else (
    set CXXFLAGS=%~2
)

echo Compiler: %CXX%
echo Flags: %CXXFLAGS%
echo.
echo Environment variables set for this session.
echo You can now run test scripts.
echo.
echo Example:
echo   scripts\run_tests.bat test.sy
echo   scripts\test_ir_only.bat
echo.

REM Keep the command prompt open
pause
