; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_test3_control.sy"
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
  %op8 = load i32, i32* %a
  %op9 = load i32, i32* %b
  %op10 = icmp ne i32 %op8, %op9
  br i1 %op10, label %label_if_then_3, label %label_if_end_3
label_if_then_3:                                                ; preds = %label_if_end_2
  ret i32 2
label_if_end_3:                                                ; preds = %label_if_end_2
  %op11 = load i32, i32* %a
  %op12 = load i32, i32* %b
  %op13 = icmp sge i32 %op11, %op12
  br i1 %op13, label %label_if_then_4, label %label_if_end_4
label_if_then_4:                                                ; preds = %label_if_end_3
  ret i32 3
label_if_end_4:                                                ; preds = %label_if_end_3
  %op14 = load i32, i32* %a
  %op15 = load i32, i32* %b
  %op16 = icmp sle i32 %op14, %op15
  br i1 %op16, label %label_if_then_5, label %label_if_end_5
label_if_then_5:                                                ; preds = %label_if_end_4
  ret i32 4
label_if_end_5:                                                ; preds = %label_if_end_4
  ret i32 0
}
