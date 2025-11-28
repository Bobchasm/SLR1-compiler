#include "ir_generator.h"
#include "parse.h"  // 完整定义ParseTreeNode
#include <iostream>
#include <cassert>

IRGenerator::IRGenerator() 
    : module_(nullptr), current_function_(nullptr), 
      current_block_(nullptr), symbol_table_(new SymbolTable()), 
      temp_counter_(0) {
}

IRGenerator::~IRGenerator() {
    delete symbol_table_;
    if (module_) {
        delete module_;
    }
}

std::string IRGenerator::generate(ASTNode* ast, const std::string& source_file) {
    // 创建模块
    module_ = new Module("sysy2022_compiler");
    
    // 设置源文件名（如果提供）
    if (!source_file.empty()) {
        module_->set_source_file_name(source_file);
    }
    
    // 声明内置函数
    declare_builtin_functions();
    
    // 遍历AST生成IR
    if (auto program = dynamic_cast<ProgramNode*>(ast)) {
        visitProgram(program);
    }
    
    // 生成IR文本
    std::string ir_code = module_->print();
    
    return ir_code;
}

// 新增：直接从ParseTreeNode生成IR
std::string IRGenerator::generateFromParseTree(ParseTreeNode* parseTree, const std::string& source_file) {
    if (!parseTree) {
        std::cerr << "Error: ParseTree is null" << std::endl;
        return "";
    }
    
    // 创建模块
    module_ = new Module("sysy2022_compiler");
    
    // 设置源文件名
    if (!source_file.empty()) {
        module_->set_source_file_name(source_file);
    }
    
    // 声明内置函数
    declare_builtin_functions();
    
    // 遍历ParseTreeNode生成IR
    visitParseTreeNode(parseTree);
    
    // 生成IR文本
    std::string ir_code = module_->print();
    
    return ir_code;
}

void IRGenerator::visitProgram(ProgramNode* node) {
    // 先处理全局声明
    for (auto& decl : node->declarations) {
        if (auto var_decl = dynamic_cast<VarDeclNode*>(decl.get())) {
            var_decl->is_global = true;
            visitVarDecl(var_decl);
        }
    }
    
    // 再处理函数定义
    for (auto& decl : node->declarations) {
        if (auto func_def = dynamic_cast<FunctionDefNode*>(decl.get())) {
            visitFunctionDef(func_def);
        }
    }
}

void IRGenerator::visitFunctionDef(FunctionDefNode* node) {
    // 构建函数类型
    Type* return_type = convert_type(node->return_type);
    std::vector<Type*> param_types;
    
    for (auto& param : node->params) {
        param_types.push_back(convert_type(param.first));
    }
    
    FunctionType* func_type = FunctionType::get(return_type, param_types);
    Function* function = Function::create(func_type, node->name, module_);
    
    // 设置当前上下文
    current_function_ = function;
    SymbolTable* old_symbol_table = symbol_table_;
    symbol_table_ = new SymbolTable(old_symbol_table); // 新的作用域
    
    // 创建入口基本块
    BasicBlock* entry_block = new BasicBlock(module_, node->name + "_ENTRY", function, false);
    set_current_block(entry_block);
    
    // 为参数分配空间
    auto args = function->get_args();
    auto arg_it = args.begin();
    for (auto& param : node->params) {
        std::string param_name = param.second;
        Instruction* alloca = AllocaInst::create_alloca(convert_type(param.first), current_block_);
        alloca->set_name(param_name);
        
        // 存储参数值
        if (arg_it != args.end()) {
            Value* arg_value = *arg_it;
            StoreInst::create_store(arg_value, alloca, current_block_);
            ++arg_it;
        }
        
        // 添加到符号表
        symbol_table_->put(param_name, alloca);
    }
    
    // 处理函数体
    for (auto& stmt : node->body) {
        if (auto var_decl = dynamic_cast<VarDeclNode*>(stmt.get())) {
            visitVarDecl(var_decl);
        } else if (auto assignment = dynamic_cast<AssignmentNode*>(stmt.get())) {
            visitAssignment(assignment);
        } else if (auto if_stmt = dynamic_cast<IfStmtNode*>(stmt.get())) {
            visitIfStmt(if_stmt);
        } else if (auto while_stmt = dynamic_cast<WhileStmtNode*>(stmt.get())) {
            visitWhileStmt(while_stmt);
        } else if (auto return_stmt = dynamic_cast<ReturnStmtNode*>(stmt.get())) {
            visitReturnStmt(return_stmt);
        } else if (auto expr = dynamic_cast<BinaryExprNode*>(stmt.get())) {
            visitBinaryExpr(expr); // 表达式语句
        } else if (auto func_call = dynamic_cast<FunctionCallNode*>(stmt.get())) {
            visitFunctionCall(func_call);
        }
    }
    
    // 确保函数有终结指令
    if (current_block_ && !current_block_->get_terminator()) {
        if (return_type->is_void_type()) {
            ReturnInst::create_void_ret(current_block_);
        } else {
            ConstantInt* zero = ConstantInt::get(0, module_);
            ReturnInst::create_ret(zero, current_block_);
        }
    }
    
    // 恢复上下文
    delete symbol_table_;
    symbol_table_ = old_symbol_table;
    current_function_ = nullptr;
    current_block_ = nullptr;
}

