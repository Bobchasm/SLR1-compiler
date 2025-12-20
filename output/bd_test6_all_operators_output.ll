; ModuleID = 'sysy2022_compiler'
source_filename = "case\bd_test6_all_operators.sy"
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
  %op2 = add i32 %op0, %op1
  store i32 %op2, i32* %result
  %op3 = load i32, i32* %a
  %op4 = load i32, i32* %b
  %op5 = sub i32 %op3, %op4
  store i32 %op5, i32* %result
  %op6 = load i32, i32* %a
  %op7 = load i32, i32* %b
  %op8 = mul i32 %op6, %op7
  store i32 %op8, i32* %result
  %op9 = load i32, i32* %a
  %op10 = load i32, i32* %b
  %op11 = sdiv i32 %op9, %op10
  store i32 %op11, i32* %result
  %op12 = load i32, i32* %a
  %op13 = load i32, i32* %b
  %op14 = load i32, i32* %a
  %op15 = sub i32 0, %op14
  store i32 %op15, i32* %result
  %op16 = load i32, i32* %a
  %op17 = load i32, i32* %b
  %op18 = icmp sgt i32 %op16, %op17
  br i1 %op18, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store i32 1, i32* %result
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  %op19 = load i32, i32* %a
  %op20 = load i32, i32* %b
  %op21 = icmp slt i32 %op19, %op20
  br i1 %op21, label %label_if_then_2, label %label_if_end_2
label_if_then_2:                                                ; preds = %label_if_end_1
  store i32 2, i32* %result
  br label %label_if_end_2
label_if_end_2:                                                ; preds = %label_if_end_1, %label_if_then_2
  %op22 = load i32, i32* %a
  %op23 = load i32, i32* %b
  %op24 = icmp sge i32 %op22, %op23
  br i1 %op24, label %label_if_then_3, label %label_if_end_3
label_if_then_3:                                                ; preds = %label_if_end_2
  store i32 3, i32* %result
  br label %label_if_end_3
label_if_end_3:                                                ; preds = %label_if_end_2, %label_if_then_3
  %op25 = load i32, i32* %a
  %op26 = load i32, i32* %b
  %op27 = icmp sle i32 %op25, %op26
  br i1 %op27, label %label_if_then_4, label %label_if_end_4
label_if_then_4:                                                ; preds = %label_if_end_3
  store i32 4, i32* %result
  br label %label_if_end_4
label_if_end_4:                                                ; preds = %label_if_end_3, %label_if_then_4
  %op28 = load i32, i32* %a
  %op29 = load i32, i32* %b
  %op30 = icmp eq i32 %op28, %op29
  br i1 %op30, label %label_if_then_5, label %label_if_end_5
label_if_then_5:                                                ; preds = %label_if_end_4
  store i32 5, i32* %result
  br label %label_if_end_5
label_if_end_5:                                                ; preds = %label_if_end_4, %label_if_then_5
  %op31 = load i32, i32* %a
  %op32 = load i32, i32* %b
  %op33 = icmp ne i32 %op31, %op32
  br i1 %op33, label %label_if_then_6, label %label_if_end_6
label_if_then_6:                                                ; preds = %label_if_end_5
  store i32 6, i32* %result
  br label %label_if_end_6
label_if_end_6:                                                ; preds = %label_if_end_5, %label_if_then_6
  %op34 = load i32, i32* %result
  ret i32 %op34
}
