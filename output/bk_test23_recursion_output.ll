; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test23_recursion.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @factorial(i32 %n) {
label_factorial_ENTRY:
  %_n = alloca i32
  store i32 %n, i32* %_n
  %op0 = load i32, i32* %_n
  %op1 = icmp sle i32 %op0, 1
  br i1 %op1, label %label_if_then_1, label %label_if_else_1
label_if_then_1:                                                ; preds = %label_factorial_ENTRY
  ret i32 1
label_if_end_1:
  ret i32 0
label_if_else_1:                                                ; preds = %label_factorial_ENTRY
  %op2 = load i32, i32* %_n
  %op3 = load i32, i32* %_n
  %op4 = sub i32 %op3, 1
  %op5 = call i32 @factorial(i32 %op4)
  %op6 = mul i32 %op2, %op5
  ret i32 %op6
}
define i32 @main() {
label_main_ENTRY:
  %n = alloca i32
  store i32 10, i32* %n
  %x = alloca i32
  %op0 = load i32, i32* %n
  %op1 = call i32 @factorial(i32 %op0)
  store i32 %op1, i32* %x
  %op2 = load i32, i32* %x
  ret i32 %op2
}
