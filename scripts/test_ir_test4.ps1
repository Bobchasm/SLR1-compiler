# IR Generator Test Script for test4.sy
# Tests the complete pipeline: lexer -> parser -> IR generator

param(
    [string]$Compiler = "G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe"
)

# Get script directory and project root
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Split-Path -Parent $ScriptDir

$SourceFile = Join-Path $ProjectRoot "test_ir_test4.cpp"
$OutputExe = Join-Path $ProjectRoot "test_ir_test4.exe"

Write-Host "=========================================" -ForegroundColor Cyan
Write-Host "IR Generator Test for test4.sy" -ForegroundColor Cyan
Write-Host "=========================================" -ForegroundColor Cyan

# Check if compiler exists
if (-not (Test-Path $Compiler)) {
    Write-Host "Error: Compiler not found at $Compiler" -ForegroundColor Red
    exit 1
}

# Create test_output directory
$TestOutputDir = Join-Path $ProjectRoot "test_output"
if (-not (Test-Path $TestOutputDir)) {
    New-Item -ItemType Directory -Path $TestOutputDir | Out-Null
}

Write-Host "Building test program..." -ForegroundColor Cyan

# Compilation command
$compileCmd = "& '$Compiler' -std=c++11 -Wall -I'$ProjectRoot/include' -I'$ProjectRoot/ir_lib/include' " +
              "-DNO_MAIN " +  # Disable main in lexer and parser
              "'$SourceFile' " +
              "'$ProjectRoot/src/lexer.cpp' " +
              "'$ProjectRoot/src/parser.cpp' " +
              "'$ProjectRoot/src/ast.cpp' " +
              "'$ProjectRoot/src/symbol_table.cpp' " +
              "'$ProjectRoot/src/ir_generator.cpp' " +
              "'$ProjectRoot/build/libir.a' " +
              "-o '$OutputExe'"

Write-Host "Compilation command:" -ForegroundColor Yellow
Write-Host $compileCmd -ForegroundColor Yellow

try {
    Invoke-Expression $compileCmd
    if ($LASTEXITCODE -eq 0) {
        Write-Host "Compilation successful!" -ForegroundColor Green

        Write-Host "`nRunning IR generator test..." -ForegroundColor Cyan
        Write-Host "=========================================" -ForegroundColor Cyan

        # Run the test
        & $OutputExe

        if ($LASTEXITCODE -eq 0) {
            Write-Host "=========================================" -ForegroundColor Cyan
            Write-Host "Test completed successfully!" -ForegroundColor Green
            Write-Host "IR output saved to: test_output/test4_ir_output.ll" -ForegroundColor Blue
        } else {
            Write-Host "Test failed with exit code: $LASTEXITCODE" -ForegroundColor Red
        }
    } else {
        Write-Host "Compilation failed!" -ForegroundColor Red
    }
} catch {
    Write-Host "Error during execution: $($_.Exception.Message)" -ForegroundColor Red
}
