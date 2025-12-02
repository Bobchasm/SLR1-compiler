; ModuleID = 'sysy2022_compiler'
source_filename = ".\case\testCon.sy"
declare i32 @getint()

declare i32 @getch()

declare i32 @getarray(i32*)

declare void @putint(i32)

declare void @putch(i32)

declare void @putarray(i32, i32*)

declare void @starttime()

declare void @stoptime()

define void @main() {
label_main_ENTRY:
  %a = alloca float
  store float 0.1, float* %a
  %b = alloca float
  store float 0.2, float* %b
  %c = alloca float
  store float 0.3, float* %c
  %op0 = load float, float* %a
  %op1 = load float, float* %b
  %op2 = fadd float %op0, %op1
  %op3 = load float, float* %c
  %op4 = fcmp oeq float %op2, %op3
  br i1 %op4, label %label_if_then, label %label_if_end
label_if_then:                                                ; preds = %label_main_ENTRY
  br label %label_if_end
label_if_end:                                                ; preds = %label_main_ENTRY, %label_if_then
  ret void
}
