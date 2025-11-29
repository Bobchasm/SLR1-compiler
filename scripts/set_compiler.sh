#!/bin/bash

# Compiler Configuration Script
# Set compiler environment variables for the test scripts

echo "========================================"
echo "Compiler Configuration"
echo "========================================"

# Set default compiler path
if [[ -z "$1" ]]; then
    export CXX="G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe"
else
    export CXX="$1"
fi

# Set default compiler flags
if [[ -z "$2" ]]; then
    export CXXFLAGS="-std=c++11 -Wall -I../include -I../ir_lib/include"
else
    export CXXFLAGS="$2"
fi

echo "Compiler: $CXX"
echo "Flags: $CXXFLAGS"
echo ""
echo "Environment variables set for this session."
echo "You can now run test scripts."
echo ""
echo "Example:"
echo "  ./scripts/test_compiler.sh test.sy"
echo "  ./scripts/test_ir_only.sh"
echo ""

# Keep the shell open (optional)
# bash
