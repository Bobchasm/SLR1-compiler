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
    else if (node->semanticType == "FunctionDef") {
        // 处理函数定义
        Type* return_type = convert_type(node->varType);
        std::vector<Type*> param_types; // 简化：暂不处理参数
        FunctionType* func_type = FunctionType::get(return_type, param_types);
        Function* function = Function::create(func_type, node->varName, module_);

        current_function_ = function;
        symbol_table_ = new SymbolTable(symbol_table_); // 新的作用域

        // 创建入口基本块
        BasicBlock* entry_block = BasicBlock::create(module_, node->varName + "_entry", function);
        set_current_block(entry_block);

        // 创建IRBuilder
        builder_ = new IRBuilder(current_block_, module_);
        builder_->set_curFunc(function);

        // 处理函数体
        for (auto* child : node->children) {
            visitParseTreeNode(child);
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
        symbol_table_ = nullptr; // 恢复父作用域
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
        return builder_->create_load(var);
    }
    else if (!node->operatorType.empty()) {
        // 二元表达式
        if (node->children.size() >= 2) {
            Value* lhs = visitParseTreeExpr(node->children[0]);
            Value* rhs = visitParseTreeExpr(node->children[1]);

            if (!lhs || !rhs) return nullptr;

            if (node->operatorType == "+") {
                return builder_->create_iadd(lhs, rhs);
            } else if (node->operatorType == "-") {
                return builder_->create_isub(lhs, rhs);
            } else if (node->operatorType == "*") {
                return builder_->create_imul(lhs, rhs);
            } else if (node->operatorType == "/") {
                return builder_->create_isdiv(lhs, rhs);
            } else if (node->operatorType == "%") {
                return builder_->create_irem(lhs, rhs);
            } else if (node->operatorType == "==") {
                return builder_->create_icmp_eq(lhs, rhs);
            } else if (node->operatorType == "!=") {
                return builder_->create_icmp_ne(lhs, rhs);
            } else if (node->operatorType == "<") {
                return builder_->create_icmp_lt(lhs, rhs);
            } else if (node->operatorType == "<=") {
                return builder_->create_icmp_le(lhs, rhs);
            } else if (node->operatorType == ">") {
                return builder_->create_icmp_gt(lhs, rhs);
            } else if (node->operatorType == ">=") {
                return builder_->create_icmp_ge(lhs, rhs);
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