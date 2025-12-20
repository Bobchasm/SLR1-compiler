; ModuleID = 'sysy2022_compiler'
source_filename = "case\_test.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @_intMainFunc(i32 %_1, i32 %_2, i32 %_3) {
label__intMainFunc_ENTRY:
  %__1 = alloca i32
  store i32 %_1, i32* %__1
  %__2 = alloca i32
  store i32 %_2, i32* %__2
  %__3 = alloca i32
  store i32 %_3, i32* %__3
  %_4 = alloca i32
  store i32 0, i32* %_4
  %op0 = load i32, i32* %__1
  %op1 = icmp ne i32 %op0, 0
  %op2 = load i32, i32* %_4
  %op3 = load i32, i32* %_4
  %op4 = add i32 %op3, 1
  %op5 = icmp eq i32 %op2, %op4
  %op6 = icmp sgt i1 %op5, i32 0
  %op7 = load i32, i32* %__1
  %op8 = icmp ne i32 %op7, 0
  %op9 = sub i32 0, i1 %op8
  %op10 = icmp ne i32 %op9, 0
  br i1 %op10, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label__intMainFunc_ENTRY
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label__intMainFunc_ENTRY, %label_if_then_1
  %op11 = load i32, i32* %__1
  %op12 = icmp eq i32 %op11, 0
  %op13 = load i32, i32* %_4
  %op14 = load i32, i32* %_4
  %op15 = add i32 %op14, 1
  %op16 = icmp eq i32 %op13, %op15
  %op17 = icmp sgt i1 %op16, i32 0
  %op18 = load i32, i32* %__1
  %op19 = icmp eq i32 %op18, 0
  %op20 = sub i32 0, i1 %op19
  %op21 = icmp ne i32 %op20, 0
  br i1 %op21, label %label_if_then_2, label %label_if_end_2
label_if_then_2:                                                ; preds = %label_if_end_1
  br label %label_if_end_2
label_if_end_2:                                                ; preds = %label_if_end_1, %label_if_then_2
  %op22 = load i32, i32* %__2
  %op23 = load i32, i32* %__3
  %op24 = icmp sgt i32 %op22, %op23
  %op25 = load i32, i32* %_4
  %op26 = load i32, i32* %_4
  %op27 = add i32 %op26, 10
  %op28 = icmp eq i32 %op25, %op27
  %op29 = icmp sgt i1 %op28, i32 0
  %op30 = load i32, i32* %__2
  %op31 = load i32, i32* %__3
  %op32 = icmp sgt i32 %op30, %op31
  %op33 = sub i32 0, i1 %op32
  %op34 = load i32, i32* %__1
  %op35 = icmp eq i32 %op34, 7
  %op36 = load i32, i32* %__2
  %op37 = load i32, i32* %__3
  %op38 = icmp sgt i32 %op36, %op37
  %op39 = load i32, i32* %_4
  %op40 = load i32, i32* %_4
  %op41 = add i32 %op40, 10
  %op42 = icmp eq i32 %op39, %op41
  %op43 = icmp sgt i1 %op42, i32 0
  %op44 = load i32, i32* %__2
  %op45 = load i32, i32* %__3
  %op46 = icmp sgt i32 %op44, %op45
  %op47 = sub i32 0, i1 %op46
  %op48 = sub i32 0, %op47
  %op49 = icmp ne i32 %op48, 0
  br i1 %op49, label %label_if_then_3, label %label_if_end_3
label_if_then_3:                                                ; preds = %label_if_end_2
  br label %label_if_end_3
label_if_end_3:                                                ; preds = %label_if_end_2, %label_if_then_3
  %op50 = load i32, i32* %_4
  ret i32 %op50
}
