#include "ir_generator.h"
#include "parse.h"  // 完整定义ParseTreeNode
#include <iostream>
#include <cassert>

IRGenerator::IRGenerator()
    : module_(nullptr), current_function_(nullptr),
      current_block_(nullptr), builder_(nullptr),
      symbol_table_(new SymbolTable()),
      temp_counter_(0) {
}

IRGenerator::~IRGenerator() {
    delete symbol_table_;
    delete builder_;
    if (module_) {
        delete module_;
    }
}

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
                int val = std::stoi(node->initValue);
                init_value = ConstantInt::get(val, module_);
            } else {
                init_value = ConstantZero::get(var_type, module_);
            }

            GlobalVariable* global_var = GlobalVariable::create(
                node->varName, module_, var_type, false, init_value
            );
            module_->add_global_variable(global_var);
            symbol_table_->put(node->varName, global_var);
        } else {
            // 局部变量 - 需要在函数上下文中创建
            if (current_function_ && current_block_) {
                AllocaInst* alloca = builder_->create_alloca(var_type);
                alloca->set_name(node->varName);
                symbol_table_->put(node->varName, alloca);

                // 初始化
                if (!node->initValue.empty()) {
                    int val = std::stoi(node->initValue);
                    ConstantInt* init_val = ConstantInt::get(val, module_);
                    builder_->create_store(init_val, alloca);
                }
            }
        }
    }
    else if (node->semanticType == "ConstDecl") {
        // 处理常量声明 - 常量被当作不可变的变量处理
        for (auto* child : node->children) {
            if (child && child->semanticType == "ConstDef") {
                visitParseTreeNode(child);
            }
        }
    }
    else if (node->semanticType == "ConstDef") {
        // 处理常量定义
        Type* const_type = convert_type(node->varType);

        // 获取常量值
        Value* const_value = nullptr;
        for (auto* child : node->children) {
            if (child && child->semanticType == "ConstInitVal") {
                const_value = visitParseTreeExpr(child);
                break;
            }
        }

        if (node->isGlobal) {
            // 全局常量
            Constant* init_value = dynamic_cast<Constant*>(const_value);
            if (!init_value) {
                init_value = ConstantZero::get(const_type, module_);
            }

            GlobalVariable* global_const = GlobalVariable::create(
                node->varName, module_, const_type, true, init_value  // true表示常量
            );
            module_->add_global_variable(global_const);
            symbol_table_->put(node->varName, global_const);
        } else {
            // 局部常量 - 存储为只读变量
            if (current_function_ && current_block_) {
                AllocaInst* alloca = builder_->create_alloca(const_type);
                alloca->set_name(node->varName);
                symbol_table_->put(node->varName, alloca);

                // 初始化常量值
                if (const_value) {
                    builder_->create_store(const_value, alloca);
                }
            }
        }
    }
    else if (node->semanticType == "FunctionDef") {
        // 处理函数定义
        Type* return_type = convert_type(node->varType);

        // 处理函数参数
        std::vector<Type*> param_types;
        std::vector<std::string> param_names;

        for (auto* child : node->children) {
            if (child && child->semanticType == "FuncFParams") {
                // 收集参数类型和名称
                collectFunctionParams(child, param_types, param_names);
                break;
            }
        }

        FunctionType* func_type = FunctionType::get(return_type, param_types);
        Function* function = Function::create(func_type, node->varName, module_);

        current_function_ = function;
        SymbolTable* parent_table = symbol_table_; // 保存父作用域
        symbol_table_ = new SymbolTable(symbol_table_); // 新的作用域

        // 创建入口基本块
        BasicBlock* entry_block = BasicBlock::create(module_, node->varName + "_entry", function);
        set_current_block(entry_block);

        // 创建IRBuilder
        builder_ = new IRBuilder(current_block_, module_);
        builder_->set_curFunc(function);

        // 处理函数参数 - 为每个参数创建alloca并存储
        auto& args = function->get_args();
        size_t i = 0;
        for (auto* arg : args) {
            if (i < param_names.size()) {
                arg->set_name(param_names[i]);

                // 创建局部变量存储参数
                AllocaInst* alloca = builder_->create_alloca(param_types[i]);
                alloca->set_name(param_names[i]);
                builder_->create_store(arg, alloca);

                // 将参数添加到符号表
                symbol_table_->put(param_names[i], alloca);
                ++i;
            }
        }

        // 处理函数体 - 处理所有非参数相关的子节点
        for (auto* child : node->children) {
            if (child && child->semanticType != "FuncFParams" &&
                child->symbol != "(" && child->symbol != ")" &&
                child->symbol != "funcType" && child->symbol != "Ident") {
                visitParseTreeNode(child);
            }
        }

        // 确保函数有终结指令
        if (current_block_ && !current_block_->get_terminator()) {
            if (return_type->is_void_type()) {
                builder_->create_void_ret();
            } else {
                ConstantInt* zero = ConstantInt::get(0, module_);
                builder_->create_ret(zero);
            }
        }

        delete symbol_table_;
        symbol_table_ = parent_table; // 恢复父作用域
        delete builder_;
        builder_ = nullptr;
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

        if (right_value && builder_) {
            builder_->create_store(right_value, left_var);
        }
    }
    else if (node->semanticType == "ReturnStmt") {
        // 处理return语句
        if (node->children.empty()) {
            builder_->create_void_ret();
        } else {
            Value* ret_val = nullptr;
            for (auto* child : node->children) {
                if (child && !child->semanticType.empty()) {
                    ret_val = visitParseTreeExpr(child);
                    break;
                }
            }
            if (ret_val) {
                builder_->create_ret(ret_val);
            } else {
                builder_->create_void_ret();
            }
        }
    }
    else if (node->semanticType == "Block") {
        // 处理代码块
        for (auto* child : node->children) {
            if (child && child->semanticType == "BlockItem") {
                visitParseTreeNode(child);
            }
        }
    }
    else if (node->semanticType == "BlockItem") {
        // 处理块项 - 递归处理子节点
        for (auto* child : node->children) {
            visitParseTreeNode(child);
        }
    }
    else if (node->semanticType == "Stmt") {
        // 处理语句 - 根据语句类型处理
        if (!node->children.empty()) {
            auto* first_child = node->children[0];
            if (first_child) {
                if (first_child->semanticType == "LVal") {
                    // 赋值语句: lVal = exp
                    handleAssignment(node);
                } else if (first_child->semanticType == "Exp") {
                    // 表达式语句: exp;
                    visitParseTreeExpr(first_child);
                } else if (first_child->semanticType == "Block") {
                    // 块语句
                    visitParseTreeNode(first_child);
                } else if (first_child->symbol == "if") {
                    // if语句
                    handleIfStatement(node);
                } else if (first_child->symbol == "return") {
                    // return语句
                    visitParseTreeNode(node);  // 让ReturnStmt处理
                }
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
    else if (node->semanticType == "Variable" || node->semanticType == "LVal") {
        // 变量引用或左值
        Value* var = symbol_table_->get(node->varName);
        if (!var) {
            std::cerr << "Error: Undefined variable '" << node->varName << "'" << std::endl;
            return nullptr;
        }
        // 加载变量值
        return builder_->create_load(var);
    }
    else if (node->semanticType == "UnaryExp") {
        // 一元表达式 (暂时只处理基础情况)
        if (!node->children.empty()) {
            return visitParseTreeExpr(node->children[0]);
        }
    }
    else if (node->semanticType == "MulExp" || node->semanticType == "AddExp") {
        // 乘法和加法表达式
        return handleBinaryExpr(node);
    }
    else if (node->semanticType == "RelExp") {
        // 关系表达式
        return handleRelExpr(node);
    }
    else if (node->semanticType == "EqExp") {
        // 相等表达式
        return handleEqExpr(node);
    }
    else if (node->semanticType == "LAndExp") {
        // 逻辑与表达式
        return handleLAndExpr(node);
    }
    else if (node->semanticType == "LOrExp") {
        // 逻辑或表达式
        return handleLOrExpr(node);
    }
    else if (node->semanticType == "Cond") {
        // 条件表达式
        return visitParseTreeExpr(node->children.empty() ? nullptr : node->children[0]);
    }
    else if (!node->operatorType.empty()) {
        // 通用二元表达式处理
        return handleBinaryExpr(node);
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

Value* IRGenerator::handleBinaryExpr(ParseTreeNode* node) {
    if (!node || node->children.size() < 3) return nullptr;

    Value* lhs = visitParseTreeExpr(node->children[0]);
    Value* rhs = visitParseTreeExpr(node->children[2]);

    if (!lhs || !rhs || !builder_) return nullptr;

    std::string op = node->children[1]->symbol;

    if (op == "+") {
        return builder_->create_iadd(lhs, rhs);
    } else if (op == "-") {
        return builder_->create_isub(lhs, rhs);
    } else if (op == "*") {
        return builder_->create_imul(lhs, rhs);
    } else if (op == "/") {
        return builder_->create_isdiv(lhs, rhs);
    } else if (op == "%") {
        return builder_->create_irem(lhs, rhs);
    }

    return nullptr;
}

Value* IRGenerator::handleRelExpr(ParseTreeNode* node) {
    if (!node || node->children.size() < 3) return nullptr;

    Value* lhs = visitParseTreeExpr(node->children[0]);
    Value* rhs = visitParseTreeExpr(node->children[2]);

    if (!lhs || !rhs || !builder_) return nullptr;

    std::string op = node->children[1]->symbol;

    if (op == "<") {
        return builder_->create_icmp_lt(lhs, rhs);
    } else if (op == ">") {
        return builder_->create_icmp_gt(lhs, rhs);
    } else if (op == "<=") {
        return builder_->create_icmp_le(lhs, rhs);
    } else if (op == ">=") {
        return builder_->create_icmp_ge(lhs, rhs);
    }

    return nullptr;
}

Value* IRGenerator::handleEqExpr(ParseTreeNode* node) {
    if (!node || node->children.size() < 3) return nullptr;

    Value* lhs = visitParseTreeExpr(node->children[0]);
    Value* rhs = visitParseTreeExpr(node->children[2]);

    if (!lhs || !rhs || !builder_) return nullptr;

    std::string op = node->children[1]->symbol;

    if (op == "==") {
        return builder_->create_icmp_eq(lhs, rhs);
    } else if (op == "!=") {
        return builder_->create_icmp_ne(lhs, rhs);
    }

    return nullptr;
}

Value* IRGenerator::handleLAndExpr(ParseTreeNode* node) {
    // 逻辑与: 转换为条件分支
    if (!node || node->children.size() < 3 || !builder_) return nullptr;

    // 创建基本块
    BasicBlock* rhs_block = BasicBlock::create(module_, new_temp() + "_land_rhs", current_function_);
    BasicBlock* false_block = BasicBlock::create(module_, new_temp() + "_land_false", current_function_);
    BasicBlock* merge_block = BasicBlock::create(module_, new_temp() + "_land_merge", current_function_);

    // 计算左操作数
    Value* lhs = visitParseTreeExpr(node->children[0]);
    if (!lhs) return nullptr;

    // 左操作数为假时跳转到false块
    builder_->create_cond_br(lhs, rhs_block, false_block);

    // 计算右操作数
    set_current_block(rhs_block);
    Value* rhs = visitParseTreeExpr(node->children[2]);
    if (!rhs) return nullptr;

    // 右操作数结果跳转到merge块
    builder_->create_br(merge_block);

    // false块
    set_current_block(false_block);
    builder_->create_br(merge_block);

    // merge块 - 这里需要Phi指令来合并结果
    set_current_block(merge_block);
    // 简化处理：返回右操作数的结果
    return rhs;
}

Value* IRGenerator::handleLOrExpr(ParseTreeNode* node) {
    // 逻辑或: 转换为条件分支
    if (!node || node->children.size() < 3 || !builder_) return nullptr;

    // 创建基本块
    BasicBlock* rhs_block = BasicBlock::create(module_, new_temp() + "_lor_rhs", current_function_);
    BasicBlock* true_block = BasicBlock::create(module_, new_temp() + "_lor_true", current_function_);
    BasicBlock* merge_block = BasicBlock::create(module_, new_temp() + "_lor_merge", current_function_);

    // 计算左操作数
    Value* lhs = visitParseTreeExpr(node->children[0]);
    if (!lhs) return nullptr;

    // 左操作数为真时跳转到true块
    builder_->create_cond_br(lhs, true_block, rhs_block);

    // 计算右操作数
    set_current_block(rhs_block);
    Value* rhs = visitParseTreeExpr(node->children[2]);
    if (!rhs) return nullptr;

    // 右操作数结果跳转到merge块
    builder_->create_br(merge_block);

    // true块
    set_current_block(true_block);
    builder_->create_br(merge_block);

    // merge块
    set_current_block(merge_block);
    // 简化处理：返回右操作数的结果
    return rhs;
}

std::string IRGenerator::new_temp() {
    return "op" + std::to_string(temp_counter_++);
}

void IRGenerator::set_current_block(BasicBlock* bb) {
    current_block_ = bb;
    if (builder_) {
        builder_->set_insert_point(bb);
    }
}

Type* IRGenerator::convert_type(const std::string& type_name) {
    if (type_name == "int") {
        return module_->get_int32_type();
    } else if (type_name == "float") {
        return module_->get_float_type();
    } else if (type_name == "void") {
        return module_->get_void_type();
    }
    return module_->get_int32_type();
}

void IRGenerator::handleAssignment(ParseTreeNode* node) {
    // 赋值语句: lVal = exp
    if (node->children.size() >= 3) {
        Value* left_var = handleLVal(node->children[0]);  // lVal
        Value* right_value = visitParseTreeExpr(node->children[2]);  // exp

        if (left_var && right_value && builder_) {
            builder_->create_store(right_value, left_var);
        }
    }
}

void IRGenerator::handleIfStatement(ParseTreeNode* node) {
    // if语句: 'if' '(' cond ')' stmt ('else' stmt)?
    if (!builder_ || node->children.size() < 5) return;

    // 解析条件
    Value* condition = nullptr;
    for (auto* child : node->children) {
        if (child && child->semanticType == "Cond") {
            condition = visitParseTreeExpr(child);
            break;
        }
    }

    if (!condition) return;

    // 创建基本块
    BasicBlock* then_block = BasicBlock::create(module_, new_temp() + "_then", current_function_);
    BasicBlock* else_block = nullptr;
    BasicBlock* merge_block = BasicBlock::create(module_, new_temp() + "_merge", current_function_);

    // 检查是否有else分支
    bool has_else = false;
    for (auto* child : node->children) {
        if (child && child->symbol == "else") {
            has_else = true;
            else_block = BasicBlock::create(module_, new_temp() + "_else", current_function_);
            break;
        }
    }

    if (!has_else) {
        else_block = merge_block;
    }

    // 创建条件分支
    builder_->create_cond_br(condition, then_block, else_block);

    // 处理then分支
    set_current_block(then_block);
    // 找到then语句
    bool found_stmt = false;
    for (auto* child : node->children) {
        if (child && child->semanticType == "Stmt" && !found_stmt) {
            visitParseTreeNode(child);
            found_stmt = true;
        }
    }

    if (!then_block->get_terminator()) {
        builder_->create_br(merge_block);
    }

    // 处理else分支
    if (has_else) {
        set_current_block(else_block);
        // 找到else语句
        bool found_else_stmt = false;
        for (size_t i = 0; i < node->children.size(); ++i) {
            if (node->children[i] && node->children[i]->symbol == "else" && i + 1 < node->children.size()) {
                visitParseTreeNode(node->children[i + 1]);
                found_else_stmt = true;
                break;
            }
        }

        if (!else_block->get_terminator()) {
            builder_->create_br(merge_block);
        }
    }

    // 设置插入点到merge块
    set_current_block(merge_block);
}

Value* IRGenerator::handleLVal(ParseTreeNode* node) {
    // lVal是左值，通常是变量标识符
    if (node && node->semanticType == "LVal") {
        Value* var = symbol_table_->get(node->varName);
        if (!var) {
            std::cerr << "Error: Undefined variable '" << node->varName << "'" << std::endl;
            return nullptr;
        }
        return var;
    }
    return nullptr;
}

void IRGenerator::collectFunctionParams(ParseTreeNode* node, std::vector<Type*>& param_types, std::vector<std::string>& param_names) {
    if (!node || node->semanticType != "FuncFParams") return;

    // 递归收集所有参数
    collectFuncFParamList(node, param_types, param_names);
}

void IRGenerator::collectFuncFParamList(ParseTreeNode* node, std::vector<Type*>& param_types, std::vector<std::string>& param_names) {
    if (!node) return;

    if (node->semanticType == "FuncFParam") {
        // 处理单个参数
        if (!node->children.empty()) {
            // 第一个子节点应该是类型
            auto* type_node = node->children[0];
            if (type_node) {
                Type* param_type = convert_type(type_node->symbol);
                param_types.push_back(param_type);
            }

            // 参数名应该在semanticType为FuncFParam的节点中
            param_names.push_back(node->varName);
        }
    }

    // 递归处理参数列表
    for (auto* child : node->children) {
        if (child && (child->semanticType == "FuncFParam" || child->semanticType == "FuncFParamList")) {
            collectFuncFParamList(child, param_types, param_names);
        }
    }
}

void IRGenerator::declare_builtin_functions() {
    // getint(): i32
    FunctionType* getint_type = FunctionType::get(module_->get_int32_type(), {});
    Function::create(getint_type, "getint", module_);

    // putint(i32): void
    std::vector<Type*> putint_params = {module_->get_int32_type()};
    FunctionType* putint_type = FunctionType::get(module_->get_void_type(), putint_params);
    Function::create(putint_type, "putint", module_);

    // getch(): i32
    FunctionType* getch_type = FunctionType::get(module_->get_int32_type(), {});
    Function::create(getch_type, "getch", module_);

    // putch(i32): void
    std::vector<Type*> putch_params = {module_->get_int32_type()};
    FunctionType* putch_type = FunctionType::get(module_->get_void_type(), putch_params);
    Function::create(putch_type, "putch", module_);

    // getarray(i32*): i32
    std::vector<Type*> getarray_params = {module_->get_int32_ptr_type()};
    FunctionType* getarray_type = FunctionType::get(module_->get_int32_type(), getarray_params);
    Function::create(getarray_type, "getarray", module_);

    // putarray(i32, i32*): void
    std::vector<Type*> putarray_params = {module_->get_int32_type(), module_->get_int32_ptr_type()};
    FunctionType* putarray_type = FunctionType::get(module_->get_void_type(), putarray_params);
    Function::create(putarray_type, "putarray", module_);

    // starttime(): void
    FunctionType* starttime_type = FunctionType::get(module_->get_void_type(), {});
    Function::create(starttime_type, "starttime", module_);

    // stoptime(): void
    FunctionType* stoptime_type = FunctionType::get(module_->get_void_type(), {});
    Function::create(stoptime_type, "stoptime", module_);
}