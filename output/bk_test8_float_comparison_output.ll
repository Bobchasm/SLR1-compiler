; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test8_float_comparison.sy"
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
  %a = alloca float
  store float 0x3DCCCCCD, float* %a
  %b = alloca float
  store float 0x3E4CCCCD, float* %b
  %c = alloca float
  store float 0x3E99999A, float* %c
  %op0 = load float, float* %a
  %op1 = load float, float* %b
  %op2 = fadd float %op0, %op1
  %op3 = load float, float* %c
  %op4 = fcmp oeq float %op2, %op3
  br i1 %op4, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  ret void
}
