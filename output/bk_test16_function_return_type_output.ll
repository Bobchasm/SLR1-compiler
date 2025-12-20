; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test16_function_return_type.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @add(i32 %a, i32 %b) {
label_add_ENTRY:
  %_a = alloca i32
  store i32 %a, i32* %_a
  %_b = alloca i32
  store i32 %b, i32* %_b
  %op0 = load i32, i32* %_a
  %op1 = load i32, i32* %_b
  %op2 = add i32 %op0, %op1
  ret i32 %op2
}
define i32 @main() {
label_main_ENTRY:
  %a = alloca i32
  store i32 1, i32* %a
  %b = alloca i32
  store i32 2, i32* %b
  %x = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = call i32 @add(i32 %op0, i32 %op1)
  store i32 %op2, i32* %x
  store i32 5, i32* %b
  %y = alloca float
  store float 0x40FBD70A, float* %y
  %op3 = load i32, i32* %b
  store i32 %op3, float* %y
  %op4 = load float, float* %y
  ret float %op4
}
