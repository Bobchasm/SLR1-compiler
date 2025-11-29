# Compiler Principles Final Project - Compiler Test Guide

## Overview

This project includes a complete compiler test script that automatically tests the entire process from lexical analysis to intermediate code generation.

## Test Scripts

### Quick Start

#### Method 1: Using Batch Script (Recommended for Windows)

```bash
# Run complete test suite
scripts\run_tests.bat

# Test single case
scripts\run_tests.bat test.sy

# Verbose output mode
scripts\run_tests.bat -v

# Clean build files
scripts\run_tests.bat -c
```

#### Method 2: Direct Bash Script (Linux/Mac)

```bash
# Run complete test suite
./scripts/test_compiler.sh

# Test single case
./scripts/test_compiler.sh test.sy

# Verbose output mode
./scripts/test_compiler.sh -v

# Clean build files
./scripts/test_compiler.sh -c
```

#### Method 3: IR Generator Test (Simplest)

```bash
# Quick IR generator test
./test_ir_only.bat
```

## Test Process

The test script executes in the following order:

1. **Environment Initialization** - Create necessary directories
2. **IR Library Build** - Compile LLVM IR related library files
3. **Component Build** - Compile lexer, parser, and full compiler
4. **Lexical Analysis Tests** - Run lexical analysis on each test case
5. **Syntax Analysis Tests** - Run syntax analysis on each test case
6. **Full Compilation Tests** - Run complete compilation (including IR generation) on each test case
7. **IR Generator Tests** - Run IR generator core functionality tests
8. **Report Generation** - Generate detailed test reports

## Test Cases

Test cases are located in the `case/` directory:

- `test.sy` - Basic test (global variables + functions)
- `test1.sy` - Simple function test
- `test2.sy` - Variable declaration test
- `test3.sy` - Expression test
- `test4.sy` - Complex expression test
- `simple_test.sy` - Simplified test
- `full_test.sy` - Full functionality test
- `debug_test.sy` - Debug test

## Output Results

Test outputs are saved in the `test_output/` directory:

- `{testcase}_lexer.txt` - Lexical analysis results
- `{testcase}_parser.txt` - Syntax analysis results
- `{testcase}.ll` - LLVM IR intermediate code
- `test_report.txt` - Complete test report

## Test Report

After testing completes, a detailed test report is generated containing:

- Results for each testing phase
- Pass/fail statistics
- Overall success rate

## Compiler Build and Test Options

The project supports multiple build and test modes using Makefile:

### Build Targets

```bash
# Build individual components
make lexer          # Build lexical analyzer
make parser         # Build syntax analyzer
make compiler       # Build complete compiler (lexer + parser + IR)
make all           # Build everything (default)

# Clean build files
make clean         # Remove build directory
make clean-all     # Remove build and output directories
```

### Test Targets

```bash
# Test individual components
make test-lexer           # Test lexical analyzer
make test-parser          # Test syntax analyzer
make test                 # Test complete compiler
make test-ir             # Generate IR to output/test.ll
make test-ir-generator   # Test IR generator core functionality

# Run specific tests
make test-lexer          # Test lexer with case/test.sy
make test-parser         # Test parser with case/test.sy
make test                # Test compiler with case/test.sy
```

### Batch Test Script

Use the batch script for easy testing:

```bash
# Run complete test suite (lexer + parser + IR)
scripts\run_tests.bat

# Test individual components
scripts\run_tests.bat lexer     # Test only lexer
scripts\run_tests.bat parser    # Test only parser
scripts\run_tests.bat ir        # Test only IR generation

# Clean build files
scripts\run_tests.bat clean
```

### Custom Compiler Configuration

You can specify custom compiler and flags by setting environment variables:

#### Windows

```bash
# Set compiler and flags
set CXX=G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe
set CXXFLAGS=-std=c++11 -Wall -Iinclude -Iir_lib/include

# Or use the configuration script
scripts\set_compiler.bat "G:/444SoftWare/MSYS-251020\ucrt64/bin/g++.exe" "-std=c++11 -Wall -Iinclude -Iir_lib/include"

# Then run tests
scripts\run_tests.bat
```

#### Linux/Mac

```bash
# Set compiler and flags
export CXX="g++"
export CXXFLAGS="-std=c++11 -Wall -I../include -I../ir_lib/include"

# Or use the configuration script
source scripts/set_compiler.sh "g++" "-std=c++11 -Wall -I../include -I../ir_lib/include"

# Then run tests
./scripts/test_compiler.sh
```

## Manual Testing Options

If you want to test individual components manually:

### Test Lexer

```bash
make test-lexer
# or
./build/lexer.exe case/test.sy
```

### Test Parser

```bash
make test-parser
# or
./build/parser.exe case/test.sy
```

### Test Full Compiler

```bash
make test
# or
./build/compiler.exe case/test.sy
```

### Test IR Generator (Recommended)

```bash
# Quick test
./scripts/test_ir_only.bat

# Or manual compilation
%CXX% %CXXFLAGS% test_ir.cpp src/ir_generator.cpp ir_lib/*.o -o test_ir.exe
./test_ir.exe
```

## Troubleshooting

### Common Issues

1. **Compilation Failure**
   - Ensure GCC/G++ compiler is installed and available
   - Check that `.o` files exist in `ir_lib/` directory

2. **Bash Not Found**
   - Install Git Bash or MSYS2
   - Download Git from: https://git-scm.com/downloads

3. **Permission Issues**
   - Run command prompt as administrator
   - Or ensure directory permissions are correct in non-admin mode

4. **Missing Dependencies**
   - Ensure project directory structure is complete
   - Check `include/` and `ir_lib/include/` directories

### Debug Mode

Use `-v` parameter for detailed output:

```bash
run_tests.bat -v
```

## 项目结构

```
compiler/
├── src/                    # 源代码
├── include/               # 头文件
├── ir_lib/               # LLVM IR库
├── case/                 # 测试用例
├── build/                # 构建输出（自动生成）
├── test_output/          # 测试输出（自动生成）
├── test_compiler.ps1     # PowerShell测试脚本
├── run_tests.bat         # 批处理脚本
└── Makefile             # Make构建脚本
```

## 成功标志

测试成功时，你应该看到：

- ✅ 所有组件编译成功
- ✅ 各阶段测试通过率高（>80%）
- ✅ 生成正确的LLVM IR代码
- ✅ 包含函数定义、变量声明等
- ✅ 测试报告显示总体成功

祝测试顺利！🎯