// 变量声明处理的核心函数，负责为全局变量和局部变量生成相应的 LLVM IR 代码
// 全局变量：使用 GlobalVariable，在数据段分配
// 局部变量：使用 AllocaInst，在栈上分配
void IRGenerator::visitVarDecl(VarDeclNode* node) {
    Type* var_type = convert_type(node->type);
    
    if (node->is_global) {
        // 全局变量
        Constant* init_value = nullptr;
        if (node->init_value) {
            init_value = dynamic_cast<Constant*>(visitNode(node->init_value.get()));
        }
        if (!init_value) {
            init_value = ConstantZero::get(var_type, module_);
        }
        
        GlobalVariable* global_var = GlobalVariable::create(
            node->name, module_, var_type, false, init_value
        );
        symbol_table_->put(node->name, global_var);
    } else {
        // 局部变量
        Instruction* alloca = AllocaInst::create_alloca(var_type, current_block_);
        alloca->set_name(node->name);
        symbol_table_->put(node->name, alloca);
        
        // 初始化
        if (node->init_value) {
            Value* init_val = visitNode(node->init_value.get());
            if (init_val) {
                StoreInst::create_store(init_val, alloca, current_block_);
            }
        }
    }
}


// 赋值语句处理函数，负责生成变量赋值的 LLVM IR 代码
void IRGenerator::visitAssignment(AssignmentNode* node) {
    // 递归处理赋值号右边的表达式，得到要存储的值
    Value* right_value = visitNode(node->value.get());
    // 从符号表中查找要赋值的目标变量，这里获取的是变量的内存地址，不是变量的值
    Value* left_var = symbol_table_->get(node->var_name);
    
    if (!left_var) {
        std::cerr << "Error: Undefined variable '" << node->var_name << "'" << std::endl;
        return;
    }
    
    if (!right_value) {
        std::cerr << "Error: Invalid right value in assignment" << std::endl;
        return;
    }
    
    StoreInst::create_store(right_value, left_var, current_block_);
}

// 二元表达式求值
Value* IRGenerator::visitBinaryExpr(BinaryExprNode* node) {
    Value* lhs = visitNode(node->left.get());
    Value* rhs = visitNode(node->right.get());
    
    if (!lhs || !rhs) {
        std::cerr << "Error: Invalid operands in binary expression" << std::endl;
        return nullptr;
    }
    
    std::string temp_name = new_temp();
    Instruction* binary_inst = nullptr;
    
    if (node->op == "+") {
        binary_inst = BinaryInst::create_add(lhs, rhs, current_block_, module_);
    } else if (node->op == "-") {
        binary_inst = BinaryInst::create_sub(lhs, rhs, current_block_, module_);
    } else if (node->op == "*") {
        binary_inst = BinaryInst::create_mul(lhs, rhs, current_block_, module_);
    } else if (node->op == "/") {
        binary_inst = BinaryInst::create_sdiv(lhs, rhs, current_block_, module_);
    } else if (node->op == "%") {
        binary_inst = BinaryInst::create_mod(lhs, rhs, current_block_, module_);
    } else if (node->op == "==") {
        binary_inst = CmpInst::create_cmp(CmpInst::EQ, lhs, rhs, current_block_, module_);
    } else if (node->op == "!=") {
        binary_inst = CmpInst::create_cmp(CmpInst::NE, lhs, rhs, current_block_, module_);
    } else if (node->op == "<") {
        binary_inst = CmpInst::create_cmp(CmpInst::LT, lhs, rhs, current_block_, module_);
    } else if (node->op == "<=") {
        binary_inst = CmpInst::create_cmp(CmpInst::LE, lhs, rhs, current_block_, module_);
    } else if (node->op == ">") {
        binary_inst = CmpInst::create_cmp(CmpInst::GT, lhs, rhs, current_block_, module_);
    } else if (node->op == ">=") {
        binary_inst = CmpInst::create_cmp(CmpInst::GE, lhs, rhs, current_block_, module_);
    } else if (node->op == "&&") {
        // 逻辑与需要特殊处理,需要短路求值，不能简单用 and 指令。
        return generate_logical_and(node);
    } else if (node->op == "||") {
        // 逻辑或需要特殊处理,需要短路求值，不能简单用 or 指令。
        return generate_logical_or(node);
    }
    
    if (binary_inst) {
        binary_inst->set_name(temp_name);
        return binary_inst;
    }
    
    return nullptr;
}

