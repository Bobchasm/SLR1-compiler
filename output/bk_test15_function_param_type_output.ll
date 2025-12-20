; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test15_function_param_type.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @addInt(i32 %a, i32 %b) {
label_addInt_ENTRY:
  %_a = alloca i32
  store i32 %a, i32* %_a
  %_b = alloca i32
  store i32 %b, i32* %_b
  %op0 = load i32, i32* %_a
  %op1 = load i32, i32* %_b
  %op2 = add i32 %op0, %op1
  ret i32 %op2
}
define float @addFloat(float %a, float %b) {
label_addFloat_ENTRY:
  %_a = alloca float
  store float %a, float* %_a
  %_b = alloca float
  store float %b, float* %_b
  %op0 = load float, float* %_a
  %op1 = load float, float* %_b
  %op2 = fadd float %op0, %op1
  ret float %op2
}
define void @main() {
label_main_ENTRY:
  %x = alloca i32
  store i32 10, i32* %x
  %y = alloca float
  store float 0x4048F5C3, float* %y
  %result1 = alloca i32
  %op0 = call i32 @addInt(i32 5, i32 10)
  store i32 %op0, i32* %result1
  %result2 = alloca i32
  %op1 = load float, float* %y
  %op2 = call i32 @addInt(float %op1, i32 10)
  store i32 %op2, i32* %result2
  %result3 = alloca float
  %op3 = load i32, i32* %x
  %op4 = sitofp i32 %op3 to float
  %op5 = call float @addFloat(float %op4, float 0x4048F5C3)
  store float %op5, float* %result3
  %result4 = alloca i32
  %op6 = load i32, i32* %x
  %op7 = load float, float* %y
  %op8 = call i32 @addInt(i32 %op6, float %op7)
  store i32 %op8, i32* %result4
  ret void
}
