#!/bin/bash

# Compiler Principles Final Project - Complete Compiler Test Script
# Test the complete process from lexical analysis to intermediate code generation

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"
OUTPUT_DIR="$PROJECT_ROOT/test_output"
CASE_DIR="$PROJECT_ROOT/case"

# Compiler configuration - allow user to specify via environment variables
if [[ -n "$CXX" ]]; then
    echo "[INFO] Using user-specified compiler: $CXX"
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
    echo "[ERROR] No suitable C++ compiler found!"
    echo "Please ensure g++ or MinGW is installed"
    echo "Or set CXX environment variable to specify compiler path"
    exit 1
fi

# Compiler flags - allow user to specify via environment variable
if [[ -z "$CXXFLAGS" ]]; then
    CXXFLAGS="-std=c++11 -Wall -I../include -I../ir_lib/include"
else
    echo "[INFO] Using user-specified CXXFLAGS: $CXXFLAGS"
fi

echo "[INFO] Using compiler: $CXX"

# Test cases
TEST_CASES=(
    "test.sy"
    "test1.sy"
    "test2.sy"
    "test3.sy"
    "test4.sy"
    "simple_test.sy"
    "full_test.sy"
    "debug_test.sy"
)

# Functions
print_step() {
    echo -e "${CYAN}========================================${NC}"
    echo -e "${CYAN}$1${NC}"
    echo -e "${CYAN}========================================${NC}"
}

print_success() {
    echo -e "${GREEN}[OK] $1${NC}"
}

print_error() {
    echo -e "${RED}[ERROR] $1${NC}"
}

print_info() {
    if [[ "$VERBOSE" == "1" ]]; then
        echo -e "${YELLOW}[INFO] $1${NC}"
    fi
}

init_environment() {
    print_step "Initializing Test Environment"

    # Create output directory
    mkdir -p "$OUTPUT_DIR"
    print_success "Created output directory: $OUTPUT_DIR"

    # Create build directory
    mkdir -p "$BUILD_DIR"
    mkdir -p "$BUILD_DIR/ir_lib"
    print_success "Created build directories"
}