void IRGenerator::visitIfStmt(IfStmtNode* node) {
    // 创建基本块
    BasicBlock* then_block = new BasicBlock(module_, "if_then", current_function_, false);
    BasicBlock* else_block = node->else_branch ? 
        new BasicBlock(module_, "if_else", current_function_, false) : nullptr;
    BasicBlock* end_block = new BasicBlock(module_, "if_end", current_function_, false);
    
    // 生成条件跳转
    Value* cond_value = visitNode(node->condition.get());
    if (else_block) {
        BranchInst::create_cond_br(cond_value, then_block, else_block, current_block_);
    } else {
        BranchInst::create_cond_br(cond_value, then_block, end_block, current_block_);
    }
    
    // 生成then块
    set_current_block(then_block);
    visitNode(node->then_branch.get());
    // 如果then块没有终结指令，跳转到end块
    if (then_block->get_terminator() == nullptr) {
        BranchInst::create_br(end_block, then_block);
    }
    
    // 生成else块（如果有）
    if (else_block) {
        set_current_block(else_block);
        visitNode(node->else_branch.get());
        if (else_block->get_terminator() == nullptr) {
            BranchInst::create_br(end_block, else_block);
        }
    }
    
    // 设置当前块为end块继续后续代码
    set_current_block(end_block);
}

void IRGenerator::visitWhileStmt(WhileStmtNode* node) {
    // 创建基本块
    BasicBlock* cond_block = new BasicBlock(module_, "while_cond", current_function_, false);
    BasicBlock* body_block = new BasicBlock(module_, "while_body", current_function_, false);
    BasicBlock* end_block = new BasicBlock(module_, "while_end", current_function_, false);
    
    // 跳转到条件判断块
    BranchInst::create_br(cond_block, current_block_);
    
    // 条件判断块
    set_current_block(cond_block);
    Value* cond_value = visitNode(node->condition.get());
    BranchInst::create_cond_br(cond_value, body_block, end_block, cond_block);
    
    // 循环体块
    set_current_block(body_block);
    visitNode(node->body.get());
    // 循环体结束后跳回条件判断
    if (body_block->get_terminator() == nullptr) {
        BranchInst::create_br(cond_block, body_block);
    }
    
    // 设置当前块为结束块
    set_current_block(end_block);
}

void IRGenerator::visitReturnStmt(ReturnStmtNode* node) {
    if (node->value) {
        Value* ret_value = visitNode(node->value.get());
        ReturnInst::create_ret(ret_value, current_block_);
    } else {
        ReturnInst::create_void_ret(current_block_);
    }
}

Value* IRGenerator::visitVariableRef(VariableRefNode* node) {
    Value* variable = symbol_table_->get(node->name);
    if (!variable) {
        std::cerr << "Error: Undefined variable '" << node->name << "'" << std::endl;
        return nullptr;
    }
    
    // 对于局部变量，需要load其值
    if (dynamic_cast<Instruction*>(variable) && 
        dynamic_cast<AllocaInst*>(variable)) {
        std::string temp_name = new_temp();
        Type* var_type = static_cast<AllocaInst*>(variable)->get_alloca_type();
        Instruction* load_inst = LoadInst::create_load(var_type, variable, current_block_);
        load_inst->set_name(temp_name);
        return load_inst;
    }
    
    return variable;
}

