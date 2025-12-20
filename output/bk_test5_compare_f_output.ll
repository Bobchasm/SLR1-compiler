; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test5_compare_f.sy"
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
  store float 0x4048F5C3, float* %a
  %b = alloca float
  store float 0x402D70A4, float* %b
  %op0 = load float, float* %a
  %op1 = load float, float* %b
  %op2 = fcmp ogt float %op0, %op1
  br i1 %op2, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store float 0x3F99999A, float* %a
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  ret i32 0
}
