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

    // 内存检查：检查parseTree的关键属性
    std::cout << "[IRGEN] ParseTree memory check:" << std::endl;
    std::cout << "[IRGEN]   semanticChildren size: " << parseTree->semanticChildren.size() << std::endl;
    std::cout << "[IRGEN]   varName: '" << parseTree->varName << "'" << std::endl;
    std::cout << "[IRGEN]   varType: '" << parseTree->varType << "'" << std::endl;

    // 创建模块
    std::cout << "[IRGEN] Creating module..." << std::endl;
    module_ = new Module("sysy2022_compiler");
    if (!module_) {
        std::cout << "[IRGEN] ERROR: Failed to create module!" << std::endl;
        return "";
    }
    std::cout << "[IRGEN] Module created successfully" << std::endl;

    // 检查模块类型系统
    std::cout << "[IRGEN] Module type system initialization check:" << std::endl;
    std::cout << "[IRGEN]   int32_type: " << (module_->get_int32_type() ? "OK" : "NULL") << std::endl;
    std::cout << "[IRGEN]   void_type: " << (module_->get_void_type() ? "OK" : "NULL") << std::endl;
    std::cout << "[IRGEN]   float_type: " << (module_->get_float_type() ? "OK" : "NULL") << std::endl;
    std::cout << "[IRGEN]   int32_ptr_type: " << (module_->get_int32_ptr_type() ? "OK" : "NULL") << std::endl;

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
    const int MAX_DEPTH = 100;

    try {

        std::string indent(depth * 2, ' ');
        std::cout << indent << "[IRGEN] visitParseTreeNode: " << (node ? node->semanticType : "null") << " (depth=" << depth << ")" << std::endl;

        if (depth > MAX_DEPTH) {
            std::cout << indent << "[IRGEN] Error: Maximum recursion depth exceeded (" << depth << ")" << std::endl;
            return;
        }

    if (!node) {
        std::cout << indent << "[IRGEN] Node is null, returning" << std::endl;
        return;
    }

    // 边界检查
    if (node->semanticChildren.empty()) {
        std::cout << indent << "[IRGEN] Warning: Node has no semantic children, semanticType: " << node->semanticType << std::endl;
    }

    DepthGuard guard(depth);  // RAII管理深度计数

    if (node->semanticType.empty()) {
        // 跳过没有语义信息的节点
        std::cout << indent << "[IRGEN] Node has empty semanticType, processing " << node->semanticChildren.size() << " semantic children" << std::endl;
        for (auto* child : node->semanticChildren) {
            visitParseTreeNode(child);
        }
        return;
    }

    // 处理不同类型的节点
    if (node->semanticType == "Program") {
        // 处理Program节点：先处理全局变量，再处理函数
        std::cout << indent << "[IRGEN] === Processing Program ===" << std::endl;

        // 检查模块状态
        if (!module_) {
            std::cout << indent << "[IRGEN] ERROR: Module is null!" << std::endl;
            return;
        }

        std::cout << indent << "[IRGEN] Module check:" << std::endl;
        std::cout << indent << "[IRGEN]   int32_type: " << (module_->get_int32_type() ? "OK" : "NULL") << std::endl;
        std::cout << indent << "[IRGEN]   void_type: " << (module_->get_void_type() ? "OK" : "NULL") << std::endl;
        std::cout << indent << "[IRGEN]   float_type: " << (module_->get_float_type() ? "OK" : "NULL") << std::endl;

        std::cout << indent << "[IRGEN] Children count: " << node->semanticChildren.size() << std::endl;

    // 关键调试点1：检查semanticChildren向量本身
    std::cout << indent << "[IRGEN] DEBUG: Checking semanticChildren vector..." << std::endl;
    if (node->semanticChildren.empty()) {
        std::cout << indent << "[IRGEN] semanticChildren is empty" << std::endl;
    } else {
        std::cout << indent << "[IRGEN] semanticChildren is not empty, size: " << node->semanticChildren.size() << std::endl;
    }

    // 详细输出每个子节点信息 - 添加逐个子节点的处理
    for (size_t i = 0; i < node->semanticChildren.size(); i++) {
        std::cout << indent << "[IRGEN] DEBUG: Starting to process child " << i << std::endl;

        auto* child = node->semanticChildren[i];
        if (child) {
            std::cout << indent << "[IRGEN] Child " << i << " is not null" << std::endl;

            // 关键调试点2：逐个输出属性，避免一次性输出可能的问题
            std::cout << indent << "[IRGEN]   Getting semanticType..." << std::endl;
            std::cout << indent << "[IRGEN]   semanticType: '" << child->semanticType << "'" << std::endl;

            std::cout << indent << "[IRGEN]   Getting varName..." << std::endl;
            std::cout << indent << "[IRGEN]   varName: '" << child->varName << "'" << std::endl;

            std::cout << indent << "[IRGEN]   Getting varType..." << std::endl;
            std::cout << indent << "[IRGEN]   varType: '" << child->varType << "'" << std::endl;

            std::cout << indent << "[IRGEN]   Getting isGlobal..." << std::endl;
            std::cout << indent << "[IRGEN]   isGlobal: " << child->isGlobal << std::endl;

            std::cout << indent << "[IRGEN]   Getting semanticChildren count..." << std::endl;
            std::cout << indent << "[IRGEN]   semanticChildren count: " << child->semanticChildren.size() << std::endl;

            // 检查关键属性
            if (child->semanticType.empty()) {
                std::cout << indent << "[IRGEN] WARNING: Child " << i << " has empty semanticType" << std::endl;
            }
            if (child->varName.empty()) {
                std::cout << indent << "[IRGEN] WARNING: Child " << i << " has empty varName" << std::endl;
            }
        } else {
            std::cout << indent << "[IRGEN] Child " << i << ": NULL" << std::endl;
        }
        std::cout << indent << "[IRGEN] DEBUG: Finished processing child " << i << std::endl;
    }

    std::cout << indent << "[IRGEN] DEBUG: Finished listing all children" << std::endl;

    // 关键调试点3：检查第一个循环开始
    std::cout << indent << "[IRGEN] DEBUG: Starting first loop (global variables)" << std::endl;

    // 第一个循环：处理全局变量
    std::cout << indent << "[IRGEN] First loop: Processing global variables" << std::endl;
    int globalVarCount = 0;
    for (auto* child : node->semanticChildren) {
        if (child) {
            std::cout << indent << "[IRGEN] Checking child for VarDecl: " << child->semanticType
                      << ", isGlobal=" << child->isGlobal << std::endl;
            if (child->semanticType == "VarDecl" && child->isGlobal) {
                std::cout << indent << "[IRGEN] Processing global VarDecl" << std::endl;
                globalVarCount++;
                visitParseTreeNode(child);
                std::cout << indent << "[IRGEN] Completed global VarDecl: " << child->varName << std::endl;
            }
        }
    }
    std::cout << indent << "[IRGEN] Processed " << globalVarCount << " global variables" << std::endl;

    // 关键调试点4：检查第二个循环开始
    std::cout << indent << "[IRGEN] DEBUG: Starting second loop (FunctionDef)" << std::endl;

    // 第二个循环：处理函数定义
    std::cout << indent << "[IRGEN] Second loop: Processing FunctionDef" << std::endl;
    int functionCount = 0;
    for (auto* child : node->semanticChildren) {
        if (child) {
            std::cout << indent << "[IRGEN] Checking child for FunctionDef: " << child->semanticType << std::endl;
            if (child->semanticType == "FunctionDef") {
                std::cout << indent << "[IRGEN] Processing FunctionDef: " << child->varName << std::endl;
                functionCount++;
                visitParseTreeNode(child);
                std::cout << indent << "[IRGEN] Completed FunctionDef: " << child->varName << std::endl;
            } else if (child->semanticType.empty()) {
                std::cout << indent << "[IRGEN] Warning: Skipping empty semanticType child in Program" << std::endl;
                // 可以选择递归处理空节点的子节点
                for (auto* grandchild : child->semanticChildren) {
                    visitParseTreeNode(grandchild);
                }
            }
        }
    }
    std::cout << indent << "[IRGEN] Processed " << functionCount << " functions" << std::endl;
    std::cout << indent << "[IRGEN] Finished all FunctionDef processing" << std::endl;
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

        // 检查关键属性
        if (node->varName.empty()) {
            std::cout << indent << "[IRGEN] ERROR: FunctionDef has empty varName!" << std::endl;
            return;
        }
        if (node->varType.empty()) {
            std::cout << indent << "[IRGEN] ERROR: FunctionDef has empty varType!" << std::endl;
            return;
        }

        std::cout << indent << "[IRGEN] FunctionDef has " << node->semanticChildren.size() << " semantic children" << std::endl;

        // 处理函数定义
        std::cout << indent << "[IRGEN] Converting return type: " << node->varType << std::endl;
        Type* return_type = convert_type(node->varType);
        if (!return_type) {
            std::cout << indent << "[IRGEN] ERROR: Failed to convert return type!" << std::endl;
            return;
        }
        std::cout << indent << "[IRGEN] Return type converted successfully" << std::endl;

        // 处理函数参数
        std::vector<Type*> param_types;
        std::vector<std::string> param_names;

        // 根据对接文档，FunctionDef的semanticChildren包含FuncParam和函数体
        std::cout << indent << "[IRGEN] Processing function parameters..." << std::endl;
        for (auto* child : node->semanticChildren) {
            if (child) {
                std::cout << indent << "[IRGEN]   Child type: '" << child->semanticType << "', varName: '" << child->varName << "', varType: '" << child->varType << "'" << std::endl;
                if (child->semanticType == "FuncParam") {
                    std::cout << indent << "[IRGEN] Found FuncParam: " << child->varName << " type: " << child->varType << std::endl;
                    // 收集参数类型和名称
                    param_types.push_back(convert_type(child->varType));
                    param_names.push_back(child->varName);
                }
            }
        }
        std::cout << indent << "[IRGEN] Collected " << param_types.size() << " parameters" << std::endl;

        std::cout << indent << "[IRGEN] Creating FunctionType with " << param_types.size() << " params..." << std::endl;
        FunctionType* func_type = FunctionType::get(return_type, param_types);
        std::cout << indent << "[IRGEN] FunctionType created successfully" << std::endl;
        std::cout << indent << "[IRGEN] Creating Function: " << node->varName << std::endl;
        Function* function = Function::create(func_type, node->varName, module_);
        std::cout << indent << "[IRGEN] Function created successfully: " << node->varName << std::endl;

        current_function_ = function;
        std::cout << indent << "[IRGEN] Set current_function_ to: " << node->varName << std::endl;
        std::cout << indent << "[IRGEN] Creating new symbol table scope for function: " << node->varName << std::endl;
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
        int body_stmt_count = 0;
        for (auto* child : node->semanticChildren) {
            if (child && child->semanticType != "FuncParam") {
                // 跳过参数，处理函数体语句
                body_stmt_count++;
                std::cout << indent << "[IRGEN] Processing function body child #" << body_stmt_count << ": " << child->semanticType << " (varName: " << child->varName << ")" << std::endl;
                visitParseTreeNode(child);
                std::cout << indent << "[IRGEN] Finished processing function body child #" << body_stmt_count << ": " << child->semanticType << std::endl;
            }
        }
        std::cout << indent << "[IRGEN] Function body processing completed, processed " << body_stmt_count << " statements" << std::endl;

        // 确保函数有终结指令
        if (current_block_ && !current_block_->get_terminator()) {
            if (return_type->is_void_type()) {
                builder_->create_void_ret();
            } else {
                ConstantInt* zero = ConstantInt::get(0, module_);
                builder_->create_ret(zero);
            }
        }

        std::cout << indent << "[IRGEN] Restoring parent symbol table for function: " << node->varName << std::endl;
        delete symbol_table_;
        symbol_table_ = parent_table; // 恢复父作用域
        delete builder_;
        builder_ = nullptr;
        current_function_ = nullptr;
        current_block_ = nullptr;
        std::cout << indent << "[IRGEN] FunctionDef processing completed for: " << node->varName << std::endl;
    }
    else if (node->semanticType == "Assignment") {
        std::cout << indent << "[IRGEN] Processing Assignment for var '" << node->varName << "'" << std::endl;
        // 处理赋值语句
        Value* left_var = symbol_table_->get(node->varName);
        if (!left_var) {
            std::cout << indent << "[IRGEN] Error: Undefined variable '" << node->varName << "'" << std::endl;
            return;
        }

        // 获取右值 - 使用semanticChildren
        Value* right_value = nullptr;
        if (!node->semanticChildren.empty()) {
            std::cout << indent << "[IRGEN] Using semanticChildren[0]" << std::endl;
            right_value = visitParseTreeExpr(node->semanticChildren[0]);
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
        // 处理return语句 - 直接使用semanticChildren
        if (node->semanticChildren.empty()) {
            std::cout << indent << "[IRGEN] Void return" << std::endl;
            builder_->create_void_ret();
        } else {
            // 添加安全检查
            if (!node->semanticChildren[0]) {
                std::cout << indent << "[IRGEN] Error: Return expression child is null" << std::endl;
                builder_->create_void_ret();
                return;
            }

            std::cout << indent << "[IRGEN] Using semanticChildren[0] for return, type: "
                      << node->semanticChildren[0]->semanticType << std::endl;
            Value* ret_val = visitParseTreeExpr(node->semanticChildren[0]);
            if (ret_val) {
                std::cout << indent << "[IRGEN] Creating return with value" << std::endl;
                builder_->create_ret(ret_val);
            } else {
                std::cout << indent << "[IRGEN] Void return (no value found)" << std::endl;
                builder_->create_void_ret();
            }
        }
    }
    else if (node->semanticType == "IfStmt") {
        std::cout << indent << "[IRGEN] Processing IfStmt" << std::endl;
        handleIfStatement(node);
    }
    else {
        std::cout << indent << "[IRGEN] Unknown semanticType: '" << node->semanticType << "', processing semantic children" << std::endl;
        // 其他节点，递归处理子节点
        for (auto* child : node->semanticChildren) {
            visitParseTreeNode(child);
        }
    }

        std::cout << indent << "[IRGEN] visitParseTreeNode completed for: " << node->semanticType << std::endl;
    } catch (const std::exception& e) {
        std::cout << "[IRGEN] Exception in visitParseTreeNode: " << e.what() << std::endl;
        std::cout << "[IRGEN] Node semanticType: " << (node ? node->semanticType : "null") << std::endl;
        std::cout << "[IRGEN] Current depth: " << depth << std::endl;
    } catch (...) {
        std::cout << "[IRGEN] Unknown exception in visitParseTreeNode" << std::endl;
        std::cout << "[IRGEN] Node semanticType: " << (node ? node->semanticType : "null") << std::endl;
        std::cout << "[IRGEN] Current depth: " << depth << std::endl;
    }
}


