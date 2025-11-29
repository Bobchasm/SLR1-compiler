; ModuleID = 'sysy2022_compiler'
source_filename = "test_manual.sy"
declare i32 @getint()

declare void @putint(i32)

declare i32 @getch()

declare void @putch(i32)

declare i32 @getarray(i32*)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @main() {
label_main_entry:
  %a = alloca i32
  store i32 1, i32* %a
  ret i32 0
}
