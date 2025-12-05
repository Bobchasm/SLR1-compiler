; ModuleID = 'sysy2022_compiler'
source_filename = "case\fr_complex_syntax_test.sy"
@GLOBAL_MAX = global i32 1000
@PI = global float 0x40490FD0
@ARRAY_SIZE = global i32 10
@global_counter = global i32 0
@global_rate = global float 0x3D4CCCCD
@temp = global i32 5
@pow_result = global float 0x40000000
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
  %x = alloca i32
  store i32 10, i32* %x
  %y = alloca i32
  store i32 20, i32* %y
  %z = alloca float
  store float 0x4048F5C3, float* %z
  %LOCAL_CONST = alloca i32
  store i32 42, i32* %LOCAL_CONST
  %result1 = alloca i32
  %op0 = load i32, i32* %x
  %op1 = load i32, i32* %y
  %op2 = mul i32 %op1, 2
  %op3 = add i32 %op0, %op2
  %op4 = sdiv i32 5, 2
  %op5 = sdiv i32 5, 2
  %op6 = sub i32 0, %op5
  %op7 = sub i32 %op3, %op6
  store i32 %op7, i32* %result1
  %result2 = alloca float
  %op8 = load float, float* %z
  %op9 = fadd float %op8, 0x3FC00000
  %op10 = fmul float %op9, 0x40000000
  %op11 = fdiv float %op10, 0x4048F5C3
  store float %op11, float* %result2
  %op12 = load i32, i32* %x
  %op13 = icmp sgt i32 %op12, 0
  br i1 %op13, label %label_if_then_1, label %label_if_else_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  %op14 = load i32, i32* %y
  %op15 = icmp sgt i32 %op14, 0
  br i1 %op15, label %label_if_then_2, label %label_if_else_2
label_if_end_1:                                                ; preds = %label_if_end_2, %label_if_else_1
  %op16 = load i32, i32* %result1
  %op17 = icmp eq i32 %op16, 0
  br i1 %op17, label %label_if_then_4, label %label_if_else_4
label_if_else_1:                                                ; preds = %label_main_ENTRY
  store i32 0, i32* @global_counter
  br label %label_if_end_1
label_if_then_2:                                                ; preds = %label_if_then_1
  %op18 = load float, float* %z
  %op19 = fcmp ogt float %op18, 0x00000000
  br i1 %op19, label %label_if_then_3, label %label_if_else_3
label_if_end_2:                                                ; preds = %label_if_end_3, %label_if_else_2
  br label %label_if_end_1
label_if_else_2:                                                ; preds = %label_if_then_1
  %op20 = load i32, i32* @global_counter
  %op21 = add i32 %op20, 2
  store i32 %op21, i32* @global_counter
  br label %label_if_end_2
label_if_then_3:                                                ; preds = %label_if_then_2
  %op22 = load i32, i32* @global_counter
  %op23 = add i32 %op22, 1
  store i32 %op23, i32* @global_counter
  br label %label_if_end_3
label_if_end_3:                                                ; preds = %label_if_then_3, %label_if_else_3
  br label %label_if_end_2
label_if_else_3:                                                ; preds = %label_if_then_2
  %op24 = load i32, i32* @global_counter
  %op25 = sub i32 %op24, 1
  store i32 %op25, i32* @global_counter
  br label %label_if_end_3
label_if_then_4:                                                ; preds = %label_if_end_1
  store i32 1, i32* %result1
  br label %label_if_end_4
