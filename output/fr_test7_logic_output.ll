; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_test7_logic.sy"
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
  %b = alloca i32
  store i32 0, i32* %b
  %c = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = load i32, i32* %a
  %op3 = sub i32 0, %op2
  %op4 = icmp ne i32 %op3, 0
  br i1 %op4, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store i32 1, i32* %c
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  %op5 = load i32, i32* %a
  %op6 = load i32, i32* %b
  %op7 = load i32, i32* %a
  %op8 = sub i32 0, %op7
  %op9 = icmp ne i32 %op8, 0
  br i1 %op9, label %label_if_then_2, label %label_if_end_2
label_if_then_2:                                                ; preds = %label_if_end_1
  store i32 2, i32* %c
  br label %label_if_end_2
label_if_end_2:                                                ; preds = %label_if_end_1, %label_if_then_2
  %op10 = load i32, i32* %a
  %op11 = load i32, i32* %b
  %op12 = load i32, i32* %a
  %op13 = sub i32 0, %op12
  %op14 = icmp ne i32 %op13, 0
  br i1 %op14, label %label_if_then_3, label %label_if_end_3
label_if_then_3:                                                ; preds = %label_if_end_2
  store i32 3, i32* %c
  br label %label_if_end_3
label_if_end_3:                                                ; preds = %label_if_end_2, %label_if_then_3
  %op15 = load i32, i32* %a
  %op16 = load i32, i32* %b
  %op17 = load i32, i32* %a
  %op18 = sub i32 0, %op17
  %op19 = icmp ne i32 %op18, 0
  br i1 %op19, label %label_if_then_4, label %label_if_end_4
label_if_then_4:                                                ; preds = %label_if_end_3
  store i32 4, i32* %c
  br label %label_if_end_4
label_if_end_4:                                                ; preds = %label_if_end_3, %label_if_then_4
  ret i32 0
}
