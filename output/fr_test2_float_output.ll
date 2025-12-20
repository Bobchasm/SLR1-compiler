; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_test2_float.sy"
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
  %x = alloca float
  store float 0x4048F5C3, float* %x
  %y = alloca float
  store float 0x40200000, float* %y
  %z = alloca float
  %op0 = load float, float* %x
  %op1 = load float, float* %y
  %op2 = fadd float %op0, %op1
  store float %op2, float* %z
  %op3 = load float, float* %x
  %op4 = load float, float* %y
  %op5 = fsub float %op3, %op4
  store float %op5, float* %z
  %op6 = load float, float* %x
  %op7 = load float, float* %y
  %op8 = fmul float %op6, %op7
  store float %op8, float* %z
  %op9 = load float, float* %x
  %op10 = load float, float* %y
  %op11 = fdiv float %op9, %op10
  store float %op11, float* %z
  %PI = alloca float
  store float 0x40490FD0, float* %PI
  ret i32 0
}
