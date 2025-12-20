; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test22_type_mismatch.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @funcInt() {
label_funcInt_ENTRY:
  ret i32 42
}
define float @funcFloat() {
label_funcFloat_ENTRY:
  ret float 0x4048F5C3
}
define void @main() {
label_main_ENTRY:
  %f = alloca float
  store float 0x41200000, float* %f
  %i = alloca i32
  store i32 3, i32* %i
  ret void
}
define i32 @testReturnType1() {
label_testReturnType1_ENTRY:
  ret i32 3
}
define float @testReturnType2() {
label_testReturnType2_ENTRY:
  ret float 0x40A00000
}
