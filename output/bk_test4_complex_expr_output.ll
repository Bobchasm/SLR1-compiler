; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test4_complex_expr.sy"
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
  store i32 5, i32* %a
  %b = alloca i32
  store i32 10, i32* %b
  %c = alloca i32
  store i32 15, i32* %c
  %d = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = load i32, i32* %c
  %op3 = mul i32 %op1, %op2
  %op4 = add i32 %op0, %op3
  store i32 %op4, i32* %d
  %op5 = load i32, i32* %a
  %op6 = load i32, i32* %b
  %op7 = add i32 %op5, %op6
  %op8 = load i32, i32* %c
  %op9 = mul i32 %op7, %op8
  store i32 %op9, i32* %d
  %op10 = load i32, i32* %a
  %op11 = load i32, i32* %b
  %op12 = mul i32 %op10, %op11
  %op13 = load i32, i32* %c
  %op14 = add i32 %op12, %op13
  store i32 %op14, i32* %d
  %op15 = load i32, i32* %a
  %op16 = load i32, i32* %b
  %op17 = sdiv i32 %op15, %op16
  %op18 = load i32, i32* %c
  %op19 = load i32, i32* %d
  %op20 = mul i32 %op18, %op19
  %op21 = add i32 %op17, %op20
  store i32 %op21, i32* %d
  %op22 = load i32, i32* %d
  %op23 = icmp sgt i32 %op22, 100
  br i1 %op23, label %label_if_then_1, label %label_if_else_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store i32 100, i32* %d
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_if_then_1, %label_if_end_2
  %op24 = load i32, i32* %d
  ret i32 %op24
label_if_else_1:                                                ; preds = %label_main_ENTRY
  %op25 = load i32, i32* %d
  %op26 = icmp sgt i32 %op25, 50
  br i1 %op26, label %label_if_then_2, label %label_if_else_2
label_if_then_2:                                                ; preds = %label_if_else_1
  store i32 50, i32* %d
  br label %label_if_end_2
label_if_end_2:                                                ; preds = %label_if_then_2, %label_if_else_2
  br label %label_if_end_1
label_if_else_2:                                                ; preds = %label_if_else_1
  store i32 0, i32* %d
  br label %label_if_end_2
}
