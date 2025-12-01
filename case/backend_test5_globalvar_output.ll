; ModuleID = 'sysy2022_compiler'
source_filename = "case\backend_test5_globalvar.sy"
@global_a = global i32 100
@global_a = global i32 100
@global_b = global i32 200
@global_b = global i32 200
declare i32 @getint()

declare void @putint(i32)

declare i32 @getch()

declare void @putch(i32)

declare i32 @getarray(i32*)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define i32 @get_sum() {
label_get_sum_ENTRY:
  %op0 = load i32, i32* @global_a
  %op1 = load i32, i32* @global_b
  %op2 = add i32 %op0, %op1
  ret i32 %op2
}
define i32 @main() {
label_main_ENTRY:
  %local = alloca i32
  %op0 = call i32 @get_sum()
  store i32 %op0, i32* %local
  store i32 50, i32* @global_a
  %op1 = load i32, i32* %local
  %op2 = icmp sgt i32 %op1, 250
  br i1 %op2, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_main_ENTRY
  ret i32 1
label_if_end:                                                ; preds = %label_main_ENTRY
  ret i32 0
}
