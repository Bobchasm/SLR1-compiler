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
    std::cout << "\n\n\n[IRGEN] generateFromParseTree called" << std::endl;

    if (!parseTree) {
        std::cout << "Error: ParseTree is null" << std::endl;
        return "";
    }

    std::cout << "[IRGEN] ParseTree semanticType: " << parseTree->semanticType << std::endl;

    // 创建模块
    std::cout << "[IRGEN] Creating module..." << std::endl;
    module_ = new Module("sysy2022_compiler");
    std::cout << "[IRGEN] Module created" << std::endl;

    // 设置源文件名
    if (!source_file.empty()) {
        module_->set_source_file_name(source_file);
    }

    // 声明内置函数
    std::cout << "[IRGEN] Declaring builtin functions..." << std::endl;
    declare_builtin_functions();
    std::cout << "[IRGEN] Builtin functions declared" << std::endl;

    // 遍历ParseTreeNode生成IR
    std::cout << "[IRGEN] Starting visitParseTreeNode..." << std::endl;
    visitParseTreeNode(parseTree);
    std::cout << "[IRGEN] visitParseTreeNode completed" << std::endl;

    // 生成IR文本
    std::cout << "[IRGEN] Generating IR text..." << std::endl;
    std::string ir_code = module_->print();
    std::cout << "[IRGEN] IR generation completed, length: " << ir_code.length() << std::endl;

    return ir_code;
}

