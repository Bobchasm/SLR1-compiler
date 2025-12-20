; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test9_float_int_return.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define float @main() {
label_main_ENTRY:
  %b = alloca float
  store float 0x40400000, float* %b
  %op0 = load float, float* %b
  ret float %op0
}
