; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test17_type_binary_expr.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define float @test_float_add() {
label_test_float_add_ENTRY:
  %a = alloca float
  store float 0x3F99999A, float* %a
  %b = alloca float
  store float 0x40066666, float* %b
  %op0 = load float, float* %a
  %op1 = load float, float* %b
  %op2 = fadd float %op0, %op1
  ret float %op2
}
define i32 @test_int_add() {
label_test_int_add_ENTRY:
  %a = alloca i32
  store i32 10, i32* %a
  %b = alloca i32
  store i32 20, i32* %b
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = add i32 %op0, %op1
  ret i32 %op2
}
define i32 @test_mixed_expr() {
label_test_mixed_expr_ENTRY:
  %a = alloca float
  store float 0x3FC00000, float* %a
  %b = alloca i32
  store i32 2, i32* %b
  %op0 = load float, float* %a
  %op1 = load i32, i32* %b
  %op2 = fadd float %op0, %op1
  ret float %op2
}
define float @test_mixed_to_float() {
label_test_mixed_to_float_ENTRY:
  %a = alloca float
  store float 0x3FC00000, float* %a
  %b = alloca i32
  store i32 2, i32* %b
  %c = alloca float
  %op0 = load float, float* %a
  %op1 = load i32, i32* %b
  %op2 = fadd float %op0, %op1
  store float %op2, float* %c
  %op3 = load float, float* %c
  ret float %op3
}
define i32 @main() {
label_main_ENTRY:
  %op0 = call float @test_float_add()
  %op1 = call i32 @test_int_add()
  %op2 = call i32 @test_mixed_expr()
  %op3 = call float @test_mixed_to_float()
  ret i32 0
}
