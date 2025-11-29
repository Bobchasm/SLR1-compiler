@echo off
REM Compiler Principles Final Project - Complete Compiler Test Script
REM Run complete compiler tests using Makefile

echo ========================================
echo Compiler Principles Final Project
echo Complete Compiler Test Script
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
            echo Example: set CXX=G:\444SoftWare\MSYS-251020\ucrt64\bin\g++.exe
            pause
            exit /b 1
        )
    )
)

REM Check for CXXFLAGS
if defined CXXFLAGS (
    echo [INFO] Using user-specified CXXFLAGS: %CXXFLAGS%
) else (
    set CXXFLAGS=-std=c++11 -Wall "-Iinclude" "-Iir_lib\include"
)

echo [INFO] Compiler: %CXX%
echo [INFO] Flags: %CXXFLAGS%

REM Parse command line arguments for test mode
if "%1"=="lexer" goto :test_lexer
if "%1"=="parser" goto :test_parser
if "%1"=="ir" goto :test_ir
if "%1"=="ir-generator" goto :test_ir_generator
if "%1"=="full" goto :test_full
if "%1"=="clean" goto :clean
if "%1"=="-c" goto :clean
if "%1"=="" goto :test_full

REM If no arguments, show help
goto :help

:test_lexer
echo [INFO] Testing Lexer...
echo [STEP 1/2] Building lexer...
if not exist "build" mkdir build
"%CXX%" %CXXFLAGS% -o build/lexer.exe src/lexer.cpp
if errorlevel 1 (
    echo [ERROR] Lexer build failed!
    goto :end
)
echo [STEP 2/2] Running lexer test...
build\lexer.exe case\test.sy
goto :end

:test_parser
echo [INFO] Testing Parser...
echo [STEP 1/2] Building parser...
if not exist "build" mkdir build
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/lexer.o src/lexer.cpp
"%CXX%" %CXXFLAGS% -o build/parser.exe src/parser.cpp build/lexer.o
if errorlevel 1 (
    echo [ERROR] Parser build failed!
    goto :end
)
echo [STEP 2/2] Running parser test...
build\parser.exe case\test.sy
goto :end

:test_ir
echo [INFO] Testing IR Generation...
echo [STEP 1/2] Building compiler...
if not exist "build" mkdir build
if not exist "build\ir_lib" mkdir build\ir_lib
REM Build IR library
for %%f in (ir_lib\src\*.cpp) do (
    "%CXX%" %CXXFLAGS% -c -o build\ir_lib\%%~nf.o %%f
)
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/lexer.o src/lexer.cpp
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/parser.o src/parser.cpp
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/ast.o src/ast.cpp
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/symbol_table.o src/symbol_table.cpp
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/ir_generator.o src/ir_generator.cpp
"%CXX%" %CXXFLAGS% -o build/compiler.exe build/lexer.o build/parser.o build/ast.o build/symbol_table.o build/ir_generator.o build/ir_lib/*.o src/main.cpp
if errorlevel 1 (
    echo [ERROR] Compiler build failed!
    goto :end
)
echo [STEP 2/2] Running IR generation test...
if not exist "output" mkdir output
build\compiler.exe case\test.sy > output\test.ll 2>&1
echo IR code saved to output\test.ll
goto :end

:test_ir_generator
echo [INFO] Testing IR Generator Core...
echo [STEP 1/2] Building IR generator test...
if not exist "build" mkdir build
if not exist "build\ir_lib" mkdir build\ir_lib
REM Build IR library
for %%f in (ir_lib\src\*.cpp) do (
    "%CXX%" %CXXFLAGS% -c -o build\ir_lib\%%~nf.o %%f
)
"%CXX%" %CXXFLAGS% -o build/test_ir.exe test_ir.cpp src/ir_generator.cpp build\ir_lib/*.o
if errorlevel 1 (
    echo [ERROR] IR generator build failed!
    goto :end
)
echo [STEP 2/2] Running IR generator core test...
build\test_ir.exe
goto :end

:test_full
echo [INFO] Running Full Compiler Test Suite...
echo [STEP 1/4] Building components...
if not exist "build" mkdir build
if not exist "build\ir_lib" mkdir build\ir_lib

echo Building IR library...
for %%f in (ir_lib\src\*.cpp) do (
    echo   Compiling %%~nf.cpp
    "%CXX%" %CXXFLAGS% -c -o build\ir_lib\%%~nf.o %%f
    if errorlevel 1 goto :build_error
)

echo Building lexer...
"%CXX%" %CXXFLAGS% -o build/lexer.exe src/lexer.cpp
if errorlevel 1 goto :build_error

echo Building parser...
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/lexer.o src/lexer.cpp
"%CXX%" %CXXFLAGS% -o build/parser.exe src/parser.cpp build/lexer.o
if errorlevel 1 goto :build_error

echo Building compiler...
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/parser.o src/parser.cpp
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/ast.o src/ast.cpp
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/symbol_table.o src/symbol_table.cpp
"%CXX%" %CXXFLAGS% -DNO_MAIN -c -o build/ir_generator.o src/ir_generator.cpp
"%CXX%" %CXXFLAGS% -o build/compiler.exe build/lexer.o build/parser.o build/ast.o build/symbol_table.o build/ir_generator.o build/ir_lib/*.o src/main.cpp
if errorlevel 1 goto :build_error

echo [STEP 2/4] Testing Lexer...
build\lexer.exe case\test.sy
if errorlevel 1 (
    echo [ERROR] Lexer test failed!
    goto :end
)

echo [STEP 3/4] Testing Parser...
build\parser.exe case\test.sy >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Parser test failed!
    goto :end
)
REM Check if parser generated expected output files
if exist "case\test_parse_tree.md" if exist "case\test_semantic_tree.txt" (
    echo Parser test completed successfully
) else (
    echo [ERROR] Parser test failed - output files not generated!
    goto :end
)

echo [STEP 4/4] Testing IR Generation...
if not exist "output" mkdir output
build\compiler.exe case\test.sy > output\test.ll 2>&1
echo IR code saved to output\test.ll
if errorlevel 1 (
    echo [ERROR] IR generation test failed!
    goto :end
)

echo.
echo ========================================
echo [SUCCESS] All tests passed!
echo ========================================
goto :end

:build_error
echo [ERROR] Build failed!
goto :end

:clean
echo [INFO] Cleaning build files...
if exist "build" rmdir /s /q "build"
if exist "output" rmdir /s /q "output"
echo [OK] Clean completed
goto :end

:help
echo.
echo Usage:
echo   run_tests.bat [mode]
echo.
echo Modes:
echo   (no args)        Run full test suite (lexer + parser + IR)
echo   lexer           Test only lexical analyzer
echo   parser          Test only syntax analyzer
echo   ir              Test only IR generation
echo   ir-generator    Test IR generator core functionality
echo   clean           Clean build files
echo.
echo Examples:
echo   run_tests.bat           # Run complete test suite
echo   run_tests.bat lexer     # Test only lexer
echo   run_tests.bat parser    # Test only parser
echo   run_tests.bat ir        # Test only IR generation
echo   run_tests.bat clean     # Clean build files
echo.
echo This script uses the Makefile to build and test components.
echo Make sure you have make and a C++ compiler installed.
echo.

:end
pause