Value* IRGenerator::visitNumber(NumberNode* node) {
    return ConstantInt::get(node->value, module_);
}

Value* IRGenerator::visitFunctionCall(FunctionCallNode* node) {
    // 从模块中查找函数
    Function* callee = nullptr;
    for (auto func : module_->get_functions()) {
        if (func->get_name() == node->func_name) {
            callee = func;
            break;
        }
    }
    
    if (!callee) {
        std::cerr << "Error: Undefined function '" << node->func_name << "'" << std::endl;
        return nullptr;
    }
    
    std::vector<Value*> args;
    for (auto& arg : node->arguments) {
        Value* arg_value = visitNode(arg.get());
        if (arg_value) {
            args.push_back(arg_value);
        }
    }
    
    std::string temp_name = new_temp();
    Instruction* call_inst = CallInst::create(callee, args, current_block_);
    
    if (call_inst && !callee->get_return_type()->is_void_type()) {
        call_inst->set_name(temp_name);
        return call_inst;
    }
    
    return call_inst;
}

// 辅助方法
Value* IRGenerator::visitNode(ASTNode* node) {
    if (!node) return nullptr;
    
    if (auto binary_expr = dynamic_cast<BinaryExprNode*>(node)) {
        return visitBinaryExpr(binary_expr);
    } else if (auto variable_ref = dynamic_cast<VariableRefNode*>(node)) {
        return visitVariableRef(variable_ref);
    } else if (auto number = dynamic_cast<NumberNode*>(node)) {
        return visitNumber(number);
    } else if (auto func_call = dynamic_cast<FunctionCallNode*>(node)) {
        return visitFunctionCall(func_call);
    } else if (auto unary_expr = dynamic_cast<UnaryExprNode*>(node)) {
        return visitUnaryExpr(unary_expr);
    }
    
    return nullptr;
}

Value* IRGenerator::visitUnaryExpr(UnaryExprNode* node) {
    // 处理单目运算符，如 -a, !b 等
    Value* operand = visitNode(node->operand.get());
    if (!operand) return nullptr;
    
    std::string temp_name = new_temp();
    Instruction* unary_inst = nullptr;
    
    if (node->op == "-") {
        // 取负：0 - operand
        ConstantInt* zero = ConstantInt::get(0, module_);
        unary_inst = BinaryInst::create_sub(zero, operand, current_block_, module_);
    } else if (node->op == "!") {
        // 逻辑非：operand == 0
        ConstantInt* zero = ConstantInt::get(0, module_);
        unary_inst = CmpInst::create_cmp(CmpInst::EQ, operand, zero, current_block_, module_);
    } else if (node->op == "+") {
        // 单目正号直接返回操作数
        return operand;
    }
    // 
    
    if (unary_inst) {
        unary_inst->set_name(temp_name);
        return unary_inst;
    }
    
    return nullptr;
}

Value* IRGenerator::generate_logical_and(BinaryExprNode* node) {
    // 短路求值：如果左边为假，直接返回假；否则返回右边的值
    BasicBlock* rhs_block = new BasicBlock(module_, "and_rhs", current_function_, false);
    BasicBlock* end_block = new BasicBlock(module_, "and_end", current_function_, false);
    
    Value* lhs = visitNode(node->left.get());
    
    // 将lhs转换为i1类型用于条件判断
    Instruction* lhs_bool = nullptr;
    if (!lhs->get_type()->is_integer_type() || 
        static_cast<IntegerType*>(lhs->get_type())->get_num_bits() != 1) {
        ConstantInt* zero = ConstantInt::get(0, module_);
        lhs_bool = CmpInst::create_cmp(CmpInst::NE, lhs, zero, current_block_, module_);
        lhs_bool->set_name(new_temp());
    } else {
        lhs_bool = static_cast<Instruction*>(lhs);
    }
    
    BranchInst::create_cond_br(lhs_bool, rhs_block, end_block, current_block_);
    
    // 处理右操作数
    set_current_block(rhs_block);
    Value* rhs = visitNode(node->right.get());
    
    // 将rhs转换为i1类型
    Instruction* rhs_bool = nullptr;
    if (!rhs->get_type()->is_integer_type() || 
        static_cast<IntegerType*>(rhs->get_type())->get_num_bits() != 1) {
        ConstantInt* zero = ConstantInt::get(0, module_);
        rhs_bool = CmpInst::create_cmp(CmpInst::NE, rhs, zero, current_block_, module_);
        rhs_bool->set_name(new_temp());
    } else {
        rhs_bool = static_cast<Instruction*>(rhs);
    }
    
    BranchInst::create_br(end_block, rhs_block);
    
    // 结束块：使用phi节点选择结果
    set_current_block(end_block);
    
    // 创建phi指令
    Instruction* phi_inst = PhiInst::create_phi(Type::get_int1_type(module_), end_block);
    PhiInst* phi = static_cast<PhiInst*>(phi_inst);
    
    // 添加phi操作数：左操作数为假的路径
    ConstantInt* false_val = ConstantInt::get(false, module_);
    BasicBlock* lhs_block = current_function_->get_basic_blocks().back();  // 获取lhs所在块
    phi->add_phi_pair_operand(false_val, lhs_block);
    
    // 添加phi操作数：右操作数的路径
    phi->add_phi_pair_operand(rhs_bool, rhs_block);
    
    phi_inst->set_name(new_temp());
    
    return phi_inst;
}

