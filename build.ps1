# 编译器构建脚本 (PowerShell)
# 使用方法: .\build.ps1

$CXX = "G:/444SoftWare/MSYS-251020/ucrt64/bin/g++.exe"
$AR = "G:/444SoftWare/MSYS-251020/ucrt64/bin/ar.exe"
$CXXFLAGS = "-std=c++11 -Wall -Iinclude -Iir_lib/include"

Write-Host "================== 编译器构建开始 ==================" -ForegroundColor Green

# 创建目录
Write-Host "`n1. 创建编译目录..." -ForegroundColor Cyan
New-Item -ItemType Directory -Force -Path "build" | Out-Null
New-Item -ItemType Directory -Force -Path "build/ir_lib" | Out-Null
New-Item -ItemType Directory -Force -Path "logs" | Out-Null
New-Item -ItemType Directory -Force -Path "process" | Out-Null

# 编译IR库
Write-Host "`n2. 编译IR库..." -ForegroundColor Cyan
$ir_sources = Get-ChildItem -Path "ir_lib/src/*.cpp"
$ir_objects = @()

foreach ($src in $ir_sources) {
    $obj = "build/ir_lib/" + $src.BaseName + ".o"
    $ir_objects += $obj
    Write-Host "   编译: $($src.Name) -> $($obj)"
    & $CXX $CXXFLAGS.Split() -c -o $obj $src.FullName
    if ($LASTEXITCODE -ne 0) {
        Write-Host "错误: 编译 $($src.Name) 失败!" -ForegroundColor Red
        exit 1
    }
}

# 创建IR库
Write-Host "`n3. 创建IR静态库..." -ForegroundColor Cyan
& $AR rcs "build/libir.a" $ir_objects
if ($LASTEXITCODE -ne 0) {
    Write-Host "错误: 创建IR库失败!" -ForegroundColor Red
    exit 1
}

# 编译主程序源文件
Write-Host "`n4. 编译主程序源文件..." -ForegroundColor Cyan

# 先编译lexer.cpp和parser.cpp，禁用其main函数
Write-Host "   编译: src/lexer.cpp -> build/lexer.o (禁用main)"
& $CXX $CXXFLAGS.Split() -DNO_MAIN -c -o "build/lexer.o" "src/lexer.cpp"
if ($LASTEXITCODE -ne 0) {
    Write-Host "错误: 编译 src/lexer.cpp 失败!" -ForegroundColor Red
    exit 1
}

Write-Host "   编译: src/parser.cpp -> build/parser.o (禁用main)"
& $CXX $CXXFLAGS.Split() -DNO_MAIN -c -o "build/parser.o" "src/parser.cpp"
if ($LASTEXITCODE -ne 0) {
    Write-Host "错误: 编译 src/parser.cpp 失败!" -ForegroundColor Red
    exit 1
}
$main_objects = @("build/lexer.o", "build/parser.o")

$sources = @(
    @{src="src/ast.cpp"; obj="build/ast.o"},
    @{src="src/symbol_table.cpp"; obj="build/symbol_table.o"},
    @{src="src/ir_generator.cpp"; obj="build/ir_generator.o"},
    @{src="src/main.cpp"; obj="build/main.o"}
)

# 不要重置$main_objects，已经包含lexer.o和parser.o
foreach ($item in $sources) {
    Write-Host "   编译: $($item.src) -> $($item.obj)"
    & $CXX $CXXFLAGS.Split() -c -o $item.obj $item.src
    if ($LASTEXITCODE -ne 0) {
        Write-Host "错误: 编译 $($item.src) 失败!" -ForegroundColor Red
        exit 1
    }
    $main_objects += $item.obj
}

# 链接生成可执行文件
Write-Host "`n5. 链接生成编译器..." -ForegroundColor Cyan
Write-Host "[DEBUG] 链接对象: $main_objects" -ForegroundColor Yellow
& $CXX $CXXFLAGS.Split() -o "build/compiler.exe" $main_objects "build/libir.a"
if ($LASTEXITCODE -ne 0) {
    Write-Host "错误: 链接失败!" -ForegroundColor Red
    exit 1
}

Write-Host "`n================== 编译成功! ==================" -ForegroundColor Green
Write-Host "`n可执行文件: build/compiler.exe" -ForegroundColor Yellow
Write-Host "使用方法: .\build\compiler.exe case\test.sy -o output.ll`n" -ForegroundColor Yellow
