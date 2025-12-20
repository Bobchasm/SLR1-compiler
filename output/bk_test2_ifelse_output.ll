; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test2_ifelse.sy"
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
  store i32 10, i32* %a
  %b = alloca i32
  store i32 20, i32* %b
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = icmp slt i32 %op0, %op1
  br i1 %op2, label %label_if_then_1, label %label_if_else_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  %op3 = load i32, i32* %b
  store i32 %op3, i32* %a
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_if_then_1, %label_if_else_1
  %op4 = load i32, i32* %a
  %op5 = load i32, i32* %b
  %op6 = icmp eq i32 %op4, %op5
  br i1 %op6, label %label_if_then_2, label %label_if_end_2
label_if_else_1:                                                ; preds = %label_main_ENTRY
  %op7 = load i32, i32* %a
  store i32 %op7, i32* %b
  br label %label_if_end_1
label_if_then_2:                                                ; preds = %label_if_end_1
  ret i32 1
label_if_end_2:                                                ; preds = %label_if_end_1
  ret i32 0
}
