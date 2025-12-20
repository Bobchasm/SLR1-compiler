; ModuleID = 'sysy2022_compiler'
source_filename = "case\kb_test12_func_argv.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @add(i32 %a, i32 %b) {
label_add_ENTRY:
  %_a = alloca i32
  store i32 %a, i32* %_a
  %_b = alloca i32
  store i32 %b, i32* %_b
  %op0 = load i32, i32* %_a
  %op1 = add i32 %op0, 1
  store i32 %op1, i32* %_a
  %op2 = load i32, i32* %_a
  %op3 = load i32, i32* %_b
  %op4 = add i32 %op2, %op3
  ret i32 %op4
}
define i32 @main() {
label_main_ENTRY:
  %c = alloca float
  store float 0x3FA66666, float* %c
  %d = alloca i32
  store i32 1, i32* %d
  %op0 = load float, float* %c
  %op1 = load i32, i32* %d
  %op2 = call i32 @add(float %op0, i32 %op1)
  ret i32 %op2
}
