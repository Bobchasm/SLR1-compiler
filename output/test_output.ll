; ModuleID = 'sysy2022_compiler'
source_filename = ".\case\test.sy"
@global_int = global i32 10
@global_float = global float 0x4048F5C3
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
  %a = alloca i32
  %op0 = load i32, i32* @global_int
  %op1 = add i32 %op0, 5
  store i32 %op1, i32* %a
  %b = alloca float
  %op2 = load float, float* @global_float
  %op3 = fmul float %op2, 0x40000000
  store float %op3, float* %b
  %op4 = load i32, i32* %a
  ret i32 %op4
}
