; ModuleID = 'sysy2022_compiler'
source_filename = "case\backend_test2_ifelse.sy"
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
  store i32 20, i32* %b
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = icmp slt i32 %op0, %op1
  br i1 %op2, label %label_if_then, label %label_if_else
label_if_then:                                                ; preds = %label_main_ENTRY
  %op3 = load i32, i32* %b
  store i32 %op3, i32* %a
  br label %label_if_end
label_if_end:                                                ; preds = %label_if_then, %label_if_else
  %op4 = load i32, i32* %a
  %op5 = load i32, i32* %b
  %op6 = icmp eq i32 %op4, %op5
  br i1 %op6, label %label_if_then, label %label_if_end
label_if_else:                                                ; preds = %label_main_ENTRY
  %op7 = load i32, i32* %a
  store i32 %op7, i32* %b
  br label %label_if_end
label_if_then:                                                ; preds = %label_if_end
  ret i32 1
label_if_end:                                                ; preds = %label_if_end
  ret i32 0
}
