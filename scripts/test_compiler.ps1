#!/usr/bin/env pwsh

<#
.SYNOPSIS
    Compiler Principles Final Project - Complete Compiler Test Script
.DESCRIPTION
    Test the complete process from lexical analysis to intermediate code generation
.AUTHOR
    Compiler Project Team
#>

param(
    [switch]$Verbose,
    [switch]$Clean,
    [string]$TestCase = "test.sy"
)

# Configuration
$ProjectRoot = $PSScriptRoot
$BuildDir = Join-Path $ProjectRoot "build"
$OutputDir = Join-Path $ProjectRoot "test_output"
$CaseDir = Join-Path $ProjectRoot "case"
$CXX = "G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe"
$CXXFLAGS = "-std=c++11", "-Wall", "-Iinclude", "-Iir_lib/include"

# Test cases list
$TestCases = @(
    "test.sy",
    "test1.sy",
    "test2.sy",
    "test3.sy",
    "test4.sy",
    "simple_test.sy",
    "full_test.sy",
    "debug_test.sy"
)

function Write-Step {
    param([string]$Message)
    Write-Host "========================================" -ForegroundColor Cyan
    Write-Host $Message -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Cyan
}

function Write-Success {
    param([string]$Message)
    Write-Host "[OK] $Message" -ForegroundColor Green
}

function Write-Error {
    param([string]$Message)
    Write-Host "[ERROR] $Message" -ForegroundColor Red
}

function Write-Info {
    param([string]$Message)
    if ($Verbose) {
        Write-Host "[INFO] $Message" -ForegroundColor Yellow
    }
}

function Initialize-TestEnvironment {
    Write-Step "初始化测试环境"

    # 创建输出目录
    if (!(Test-Path $OutputDir)) {
        New-Item -ItemType Directory -Path $OutputDir | Out-Null
        Write-Success "创建输出目录: $OutputDir"
    }

    # 创建构建目录
    if (!(Test-Path $BuildDir)) {
        New-Item -ItemType Directory -Path $BuildDir | Out-Null
        Write-Success "创建构建目录: $BuildDir"
    }

    $IrLibBuildDir = Join-Path $BuildDir "ir_lib"
    if (!(Test-Path $IrLibBuildDir)) {
        New-Item -ItemType Directory -Path $IrLibBuildDir | Out-Null
        Write-Success "创建IR库构建目录: $IrLibBuildDir"
    }
}

function Build-IRLibrary {
    Write-Step "构建IR库"

    $IrLibSrcDir = Join-Path $ProjectRoot "ir_lib/src"
    $IrLibObjDir = Join-Path $BuildDir "ir_lib"

    # 编译IR库源文件
    $IrLibSrcFiles = Get-ChildItem -Path $IrLibSrcDir -Filter "*.cpp"
    foreach ($srcFile in $IrLibSrcFiles) {
        $objFile = Join-Path $IrLibObjDir ($srcFile.BaseName + ".o")
        $compileCmd = "$CXX $CXXFLAGS -c -o `"$objFile`" `"$($srcFile.FullName)`""

        Write-Info "编译: $($srcFile.Name)"
        $result = Invoke-Expression $compileCmd 2>&1
        if ($LASTEXITCODE -ne 0) {
            Write-Error "IR库编译失败: $($srcFile.Name)"
            Write-Error $result
            exit 1
        }
    }

    Write-Success "IR库构建完成"
}

