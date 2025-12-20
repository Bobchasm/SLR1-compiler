; ModuleID = 'sysy2022_compiler'
source_filename = "case\bk_test24_complex_logic.sy"
@temp = global i32 1
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
  store i32 2, i32* %c
  %result1 = alloca i32
  %op0 = load i32, i32* %a
  %op1 = icmp sgt i32 %op0, 0
  %op2 = load i32, i32* %b
  %op3 = icmp sge i32 %op2, 0
  %op4 = load i32, i32* %a
  %op5 = icmp sgt i32 %op4, 0
  %op6 = sub i32 0, i1 %op5
  %op7 = load i32, i32* %c
  %op8 = icmp sle i32 %op7, 2
  %op9 = load i32, i32* %a
  %op10 = load i32, i32* %b
  %op11 = icmp ne i32 %op9, %op10
  %op12 = load i32, i32* %c
  %op13 = icmp sle i32 %op12, 2
  %op14 = sub i32 0, i1 %op13
  %op15 = load i32, i32* %a
  %op16 = icmp sgt i32 %op15, 0
  %op17 = load i32, i32* %b
  %op18 = icmp sge i32 %op17, 0
  %op19 = load i32, i32* %a
  %op20 = icmp sgt i32 %op19, 0
  %op21 = sub i32 0, i1 %op20
  %op22 = sub i32 0, %op21
  %op23 = icmp ne i32 %op22, 0
  br i1 %op23, label %label_if_then_1, label %label_if_end_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  store i32 10, i32* %result1
  br label %label_if_end_1
label_if_end_1:                                                ; preds = %label_main_ENTRY, %label_if_then_1
  %op24 = load i32, i32* %a
  %op25 = load i32, i32* %b
  %op26 = load i32, i32* %a
  %op27 = sub i32 0, %op26
  %op28 = load i32, i32* %c
  %op29 = icmp sgt i32 %op28, 0
  %op30 = load i32, i32* %a
  %op31 = load i32, i32* %b
  %op32 = load i32, i32* %a
  %op33 = sub i32 0, %op32
  %op34 = sub i32 0, %op33
  %op35 = icmp ne i32 %op34, 0
  br i1 %op35, label %label_if_then_2, label %label_if_else_2
label_if_then_2:                                                ; preds = %label_if_end_1
  ret i32 1
label_if_end_2:
  %x = alloca i32
  store i32 5, i32* %x
  %y = alloca i32
  store i32 10, i32* %y
  %z = alloca i32
  store i32 15, i32* %z
  %op36 = load i32, i32* %x
  %op37 = icmp sgt i32 %op36, 0
  %op38 = load i32, i32* %y
  %op39 = icmp sgt i32 %op38, 0
  %op40 = load i32, i32* %x
  %op41 = icmp sgt i32 %op40, 0
  %op42 = sub i32 0, i1 %op41
  %op43 = load i32, i32* %z
  %op44 = icmp sgt i32 %op43, 20
  %op45 = load i32, i32* %x
  %op46 = icmp sgt i32 %op45, 0
  %op47 = load i32, i32* %y
  %op48 = icmp sgt i32 %op47, 0
  %op49 = load i32, i32* %x
  %op50 = icmp sgt i32 %op49, 0
  %op51 = sub i32 0, i1 %op50
  %op52 = sub i32 0, %op51
  %op53 = load i32, i32* %x
  %op54 = icmp slt i32 %op53, 10
  %op55 = load i32, i32* %y
  %op56 = icmp slt i32 %op55, 5
  %op57 = load i32, i32* %x
  %op58 = icmp slt i32 %op57, 10
  %op59 = sub i32 0, i1 %op58
  %op60 = load i32, i32* %z
  %op61 = icmp ne i32 %op60, 0
  %op62 = load i32, i32* %x
  %op63 = icmp slt i32 %op62, 10
  %op64 = load i32, i32* %y
  %op65 = icmp slt i32 %op64, 5
  %op66 = load i32, i32* %x
  %op67 = icmp slt i32 %op66, 10
  %op68 = sub i32 0, i1 %op67
  %op69 = sub i32 0, %op68
  %op70 = load i32, i32* %x
  %op71 = icmp sgt i32 %op70, 0
  %op72 = load i32, i32* %y
  %op73 = icmp sgt i32 %op72, 0
  %op74 = load i32, i32* %x
  %op75 = icmp sgt i32 %op74, 0
  %op76 = sub i32 0, i1 %op75
  %op77 = load i32, i32* %z
  %op78 = icmp sgt i32 %op77, 20
  %op79 = load i32, i32* %x
  %op80 = icmp sgt i32 %op79, 0
  %op81 = load i32, i32* %y
  %op82 = icmp sgt i32 %op81, 0
  %op83 = load i32, i32* %x
  %op84 = icmp sgt i32 %op83, 0
  %op85 = sub i32 0, i1 %op84
  %op86 = sub i32 0, %op85
  %op87 = sub i32 0, %op86
  %op88 = icmp ne i32 %op87, 0
  br i1 %op88, label %label_if_then_3, label %label_if_end_3
label_if_else_2:                                                ; preds = %label_if_end_1
  ret i32 0
label_if_then_3:                                                ; preds = %label_if_end_2
  br label %label_if_end_3
label_if_end_3:                                                ; preds = %label_if_end_2, %label_if_then_3
  ret i32 0
}