Value* IRGenerator::visitParseTreeExpr(ParseTreeNode* node) {
    if (!node) {
        std::cout << "[IRGEN] visitParseTreeExpr: node is null" << std::endl;
        return nullptr;
    }

    std::cout << "[IRGEN] visitParseTreeExpr: semanticType='" << node->semanticType
              << "', value='" << node->value << "', varName='" << node->varName
              << "', operatorType='" << node->operatorType << "'" << std::endl;
    std::cout << "[IRGEN]   has " << node->semanticChildren.size() << " children" << std::endl;

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
        // 一元表达式
        if (!node->semanticChildren.empty()) {
            Value* operand = visitParseTreeExpr(node->semanticChildren[0]);
            if (!operand) return nullptr;

            std::string op = node->operatorType;
            if (op == "-") {
                // 负号：0 - operand
                Value* zero = ConstantInt::get(0, module_);
                return builder_->create_isub(zero, operand);
            } else if (op == "!") {
                // 逻辑非：转换为比较 operand == 0
                Value* zero = ConstantInt::get(0, module_);
                return builder_->create_icmp_eq(operand, zero);
            } else if (op == "+") {
                // 正号：直接返回操作数
                return operand;
            }
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
    else if (node->semanticType == "FunctionCall") {
        // 函数调用
        std::string funcName = node->varName;
        std::vector<Value*> args;

        // 计算所有参数
        for (auto* arg : node->semanticChildren) {
            Value* argValue = visitParseTreeExpr(arg);
            if (argValue) {
                args.push_back(argValue);
            }
        }

        // 获取函数
        Function* func = nullptr;
        auto functions = module_->get_functions();
        for (auto* f : functions) {
            if (f->get_name() == funcName) {
                func = f;
                break;
            }
        }
        if (!func) {
            std::cout << "Error: Function '" << funcName << "' not found" << std::endl;
            return nullptr;
        }

        // 生成call指令
        return builder_->create_call(func, args);
    }
    else if (node->semanticType == "Cond") {
        // 条件表达式
        return visitParseTreeExpr(node->semanticChildren.empty() ? nullptr : node->semanticChildren[0]);
    }
    else if (!node->operatorType.empty()) {
        // 通用二元表达式处理
        return handleBinaryExpr(node);
    }
    else {
        // 递归处理子表达式
        for (auto* child : node->semanticChildren) {
            Value* result = visitParseTreeExpr(child);
            if (result) return result;
        }
    }

    return nullptr;
}

Value* IRGenerator::handleBinaryExpr(ParseTreeNode* node) {
    if (!node) return nullptr;

    // 语义树结构：semanticChildren[0]和semanticChildren[1]分别是左右操作数
    // 操作符通过operatorType获取

    if (node->semanticChildren.size() >= 2) {
        Value* lhs = visitParseTreeExpr(node->semanticChildren[0]);
        Value* rhs = visitParseTreeExpr(node->semanticChildren[1]);

        if (!lhs || !rhs || !builder_) return nullptr;

        std::string op = node->operatorType;
        if (op.empty()) return nullptr;

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

    // 语义树结构：semanticChildren[0]和semanticChildren[1]分别是左右操作数
    // 操作符通过operatorType获取
    if (node->semanticChildren.size() < 2) return nullptr;

    Value* lhs = visitParseTreeExpr(node->semanticChildren[0]);
    Value* rhs = visitParseTreeExpr(node->semanticChildren[1]);

    if (!lhs || !rhs || !builder_) return nullptr;

    std::string op = node->operatorType;
    if (op.empty()) return nullptr;

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

    // 语义树结构：semanticChildren[0]和semanticChildren[1]分别是左右操作数
    // 操作符通过operatorType获取
    if (node->semanticChildren.size() < 2) return nullptr;

    Value* lhs = visitParseTreeExpr(node->semanticChildren[0]);
    Value* rhs = visitParseTreeExpr(node->semanticChildren[1]);

    if (!lhs || !rhs || !builder_) return nullptr;

    std::string op = node->operatorType;
    if (op.empty()) return nullptr;

    if (op == "==") {
        return builder_->create_icmp_eq(lhs, rhs);
    } else if (op == "!=") {
        return builder_->create_icmp_ne(lhs, rhs);
    }

    return nullptr;
}

Value* IRGenerator::handleLAndExpr(ParseTreeNode* node) {
    // 逻辑与: lhs && rhs
    if (!node || node->semanticChildren.size() < 2 || !builder_) return nullptr;

    // 计算左右操作数
    Value* lhs = visitParseTreeExpr(node->semanticChildren[0]);
    Value* rhs = visitParseTreeExpr(node->semanticChildren[1]);
    if (!lhs || !rhs) return nullptr;

    // 逻辑与：两个操作数都为真时结果为真
    // 先将操作数转换为i1类型（如果需要）
    if (lhs->get_type()->is_int32_type()) {
        Value* zero = ConstantInt::get(0, module_);
        lhs = builder_->create_icmp_ne(lhs, zero);
    }
    if (rhs->get_type()->is_int32_type()) {
        Value* zero = ConstantInt::get(0, module_);
        rhs = builder_->create_icmp_ne(rhs, zero);
    }

    // 生成逻辑与操作
    return builder_->create_iand(lhs, rhs);
}

Value* IRGenerator::handleLOrExpr(ParseTreeNode* node) {
    // 逻辑或: lhs || rhs
    if (!node || node->semanticChildren.size() < 2 || !builder_) return nullptr;

    // 计算左右操作数
    Value* lhs = visitParseTreeExpr(node->semanticChildren[0]);
    Value* rhs = visitParseTreeExpr(node->semanticChildren[1]);
    if (!lhs || !rhs) return nullptr;

    // 逻辑或：只要有一个操作数为真，结果就为真
    // 先将操作数转换为i1类型（如果需要）
    if (lhs->get_type()->is_int32_type()) {
        Value* zero = ConstantInt::get(0, module_);
        lhs = builder_->create_icmp_ne(lhs, zero);
    }
    if (rhs->get_type()->is_int32_type()) {
        Value* zero = ConstantInt::get(0, module_);
        rhs = builder_->create_icmp_ne(rhs, zero);
    }

    // 生成逻辑或操作
    return builder_->create_ior(lhs, rhs);
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
    try {
        if (!module_) {
            std::cout << "[IRGEN] ERROR: Module is null in convert_type!" << std::endl;
            return nullptr;
        }

        std::cout << "[IRGEN] Converting type: '" << type_name << "'" << std::endl;

        if (type_name == "int") {
            auto* type = module_->get_int32_type();
            std::cout << "[IRGEN]   -> int32_type: " << (type ? "OK" : "NULL") << std::endl;
            return type;
        } else if (type_name == "float") {
            auto* type = module_->get_float_type();
            std::cout << "[IRGEN]   -> float_type: " << (type ? "OK" : "NULL") << std::endl;
            return type;
        } else if (type_name == "void") {
            auto* type = module_->get_void_type();
            std::cout << "[IRGEN]   -> void_type: " << (type ? "OK" : "NULL") << std::endl;
            return type;
        }

        std::cout << "[IRGEN] WARNING: Unknown type '" << type_name << "', defaulting to int32" << std::endl;
        return module_->get_int32_type();
    } catch (const std::exception& e) {
        std::cout << "[IRGEN] Exception in convert_type: " << e.what() << std::endl;
        return nullptr;
    } catch (...) {
        std::cout << "[IRGEN] Unknown exception in convert_type" << std::endl;
        return nullptr;
    }
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
    // IfStmt结构: semanticChildren[0]=Condition, [1]=ThenBranch, [2]=ElseBranch(可选)
    if (!builder_ || node->semanticChildren.size() < 2) return;

    // 获取Condition节点的子节点作为条件表达式
    ParseTreeNode* conditionNode = node->semanticChildren[0];
    Value* condition = visitParseTreeExpr(conditionNode->semanticChildren.empty() ?
                                          nullptr : conditionNode->semanticChildren[0]);
    if (!condition) return;

    // 创建基本块
    BasicBlock* then_block = BasicBlock::create(module_, new_temp() + "_then", current_function_);
    BasicBlock* else_block = nullptr;
    BasicBlock* merge_block = BasicBlock::create(module_, new_temp() + "_merge", current_function_);

    // 检查是否有else分支
    bool has_else = (node->semanticChildren.size() >= 3);

    if (has_else) {
        else_block = BasicBlock::create(module_, new_temp() + "_else", current_function_);
    } else {
        else_block = merge_block;
    }

    // 创建条件分支
    builder_->create_cond_br(condition, then_block, else_block);

    // 处理then分支
    set_current_block(then_block);
    ParseTreeNode* thenBranch = node->semanticChildren[1];
    for (auto* stmt : thenBranch->semanticChildren) {
        visitParseTreeNode(stmt);
    }

    if (!then_block->get_terminator()) {
        builder_->create_br(merge_block);
    }

    // 处理else分支
    if (has_else) {
        set_current_block(else_block);
        ParseTreeNode* elseBranch = node->semanticChildren[2];
        for (auto* stmt : elseBranch->semanticChildren) {
            visitParseTreeNode(stmt);
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
        if (!node->semanticChildren.empty()) {
            // 第一个子节点应该是类型
            auto* type_node = node->semanticChildren[0];
            if (type_node) {
                Type* param_type = convert_type(type_node->symbol);
                param_types.push_back(param_type);
            }

            // 参数名应该在semanticType为FuncFParam的节点中
            param_names.push_back(node->varName);
        }
    }

    // 递归处理参数列表
    for (auto* child : node->semanticChildren) {
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