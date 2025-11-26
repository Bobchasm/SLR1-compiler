CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
PROCESS_DIR = process

LEXER_SRC = $(SRC_DIR)/lexer.cpp
LEXER_OBJ = $(BUILD_DIR)/lexer.o
LEXER_EXE = $(BUILD_DIR)/lexer

.PHONY: all
all: lexer

$(BUILD_DIR):
	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

$(PROCESS_DIR):
	@if not exist "$(PROCESS_DIR)" mkdir "$(PROCESS_DIR)"

# 词法分析器
lexer: $(BUILD_DIR) $(PROCESS_DIR) $(LEXER_EXE)

$(LEXER_EXE): $(LEXER_SRC) $(INCLUDE_DIR)/lexer.h
	$(CXX) $(CXXFLAGS) -o $@ $(LEXER_SRC)
	@echo Lexer compiled successfully!


.PHONY: clean
clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(PROCESS_DIR)" rmdir /s /q "$(PROCESS_DIR)"
	@echo Cleaned build and process directories.


.PHONY: run
run: lexer
	$(LEXER_EXE)


.PHONY: test
test: lexer
	$(LEXER_EXE) case/test.sy


.PHONY: help
help:
	@echo Available targets:
	@echo   all (default) - Build the lexer
	@echo   lexer         - Build the lexer executable
	@echo   clean         - Remove build files
	@echo   run           - Run lexer with standard input
	@echo   test          - Run lexer with case/test.sy
	@echo   help          - Show this help message