void IRGenerator::visitParseTreeNode(ParseTreeNode* node) {
    static int depth = 0;
    std::string indent(depth * 2, ' ');
    std::cout << indent << "[IRGEN] visitParseTreeNode: " << (node ? node->semanticType : "null") << " (depth=" << depth << ")" << std::endl;

    if (!node) {
        std::cout << indent << "[IRGEN] Node is null, returning" << std::endl;
        return;
    }

    depth++;

    if (node->semanticType.empty()) {
        // 跳过没有语义信息的节点
        std::cout << indent << "[IRGEN] Node has empty semanticType, processing " << node->semanticChildren.size() << " semantic children" << std::endl;
        if (node) {
            for (auto* child : node->semanticChildren) {
                visitParseTreeNode(child);
            }
        }
        depth--;
        return;
    }

    // 处理不同类型的节点
    if (node->semanticType == "Program") {
        // 处理Program节点：先处理全局变量，再处理函数
        std::cout << indent << "[IRGEN] Processing Program node with " << node->semanticChildren.size() << " children" << std::endl;
        for (auto* child : node->semanticChildren) {
            if (child) {
                std::cout << indent << "[IRGEN] Child semanticType: '" << child->semanticType << "'" << std::endl;
                if (child->semanticType == "VarDecl" && child->isGlobal) {
                    std::cout << indent << "[IRGEN] Processing global VarDecl" << std::endl;
                    visitParseTreeNode(child);
                }
            }
        }
        for (auto* child : node->semanticChildren) {
            if (child && child->semanticType == "FunctionDef") {
                std::cout << indent << "[IRGEN] Processing FunctionDef" << std::endl;
                visitParseTreeNode(child);
            }
        }
    }
    else if (node->semanticType == "VarDecl") {
        std::cout << indent << "[IRGEN] Processing VarDecl: " << node->varName << std::endl;
        // 处理变量声明
        Type* var_type = convert_type(node->varType);

        if (node->isGlobal) {
            std::cout << indent << "[IRGEN] Creating global variable" << std::endl;
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
            std::cout << indent << "[IRGEN] Creating local variable" << std::endl;
            // 局部变量 - 需要在函数上下文中创建
            if (current_function_ && current_block_) {
                AllocaInst* alloca = builder_->create_alloca(var_type);
                alloca->set_name(node->varName);
                symbol_table_->put(node->varName, alloca);

                // 初始化 - 检查字符串初始化值或表达式子节点
                if (!node->initValue.empty()) {
                    // 简单的常量初始化
                    int val = std::stoi(node->initValue);
                    ConstantInt* init_val = ConstantInt::get(val, module_);
                    builder_->create_store(init_val, alloca);
                } else if (!node->semanticChildren.empty()) {
                    // 表达式初始化
                    for (auto* child : node->semanticChildren) {
                        if (child && !child->semanticType.empty()) {
                            Value* init_expr = visitParseTreeExpr(child);
                            if (init_expr) {
                                builder_->create_store(init_expr, alloca);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else if (node->semanticType == "FunctionDef") {
        std::cout << indent << "[IRGEN] Processing FunctionDef: " << node->varName << std::endl;
        // 处理函数定义
        Type* return_type = convert_type(node->varType);

        // 处理函数参数
        std::vector<Type*> param_types;
        std::vector<std::string> param_names;

        // 根据对接文档，FunctionDef的semanticChildren包含FuncParam和函数体
        for (auto* child : node->semanticChildren) {
            if (child && child->semanticType == "FuncParam") {
                // 收集参数类型和名称
                param_types.push_back(convert_type(child->varType));
                param_names.push_back(child->varName);
            }
        }

        FunctionType* func_type = FunctionType::get(return_type, param_types);
        Function* function = Function::create(func_type, node->varName, module_);

        current_function_ = function;
        SymbolTable* parent_table = symbol_table_; // 保存父作用域
        symbol_table_ = new SymbolTable(symbol_table_); // 新的作用域

        // 创建入口基本块
        BasicBlock* entry_block = BasicBlock::create(module_, node->varName + "_entry", function, false);
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

        // 处理函数体 - 根据对接文档，semanticChildren包含FuncParam和函数体语句
        std::cout << indent << "[IRGEN] Processing function body, children count: " << node->semanticChildren.size() << std::endl;
        for (auto* child : node->semanticChildren) {
            if (child && child->semanticType != "FuncParam") {
                // 跳过参数，处理函数体语句
                std::cout << indent << "[IRGEN] Processing function body child: " << child->semanticType << std::endl;
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
        std::cout << indent << "[IRGEN] Processing Assignment for var '" << node->varName << "'" << std::endl;
        // 处理赋值语句
        Value* left_var = symbol_table_->get(node->varName);
        if (!left_var) {
            std::cout << indent << "[IRGEN] Error: Undefined variable '" << node->varName << "'" << std::endl;
            depth--;
            return;
        }

        // 获取右值 - 先尝试semanticChildren，如果为空则尝试children
        Value* right_value = nullptr;
        if (!node->semanticChildren.empty()) {
            std::cout << indent << "[IRGEN] Using semanticChildren[0]" << std::endl;
            right_value = visitParseTreeExpr(node->semanticChildren[0]);
        } else if (!node->children.empty()) {
            std::cout << indent << "[IRGEN] semanticChildren empty, trying children" << std::endl;
            // 找到表达式子节点（跳过lVal和等号）
            for (auto* child : node->children) {
                if (child && (child->semanticType == "Number" || child->semanticType == "Variable" ||
                              child->semanticType == "BinaryExpr" || child->semanticType == "UnaryExp")) {
                    right_value = visitParseTreeExpr(child);
                    break;
                }
            }
        }

        if (right_value && builder_) {
            std::cout << indent << "[IRGEN] Creating store instruction" << std::endl;
            builder_->create_store(right_value, left_var);
        } else {
            std::cout << indent << "[IRGEN] Warning: Could not get right value for assignment" << std::endl;
        }
    }
    else if (node->semanticType == "ReturnStmt") {
        std::cout << indent << "[IRGEN] Processing ReturnStmt" << std::endl;
        // 处理return语句 - 先尝试semanticChildren，如果为空则尝试children
        if (node->semanticChildren.empty() && node->children.empty()) {
            std::cout << indent << "[IRGEN] Void return" << std::endl;
            builder_->create_void_ret();
        } else {
            Value* ret_val = nullptr;
            if (!node->semanticChildren.empty()) {
                std::cout << indent << "[IRGEN] Using semanticChildren[0] for return" << std::endl;
                ret_val = visitParseTreeExpr(node->semanticChildren[0]);
            } else if (!node->children.empty()) {
                std::cout << indent << "[IRGEN] semanticChildren empty, trying children for return" << std::endl;
                // 找到返回值表达式
                for (auto* child : node->children) {
                    if (child && (child->semanticType == "Number" || child->semanticType == "Variable" ||
                                  child->semanticType == "BinaryExpr" || child->semanticType == "UnaryExp")) {
                        ret_val = visitParseTreeExpr(child);
                        break;
                    }
                }
            }
            if (ret_val) {
                std::cout << indent << "[IRGEN] Creating return with value" << std::endl;
                builder_->create_ret(ret_val);
            } else {
                std::cout << indent << "[IRGEN] Void return (no value found)" << std::endl;
                builder_->create_void_ret();
            }
        }
    }
    else {
        std::cout << indent << "[IRGEN] Unknown semanticType: '" << node->semanticType << "', processing children" << std::endl;
        // 其他节点，递归处理子节点
        for (auto* child : node->children) {
            visitParseTreeNode(child);
        }
    }

    depth--;
    std::cout << indent << "[IRGEN] visitParseTreeNode completed for: " << node->semanticType << std::endl;
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
            std::cout << "Error: Undefined variable '" << node->varName << "'" << std::endl;
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
    else if (node->semanticType == "BinaryExpr" ||
             node->semanticType == "MulExp" ||
             node->semanticType == "AddExp") {
        // 二元表达式 - 根据操作符类型决定处理方式
        if (!node->operatorType.empty()) {
            std::string op = node->operatorType;
            if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
                return handleBinaryExpr(node);
            } else if (op == ">" || op == "<" || op == ">=" || op == "<=") {
                return handleRelExpr(node);
            } else if (op == "==" || op == "!=") {
                return handleEqExpr(node);
            }
        }
        // 默认当作二元表达式处理
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
    if (!node) return nullptr;

    // 处理两种结构：
    // 1. 三个子节点: lhs, op, rhs
    // 2. 两个子节点 + operatorType: lhs, rhs (操作符在operatorType中)

    if (node->children.size() >= 2) {
        Value* lhs = visitParseTreeExpr(node->children[0]);
        Value* rhs = visitParseTreeExpr(node->children[1]);

        if (!lhs || !rhs || !builder_) return nullptr;

        std::string op;
        if (node->children.size() >= 3) {
            // 三个子节点的情况
            op = node->children[1]->symbol;
        } else if (!node->operatorType.empty()) {
            // 使用operatorType的情况
            op = node->operatorType;
        } else {
            return nullptr;
        }

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

    return nullptr;
}

Value* IRGenerator::handleRelExpr(ParseTreeNode* node) {
    if (!node) return nullptr;

    Value* lhs = nullptr;
    Value* rhs = nullptr;
    std::string op;

    if (node->children.size() >= 2) {
        lhs = visitParseTreeExpr(node->children[0]);
        rhs = visitParseTreeExpr(node->children[1]);

        if (node->children.size() >= 3) {
            op = node->children[1]->symbol;
            rhs = visitParseTreeExpr(node->children[2]);
        } else if (!node->operatorType.empty()) {
            op = node->operatorType;
        }
    }

    if (!lhs || !rhs || !builder_) return nullptr;

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
    if (!node) return nullptr;

    Value* lhs = nullptr;
    Value* rhs = nullptr;
    std::string op;

    if (node->children.size() >= 2) {
        lhs = visitParseTreeExpr(node->children[0]);
        rhs = visitParseTreeExpr(node->children[1]);

        if (node->children.size() >= 3) {
            op = node->children[1]->symbol;
            rhs = visitParseTreeExpr(node->children[2]);
        } else if (!node->operatorType.empty()) {
            op = node->operatorType;
        }
    }

    if (!lhs || !rhs || !builder_) return nullptr;

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
    // 赋值语句: varName = semanticChildren[0]
    if (!node->varName.empty() && !node->semanticChildren.empty()) {
        // 获取变量地址
        Value* left_var = symbol_table_->get(node->varName);
        if (!left_var) {
            std::cout << "Error: Variable '" << node->varName << "' not found in symbol table" << std::endl;
            return;
        }

        // 计算右侧表达式的值
        Value* right_value = visitParseTreeExpr(node->semanticChildren[0]);

        if (left_var && right_value && builder_) {
            builder_->create_store(right_value, left_var);
        }
    }
}

void IRGenerator::handleIfStatement(ParseTreeNode* node) {
    // IfStmt结构: [condition, then_stmt, else_stmt]
    if (!builder_ || node->children.size() < 2) return;

    // 第一个子节点是条件
    Value* condition = visitParseTreeExpr(node->children[0]);
    if (!condition) return;

    // 创建基本块
    BasicBlock* then_block = BasicBlock::create(module_, new_temp() + "_then", current_function_);
    BasicBlock* else_block = nullptr;
    BasicBlock* merge_block = BasicBlock::create(module_, new_temp() + "_merge", current_function_);

    // 检查是否有else分支（第三个子节点）
    bool has_else = (node->children.size() >= 3);

    if (has_else) {
        else_block = BasicBlock::create(module_, new_temp() + "_else", current_function_);
    } else {
        else_block = merge_block;
    }

    // 创建条件分支
    builder_->create_cond_br(condition, then_block, else_block);

    // 处理then分支（第二个子节点）
    set_current_block(then_block);
    if (node->children.size() >= 2) {
        visitParseTreeNode(node->children[1]);
    }

    if (!then_block->get_terminator()) {
        builder_->create_br(merge_block);
    }

    // 处理else分支（第三个子节点）
    if (has_else) {
        set_current_block(else_block);
        visitParseTreeNode(node->children[2]);

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
            std::cout << "Error: Undefined variable '" << node->varName << "'" << std::endl;
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