Value* IRGenerator::generate_logical_or(BinaryExprNode* node) {
    // 短路求值：如果左边为真，直接返回真；否则返回右边的值
    BasicBlock* rhs_block = new BasicBlock(module_, "or_rhs", current_function_, false);
    BasicBlock* end_block = new BasicBlock(module_, "or_end", current_function_, false);
    
    Value* lhs = visitNode(node->left.get());
    
    // 将lhs转换为i1类型用于条件判断
    Instruction* lhs_bool = nullptr;
    if (!lhs->get_type()->is_integer_type() || 
        static_cast<IntegerType*>(lhs->get_type())->get_num_bits() != 1) {
        ConstantInt* zero = ConstantInt::get(0, module_);
        lhs_bool = CmpInst::create_cmp(CmpInst::NE, lhs, zero, current_block_, module_);
        lhs_bool->set_name(new_temp());
    } else {
        lhs_bool = static_cast<Instruction*>(lhs);
    }
    
    BranchInst::create_cond_br(lhs_bool, end_block, rhs_block, current_block_);
    
    // 处理右操作数
    set_current_block(rhs_block);
    Value* rhs = visitNode(node->right.get());
    
    // 将rhs转换为i1类型
    Instruction* rhs_bool = nullptr;
    if (!rhs->get_type()->is_integer_type() || 
        static_cast<IntegerType*>(rhs->get_type())->get_num_bits() != 1) {
        ConstantInt* zero = ConstantInt::get(0, module_);
        rhs_bool = CmpInst::create_cmp(CmpInst::NE, rhs, zero, current_block_, module_);
        rhs_bool->set_name(new_temp());
    } else {
        rhs_bool = static_cast<Instruction*>(rhs);
    }
    
    BranchInst::create_br(end_block, rhs_block);
    
    // 结束块：使用phi节点选择结果
    set_current_block(end_block);
    
    // 创建phi指令
    Instruction* phi_inst = PhiInst::create_phi(Type::get_int1_type(module_), end_block);
    PhiInst* phi = static_cast<PhiInst*>(phi_inst);
    
    // 添加phi操作数：左操作数为真的路径
    ConstantInt* true_val = ConstantInt::get(true, module_);
    BasicBlock* lhs_block = current_function_->get_basic_blocks().back();  // 获取lhs所在块
    phi->add_phi_pair_operand(true_val, lhs_block);
    
    // 添加phi操作数：右操作数的路径
    phi->add_phi_pair_operand(rhs_bool, rhs_block);
    
    phi_inst->set_name(new_temp());
    
    return phi_inst;
}

std::string IRGenerator::new_temp() {
    return "op" + std::to_string(temp_counter_++);
}

void IRGenerator::set_current_block(BasicBlock* bb) {
    current_block_ = bb;
}

// Type* var_type = convert_type(node->type);  // "int" → i32
Type* IRGenerator::convert_type(const std::string& type_name) {
    if (type_name == "int") {
        return Type::get_int32_type(module_);
    } else if (type_name == "float") {
        return Type::get_float_type(module_);
    } else if (type_name == "void") {
        return Type::get_void_type(module_);
    }
    return Type::get_int32_type(module_);
}