label_if_end_4:                                                ; preds = %label_if_then_4, %label_if_end_5
  %op26 = load i32, i32* %x
  %op27 = icmp sgt i32 %op26, 0
  %op28 = load i32, i32* %y
  %op29 = icmp sgt i32 %op28, 0
  %op30 = load i32, i32* %x
  %op31 = icmp sgt i32 %op30, 0
  %op32 = sub i32 0, i1 %op31
  %op33 = load float, float* %z
  %op34 = fcmp ogt float %op33, 20
  %op35 = load i32, i32* %x
  %op36 = icmp sgt i32 %op35, 0
  %op37 = load i32, i32* %y
  %op38 = icmp sgt i32 %op37, 0
  %op39 = load i32, i32* %x
  %op40 = icmp sgt i32 %op39, 0
  %op41 = sub i32 0, i1 %op40
  %op42 = sub i32 0, %op41
  %op43 = load i32, i32* %x
  %op44 = icmp slt i32 %op43, 10
  %op45 = load i32, i32* %y
  %op46 = icmp slt i32 %op45, 5
  %op47 = load i32, i32* %x
  %op48 = icmp slt i32 %op47, 10
  %op49 = sub i32 0, i1 %op48
  %op50 = load float, float* %z
  %op51 = fcmp one float %op50, 0
  %op52 = load i32, i32* %x
  %op53 = icmp slt i32 %op52, 10
  %op54 = load i32, i32* %y
  %op55 = icmp slt i32 %op54, 5
  %op56 = load i32, i32* %x
  %op57 = icmp slt i32 %op56, 10
  %op58 = sub i32 0, i1 %op57
  %op59 = sub i32 0, %op58
  %op60 = load i32, i32* %x
  %op61 = icmp sgt i32 %op60, 0
  %op62 = load i32, i32* %y
  %op63 = icmp sgt i32 %op62, 0
  %op64 = load i32, i32* %x
  %op65 = icmp sgt i32 %op64, 0
  %op66 = sub i32 0, i1 %op65
  %op67 = load float, float* %z
  %op68 = fcmp ogt float %op67, 20
  %op69 = load i32, i32* %x
  %op70 = icmp sgt i32 %op69, 0
  %op71 = load i32, i32* %y
  %op72 = icmp sgt i32 %op71, 0
  %op73 = load i32, i32* %x
  %op74 = icmp sgt i32 %op73, 0
  %op75 = sub i32 0, i1 %op74
  %op76 = sub i32 0, %op75
  %op77 = sub i32 0, %op76
  %op78 = icmp ne i32 %op77, 0
  br i1 %op78, label %label_if_then_7, label %label_if_end_7
label_if_else_4:                                                ; preds = %label_if_end_1
  %op79 = load i32, i32* %result1
  %op80 = icmp eq i32 %op79, 1
  br i1 %op80, label %label_if_then_5, label %label_if_else_5
label_if_then_5:                                                ; preds = %label_if_else_4
  store i32 2, i32* %result1
  br label %label_if_end_5
label_if_end_5:                                                ; preds = %label_if_then_5, %label_if_end_6
  br label %label_if_end_4
label_if_else_5:                                                ; preds = %label_if_else_4
  %op81 = load i32, i32* %result1
  %op82 = icmp eq i32 %op81, 2
  br i1 %op82, label %label_if_then_6, label %label_if_else_6
label_if_then_6:                                                ; preds = %label_if_else_5
  store i32 3, i32* %result1
  br label %label_if_end_6
label_if_end_6:                                                ; preds = %label_if_then_6, %label_if_else_6
  br label %label_if_end_5
label_if_else_6:                                                ; preds = %label_if_else_5
  store i32 0, i32* %result1
  br label %label_if_end_6
label_if_then_7:                                                ; preds = %label_if_end_4
  br label %label_if_end_7
label_if_end_7:                                                ; preds = %label_if_end_4, %label_if_then_7
  %fact_result = alloca i32
  %power_result = alloca float
  %complex_expr = alloca i32
  %op83 = load i32, i32* %x
  %op84 = add i32 %op83, 1
  %op85 = mul i32 %op84, 2
  %op86 = sub i32 %op85, 3
  %op87 = sdiv i32 %op86, 4
  %op88 = load i32, i32* %x
  %op89 = add i32 %op88, 1
  %op90 = mul i32 %op89, 2
  %op91 = sub i32 %op90, 3
  %op92 = sdiv i32 %op91, 4
  %op93 = sub i32 0, %op92
  store i32 %op93, i32* %complex_expr
  %op94 = load i32, i32* %x
  %op95 = icmp sge i32 %op94, 0
  %op96 = load i32, i32* %y
  %op97 = icmp sle i32 %op96, 100
  %op98 = load i32, i32* %x
  %op99 = icmp sge i32 %op98, 0
  %op100 = sub i32 0, i1 %op99
  %op101 = icmp ne i32 %op100, 0
  br i1 %op101, label %label_if_then_8, label %label_if_end_8
