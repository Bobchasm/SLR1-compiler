; ModuleID = 'sysy2022_compiler'
source_filename = "case\test9.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define void @_(i32 %_1, i32 %_2, i32 %_3) {
label___ENTRY:
  %__1 = alloca i32
  store i32 %_1, i32* %__1
  %__2 = alloca i32
  store i32 %_2, i32* %__2
  %__3 = alloca i32
  store i32 %_3, i32* %__3
  %_a = alloca i32
  store i32 0, i32* %_a
  %_b = alloca i32
  store i32 0, i32* %_b
  %_c = alloca i32
  store i32 0, i32* %_c
  %op0 = load i32, i32* %__1
  %op1 = add i32 %op0, 1
  %op2 = icmp sgt i32 %op1, 0
  %op3 = load i32, i32* %__2
  %op4 = add i32 %op3, 2
  %op5 = icmp sgt i32 %op4, 0
  %op6 = load i32, i32* %__1
  %op7 = add i32 %op6, 1
  %op8 = icmp sgt i32 %op7, 0
  %op9 = sub i32 0, i1 %op8
  %op10 = load i32, i32* %__3
  %op11 = add i32 %op10, 3
  %op12 = icmp sgt i32 %op11, 0
  %op13 = load i32, i32* %__1
  %op14 = add i32 %op13, 1
  %op15 = icmp sgt i32 %op14, 0
  %op16 = load i32, i32* %__2
  %op17 = add i32 %op16, 2
  %op18 = icmp sgt i32 %op17, 0
  %op19 = load i32, i32* %__1
  %op20 = add i32 %op19, 1
  %op21 = icmp sgt i32 %op20, 0
  %op22 = sub i32 0, i1 %op21
  %op23 = sub i32 0, %op22
  %op24 = icmp ne i32 %op23, 0
  br i1 %op24, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label___ENTRY
  %op25 = load i32, i32* %_a
  %op26 = load i32, i32* %_b
  %op27 = add i32 %op25, %op26
  %op28 = load i32, i32* %_c
  %op29 = add i32 %op27, %op28
  %op30 = load i32, i32* %_a
  %op31 = load i32, i32* %_b
  %op32 = add i32 %op30, %op31
  %op33 = load i32, i32* %_c
  %op34 = add i32 %op32, %op33
  %op35 = sub i32 0, %op34
  %op36 = icmp eq i32 %op35, 0
  br i1 %op36, label %label_if_then_2, label %label_if_else_2
label_if_end_1:                                                ; preds = %label___ENTRY, %label_if_end_2
  %op37 = load i32, i32* %_a
  %op38 = icmp eq i32 %op37, 1
  br i1 %op38, label %label_if_then_3, label %label_if_else_3
label_if_then_2:                                                ; preds = %label_if_then_1
  ret void
label_if_end_2:
  br label %label_if_end_1
label_if_else_2:                                                ; preds = %label_if_then_1
  ret void
label_if_then_3:                                                ; preds = %label_if_end_1
  ret void
label_if_end_3:
  ret void
label_if_else_3:                                                ; preds = %label_if_end_1
  ret void
}