build_ir_library() {
    print_step "Building IR Library"

    local ir_lib_src="$PROJECT_ROOT/ir_lib/src"
    local ir_lib_obj="$BUILD_DIR/ir_lib"

    # Compile IR library source files
    for src_file in "$ir_lib_src"/*.cpp; do
        if [[ -f "$src_file" ]]; then
            local obj_file="$ir_lib_obj/$(basename "${src_file%.cpp}.o")"
            print_info "Compiling: $(basename "$src_file")"
            if ! "$CXX" $CXXFLAGS -c -o "$obj_file" "$src_file"; then
                print_error "IR library compilation failed: $(basename "$src_file")"
                exit 1
            fi
        fi
    done

    print_success "IR library built successfully"
}

build_component() {
    local name="$1"
    shift
    local sources=("$@")

    print_step "Building $name"

    local obj_files=()
    for source in "${sources[@]}"; do
        local src_path="$PROJECT_ROOT/$source"
        local obj_name="$(basename "${source%.cpp}.o")"
        local obj_path="$BUILD_DIR/$obj_name"

        print_info "Compiling: $source"
        if ! "$CXX" $CXXFLAGS -c -o "$obj_path" "$src_path"; then
            print_error "$name compilation failed: $source"
            exit 1
        fi

        obj_files+=("$obj_path")
    done

    # Link executable
    local exe_path="$BUILD_DIR/${name}.exe"
    local link_cmd="$CXX $CXXFLAGS -o \"$exe_path\""

    if [[ "$name" == "compiler" ]]; then
        # Add IR library objects for compiler
        for obj in "$BUILD_DIR"/ir_lib/*.o; do
            link_cmd="$link_cmd \"$obj\""
        done
    elif [[ "$name" == "parser" ]]; then
        # Add lexer object for parser
        link_cmd="$link_cmd \"$BUILD_DIR/lexer.o\""
    fi

    # Add component objects
    for obj in "${obj_files[@]}"; do
        link_cmd="$link_cmd \"$obj\""
    done

    if ! eval "$link_cmd"; then
        print_error "$name linking failed"
        exit 1
    fi

    print_success "$name built: $exe_path"
}

build_all_components() {
    print_step "Building All Components"

    # Build lexer
    build_component "lexer" "src/lexer.cpp"

    # Build parser
    build_component "parser" "src/parser.cpp"

    # Build full compiler
    build_component "compiler" \
        "src/lexer.cpp" \
        "src/parser.cpp" \
        "src/ast.cpp" \
        "src/symbol_table.cpp" \
        "src/ir_generator.cpp" \
        "src/main.cpp"
}

test_lexer() {
    local test_file="$1"
    print_step "Testing Lexer - $test_file"

    local input_file="$CASE_DIR/$test_file"
    local output_file="$OUTPUT_DIR/${test_file}_lexer.txt"
    local exe_path="$BUILD_DIR/lexer.exe"

    if [[ ! -f "$input_file" ]]; then
        print_error "Test file not found: $input_file"
        return 1
    fi

    print_info "Running lexer: $test_file"
    if "$exe_path" "$input_file" > "$output_file" 2>&1; then
        print_success "Lexer test passed: $output_file"
        if [[ "$VERBOSE" == "1" ]]; then
            cat "$output_file"
        fi
        return 0
    else
        print_error "Lexer test failed: $test_file"
        return 1
    fi
}

test_parser() {
    local test_file="$1"
    print_step "Testing Parser - $test_file"

    local input_file="$CASE_DIR/$test_file"
    local output_file="$OUTPUT_DIR/${test_file}_parser.txt"
    local exe_path="$BUILD_DIR/parser.exe"

    if [[ ! -f "$input_file" ]]; then
        print_error "Test file not found: $input_file"
        return 1
    fi

    print_info "Running parser: $test_file"
    if "$exe_path" "$input_file" > "$output_file" 2>&1; then
        print_success "Parser test passed: $output_file"
        if [[ "$VERBOSE" == "1" ]]; then
            cat "$output_file"
        fi
        return 0
    else
        print_error "Parser test failed: $test_file"
        return 1
    fi
}

test_compiler() {
    local test_file="$1"
    print_step "Testing Compiler - $test_file"

    local input_file="$CASE_DIR/$test_file"
    local output_file="$OUTPUT_DIR/${test_file}.ll"
    local exe_path="$BUILD_DIR/compiler.exe"

    if [[ ! -f "$input_file" ]]; then
        print_error "Test file not found: $input_file"
        return 1
    fi

    print_info "Running compiler: $test_file"
    if "$exe_path" "$input_file" > "$output_file" 2>&1; then
        print_success "Compiler test passed: $output_file"

        # Check if IR contains main function
        if grep -q "define.*@main" "$output_file"; then
            print_success "IR generation successful - main function found"
        else
            print_error "IR generation may have issues - main function not found"
        fi

        if [[ "$VERBOSE" == "1" ]]; then
            cat "$output_file"
        fi
        return 0
    else
        print_error "Compiler test failed: $test_file"
        return 1
    fi
}

test_ir_generator() {
    print_step "Testing IR Generator Core"

    local exe_path="$BUILD_DIR/test_ir.exe"

    print_info "Compiling IR generator test"
    local compile_cmd="$CXX $CXXFLAGS -o \"$exe_path\" ../test_ir.cpp ../src/ir_generator.cpp"
    for obj in "$PROJECT_ROOT"/ir_lib/BasicBlock.o \
               "$PROJECT_ROOT"/ir_lib/Constant.o \
               "$PROJECT_ROOT"/ir_lib/Function.o \
               "$PROJECT_ROOT"/ir_lib/GlobalVariable.o \
               "$PROJECT_ROOT"/ir_lib/Instruction.o \
               "$PROJECT_ROOT"/ir_lib/IRprinter.o \
               "$PROJECT_ROOT"/ir_lib/Module.o \
               "$PROJECT_ROOT"/ir_lib/Type.o \
               "$PROJECT_ROOT"/ir_lib/User.o \
               "$PROJECT_ROOT"/ir_lib/Value.o; do
        compile_cmd="$compile_cmd \"$obj\""
    done

    if ! eval "$compile_cmd"; then
        print_error "IR generator test compilation failed"
        return 1
    fi

    print_info "Running IR generator test"
    if "$exe_path"; then
        print_success "IR generator test passed"
        return 0
    else
        print_error "IR generator test failed"
        return 1
    fi
}

generate_report() {
    local results_file="$1"
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')

    print_step "Generating Test Report"

    local report_file="$OUTPUT_DIR/test_report.txt"

    cat > "$report_file" << EOF
Compiler Principles Final Project - Test Report
Generated: $timestamp

========================================
Test Results Summary
========================================

Total tests run: $total_tests
Tests passed: $passed_tests
Success rate: $success_rate%

========================================
Detailed Results
========================================

Build Phase:
  IR Library: $([[ "${build_results[ir_lib]}" == "1" ]] && echo "PASS" || echo "FAIL")
  Lexer: $([[ "${build_results[lexer]}" == "1" ]] && echo "PASS" || echo "FAIL")
  Parser: $([[ "${build_results[parser]}" == "1" ]] && echo "PASS" || echo "FAIL")
  Compiler: $([[ "${build_results[compiler]}" == "1" ]] && echo "PASS" || echo "FAIL")

Test Phase:
EOF

    # Add test results
    for test_case in "${TEST_CASES[@]}"; do
        echo "  $test_case:" >> "$report_file"
        echo "    Lexer: $([[ "${test_results[lexer,$test_case]}" == "1" ]] && echo "PASS" || echo "FAIL")" >> "$report_file"
        echo "    Parser: $([[ "${test_results[parser,$test_case]}" == "1" ]] && echo "PASS" || echo "FAIL")" >> "$report_file"
        echo "    Compiler: $([[ "${test_results[compiler,$test_case]}" == "1" ]] && echo "PASS" || echo "FAIL")" >> "$report_file"
    done

    cat >> "$report_file" << EOF

IR Generator: $([[ "${ir_generator_result}" == "1" ]] && echo "PASS" || echo "FAIL")

========================================
Output Files
========================================

Test outputs are saved in: $OUTPUT_DIR
- Lexer results: {testcase}_lexer.txt
- Parser results: {testcase}_parser.txt
- IR code: {testcase}.ll
- Manual IR test: manual_test_output.ll

========================================
EOF

    print_success "Test report generated: $report_file"
}

run_full_test() {
    local test_case="$1"

    print_step "Starting Complete Test Suite"

    # Initialize counters
    declare -A build_results
    declare -A test_results
    total_tests=0
    passed_tests=0

    # 1. Initialize environment
    init_environment
    build_results[environment]=1

    # 2. Build IR library
    build_ir_library
    build_results[ir_lib]=1

    # 3. Build all components
    build_all_components
    build_results[lexer]=1
    build_results[parser]=1
    build_results[compiler]=1

    # 4. Determine test cases to run
    local test_cases_to_run
    if [[ -n "$test_case" && "$test_case" != "all" ]]; then
        test_cases_to_run=("$test_case")
    else
        test_cases_to_run=("${TEST_CASES[@]}")
    fi

    # 5. Run tests
    for test_file in "${test_cases_to_run[@]}"; do
        print_step "Test Case: $test_file"

        # Lexer test
        if test_lexer "$test_file"; then
            test_results[lexer,$test_file]=1
            ((passed_tests++))
        fi
        ((total_tests++))

        # Parser test
        if test_parser "$test_file"; then
            test_results[parser,$test_file]=1
            ((passed_tests++))
        fi
        ((total_tests++))

        # Compiler test
        if test_compiler "$test_file"; then
            test_results[compiler,$test_file]=1
            ((passed_tests++))
        fi
        ((total_tests++))
    done

    # 6. Test IR generator core
    local ir_generator_result=0
    if test_ir_generator; then
        ir_generator_result=1
        ((passed_tests++))
    fi
    ((total_tests++))

    # 7. Generate report
    success_rate=$(( passed_tests * 100 / total_tests ))%
    generate_report

    # 8. Final summary
    print_step "Test Suite Complete"

    echo
    echo -e "${CYAN}Final Summary:${NC}"
    echo "  Total tests: $total_tests"
    echo -e "  Passed: ${GREEN}$passed_tests${NC}"
    echo -e "  Success rate: ${BLUE}$success_rate${NC}"
    echo
    echo -e "${CYAN}Output directory: $OUTPUT_DIR${NC}"
    echo -e "${CYAN}Test report: $OUTPUT_DIR/test_report.txt${NC}"
}

# Parse command line arguments
VERBOSE=0
CLEAN=0
TEST_CASE=""

while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        -c|--clean)
            CLEAN=1
            shift
            ;;
        -*)
            echo "Unknown option: $1"
            echo "Usage: $0 [-v|--verbose] [-c|--clean] [test_case]"
            exit 1
            ;;
        *)
            TEST_CASE="$1"
            shift
            ;;
    esac
done

# Main execution
if [[ "$CLEAN" == "1" ]]; then
    print_step "Cleaning Build Files"
    if [[ -d "$BUILD_DIR" ]]; then
        rm -rf "$BUILD_DIR"
        print_success "Build directory cleaned"
    else
        print_info "Build directory does not exist"
    fi
    exit 0
fi

run_full_test "$TEST_CASE"
