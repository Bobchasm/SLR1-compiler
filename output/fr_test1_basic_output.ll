; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_test1_basic.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @main() {
label_main_ENTRY:
  %a = alloca i32
  store i32 10, i32* %a
  %b = alloca i32
  store i32 20, i32* %b
  %c = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = add i32 %op0, %op1
  store i32 %op2, i32* %c
  %op3 = load i32, i32* %a
  %op4 = load i32, i32* %b
  %op5 = sub i32 %op3, %op4
  store i32 %op5, i32* %c
  %op6 = load i32, i32* %a
  %op7 = load i32, i32* %b
  %op8 = mul i32 %op6, %op7
  store i32 %op8, i32* %c
  %op9 = load i32, i32* %a
  %op10 = load i32, i32* %b
  %op11 = sdiv i32 %op9, %op10
  store i32 %op11, i32* %c
  %op12 = load i32, i32* %a
  %op13 = load i32, i32* %b
  %op14 = load i32, i32* %a
  %op15 = sub i32 0, %op14
  store i32 %op15, i32* %c
  ret i32 0
}
