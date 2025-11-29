; ModuleID = 'sysy2022_compiler'
source_filename = "complex_test.sy"
declare i32 @getint()

declare void @putint(i32)

declare i32 @getch()

declare void @putch(i32)

declare i32 @getarray(i32*)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @main() {
label_main_entry:
  %a = alloca i32
  store i32 1, i32* %a
  %b = alloca i32
  store i32 2, i32* %b
  %c = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = mul i32 %op1, 3
  %op3 = add i32 %op0, %op2
  store i32 %op3, i32* %c
  %op4 = load i32, i32* %c
  %op5 = icmp sgt i32 %op4, 5
  br i1 %op5, label %label_op0_then, label %label_op2_else
label_op0_then:                                                ; preds = %label_main_entry
  ret i32 1
label_op1_merge:
  ret i32 0
label_op2_else:                                                ; preds = %label_main_entry
  ret i32 0
}
