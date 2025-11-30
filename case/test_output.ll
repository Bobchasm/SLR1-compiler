; ModuleID = 'sysy2022_compiler'
source_filename = ".\case\test.sy"
@a = global i32 10
@a = global i32 10
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
  store i32 10, i32* @a
  ret i32 0
}
