; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test20_type_conversion_float_to_int.sy"
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
  store i32 2, i32* %a
  %b = alloca i32
  store i32 3, i32* %b
  %x = alloca float
  store float 0x41280000, float* %x
  %c = alloca i32
  %op0 = load float, float* %x
  store float %op0, i32* %c
  ret i32 0
}
define i32 @test_return() {
label_test_return_ENTRY:
  %x = alloca float
  store float 0x40B66666, float* %x
  %op0 = load float, float* %x
  ret float %op0
}
define void @test_param(i32 %y) {
label_test_param_ENTRY:
  %_y = alloca i32
  store i32 %y, i32* %_y
  ret void
}
define i32 @test_call() {
label_test_call_ENTRY:
  %x = alloca float
  store float 0x40533333, float* %x
  %op0 = load float, float* %x
  call void @test_param(float %op0)
  call void @test_param(i32 42)
  ret i32 0
}
