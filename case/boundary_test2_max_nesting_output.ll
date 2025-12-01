; ModuleID = 'sysy2022_compiler'
source_filename = "case\boundary_test2_max_nesting.sy"
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
  %op0 = load i32, i32* %a
  %op1 = icmp sgt i32 %op0, 0
  br i1 %op1, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_main_ENTRY
  %op2 = load i32, i32* %a
  %op3 = icmp sgt i32 %op2, 0
  br i1 %op3, label %label_if_then, label %label_if_end
label_if_end:                                                ; preds = %label_main_ENTRY
  %op4 = load i32, i32* %a
  ret i32 %op4
label_if_then:                                                ; preds = %label_if_then
  %op5 = load i32, i32* %a
  %op6 = icmp sgt i32 %op5, 0
  br i1 %op6, label %label_if_then, label %label_if_end
label_if_end:                                                ; preds = %label_if_then
  ret i32 0
label_if_then:                                                ; preds = %label_if_then
  %op7 = load i32, i32* %a
  %op8 = icmp sgt i32 %op7, 0
  br i1 %op8, label %label_if_then, label %label_if_end
label_if_end:                                                ; preds = %label_if_then
  ret i32 0
label_if_then:                                                ; preds = %label_if_then
  %op9 = load i32, i32* %a
  %op10 = icmp sgt i32 %op9, 0
  br i1 %op10, label %label_if_then, label %label_if_end
label_if_end:                                                ; preds = %label_if_then
  ret i32 0
label_if_then:                                                ; preds = %label_if_then
  store i32 100, i32* %a
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_then, %label_if_then
  ret i32 0
}
