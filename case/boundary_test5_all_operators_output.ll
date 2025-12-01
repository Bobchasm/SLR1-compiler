; ModuleID = 'sysy2022_compiler'
source_filename = "case\boundary_test5_all_operators.sy"
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
  %op14 = srem i32 %op12, %op13
  store i32 %op14, i32* %result
  %op15 = load i32, i32* %a
  %op16 = load i32, i32* %b
  %op17 = icmp sgt i32 %op15, %op16
  br i1 %op17, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_main_ENTRY
  store i32 1, i32* %result
  br label %label_if_end
label_if_end:                                                ; preds = %label_main_ENTRY, %label_if_then
  %op18 = load i32, i32* %a
  %op19 = load i32, i32* %b
  %op20 = icmp slt i32 %op18, %op19
  br i1 %op20, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_if_end
  store i32 2, i32* %result
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_end, %label_if_then
  %op21 = load i32, i32* %a
  %op22 = load i32, i32* %b
  %op23 = icmp sge i32 %op21, %op22
  br i1 %op23, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_if_end
  store i32 3, i32* %result
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_end, %label_if_then
  %op24 = load i32, i32* %a
  %op25 = load i32, i32* %b
  %op26 = icmp sle i32 %op24, %op25
  br i1 %op26, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_if_end
  store i32 4, i32* %result
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_end, %label_if_then
  %op27 = load i32, i32* %a
  %op28 = load i32, i32* %b
  %op29 = icmp eq i32 %op27, %op28
  br i1 %op29, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_if_end
  store i32 5, i32* %result
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_end, %label_if_then
  %op30 = load i32, i32* %a
  %op31 = load i32, i32* %b
  %op32 = icmp ne i32 %op30, %op31
  br i1 %op32, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_if_end
  store i32 6, i32* %result
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_end, %label_if_then
  %op33 = load i32, i32* %result
  ret i32 %op33
}
