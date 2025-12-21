; ModuleID = 'sysy2022_compiler'
source_filename = ".\case\fr_complex_syntax_test.sy"
@GLOBAL_MAX = global i32 1000
@PI = global float 0x40490FD0
@ARRAY_SIZE = global i32 10
@global_counter = global i32 0
@global_rate = global float 0x3D4CCCCD
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
  %result2 = alloca float
  %op5 = load float, float* %z
  %op6 = fadd float %op5, 0x3FC00000
  %op7 = fmul float %op6, 0x40000000
  %op8 = fdiv float %op7, 0x4048F5C3
  store float %op8, float* %result2
  %op9 = load i32, i32* %x
  %op10 = icmp sgt i32 %op9, 0
  br i1 %op10, label %label_if_then_1, label %label_if_else_1
label_if_then_1:                                                ; preds = %label_main_ENTRY
  %op11 = load i32, i32* %y
  %op12 = icmp sgt i32 %op11, 0
  br i1 %op12, label %label_if_then_2, label %label_if_else_2
label_if_end_1:                                                ; preds = %label_if_end_2, %label_if_else_1
  %op13 = load i32, i32* %result1
  %op14 = icmp eq i32 %op13, 0
  br i1 %op14, label %label_if_then_4, label %label_if_else_4
label_if_else_1:                                                ; preds = %label_main_ENTRY
  store i32 0, i32* @global_counter
  br label %label_if_end_1
label_if_then_2:                                                ; preds = %label_if_then_1
  %op15 = load float, float* %z
  %op16 = fcmp ogt float %op15, 0x00000000
  br i1 %op16, label %label_if_then_3, label %label_if_else_3
label_if_end_2:                                                ; preds = %label_if_end_3, %label_if_else_2
  br label %label_if_end_1
label_if_else_2:                                                ; preds = %label_if_then_1
  %op17 = load i32, i32* @global_counter
  %op18 = add i32 %op17, 2
  store i32 %op18, i32* @global_counter
  br label %label_if_end_2
label_if_then_3:                                                ; preds = %label_if_then_2
  %op19 = load i32, i32* @global_counter
  %op20 = add i32 %op19, 1
  store i32 %op20, i32* @global_counter
  br label %label_if_end_3
label_if_end_3:                                                ; preds = %label_if_then_3, %label_if_else_3
  br label %label_if_end_2
label_if_else_3:                                                ; preds = %label_if_then_2
  %op21 = load i32, i32* @global_counter
  %op22 = sub i32 %op21, 1
  store i32 %op22, i32* @global_counter
  br label %label_if_end_3
label_if_then_4:                                                ; preds = %label_if_end_1
  store i32 1, i32* %result1
  br label %label_if_end_4
label_if_end_4:                                                ; preds = %label_if_then_4, %label_if_end_5
  %op23 = load i32, i32* %x
  %op24 = icmp sgt i32 %op23, 0
  %op25 = load i32, i32* %y
  %op26 = icmp sgt i32 %op25, 0
  %op27 = load float, float* %z
  %op28 = fcmp ogt float %op27, 20
  %op29 = load i32, i32* %x
  %op30 = icmp slt i32 %op29, 10
  %op31 = load i32, i32* %y
  %op32 = icmp slt i32 %op31, 5
  %op33 = load float, float* %z
  %op34 = fcmp one float %op33, 0
  %fact_result = alloca i32
  %power_result = alloca float
  %complex_expr = alloca i32
  %op35 = load i32, i32* %x
  %op36 = add i32 %op35, 1
  %op37 = mul i32 %op36, 2
  %op38 = sub i32 %op37, 3
  %op39 = sdiv i32 %op38, 4
  %op40 = load i32, i32* %x
  %op41 = icmp sge i32 %op40, 0
  %op42 = load i32, i32* %y
  %op43 = icmp sle i32 %op42, 100
  %op44 = load i32, i32* %x
  %op45 = icmp sgt i32 %op44, 0
  %op46 = load i32, i32* %y
  %op47 = icmp slt i32 %op46, 0
  %op48 = load i32, i32* %fact_result
  %op49 = load float, float* %power_result
  %op50 = fadd float %op48, %op49
  ret float %op50
label_if_else_4:                                                ; preds = %label_if_end_1
  %op51 = load i32, i32* %result1
  %op52 = icmp eq i32 %op51, 1
  br i1 %op52, label %label_if_then_5, label %label_if_else_5
label_if_then_5:                                                ; preds = %label_if_else_4
  store i32 2, i32* %result1
  br label %label_if_end_5
label_if_end_5:                                                ; preds = %label_if_then_5, %label_if_end_6
  br label %label_if_end_4
