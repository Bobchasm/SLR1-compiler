; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test3_function.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @add(i32 %x, i32 %y) {
label_add_ENTRY:
  %_x = alloca i32
  store i32 %x, i32* %_x
  %_y = alloca i32
  store i32 %y, i32* %_y
  %result = alloca i32
  %op0 = load i32, i32* %_x
  %op1 = load i32, i32* %_y
  %op2 = add i32 %op0, %op1
  store i32 %op2, i32* %result
  %op3 = load i32, i32* %result
  ret i32 %op3
}
define i32 @multiply(i32 %a, i32 %b) {
label_multiply_ENTRY:
  %_a = alloca i32
  store i32 %a, i32* %_a
  %_b = alloca i32
  store i32 %b, i32* %_b
  %op0 = load i32, i32* %_a
  %op1 = load i32, i32* %_b
  %op2 = mul i32 %op0, %op1
  ret i32 %op2
}
define i32 @main() {
label_main_ENTRY:
  %sum = alloca i32
  %op0 = call i32 @add(i32 10, i32 20)
  store i32 %op0, i32* %sum
  %product = alloca i32
  %op1 = call i32 @multiply(i32 5, i32 6)
  store i32 %op1, i32* %product
  %total = alloca i32
  %op2 = load i32, i32* %sum
  %op3 = load i32, i32* %product
  %op4 = add i32 %op2, %op3
  store i32 %op4, i32* %total
  %op5 = load i32, i32* %total
  ret i32 %op5
}
