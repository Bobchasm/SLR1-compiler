; ModuleID = 'sysy2022_compiler'
source_filename = "case\test1.sy"
declare i32 @getint()

declare void @putint(i32)

declare i32 @getch()

declare void @putch(i32)

declare i32 @getarray(i32*)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @main() {
label_main_entry:
  %a = alloca i32
  store i32 1, i32* %a
  %b = alloca i32
  store i32 2, i32* %b

  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = add i32 %op0, %op1
  %op3 = sub i32 %op2, 1
  store i32 %op3, i32* %a
  ret i32 0

  %op4 = load i32, i32* %b
  %op5 = mul i32 %op4, 2
  %op6 = sdiv i32 %op5, 1
  %op7 = srem i32 %op6, 2
  store i32 %op7, i32* %b
  
  ret i32 1
}