label_if_else_5:                                                ; preds = %label_if_else_4
  %op53 = load i32, i32* %result1
  %op54 = icmp eq i32 %op53, 2
  br i1 %op54, label %label_if_then_6, label %label_if_else_6
label_if_then_6:                                                ; preds = %label_if_else_5
  store i32 3, i32* %result1
  br label %label_if_end_6
label_if_end_6:                                                ; preds = %label_if_then_6, %label_if_else_6
  br label %label_if_end_5
label_if_else_6:                                                ; preds = %label_if_else_5
  store i32 0, i32* %result1
  br label %label_if_end_6
}
define i32 @factorial(i32 %n) {
label_factorial_ENTRY:
  %_n = alloca i32
  store i32 %n, i32* %_n
  %op0 = load i32, i32* %_n
  %op1 = icmp sle i32 %op0, 1
  br i1 %op1, label %label_if_then_7, label %label_if_else_7
label_if_then_7:                                                ; preds = %label_factorial_ENTRY
  ret i32 1
label_if_else_7:                                                ; preds = %label_factorial_ENTRY
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
  br i1 %op1, label %label_if_then_8, label %label_if_else_8
label_if_then_8:                                                ; preds = %label_power_ENTRY
  ret float 0x3F800000
label_if_else_8:                                                ; preds = %label_power_ENTRY
  %op2 = load i32, i32* %_exp
  %op3 = icmp sgt i32 %op2, 0
  br i1 %op3, label %label_if_then_9, label %label_if_else_9
label_if_then_9:                                                ; preds = %label_if_else_8
  %result = alloca float
  store float 0x3F800000, float* %result
  %i = alloca i32
  store i32 0, i32* %i
  %op4 = load float, float* %result
  ret float %op4
label_if_else_9:                                                ; preds = %label_if_else_8
  %op5 = load float, float* %_base
  %op6 = load i32, i32* %_exp
  %op7 = sub i32 0, %op6
  %op8 = call float @power(float %op5, i32 %op7)
  %op9 = fdiv float 0x3F800000, %op8
  ret float %op9
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
  br i1 %op1, label %label_if_then_10, label %label_if_else_10
label_if_then_10:                                                ; preds = %label_complex_calculation_ENTRY
  %op2 = load i32, i32* %_b
  %op3 = icmp sgt i32 %op2, 0
  br i1 %op3, label %label_if_then_11, label %label_if_else_11
label_if_end_10:                                                ; preds = %label_if_end_11, %label_if_else_10
  %op4 = load i32, i32* %result
  ret i32 %op4
label_if_else_10:                                                ; preds = %label_complex_calculation_ENTRY
  store i32 0, i32* %result
  br label %label_if_end_10
label_if_then_11:                                                ; preds = %label_if_then_10
  %op5 = load float, float* %_c
  %op6 = fcmp ogt float %op5, 0x00000000
  br i1 %op6, label %label_if_then_12, label %label_if_else_12
label_if_end_11:                                                ; preds = %label_if_end_12, %label_if_else_11
  br label %label_if_end_10
label_if_else_11:                                                ; preds = %label_if_then_10
  %op7 = load i32, i32* %_a
  %op8 = load i32, i32* %_b
  %op9 = mul i32 %op7, %op8
  %op10 = load i32, i32* %_d
  %op11 = mul i32 %op9, %op10
  store i32 %op11, i32* %result
  br label %label_if_end_11
label_if_then_12:                                                ; preds = %label_if_then_11
  %op12 = load i32, i32* %_a
  %op13 = load i32, i32* %_b
  %op14 = add i32 %op12, %op13
  %op15 = load float, float* %_c
  %op16 = fmul float %op14, %op15
  %op17 = load i32, i32* %_d
  %op18 = fmul float %op16, %op17
  store float %op18, i32* %result
  br label %label_if_end_12
label_if_end_12:                                                ; preds = %label_if_then_12, %label_if_else_12
  br label %label_if_end_11
label_if_else_12:                                                ; preds = %label_if_then_11
  %op19 = load i32, i32* %_a
  %op20 = load i32, i32* %_b
  %op21 = sub i32 %op19, %op20
  %op22 = load i32, i32* %_d
  %op23 = sdiv i32 %op21, %op22
  store i32 %op23, i32* %result
  br label %label_if_end_12
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
  br i1 %op9, label %label_if_then_13, label %label_if_end_13
label_if_then_13:                                                ; preds = %label_test_expressions_ENTRY
  %op10 = load i32, i32* @global_counter
  %op11 = load i32, i32* %result1
  %op12 = add i32 %op10, %op11
  store i32 %op12, i32* @global_counter
  br label %label_if_end_13
label_if_end_13:                                                ; preds = %label_test_expressions_ENTRY, %label_if_then_13
  ret void
}
