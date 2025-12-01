; ModuleID = 'sysy2022_compiler'
source_filename = "case\test1.sy"
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
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = add i32 %op0, %op1
  %op3 = sub i32 %op2, 1
  store i32 %op3, i32* %a
  %op4 = load i32, i32* %a
  %op5 = icmp eq i32 %op4, 2
  br i1 %op5, label %label_if_then, label %label_if_else
label_if_then:                                                ; preds = %label_main_ENTRY
  ret i32 0
label_if_end:
  ret i32 0
label_if_else:                                                ; preds = %label_main_ENTRY
  %op6 = load i32, i32* %b
  %op7 = mul i32 %op6, 2
  %op8 = sdiv i32 %op7, 1
  %op9 = srem i32 %op8, 2
  store i32 %op9, i32* %b
  ret i32 1
}