function Build-Component {
    param([string]$Name, [string[]]$Sources, [string[]]$ExtraArgs = @())

    Write-Step "构建 $Name"

    $objFiles = @()
    foreach ($source in $Sources) {
        $srcPath = Join-Path $ProjectRoot $source
        $objName = [System.IO.Path]::GetFileNameWithoutExtension($source) + ".o"
        $objPath = Join-Path $BuildDir $objName

        $compileArgs = $CXXFLAGS + @("-c", "-o", "`"$objPath`"", "`"$srcPath`"") + $ExtraArgs
        $compileCmd = "$CXX " + ($compileArgs -join " ")

        Write-Info "编译: $source"
        $result = Invoke-Expression $compileCmd 2>&1
        if ($LASTEXITCODE -ne 0) {
            Write-Error "$Name 编译失败: $source"
            Write-Error $result
            exit 1
        }

        $objFiles += "`"$objPath`""
    }

    # 链接可执行文件
    $exePath = Join-Path $BuildDir "$Name.exe"
    $irLibObjs = Get-ChildItem -Path (Join-Path $BuildDir "ir_lib") -Filter "*.o" | ForEach-Object { "`"$($_.FullName)`"" }

    if ($Name -eq "compiler") {
        $linkCmd = "$CXX $CXXFLAGS -o `"$exePath`" " + ($objFiles -join " ") + " " + ($irLibObjs -join " ")
    } elseif ($Name -eq "parser") {
        $lexerObj = Join-Path $BuildDir "lexer.o"
        $linkCmd = "$CXX $CXXFLAGS -o `"$exePath`" " + ($objFiles -join " ") + " `"$lexerObj`""
    } else {
        $linkCmd = "$CXX $CXXFLAGS -o `"$exePath`" " + ($objFiles -join " ")
    }

    $result = Invoke-Expression $linkCmd 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Error "$Name 链接失败"
        Write-Error $result
        exit 1
    }

    Write-Success "$Name 构建完成: $exePath"
}

function Build-AllComponents {
    Write-Step "构建所有组件"

    # 构建词法分析器
    Build-Component -Name "lexer" -Sources @("src/lexer.cpp")

    # 构建语法分析器
    Build-Component -Name "parser" -Sources @("src/parser.cpp")

    # 构建完整编译器
    Build-Component -Name "compiler" -Sources @("src/lexer.cpp", "src/parser.cpp", "src/ast.cpp", "src/symbol_table.cpp", "src/ir_generator.cpp", "src/main.cpp") -ExtraArgs @("-DNO_MAIN")
}

function Test-Lexer {
    param([string]$TestFile)

    Write-Step "测试词法分析器 - $TestFile"

    $inputFile = Join-Path $CaseDir $TestFile
    $outputFile = Join-Path $OutputDir "$($TestFile)_lexer.txt"
    $exePath = Join-Path $BuildDir "lexer.exe"

    if (!(Test-Path $inputFile)) {
        Write-Error "测试文件不存在: $inputFile"
        return $false
    }

    Write-Info "运行词法分析: $TestFile"
    $result = & $exePath $inputFile | Out-File -FilePath $outputFile -Encoding UTF8

    if ($LASTEXITCODE -eq 0) {
        Write-Success "词法分析完成: $outputFile"
        if ($Verbose) {
            Get-Content $outputFile | Write-Host
        }
        return $true
    } else {
        Write-Error "词法分析失败: $TestFile"
        return $false
    }
}

function Test-Parser {
    param([string]$TestFile)

    Write-Step "测试语法分析器 - $TestFile"

    $inputFile = Join-Path $CaseDir $TestFile
    $outputFile = Join-Path $OutputDir "$($TestFile)_parser.txt"
    $exePath = Join-Path $BuildDir "parser.exe"

    if (!(Test-Path $inputFile)) {
        Write-Error "测试文件不存在: $inputFile"
        return $false
    }

    Write-Info "运行语法分析: $TestFile"
    $result = & $exePath $inputFile | Out-File -FilePath $outputFile -Encoding UTF8

    if ($LASTEXITCODE -eq 0) {
        Write-Success "语法分析完成: $outputFile"
        if ($Verbose) {
            Get-Content $outputFile | Write-Host
        }
        return $true
    } else {
        Write-Error "语法分析失败: $TestFile"
        return $false
    }
}

function Test-Compiler {
    param([string]$TestFile)

    Write-Step "测试完整编译器 - $TestFile"

    $inputFile = Join-Path $CaseDir $TestFile
    $outputFile = Join-Path $OutputDir "$($TestFile).ll"
    $exePath = Join-Path $BuildDir "compiler.exe"

    if (!(Test-Path $inputFile)) {
        Write-Error "测试文件不存在: $inputFile"
        return $false
    }

    Write-Info "运行完整编译器: $TestFile"
    $result = & $exePath $inputFile | Out-File -FilePath $outputFile -Encoding UTF8

    if ($LASTEXITCODE -eq 0) {
        Write-Success "编译器运行完成: $outputFile"

        # 检查IR输出是否正确
        $content = Get-Content $outputFile -Raw
        if ($content -match "define.*@main") {
            Write-Success "IR生成成功 - 包含main函数定义"
        } else {
            Write-Error "IR生成可能有问题 - 未找到main函数定义"
        }

        if ($Verbose) {
            Get-Content $outputFile | Write-Host
        }
        return $true
    } else {
        Write-Error "编译器运行失败: $TestFile"
        return $false
    }
}

function Test-IRGenerator {
    Write-Step "测试IR生成器核心功能"

    $exePath = Join-Path $BuildDir "test_ir.exe"
    $compileCmd = "$CXX $CXXFLAGS -o `"$exePath`" test_ir.cpp src/ir_generator.cpp " + `
                  ((Get-ChildItem -Path (Join-Path $ProjectRoot "ir_lib") -Filter "*.o").FullName | ForEach-Object { "`"$_`"" }) -join " "

    Write-Info "编译IR生成器测试程序"
    $result = Invoke-Expression $compileCmd 2>&1
    if ($LASTEXITCODE -ne 0) {
        Write-Error "IR生成器测试编译失败"
        Write-Error $result
        return $false
    }

    Write-Info "运行IR生成器测试"
    $result = & $exePath

    if ($LASTEXITCODE -eq 0) {
        Write-Success "IR生成器测试通过"
        return $true
    } else {
        Write-Error "IR生成器测试失败"
        return $false
    }
}

function Generate-TestReport {
    param([hashtable]$Results)

    Write-Step "生成测试报告"

    $reportFile = Join-Path $OutputDir "test_report.txt"
    $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"

    $report = @"
编译原理大作业 - 测试报告
生成时间: $timestamp

=======================================
测试结果汇总
=======================================

"@

    $totalTests = 0
    $passedTests = 0

    foreach ($phase in $Results.Keys) {
        $phaseResults = $Results[$phase]
        $phasePassed = ($phaseResults.Values | Where-Object { $_ -eq $true }).Count
        $phaseTotal = $phaseResults.Count
        $totalTests += $phaseTotal
        $passedTests += $phasePassed

        $report += "阶段: $phase`n"
        $report += "  通过: $phasePassed/$phaseTotal`n"

        foreach ($test in $phaseResults.Keys) {
            $status = if ($phaseResults[$test]) { "✓ 通过" } else { "✗ 失败" }
            $report += "    $test : $status`n"
        }
        $report += "`n"
    }

    $successRate = if ($totalTests -gt 0) { [math]::Round(($passedTests / $totalTests) * 100, 2) } else { 0 }
    $report += "=======================================`n"
    $report += "总体结果: $passedTests/$totalTests ($successRate%)`n"
    $report += "=======================================`n"

    $report | Out-File -FilePath $reportFile -Encoding UTF8
    Write-Success "测试报告已生成: $reportFile"
}

function Run-FullTest {
    param([string]$TestCase)

    Write-Step "开始完整测试流程"

    $results = @{
        "IR库构建" = @{}
        "组件构建" = @{}
        "词法分析" = @{}
        "语法分析" = @{}
        "完整编译" = @{}
        "IR生成器" = @{}
    }

    # 1. 初始化环境
    Initialize-TestEnvironment
    $results["IR库构建"]["环境初始化"] = $true

    # 2. 构建IR库
    Build-IRLibrary
    $results["IR库构建"]["IR库编译"] = $true

    # 3. 构建所有组件
    Build-AllComponents
    $results["组件构建"]["词法分析器"] = $true
    $results["组件构建"]["语法分析器"] = $true
    $results["组件构建"]["完整编译器"] = $true

    # 4. 确定测试用例
    $testCasesToRun = if ($TestCase -and $TestCase -ne "all") {
        @($TestCase)
    } else {
        $TestCases
    }

    # 5. 运行各项测试
    foreach ($testFile in $testCasesToRun) {
        Write-Step "测试用例: $testFile"

        # 词法分析测试
        $results["词法分析"][$testFile] = Test-Lexer -TestFile $testFile

        # 语法分析测试
        $results["语法分析"][$testFile] = Test-Parser -TestFile $testFile

        # 完整编译测试
        $results["完整编译"][$testFile] = Test-Compiler -TestFile $testFile
    }

    # 6. 测试IR生成器核心功能
    $results["IR生成器"]["核心功能测试"] = Test-IRGenerator

    # 7. 生成测试报告
    Generate-TestReport -Results $results

    # 8. 输出最终结果
    Write-Step "测试完成"

    $totalTests = 0
    $passedTests = 0
    foreach ($phase in $results.Values) {
        foreach ($result in $phase.Values) {
            $totalTests++
            if ($result) { $passedTests++ }
        }
    }

    $successRate = if ($totalTests -gt 0) { [math]::Round(($passedTests / $totalTests) * 100, 2) } else { 0 }

    Write-Host ""
    Write-Host "🎯 测试总结:" -ForegroundColor Cyan
    Write-Host "   总测试数: $totalTests" -ForegroundColor White
    Write-Host "   通过测试: $passedTests" -ForegroundColor Green
    Write-Host "   成功率: $successRate%" -ForegroundColor $(if ($successRate -eq 100) { "Green" } elseif ($successRate -ge 80) { "Yellow" } else { "Red" })
    Write-Host ""
    Write-Host "📁 输出目录: $OutputDir" -ForegroundColor Cyan
    Write-Host "📄 测试报告: $(Join-Path $OutputDir "test_report.txt")" -ForegroundColor Cyan
}

# 主程序
if ($Clean) {
    Write-Step "清理构建文件"
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
        Write-Success "清理完成"
    } else {
        Write-Info "构建目录不存在，无需清理"
    }
    exit 0
}

Run-FullTest -TestCase $TestCase
