; ModuleID = 'sysy2022_compiler'
source_filename = "case\kb_test10_float_literal_declaration.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define void @main() {
label_main_ENTRY:
  %a = alloca float
  store float 0x3DCCCCCD, float* %a
  %b = alloca float
  store float 0x3F9D70A4, float* %b
  %c = alloca float
  store float 0x3F800000, float* %c
  ret void
}
