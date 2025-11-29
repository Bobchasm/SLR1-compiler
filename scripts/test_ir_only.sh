#!/bin/bash

# IR Generator Test Script - Simplified version
# Only tests the IR generation functionality

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Compiler configuration - allow user to specify via environment variables
if [[ -n "$CXX" ]]; then
    echo -e "${YELLOW}[INFO] Using user-specified compiler: $CXX${NC}"
elif command -v g++ &> /dev/null; then
    CXX="g++"
elif command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    CXX="x86_64-w64-mingw32-g++"
elif [[ -x "/mingw64/bin/g++.exe" ]]; then
    CXX="/mingw64/bin/g++.exe"
elif [[ -x "/usr/bin/g++" ]]; then
    CXX="/usr/bin/g++"
elif [[ -x "G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe" ]]; then
    CXX="G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe"
else
    echo -e "${RED}[ERROR] No C++ compiler found!${NC}"
    echo -e "${YELLOW}Or set CXX environment variable to specify compiler path${NC}"
    exit 1
fi

# Compiler flags - allow user to specify via environment variable
if [[ -z "$CXXFLAGS" ]]; then
    CXXFLAGS="-std=c++11 -Wall -I../include -I../ir_lib/include"
else
    echo -e "${YELLOW}[INFO] Using user-specified CXXFLAGS: $CXXFLAGS${NC}"
fi

echo -e "${CYAN}========================================${NC}"
echo -e "${CYAN}IR Generator Test Script${NC}"
echo -e "${CYAN}========================================${NC}"
echo -e "${YELLOW}[INFO] Using compiler: $CXX${NC}"

# Create output directory
mkdir -p "$PROJECT_ROOT/test_output"

echo -e "${CYAN}Building IR generator test...${NC}"

# Compile test
if "$CXX" -std=c++11 -Wall -I../include -I../ir_lib/include \
    ../test_ir.cpp ../src/ir_generator.cpp \
    ../ir_lib/BasicBlock.o ../ir_lib/Constant.o ../ir_lib/Function.o \
    ../ir_lib/GlobalVariable.o ../ir_lib/Instruction.o ../ir_lib/IRprinter.o \
    ../ir_lib/Module.o ../ir_lib/Type.o ../ir_lib/User.o ../ir_lib/Value.o \
    -o test_ir.exe 2>/dev/null; then

    echo -e "${GREEN}[OK] Compilation successful${NC}"

    echo -e "${CYAN}Running IR generator test...${NC}"
    echo -e "${CYAN}========================================${NC}"

    # Run test
    if ./test_ir.exe; then
        echo -e "${CYAN}========================================${NC}"
        echo -e "${GREEN}[OK] IR generator test passed!${NC}"
        echo -e "${BLUE}Output saved to: test_output/manual_test_output.ll${NC}"
        exit 0
    else
        echo -e "${RED}[ERROR] IR generator test failed!${NC}"
        exit 1
    fi
else
    echo -e "${RED}[ERROR] Compilation failed!${NC}"
    echo -e "${YELLOW}This might be due to missing dependencies.${NC}"
    echo -e "${YELLOW}Make sure ir_lib/*.o files exist.${NC}"
    exit 1
fi
