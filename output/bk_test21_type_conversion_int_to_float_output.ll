; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test21_type_conversion_int_to_float.sy"
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
  %a = alloca float
  store float 0x3F800000, float* %a
  %b = alloca float
  store float 0x40000000, float* %b
  %x = alloca i32
  store i32 10, i32* %x
  %c = alloca float
  %op0 = load i32, i32* %x
  store i32 %op0, float* %c
  ret i32 0
}
define float @test_return() {
label_test_return_ENTRY:
  %x = alloca i32
  store i32 5, i32* %x
  %op0 = load i32, i32* %x
  ret i32 %op0
}
define void @test_param(float %y) {
label_test_param_ENTRY:
  %_y = alloca float
  store float %y, float* %_y
  ret void
}
define i32 @test_call() {
label_test_call_ENTRY:
  %x = alloca i32
  store i32 3, i32* %x
  %op0 = load i32, i32* %x
  %op1 = sitofp i32 %op0 to float
  call void @test_param(float %op1)
  call void @test_param(float 0x42280000)
  ret i32 0
}
