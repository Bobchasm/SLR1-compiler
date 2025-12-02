; ModuleID = 'sysy2022_compiler'
source_filename = ".\case\test_function_call.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define void @foo(float %f) {
label_foo_ENTRY:
  %_f = alloca float
  store float %f, float* %_f
  %a = alloca i32
  store i32 1, i32* %a
  ret void
}
define void @main() {
label_main_ENTRY:
  %op0 = sitofp i32 1 to float
  call void @foo(float %op0)
  ret void
}
