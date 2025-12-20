; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_test5_const.sy"
@GLOBAL_CONST = global i32 100
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
  %LOCAL_CONST = alloca i32
  store i32 50, i32* %LOCAL_CONST
  %a = alloca i32
  %op0 = load i32, i32* @GLOBAL_CONST
  store i32 %op0, i32* %a
  %b = alloca i32
  %op1 = load i32, i32* %LOCAL_CONST
  store i32 %op1, i32* %b
  %PI = alloca float
  store float 0x4048F5C3, float* %PI
  %r = alloca float
  store float 0x41200000, float* %r
  %area = alloca float
  %op2 = load float, float* %PI
  %op3 = load float, float* %r
  %op4 = fmul float %op2, %op3
  %op5 = load float, float* %r
  %op6 = fmul float %op4, %op5
  store float %op6, float* %area
  ret i32 0
}
