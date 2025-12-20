; ModuleID = 'sysy2022_compiler'
source_filename = "case\bd_test2_max_nesting.sy"
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
  store i32 1, i32* %a
  %op0 = load i32, i32* %a
  %op1 = icmp sgt i32 %op0, 0
  br i1 %op1, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  %op2 = load i32, i32* %a
  %op3 = icmp sgt i32 %op2, 0
  br i1 %op3, label %label_if_then_2, label %label_if_end_2
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_end_2
  %op4 = load i32, i32* %a
  ret i32 %op4
label_if_then_2:                                                ; preds = %label_if_then_1
  %op5 = load i32, i32* %a
  %op6 = icmp sgt i32 %op5, 0
  br i1 %op6, label %label_if_then_3, label %label_if_end_3
label_if_end_2:                                                ; preds = %label_if_then_1, %label_if_end_3
  br label %label_if_end_1
label_if_then_3:                                                ; preds = %label_if_then_2
  %op7 = load i32, i32* %a
  %op8 = icmp sgt i32 %op7, 0
  br i1 %op8, label %label_if_then_4, label %label_if_end_4
label_if_end_3:                                                ; preds = %label_if_then_2, %label_if_end_4
  br label %label_if_end_2
label_if_then_4:                                                ; preds = %label_if_then_3
  %op9 = load i32, i32* %a
  %op10 = icmp sgt i32 %op9, 0
  br i1 %op10, label %label_if_then_5, label %label_if_end_5
label_if_end_4:                                                ; preds = %label_if_then_3, %label_if_end_5
  br label %label_if_end_3
label_if_then_5:                                                ; preds = %label_if_then_4
  store i32 100, i32* %a
  br label %label_if_end_5
label_if_end_5:                                                ; preds = %label_if_then_4, %label_if_then_5
  br label %label_if_end_4
}