label_if_then_8:                                                ; preds = %label_if_end_7
  %op102 = load float, float* @global_rate
  %op103 = fadd float %op102, 0x3C23D70A
  store float %op103, float* @global_rate
  br label %label_if_end_8
label_if_end_8:                                                ; preds = %label_if_end_7, %label_if_then_8
  %op104 = load i32, i32* %x
  %op105 = icmp sgt i32 %op104, 0
  %op106 = load i32, i32* %y
  %op107 = icmp slt i32 %op106, 0
  %op108 = load i32, i32* %x
  %op109 = icmp sgt i32 %op108, 0
  %op110 = sub i32 0, i1 %op109
  %op111 = icmp ne i32 %op110, 0
  br i1 %op111, label %label_if_then_9, label %label_if_end_9
label_if_then_9:                                                ; preds = %label_if_end_8
  %op112 = load i32, i32* @global_counter
  %op113 = add i32 %op112, 1
  store i32 %op113, i32* @global_counter
  br label %label_if_end_9
label_if_end_9:                                                ; preds = %label_if_end_8, %label_if_then_9
  %op114 = load i32, i32* %fact_result
  %op115 = load float, float* %power_result
  %op116 = fadd float %op114, %op115
  ret float %op116
}
define i32 @factorial(i32 %n) {
label_factorial_ENTRY:
  %_n = alloca i32
  store i32 %n, i32* %_n
  %op0 = load i32, i32* %_n
  %op1 = icmp sle i32 %op0, 1
  br i1 %op1, label %label_if_then_10, label %label_if_else_10
label_if_then_10:                                                ; preds = %label_factorial_ENTRY
  ret i32 1
label_if_end_10:
  ret i32 0
label_if_else_10:                                                ; preds = %label_factorial_ENTRY
  %op2 = load i32, i32* %_n
  %op3 = load i32, i32* %_n
  %op4 = sub i32 %op3, 1
  %op5 = call i32 @factorial(i32 %op4)
  %op6 = mul i32 %op2, %op5
  ret i32 %op6
}
define float @power(float %base, i32 %exp) {
label_power_ENTRY:
  %_base = alloca float
  store float %base, float* %_base
  %_exp = alloca i32
  store i32 %exp, i32* %_exp
  %op0 = load i32, i32* %_exp
  %op1 = icmp eq i32 %op0, 0
  br i1 %op1, label %label_if_then_11, label %label_if_else_11
label_if_then_11:                                                ; preds = %label_power_ENTRY
  ret float 0x3F800000
label_if_end_11:                                                ; preds = %label_if_end_12
  %op2 = load float, float* %result
  ret float %op2
label_if_else_11:                                                ; preds = %label_power_ENTRY
  %op3 = load i32, i32* %_exp
  %op4 = icmp sgt i32 %op3, 0
  br i1 %op4, label %label_if_then_12, label %label_if_else_12
label_if_then_12:                                                ; preds = %label_if_else_11
  %result = alloca float
  store float 0x3F800000, float* %result
  %i = alloca i32
  store i32 0, i32* %i
  %op5 = load float, float* %result
  ret float %op5
label_if_end_12:
  br label %label_if_end_11
label_if_else_12:                                                ; preds = %label_if_else_11
  %op6 = load float, float* %_base
  %op7 = load i32, i32* %_exp
  %op8 = sub i32 0, %op7
  %op9 = call float @power(float %op6, i32 %op8)
  %op10 = fdiv float 0x3F800000, %op9
  ret float %op10
}
define void @swap(i32 %a, i32 %b) {
label_swap_ENTRY:
  %_a = alloca i32
  store i32 %a, i32* %_a
  %_b = alloca i32
  store i32 %b, i32* %_b
  %temp = alloca i32
  %op0 = load i32, i32* %_a
  store i32 %op0, i32* %temp
  %op1 = load i32, i32* %_b
  store i32 %op1, i32* %_a
  %op2 = load i32, i32* %temp
  store i32 %op2, i32* %_b
  ret void
}
define i32 @complex_calculation(i32 %a, i32 %b, float %c, i32 %d) {
label_complex_calculation_ENTRY:
  %_a = alloca i32
  store i32 %a, i32* %_a
  %_b = alloca i32
  store i32 %b, i32* %_b
  %_c = alloca float
  store float %c, float* %_c
  %_d = alloca i32
  store i32 %d, i32* %_d
  %multiplier = alloca float
  store float 0x40200000, float* %multiplier
  %result = alloca i32
  store i32 0, i32* %result
  %op0 = load i32, i32* %_a
  %op1 = icmp sgt i32 %op0, 0
  br i1 %op1, label %label_if_then_13, label %label_if_else_13
label_if_then_13:                                                ; preds = %label_complex_calculation_ENTRY
  %op2 = load i32, i32* %_b
  %op3 = icmp sgt i32 %op2, 0
  br i1 %op3, label %label_if_then_14, label %label_if_else_14
label_if_end_13:                                                ; preds = %label_if_end_14, %label_if_else_13
  %op4 = load i32, i32* %result
  ret i32 %op4
label_if_else_13:                                                ; preds = %label_complex_calculation_ENTRY
  store i32 0, i32* %result
  br label %label_if_end_13
label_if_then_14:                                                ; preds = %label_if_then_13
  %op5 = load float, float* %_c
  %op6 = fcmp ogt float %op5, 0x00000000
  br i1 %op6, label %label_if_then_15, label %label_if_else_15
label_if_end_14:                                                ; preds = %label_if_end_15, %label_if_else_14
  br label %label_if_end_13
label_if_else_14:                                                ; preds = %label_if_then_13
  %op7 = load i32, i32* %_a
  %op8 = load i32, i32* %_b
  %op9 = mul i32 %op7, %op8
  %op10 = load i32, i32* %_d
  %op11 = mul i32 %op9, %op10
  store i32 %op11, i32* %result
  br label %label_if_end_14
label_if_then_15:                                                ; preds = %label_if_then_14
  %op12 = load i32, i32* %_a
  %op13 = load i32, i32* %_b
  %op14 = add i32 %op12, %op13
  %op15 = load float, float* %_c
  %op16 = fmul float %op14, %op15
  %op17 = load i32, i32* %_d
  %op18 = fmul float %op16, %op17
  store float %op18, i32* %result
  br label %label_if_end_15
label_if_end_15:                                                ; preds = %label_if_then_15, %label_if_else_15
  br label %label_if_end_14
label_if_else_15:                                                ; preds = %label_if_then_14
  %op19 = load i32, i32* %_a
  %op20 = load i32, i32* %_b
  %op21 = sub i32 %op19, %op20
  %op22 = load i32, i32* %_d
  %op23 = sdiv i32 %op21, %op22
  store i32 %op23, i32* %result
  br label %label_if_end_15
}
define void @test_expressions() {
label_test_expressions_ENTRY:
  %a = alloca i32
  store i32 10, i32* %a
  %b = alloca i32
  store i32 20, i32* %b
  %c = alloca float
  store float 0x4048F5C3, float* %c
  %result1 = alloca i32
  %op0 = load i32, i32* %a
  %op1 = load i32, i32* %b
  %op2 = load float, float* %c
  %op3 = call i32 @complex_calculation(i32 %op0, i32 %op1, float %op2, i32 5)
  store i32 %op3, i32* %result1
  %result2 = alloca i32
  %op4 = call i32 @complex_calculation(i32 1, i32 2, float 0x3FC00000, i32 3)
  %op5 = call i32 @factorial(i32 %op4)
  store i32 %op5, i32* %result2
  %op6 = load i32, i32* %a
  %op7 = load i32, i32* %b
  %op8 = add i32 %op6, %op7
  %op9 = icmp sgt i32 %op8, 25
  br i1 %op9, label %label_if_then_16, label %label_if_end_16
label_if_then_16:                                                ; preds = %label_test_expressions_ENTRY
  %op10 = load i32, i32* @global_counter
  %op11 = load i32, i32* %result1
  %op12 = add i32 %op10, %op11
  store i32 %op12, i32* @global_counter
  br label %label_if_end_16
label_if_end_16:                                                ; preds = %label_test_expressions_ENTRY, %label_if_then_16
  ret void
}
