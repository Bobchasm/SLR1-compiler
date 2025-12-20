; ModuleID = 'sysy2022_compiler'
source_filename = "case\le_test2_comment.sy"
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
  store i32 10, i32* %a
  %op0 = load i32, i32* %a
  ret i32 %op0
}
