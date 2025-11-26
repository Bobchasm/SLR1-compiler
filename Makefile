CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
PROCESS_DIR = process

LEXER_SRC = $(SRC_DIR)/lexer.cpp
LEXER_OBJ = $(BUILD_DIR)/lexer.o
LEXER_EXE = $(BUILD_DIR)/lexer

PARSER_SRC = $(SRC_DIR)/parser.cpp
PARSER_OBJ = $(BUILD_DIR)/parser.o
PARSER_EXE = $(BUILD_DIR)/parser

COMPILER_EXE = $(BUILD_DIR)/compiler
COMPILER_OBJS = $(LEXER_OBJ) $(PARSER_OBJ)

.PHONY: all
all: compiler

$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

$(PROCESS_DIR):
	@if not exist "$(PROCESS_DIR)" mkdir "$(PROCESS_DIR)"

# 编译词法分析器（独立可执行）
lexer: $(BUILD_DIR) $(PROCESS_DIR) $(LEXER_EXE)

$(LEXER_EXE): $(LEXER_SRC) $(INCLUDE_DIR)/lexer.h
	$(CXX) $(CXXFLAGS) -o $@ $(LEXER_SRC)
	@echo Lexer compiled successfully!

# 编译语法分析器（独立可执行）
parser: $(BUILD_DIR) $(PROCESS_DIR) $(PARSER_EXE)

$(PARSER_EXE): $(PARSER_SRC) $(INCLUDE_DIR)/parse.h $(INCLUDE_DIR)/lexer.h
	$(CXX) $(CXXFLAGS) -DNO_MAIN -o $@ $(PARSER_SRC) $(SRC_DIR)/lexer.cpp
	@echo Parser compiled successfully!

# 编译完整编译器（词法+语法）
compiler: $(BUILD_DIR) $(PROCESS_DIR) $(COMPILER_EXE)

$(COMPILER_EXE): $(PARSER_SRC) $(LEXER_SRC) $(INCLUDE_DIR)/parse.h $(INCLUDE_DIR)/lexer.h
	$(CXX) $(CXXFLAGS) -DNO_MAIN -o $@ $(PARSER_SRC) $(LEXER_SRC)
	@echo Compiler compiled successfully!


.PHONY: clean
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(PROCESS_DIR)" rmdir /s /q "$(PROCESS_DIR)"
	@echo Cleaned build and process directories.

.PHONY: run-lexer
run-lexer: lexer
	$(LEXER_EXE)

.PHONY: run-parser
run-parser: parser
	$(PARSER_EXE)

.PHONY: run
run: compiler
	$(COMPILER_EXE)

.PHONY: test-lexer
test-lexer: lexer
	$(LEXER_EXE) case/test.sy

.PHONY: test-parser
test-parser: parser
	$(PARSER_EXE) case/test.sy

.PHONY: test
test: compiler
	$(COMPILER_EXE) case/test.sy


.PHONY: help
help:
	@echo Available targets:
	@echo   all (default)  - Build the complete compiler
	@echo   compiler       - Build the complete compiler (lexer + parser)
	@echo   lexer          - Build only the lexer
	@echo   parser         - Build only the parser
	@echo   clean          - Remove build files
	@echo   run            - Run compiler with standard input
	@echo   run-lexer      - Run lexer with standard input
	@echo   run-parser     - Run parser with standard input
	@echo   test           - Run compiler with case/test.sy
	@echo   test-lexer     - Run lexer with case/test.sy
	@echo   test-parser    - Run parser with case/test.sy
	@echo   help           - Show this help message