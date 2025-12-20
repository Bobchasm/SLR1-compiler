; ModuleID = 'sysy2022_compiler'
source_filename = "case\bd_test5_compare.sy"
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
  store i32 3, i32* %b
  %result = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = icmp sgt i32 %op0, %op1
  br i1 %op2, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store i32 1, i32* %result
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  %op3 = load i32, i32* %a
  %op4 = load i32, i32* %b
  %op5 = icmp slt i32 %op3, %op4
  br i1 %op5, label %label_if_then_2, label %label_if_end_2
label_if_then_2:                                                ; preds = %label_if_end_1
  store i32 2, i32* %result
  br label %label_if_end_2
label_if_end_2:                                                ; preds = %label_if_end_1, %label_if_then_2
  %op6 = load i32, i32* %a
  %op7 = load i32, i32* %b
  %op8 = icmp sge i32 %op6, %op7
  br i1 %op8, label %label_if_then_3, label %label_if_end_3
label_if_then_3:                                                ; preds = %label_if_end_2
  store i32 3, i32* %result
  br label %label_if_end_3
label_if_end_3:                                                ; preds = %label_if_end_2, %label_if_then_3
  %op9 = load i32, i32* %a
  %op10 = load i32, i32* %b
  %op11 = icmp sle i32 %op9, %op10
  br i1 %op11, label %label_if_then_4, label %label_if_end_4
label_if_then_4:                                                ; preds = %label_if_end_3
  store i32 4, i32* %result
  br label %label_if_end_4
label_if_end_4:                                                ; preds = %label_if_end_3, %label_if_then_4
  %op12 = load i32, i32* %a
  %op13 = load i32, i32* %b
  %op14 = icmp eq i32 %op12, %op13
  br i1 %op14, label %label_if_then_5, label %label_if_end_5
label_if_then_5:                                                ; preds = %label_if_end_4
  store i32 5, i32* %result
  br label %label_if_end_5
label_if_end_5:                                                ; preds = %label_if_end_4, %label_if_then_5
  %op15 = load i32, i32* %a
  %op16 = load i32, i32* %b
  %op17 = icmp ne i32 %op15, %op16
  br i1 %op17, label %label_if_then_6, label %label_if_end_6
label_if_then_6:                                                ; preds = %label_if_end_5
  store i32 6, i32* %result
  br label %label_if_end_6
label_if_end_6:                                                ; preds = %label_if_end_5, %label_if_then_6
  %op18 = load i32, i32* %result
  ret i32 %op18
}
