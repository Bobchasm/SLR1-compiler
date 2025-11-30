# Compiler configuration - allow override via environment variables
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

# On Windows, try to find available compiler
ifeq ($(OS),Windows_NT)
    # Check if standard g++ exists
    ifeq ($(shell where g++ 2>nul),)
        # Try MSYS g++
        ifneq ($(wildcard G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe),)
            CXX = G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe
        endif
    endif
endif

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
PROCESS_DIR = process
IR_LIB_DIR = ir_lib

# 源文件定义
LEXER_SRC = $(SRC_DIR)/lexer.cpp
LEXER_OBJ = $(BUILD_DIR)/lexer.o
LEXER_EXE = $(BUILD_DIR)/lexer

PARSER_SRC = $(SRC_DIR)/parser.cpp
PARSER_OBJ = $(BUILD_DIR)/parser.o
PARSER_EXE = $(BUILD_DIR)/parser

# 中间代码生成相关
AST_SRC = $(SRC_DIR)/ast.cpp
AST_OBJ = $(BUILD_DIR)/ast.o

SYMBOL_TABLE_SRC = $(SRC_DIR)/symbol_table.cpp
SYMBOL_TABLE_OBJ = $(BUILD_DIR)/symbol_table.o

SEMANTIC_ANALYZER_SRC = $(SRC_DIR)/semantic_analyzer.cpp
SEMANTIC_ANALYZER_OBJ = $(BUILD_DIR)/semantic_analyzer.o

IR_GENERATOR_SRC = $(SRC_DIR)/ir_generator.cpp
IR_GENERATOR_OBJ = $(BUILD_DIR)/ir_generator.o

