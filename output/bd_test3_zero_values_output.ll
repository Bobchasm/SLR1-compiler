; ModuleID = 'sysy2022_compiler'
source_filename = "case\bd_test3_zero_values.sy"
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
  %zero = alloca i32
  store i32 0, i32* %zero
  %negative = alloca i32
  store i32 0, i32* %negative
  %positive = alloca i32
  store i32 0, i32* %positive
  %result1 = alloca i32
  %op0 = add i32 0, 0
  store i32 %op0, i32* %result1
  %result2 = alloca i32
  %op1 = mul i32 10, 0
  store i32 %op1, i32* %result2
  %result3 = alloca i32
  %op2 = sdiv i32 0, 10
  store i32 %op2, i32* %result3
  %op3 = load i32, i32* %zero
  %op4 = icmp eq i32 %op3, 0
  br i1 %op4, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store i32 1, i32* %result1
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  %op5 = load i32, i32* %zero
  %op6 = icmp ne i32 %op5, 0
  br i1 %op6, label %label_if_then_2, label %label_if_end_2
label_if_then_2:                                                ; preds = %label_if_end_1
  store i32 1, i32* %result2
  br label %label_if_end_2
label_if_end_2:                                                ; preds = %label_if_end_1, %label_if_then_2
  ret i32 0
}