void IRGenerator::declare_builtin_functions() {
    // getint(): i32
    FunctionType* getint_type = FunctionType::get(Type::get_int32_type(module_), {});
    Function::create(getint_type, "getint", module_);
    
    // putint(i32): void
    std::vector<Type*> putint_params = {Type::get_int32_type(module_)};
    FunctionType* putint_type = FunctionType::get(Type::get_void_type(module_), putint_params);
    Function::create(putint_type, "putint", module_);
    
    // getch(): i32
    FunctionType* getch_type = FunctionType::get(Type::get_int32_type(module_), {});
    Function::create(getch_type, "getch", module_);
    
    // putch(i32): void
    std::vector<Type*> putch_params = {Type::get_int32_type(module_)};
    FunctionType* putch_type = FunctionType::get(Type::get_void_type(module_), putch_params);
    Function::create(putch_type, "putch", module_);
    
    // getarray(i32*): i32
    std::vector<Type*> getarray_params = {Type::get_int32_ptr_type(module_)};
    FunctionType* getarray_type = FunctionType::get(Type::get_int32_type(module_), getarray_params);
    Function::create(getarray_type, "getarray", module_);
    
    // putarray(i32, i32*): void
    std::vector<Type*> putarray_params = {Type::get_int32_type(module_), Type::get_int32_ptr_type(module_)};
    FunctionType* putarray_type = FunctionType::get(Type::get_void_type(module_), putarray_params);
    Function::create(putarray_type, "putarray", module_);
    
    // starttime(): void
    FunctionType* starttime_type = FunctionType::get(Type::get_void_type(module_), {});
    Function::create(starttime_type, "starttime", module_);
    
    // stoptime(): void
    FunctionType* stoptime_type = FunctionType::get(Type::get_void_type(module_), {});
    Function::create(stoptime_type, "stoptime", module_);
}

// ==================== ParseTreeNode访问方法 ====================

void IRGenerator::visitParseTreeNode(ParseTreeNode* node) {
    if (!node || node->semanticType.empty()) {
        // 跳过没有语义信息的节点
        if (node) {
            for (auto* child : node->children) {
                visitParseTreeNode(child);
            }
        }
        return;
    }
    
    if (node->semanticType == "Program") {
        // 处理Program节点：先处理全局变量，再处理函数
        for (auto* child : node->children) {
            if (child && child->semanticType == "VarDecl" && child->isGlobal) {
                visitParseTreeNode(child);
            }
        }
        for (auto* child : node->children) {
            if (child && child->semanticType == "FunctionDef") {
                visitParseTreeNode(child);
            }
        }
    }
    else if (node->semanticType == "VarDecl") {
        // 处理变量声明
        Type* var_type = convert_type(node->varType);
        
        if (node->isGlobal) {
            // 全局变量
            Constant* init_value = nullptr;
            if (!node->initValue.empty()) {
                // 解析初始值
                int val = std::stoi(node->initValue);
                init_value = ConstantInt::get(val, module_);
            } else {
                init_value = ConstantZero::get(var_type, module_);
            }
            
            GlobalVariable* global_var = GlobalVariable::create(
                node->varName, module_, var_type, false, init_value
            );
            symbol_table_->put(node->varName, global_var);
        } else {
            // 局部变量
            Instruction* alloca = AllocaInst::create_alloca(var_type, current_block_);
            alloca->set_name(node->varName);
            symbol_table_->put(node->varName, alloca);
            
            // 初始化
            if (!node->initValue.empty()) {
                int val = std::stoi(node->initValue);
                ConstantInt* init_val = ConstantInt::get(val, module_);
                StoreInst::create_store(init_val, alloca, current_block_);
            }
            // 如果有子表达式，使用子表达式的值
            for (auto* child : node->children) {
                if (child && !child->semanticType.empty()) {
                    Value* init_val = visitParseTreeExpr(child);
                    if (init_val) {
                        StoreInst::create_store(init_val, alloca, current_block_);
                    }
                }
            }
        }
    }
    else if (node->semanticType == "FunctionDef") {
        // 处理函数定义
        Type* return_type = convert_type(node->varType);
        FunctionType* func_type = FunctionType::get(return_type, {}); // 简化：暂不处理参数
        Function* function = Function::create(func_type, node->varName, module_);
        
        current_function_ = function;
        SymbolTable* old_symbol_table = symbol_table_;
        symbol_table_ = new SymbolTable(old_symbol_table);
        
        // 创建入口基本块
        BasicBlock* entry_block = new BasicBlock(module_, node->varName + "_ENTRY", function, false);
        set_current_block(entry_block);
        
        // 处理函数体
        for (auto* child : node->children) {
            visitParseTreeNode(child);
        }
        
        // 确保函数有终结指令
        if (current_block_ && !current_block_->get_terminator()) {
            if (return_type->is_void_type()) {
                ReturnInst::create_void_ret(current_block_);
            } else {
                ConstantInt* zero = ConstantInt::get(0, module_);
                ReturnInst::create_ret(zero, current_block_);
            }
        }
        
        delete symbol_table_;
        symbol_table_ = old_symbol_table;
        current_function_ = nullptr;
        current_block_ = nullptr;
    }
    else if (node->semanticType == "Assignment") {
        // 处理赋值语句
        Value* left_var = symbol_table_->get(node->varName);
        if (!left_var) {
            std::cerr << "Error: Undefined variable '" << node->varName << "'" << std::endl;
            return;
        }
        
        // 获取右值
        Value* right_value = nullptr;
        for (auto* child : node->children) {
            if (child && !child->semanticType.empty()) {
                right_value = visitParseTreeExpr(child);
                break;
            }
        }
        
        if (right_value) {
            StoreInst::create_store(right_value, left_var, current_block_);
        }
    }
    else if (node->semanticType == "ReturnStmt") {
        // 处理return语句
        if (node->children.empty()) {
            ReturnInst::create_void_ret(current_block_);
        } else {
            Value* ret_val = nullptr;
            for (auto* child : node->children) {
                if (child && !child->semanticType.empty()) {
                    ret_val = visitParseTreeExpr(child);
                    break;
                }
            }
            if (ret_val) {
                ReturnInst::create_ret(ret_val, current_block_);
            } else {
                ReturnInst::create_void_ret(current_block_);
            }
        }
    }
    else {
        // 其他节点，递归处理子节点
        for (auto* child : node->children) {
            visitParseTreeNode(child);
        }
    }
}