# IR库
IR_LIB_SRCS = $(wildcard $(IR_LIB_DIR)/src/*.cpp)
IR_LIB_OBJS = $(patsubst $(IR_LIB_DIR)/src/%.cpp,$(BUILD_DIR)/ir_lib/%.o,$(IR_LIB_SRCS))
IR_LIB = $(BUILD_DIR)/libir.a

# 完整编译器相关
MAIN_SRC = $(SRC_DIR)/main.cpp
MAIN_OBJ = $(BUILD_DIR)/main.o
COMPILER_EXE = $(BUILD_DIR)/compiler.exe
COMPILER_OBJS = $(LEXER_OBJ) $(PARSER_OBJ) $(AST_OBJ) $(SYMBOL_TABLE_OBJ) $(SEMANTIC_ANALYZER_OBJ) $(IR_GENERATOR_OBJ) $(MAIN_OBJ)

.PHONY: all
all: compiler

# 创建目录
$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
	@if not exist "$(BUILD_DIR)\ir_lib" mkdir "$(BUILD_DIR)\ir_lib"

$(PROCESS_DIR):
	@if not exist "$(PROCESS_DIR)" mkdir "$(PROCESS_DIR)"

# ==================== IR库编译 ====================
$(IR_LIB): $(BUILD_DIR) $(IR_LIB_OBJS)
	ar rcs $@ $(IR_LIB_OBJS)
	@echo IR library compiled successfully!

$(BUILD_DIR)/ir_lib/%.o: $(IR_LIB_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS) -Iir_lib/include -c -o $@ $<

# ==================== 词法分析器 ====================
lexer: $(BUILD_DIR) $(PROCESS_DIR) $(LEXER_EXE)

$(LEXER_EXE): $(LEXER_SRC) $(INCLUDE_DIR)/lexer.h
	$(CXX) $(CXXFLAGS) -o $@ $(LEXER_SRC)
	@echo Lexer compiled successfully!

# ==================== 语法分析器 ====================
parser: $(BUILD_DIR) $(PROCESS_DIR) $(PARSER_EXE)

$(PARSER_EXE): $(PARSER_SRC) $(INCLUDE_DIR)/parse.h $(INCLUDE_DIR)/lexer.h $(LEXER_SRC)
	$(CXX) $(CXXFLAGS) -DNO_MAIN -c -o $(LEXER_OBJ) $(LEXER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(PARSER_SRC) $(LEXER_OBJ)
	@echo Parser compiled successfully!

# ==================== 完整编译器（词法+语法+IR生成） ====================
compiler: $(BUILD_DIR) $(PROCESS_DIR) $(IR_LIB) $(COMPILER_EXE)

$(COMPILER_EXE): $(COMPILER_OBJS) $(IR_LIB)
	$(CXX) $(CXXFLAGS) -o $@ $(COMPILER_OBJS) $(IR_LIB)
	@echo Compiler with IR generation compiled successfully!
	@echo =========================================
	@echo Generated: $(COMPILER_EXE)
	@echo Usage: $(COMPILER_EXE) source_file.sy
	@echo =========================================

# 编译各个组件
$(LEXER_OBJ): $(LEXER_SRC) $(INCLUDE_DIR)/lexer.h
	$(CXX) $(CXXFLAGS) -DNO_MAIN -c -o $@ $(LEXER_SRC)

$(PARSER_OBJ): $(PARSER_SRC) $(INCLUDE_DIR)/parse.h $(INCLUDE_DIR)/lexer.h
	$(CXX) $(CXXFLAGS) -DNO_MAIN -c -o $@ $(PARSER_SRC)

$(AST_OBJ): $(AST_SRC) $(INCLUDE_DIR)/ast.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(AST_SRC)

$(SYMBOL_TABLE_OBJ): $(SYMBOL_TABLE_SRC) $(INCLUDE_DIR)/symbol_table.h
	$(CXX) $(CXXFLAGS) -Iir_lib/include -c -o $@ $(SYMBOL_TABLE_SRC)

$(SEMANTIC_ANALYZER_OBJ): $(SEMANTIC_ANALYZER_SRC) $(INCLUDE_DIR)/semantic_analyzer.h $(INCLUDE_DIR)/parse.h
	$(CXX) $(CXXFLAGS) -c -o $@ $(SEMANTIC_ANALYZER_SRC)

$(IR_GENERATOR_OBJ): $(IR_GENERATOR_SRC) $(INCLUDE_DIR)/ir_generator.h $(INCLUDE_DIR)/ast.h
	$(CXX) $(CXXFLAGS) -Iir_lib/include -c -o $@ $(IR_GENERATOR_SRC)

$(MAIN_OBJ): $(MAIN_SRC) $(INCLUDE_DIR)/parse.h $(INCLUDE_DIR)/ast.h $(INCLUDE_DIR)/ir_generator.h $(INCLUDE_DIR)/symbol_table.h
	$(CXX) $(CXXFLAGS) -Iir_lib/include -c -o $@ $(MAIN_SRC)

# ==================== 清理 ====================
.PHONY: clean
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@echo Cleaned build directory.

.PHONY: clean-all
clean-all:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "output" rmdir /s /q "output"
	@echo Cleaned build and output directories.

# ==================== 运行目标 ====================
.PHONY: run-lexer
run-lexer: lexer
	$(LEXER_EXE)

.PHONY: run-parser
run-parser: parser
	$(PARSER_EXE)

.PHONY: run
run: compiler
	$(COMPILER_EXE)

# ==================== 测试目标 ====================
.PHONY: test-lexer
test-lexer: lexer
	$(LEXER_EXE) case/test.sy

.PHONY: test-parser
test-parser: parser
	$(PARSER_EXE) case/test.sy

.PHONY: test
test: compiler
	$(COMPILER_EXE) case/test.sy

# 测试IR生成并保存到文件
.PHONY: test-ir
test-ir: compiler
	@if not exist "output" mkdir "output"
	$(COMPILER_EXE) case/test.sy > output/test_debug.ll 2>&1
	@echo Debug output saved to output/test_debug.ll

.PHONY: test-ir-clean
test-ir-clean: compiler
	@if not exist "output" mkdir "output"
	$(COMPILER_EXE) case/test.sy 2> output/test.ll
	@echo Clean IR code saved to output/test.ll

# 测试IR生成器核心功能
.PHONY: test-ir-generator
test-ir-generator: $(BUILD_DIR) $(IR_LIB)
	$(CXX) $(CXXFLAGS) -o $(BUILD_DIR)/test_ir.exe test_ir.cpp $(IR_GENERATOR_SRC) $(IR_LIB)
	@echo IR generator test compiled successfully!
	@echo =========================================
	@echo Running IR generator test...
	@echo =========================================
	$(BUILD_DIR)/test_ir.exe

# ==================== 调试和信息 ====================
.PHONY: debug
debug: CXXFLAGS += -g -DDEBUG
debug: clean compiler

.PHONY: info
info:
	@echo =========================================
	@echo Compiler Project Information
	@echo =========================================
	@echo Source files:
	@echo   - Lexer:         $(LEXER_SRC)
	@echo   - Parser:        $(PARSER_SRC)
	@echo   - AST:           $(AST_SRC)
	@echo   - Symbol Table:  $(SYMBOL_TABLE_SRC)
	@echo   - IR Generator:  $(IR_GENERATOR_SRC)
	@echo   - Main:          $(MAIN_SRC)
	@echo Build targets:
	@echo   - Lexer exe:     $(LEXER_EXE)
	@echo   - Parser exe:    $(PARSER_EXE)
	@echo   - Compiler exe:  $(COMPILER_EXE)
	@echo   - IR library:    $(IR_LIB)
	@echo =========================================

.PHONY: help
help:
	@echo =========================================
	@echo SysY Compiler - Available Targets
	@echo =========================================
	@echo Build targets:
	@echo   all (default)  - Build the complete compiler
	@echo   compiler       - Build complete compiler (lexer + parser + IR)
	@echo   lexer          - Build only the lexer
	@echo   parser         - Build only the parser
	@echo Run targets:
	@echo   run            - Run compiler with stdin
	@echo   run-lexer      - Run lexer with stdin
	@echo   run-parser     - Run parser with stdin
	@echo Test targets:
	@echo   test           - Run compiler with case/test.sy
	@echo   test-lexer     - Run lexer with case/test.sy
	@echo   test-parser    - Run parser with case/test.sy
	@echo   test-ir        - Generate IR to output/test.ll
	@echo   test-ir-generator - Test IR generator core functionality
	@echo Utility targets:
	@echo   clean          - Remove build files
	@echo   clean-all      - Remove build and output files
	@echo   debug          - Build with debug symbols
	@echo   info           - Show project information
	@echo   help           - Show this help message
	@echo =========================================
