; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test18_type_complex.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @add_int(i32 %x, i32 %y) {
label_add_int_ENTRY:
  %_x = alloca i32
  store i32 %x, i32* %_x
  %_y = alloca i32
  store i32 %y, i32* %_y
  %op0 = load i32, i32* %_x
  %op1 = load i32, i32* %_y
  %op2 = add i32 %op0, %op1
  ret i32 %op2
}
define float @add_float(float %x, float %y) {
label_add_float_ENTRY:
  %_x = alloca float
  store float %x, float* %_x
  %_y = alloca float
  store float %y, float* %_y
  %op0 = load float, float* %_x
  %op1 = load float, float* %_y
  %op2 = fadd float %op0, %op1
  ret float %op2
}
define i32 @main() {
label_main_ENTRY:
  %f1 = alloca float
  store float 0x41200000, float* %f1
  %f2 = alloca float
  %op0 = call i32 @add_int(i32 5, i32 3)
  store i32 %op0, float* %f2
  %i1 = alloca i32
  store i32 3, i32* %i1
  %i2 = alloca i32
  %op1 = call float @add_float(float 0x40000000, float 0x3F800000)
  store float %op1, i32* %i2
  %result1 = alloca i32
  %op2 = call i32 @add_int(i32 10, i32 20)
  store i32 %op2, i32* %result1
  %result2 = alloca float
  %op3 = call float @add_float(float 0x3FC00000, float 0x40200000)
  store float %op3, float* %result2
  %result3 = alloca i32
  %op4 = call i32 @add_int(i32 1, i32 2)
  store i32 %op4, i32* %result3
  %result4 = alloca float
  %op5 = call float @add_float(float 0x3F800000, float 0x40000000)
  store float %op5, float* %result4
  %fa = alloca float
  store float 0x3FC00000, float* %fa
  %ia = alloca i32
  store i32 2, i32* %ia
  %fb = alloca float
  %op6 = load float, float* %fa
  %op7 = load i32, i32* %ia
  %op8 = fadd float %op6, %op7
  store float %op8, float* %fb
  %ib = alloca i32
  %op9 = load float, float* %fa
  %op10 = load i32, i32* %ia
  %op11 = fadd float %op9, %op10
  store float %op11, i32* %ib
  ret i32 0
}
