; ModuleID = 'sysy2022_compiler'
source_filename = ".\case\testflo.sy"
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
  %a = alloca float
  store float %a, float* %a
  %b = alloca float
  store float %b, float* %b
  %op0 = load float, float* %a
  %op1 = load float, float* %b
  %op2 = fadd float %op0, %op1
  ret float %op2
}
define float @main() {
label_main_ENTRY:
  %a = alloca float
  store float 1.2, float* %a
  %b = alloca float
  store i32 2, float* %b
  %x = alloca float
  %op0 = load float, float* %a
  %op1 = load float, float* %b
  %op2 = call float @add(float %op0, float %op1)
  store float %op2, float* %x
  store i32 5, float* %b
  %y = alloca float
  store float 7.87, float* %y
  %op3 = load float, float* %b
  store float %op3, float* %y
  %op4 = load float, float* %y
  ret float %op4
}
