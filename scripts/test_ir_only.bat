@echo off
REM IR Generator Test Script - Windows Batch Version

echo ========================================
echo IR Generator Test Script
echo ========================================

REM Check for compiler - allow user to specify via environment variable
if defined CXX (
    echo [INFO] Using user-specified compiler: %CXX%
) else (
    REM Try standard g++ first
    where g++ >nul 2>&1
    if not errorlevel 1 (
        set CXX=g++
    ) else (
        REM Try MSYS g++
        if exist "G:\444SoftWare\MSYS-251020\ucrt64\bin\g++.exe" (
            set CXX=G:\444SoftWare\MSYS-251020\ucrt64\bin\g++.exe
        ) else (
            echo [ERROR] No C++ compiler found!
            echo Please install MinGW or MSYS2 with g++
            echo Or set CXX environment variable to specify compiler path
            pause
            exit /b 1
        )
    )
)

echo [INFO] Using compiler: %CXX%

REM Check for CXXFLAGS
if defined CXXFLAGS (
    echo [INFO] Using user-specified CXXFLAGS: %CXXFLAGS%
) else (
    set CXXFLAGS=-std=c++11 -Wall "-I..\include" "-I..\ir_lib\include"
)

REM Create output directory
if not exist "..\test_output" mkdir "..\test_output"

echo [INFO] Building IR generator test...

REM Compile
echo [DEBUG] Compiling with: %CXX%
%CXX% -std=c++11 -Wall "-Iinclude" "-Iir_lib\include" "test_ir.cpp" "src\ir_generator.cpp" "ir_lib\BasicBlock.o" "ir_lib\Constant.o" "ir_lib\Function.o" "ir_lib\GlobalVariable.o" "ir_lib\Instruction.o" "ir_lib\IRprinter.o" "ir_lib\Module.o" "ir_lib\Type.o" "ir_lib\User.o" "ir_lib\Value.o" -o scripts\test_ir.exe

if errorlevel 1 (
    echo [ERROR] Compilation failed!
    echo Make sure ir_lib/*.o files exist and compiler is properly installed
    pause
    exit /b 1
)

echo [OK] Compilation successful
echo.
echo Running IR generator test...
echo ========================================

REM Run test
scripts\test_ir.exe

if errorlevel 1 (
    echo.
    echo ========================================
    echo [ERROR] IR generator test failed!
    pause
    exit /b 1
) else (
    echo.
    echo ========================================
    echo [OK] IR generator test passed!
    echo Output saved to: ..\test_output\manual_test_output.ll
    echo.
    pause
)
