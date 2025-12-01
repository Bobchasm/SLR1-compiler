; ModuleID = 'sysy2022_compiler'
source_filename = "case\boundary_test4_long_expr.sy"
declare i32 @getint()

declare void @putint(i32)

declare i32 @getch()

declare void @putch(i32)

declare i32 @getarray(i32*)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @main() {
label_main_ENTRY:
  %a = alloca i32
  store i32 1, i32* %a
  %b = alloca i32
  store i32 2, i32* %b
  %c = alloca i32
  store i32 3, i32* %c
  %d = alloca i32
  store i32 4, i32* %d
  %e = alloca i32
  store i32 5, i32* %e
  %result = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = load i32, i32* %c
  %op3 = mul i32 %op1, %op2
  %op4 = add i32 %op0, %op3
  %op5 = load i32, i32* %d
  %op6 = load i32, i32* %e
  %op7 = mul i32 %op5, %op6
  %op8 = add i32 %op4, %op7
  %op9 = load i32, i32* %a
  %op10 = load i32, i32* %b
  %op11 = mul i32 %op9, %op10
  %op12 = add i32 %op8, %op11
  %op13 = load i32, i32* %c
  %op14 = load i32, i32* %d
  %op15 = mul i32 %op13, %op14
  %op16 = add i32 %op12, %op15
  %op17 = load i32, i32* %e
  %op18 = load i32, i32* %a
  %op19 = mul i32 %op17, %op18
  %op20 = add i32 %op16, %op19
  store i32 %op20, i32* %result
  %op21 = load i32, i32* %a
  %op22 = load i32, i32* %b
  %op23 = icmp slt i32 %op21, %op22
  br i1 %op23, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_main_ENTRY
  %op24 = load i32, i32* %b
  %op25 = load i32, i32* %c
  %op26 = icmp slt i32 %op24, %op25
  br i1 %op26, label %label_if_then, label %label_if_end
label_if_end:                                                ; preds = %label_main_ENTRY
  %op27 = load i32, i32* %result
  ret i32 %op27
label_if_then:                                                ; preds = %label_if_then
  %op28 = load i32, i32* %c
  %op29 = load i32, i32* %d
  %op30 = icmp slt i32 %op28, %op29
  br i1 %op30, label %label_if_then, label %label_if_end
label_if_end:                                                ; preds = %label_if_then
  ret i32 0
label_if_then:                                                ; preds = %label_if_then
  %op31 = load i32, i32* %d
  %op32 = load i32, i32* %e
  %op33 = icmp slt i32 %op31, %op32
  br i1 %op33, label %label_if_then, label %label_if_end
label_if_end:                                                ; preds = %label_if_then
  ret i32 0
label_if_then:                                                ; preds = %label_if_then
  store i32 100, i32* %result
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_then, %label_if_then
  ret i32 0
}
