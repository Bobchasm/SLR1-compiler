; ModuleID = 'sysy2022_compiler'
source_filename = "case/test6.sy"
declare i32 @getint()

declare void @putint(i32)

declare i32 @getch()

declare void @putch(i32)

declare i32 @getarray(i32*)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @add(i32 %a, i32 %b) {
label_add_entry:
  %a = alloca i32
  store i32 %a, i32* %a
  %b = alloca i32
  store i32 %b, i32* %b
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = add i32 %op0, %op1
  ret i32 %op2
}
define i32 @main() {
label_main_entry:
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
  store i32 7, float* %y
  %op3 = load i32, i32* %b
  store i32 %op3, float* %y
  %op4 = load float, float* %y
  ret float %op4
}