Value* IRGenerator::visitParseTreeExpr(ParseTreeNode* node) {
    if (!node) return nullptr;
    
    if (node->semanticType == "Number") {
        // 数字常量
        int val = std::stoi(node->value);
        return ConstantInt::get(val, module_);
    }
    else if (node->semanticType == "Variable") {
        // 变量引用
        Value* var = symbol_table_->get(node->varName);
        if (!var) {
            std::cerr << "Error: Undefined variable '" << node->varName << "'" << std::endl;
            return nullptr;
        }
        // 加载变量值
        std::string temp_name = new_temp();
        Type* var_type = Type::get_int32_type(module_);  // 默认int类型
        Instruction* load_inst = LoadInst::create_load(var_type, var, current_block_);
        load_inst->set_name(temp_name);
        return load_inst;
    }
    else if (!node->operatorType.empty()) {
        // 二元表达式（根据operatorType判断）
        if (node->children.size() >= 2) {
            Value* lhs = visitParseTreeExpr(node->children[0]);
            Value* rhs = visitParseTreeExpr(node->children[1]);
            
            if (!lhs || !rhs) return nullptr;
            
            std::string temp_name = new_temp();
            Instruction* binary_inst = nullptr;
            
            if (node->operatorType == "+") {
                binary_inst = BinaryInst::create_add(lhs, rhs, current_block_, module_);
            } else if (node->operatorType == "-") {
                binary_inst = BinaryInst::create_sub(lhs, rhs, current_block_, module_);
            } else if (node->operatorType == "*") {
                binary_inst = BinaryInst::create_mul(lhs, rhs, current_block_, module_);
            } else if (node->operatorType == "/") {
                binary_inst = BinaryInst::create_sdiv(lhs, rhs, current_block_, module_);
            } else if (node->operatorType == "%") {
                binary_inst = BinaryInst::create_mod(lhs, rhs, current_block_, module_);
            }
            
            if (binary_inst) {
                binary_inst->set_name(temp_name);
                return binary_inst;
            }
        }
    }
    else {
        // 递归处理子表达式
        for (auto* child : node->children) {
            Value* result = visitParseTreeExpr(child);
            if (result) return result;
        }
    }
    
    return nullptr;
}