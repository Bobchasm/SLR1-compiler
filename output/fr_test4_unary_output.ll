; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_test4_unary.sy"
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
  %op0 = load i32, i32* %a
  %op1 = sub i32 0, %op0
  store i32 %op1, i32* %b
  %c = alloca i32
  %op2 = load i32, i32* %a
  store i32 %op2, i32* %c
  %op3 = icmp ne i32 0, 0
  br i1 %op3, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store i32 1, i32* %a
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  %op4 = load i32, i32* %a
  %op5 = icmp ne i32 %op4, 0
  br i1 %op5, label %label_if_then_2, label %label_if_end_2
label_if_then_2:                                                ; preds = %label_if_end_1
  store i32 2, i32* %b
  br label %label_if_end_2
label_if_end_2:                                                ; preds = %label_if_end_1, %label_if_then_2
  ret i32 0
}
