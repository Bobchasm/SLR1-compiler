; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_test6_function.sy"
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
  %op0 = load i32, i32* %_x
  %op1 = load i32, i32* %_y
  %op2 = add i32 %op0, %op1
  ret i32 %op2
}
define float @multiply(float %a, float %b) {
label_multiply_ENTRY:
  %_a = alloca float
  store float %a, float* %_a
  %_b = alloca float
  store float %b, float* %_b
  %op0 = load float, float* %_a
  %op1 = load float, float* %_b
  %op2 = fmul float %op0, %op1
  ret float %op2
}
define void @print_hello() {
label_print_hello_ENTRY:
  ret void
}
define i32 @main() {
label_main_ENTRY:
  %result1 = alloca i32
  %op0 = call i32 @add(i32 10, i32 20)
  store i32 %op0, i32* %result1
  %result2 = alloca float
  %op1 = call float @multiply(float 0x4048F5C3, float 0x40000000)
  store float %op1, float* %result2
  call void @print_hello()
  ret i32 0
}
