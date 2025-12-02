; ModuleID = 'sysy2022_compiler'
source_filename = ".\case\testfloint.sy"
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
  %a = alloca i32
  store float 1.2, i32* %a
  ret void
}
