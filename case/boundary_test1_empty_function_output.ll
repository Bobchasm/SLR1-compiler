; ModuleID = 'sysy2022_compiler'
source_filename = "case\boundary_test1_empty_function.sy"
declare i32 @getint()

declare void @putint(i32)

declare i32 @getch()

declare void @putch(i32)

declare i32 @getarray(i32*)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define void @emptyFunc() {
label_emptyFunc_ENTRY:
  ret void
}
define i32 @main() {
label_main_ENTRY:
  ret i32 0
}
