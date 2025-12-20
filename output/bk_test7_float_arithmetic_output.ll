; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test7_float_arithmetic.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define float @add(float %a, float %b) {
label_add_ENTRY:
  %_a = alloca float
  store float %a, float* %_a
  %_b = alloca float
  store float %b, float* %_b
  %op0 = load float, float* %_a
  %op1 = load float, float* %_b
  %op2 = fadd float %op0, %op1
  ret float %op2
}
define float @main() {
label_main_ENTRY:
  %a = alloca float
  store float 0x3F99999A, float* %a
  %b = alloca float
  store float 0x40000000, float* %b
  %x = alloca float
  %op0 = load float, float* %a
  %op1 = load float, float* %b
  %op2 = call float @add(float %op0, float %op1)
  store float %op2, float* %x
  store float 0x40A00000, float* %b
  %y = alloca float
  store float 0x40FBD70A, float* %y
  %op3 = load float, float* %b
  store float %op3, float* %y
  %op4 = load float, float* %y
  ret float %op4
}
