#include "ir_generator.h"
#include "parse.h"
#include <iostream>
#include <cassert>
#include <set>

using namespace std;

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

string IRGenerator::generateFromParseTree(ParseTreeNode* parseTree, const string& source_file) {
    // cout << "\n\n\n[IRGEN] generateFromParseTree called" << endl;

    if (!parseTree) {
        cout << "Error: ParseTree is null" << endl;
        return "";
    }

    // cout << "[IRGEN] ParseTree semanticType: " << parseTree->semanticType << endl;

    // 内存检查：检查ParseTree的关键属性
    // cout << "[IRGEN] ParseTree memory check:" << endl;
    // cout << "[IRGEN]   semanticChildren size: " << parseTree->semanticChildren.size() << endl;
    // cout << "[IRGEN]   varName: '" << parseTree->varName << "'" << endl;
    // cout << "[IRGEN]   varType: '" << parseTree->varType << "'" << endl;

    // 创建模块
    // cout << "[IRGEN] Creating module..." << endl;
    module_ = new Module("sysy2022_compiler");
    if (!module_) {
        cout << "[IRGEN] ERROR: Failed to create module!" << endl;
        return "";
    }
    // cout << "[IRGEN] Module created successfully" << endl;

    // 检查模块类型系统
    // cout << "[IRGEN] Module type system initialization check:" << endl;
    // cout << "[IRGEN]   int32_type: " << (module_->get_int32_type() ? "OK" : "NULL") << endl;
    // cout << "[IRGEN]   void_type: " << (module_->get_void_type() ? "OK" : "NULL") << endl;
    // cout << "[IRGEN]   float_type: " << (module_->get_float_type() ? "OK" : "NULL") << endl;
    // cout << "[IRGEN]   int32_ptr_type: " << (module_->get_int32_ptr_type() ? "OK" : "NULL") << endl;

    // 设置源文件名
    if (!source_file.empty()) {
        module_->set_source_file_name(source_file);
    }

    // 声明内置函数
    // cout << "[IRGEN] Declaring builtin functions..." << endl;
    declare_builtin_functions();
    // cout << "[IRGEN] Builtin functions declared" << endl;

    // 遍历ParseTreeNode生成IR
    // cout << "[IRGEN] Starting visitParseTreeNode..." << endl;
    visitParseTreeNode(parseTree);
    // cout << "[IRGEN] visitParseTreeNode completed" << endl;

    // 生成IR文本
    // cout << "[IRGEN] Generating IR text..." << endl;
    string ir_code = module_->print();
    // cout << "[IRGEN] IR generation completed, length: " << ir_code.length() << endl;

    return ir_code;
}


void IRGenerator::visitParseTreeNode(ParseTreeNode* node) {
    static int depth = 0;
    const int MAX_DEPTH = 1000;

    try {

        string indent(depth * 2, ' ');
        cout << indent << "[IRGEN] visitParseTreeNode: " << (node ? node->semanticType : "null") << " (depth=" << depth << ")" << endl;

        if (depth > MAX_DEPTH) {
            cout << indent << "[IRGEN] Error: Maximum recursion depth exceeded (" << depth << ")" << endl;
            return;
        }

    if (!node) {
        cout << indent << "[IRGEN] Node is null, returning" << endl;
        return;
    }

    // 边界检查
    if (node->semanticChildren.empty()) {
        cout << indent << "[IRGEN] Warning: Node has no semantic children, semanticType: " << node->semanticType << endl;
    }

    DepthGuard guard(depth);  // RAII管理深度计数

    if (node->semanticType.empty()) {
        // 跳过没有语义信息的节点
        cout << indent << "[IRGEN] Node has empty semanticType, processing " << node->semanticChildren.size() << " semantic children" << endl;
        for (auto* child : node->semanticChildren) {
            visitParseTreeNode(child);
        }
        return;
    }

    // 处理不同类型的节点
    if (node->semanticType == "Program") {
        // 处理Program节点：先处理全局变量，再处理函数
        // cout << indent << "[IRGEN] === Processing Program ===" << endl;

        // 检查模块状态
        if (!module_) {
            // cout << indent << "[IRGEN] ERROR: Module is null!" << endl;
            return;
        }

        // cout << indent << "[IRGEN] Module check:" << endl;
        // cout << indent << "[IRGEN]   int32_type: " << (module_->get_int32_type() ? "OK" : "NULL") << endl;
        // cout << indent << "[IRGEN]   void_type: " << (module_->get_void_type() ? "OK" : "NULL") << endl;
        // cout << indent << "[IRGEN]   float_type: " << (module_->get_float_type() ? "OK" : "NULL") << endl;

        // cout << indent << "[IRGEN] Children count: " << node->semanticChildren.size() << endl;

    // 关键调试点：检查semanticChildren向量本身
    // cout << indent << "[IRGEN] DEBUG: Checking semanticChildren vector..." << endl;
    if (node->semanticChildren.empty()) {
        cout << indent << "[IRGEN] semanticChildren is empty" << endl;
    } else {
        cout << indent << "[IRGEN] semanticChildren is not empty, size: " << node->semanticChildren.size() << endl;
    }

    // 详细输出每个子节点信息 - 添加逐个子节点的处理
    for (size_t i = 0; i < node->semanticChildren.size(); i++) {
        // cout << indent << "[IRGEN] DEBUG: Starting to process child " << i << endl;

        auto* child = node->semanticChildren[i];
        if (child) {
            // cout << indent << "[IRGEN] Child " << i << " is not null" << endl;

            // // 关键调试点2：逐个输出属性，避免一次性输出可能的问题
            // cout << indent << "[IRGEN]   Getting semanticType..." << endl;
            // cout << indent << "[IRGEN]   semanticType: '" << child->semanticType << "'" << endl;

            // cout << indent << "[IRGEN]   Getting varName..." << endl;
            // cout << indent << "[IRGEN]   varName: '" << child->varName << "'" << endl;

            // cout << indent << "[IRGEN]   Getting varType..." << endl;
            // cout << indent << "[IRGEN]   varType: '" << child->varType << "'" << endl;

            // cout << indent << "[IRGEN]   Getting isGlobal..." << endl;
            // cout << indent << "[IRGEN]   isGlobal: " << child->isGlobal << endl;

            // cout << indent << "[IRGEN]   Getting semanticChildren count..." << endl;
            // cout << indent << "[IRGEN]   semanticChildren count: " << child->semanticChildren.size() << endl;

            // 检查关键属性
            if (child->semanticType.empty()) {
                cout << indent << "[IRGEN] WARNING: Child " << i << " has empty semanticType" << endl;
            }
            if (child->varName.empty()) {
                cout << indent << "[IRGEN] WARNING: Child " << i << " has empty varName" << endl;
            }
        } else {
            cout << indent << "[IRGEN] Child " << i << ": NULL" << endl;
        }
        cout << indent << "[IRGEN] DEBUG: Finished processing child " << i << endl;
    }

    // cout << indent << "[IRGEN] DEBUG: Finished listing all children" << endl;

    // // 关键调试点3：检查第一个循环开始
    // cout << indent << "[IRGEN] DEBUG: Starting first loop (global variables)" << endl;

    // // 第一个循环：处理全局变量
    // cout << indent << "[IRGEN] First loop: Processing global variables" << endl;
    
    // 【修改点1：修复全局变量重复生成问题】
    // 使用集合记录已处理的全局变量，避免重复
    set<string> processed_globals;
    int globalVarCount = 0;
    for (auto* child : node->semanticChildren) {
        if (child) {
            cout << indent << "[IRGEN] Checking child for VarDecl: " << child->semanticType
                      << ", isGlobal=" << child->isGlobal << endl;
            if (child->semanticType == "VarDecl" && child->isGlobal) {
                // 检查是否已经处理过这个全局变量
                if (processed_globals.find(child->varName) == processed_globals.end()) {
                    cout << indent << "[IRGEN] Processing global VarDecl: " << child->varName << endl;
                    globalVarCount++;
                    visitParseTreeNode(child);
                    processed_globals.insert(child->varName);
                    cout << indent << "[IRGEN] Completed global VarDecl: " << child->varName << endl;
                } else {
                    cout << indent << "[IRGEN] Warning: Skipping duplicate global variable: " << child->varName << endl;
                }
            }
        }
    }
    // cout << indent << "[IRGEN] Processed " << globalVarCount << " global variables" << endl;

    // 关键调试点4：检查第二个循环开始
    // cout << indent << "[IRGEN] DEBUG: Starting second loop (FunctionDef)" << endl;

    // 第二个循环：处理函数定义
    // cout << indent << "[IRGEN] Second loop: Processing FunctionDef" << endl;
    int functionCount = 0;
    for (auto* child : node->semanticChildren) {
        if (child) {
            // cout << indent << "[IRGEN] Checking child for FunctionDef: " << child->semanticType << endl;
            if (child->semanticType == "FunctionDef") {
                // cout << indent << "[IRGEN] Processing FunctionDef: " << child->varName << endl;
                functionCount++;
                visitParseTreeNode(child);
                // cout << indent << "[IRGEN] Completed FunctionDef: " << child->varName << endl;
            } else if (child->semanticType.empty()) {
                // cout << indent << "[IRGEN] Warning: Skipping empty semanticType child in Program" << endl;
                // 可以递归处理空节点的子节点
                for (auto* grandchild : child->semanticChildren) {
                    visitParseTreeNode(grandchild);
                }
            }
        }
    }
    // cout << indent << "[IRGEN] Processed " << functionCount << " functions" << endl;
    // cout << indent << "[IRGEN] Finished all FunctionDef processing" << endl;
    }
    else if (node->semanticType == "VarDecl") {
        // cout << indent << "[IRGEN] Processing VarDecl: " << node->varName << endl;
        
        // 【修改点2：在VarDecl处理中添加模块级全局变量检查】
        if (node->isGlobal) {
            // 检查模块中是否已存在同名全局变量
            auto global_vars = module_->get_global_variable();
            for (auto* gv : global_vars) {
                if (gv->get_name() == node->varName) {
                    cout << indent << "[IRGEN] ⚠️ERROR: Global variable '" << node->varName 
                              << "' already exists in module! This should not happen!" << endl;
                    return;  // 直接返回，不重复处理
                }
            }
            
            // 检查符号表中是否已存在同名全局变量（双重保险）
            Value* existing_var = symbol_table_->get(node->varName);
            if (existing_var) {
                cout << indent << "[IRGEN] Warning: Global variable '" << node->varName 
                          << "' already exists in symbol table, skipping duplicate" << endl;
                return;  // 直接返回，不重复处理
            }
        }
        
        // 处理变量声明
        Type* var_type = convert_type(node->varType);

        if (node->isGlobal) {
            cout << indent << "[IRGEN]  Creating global variable: " << node->varName << endl;
            // 全局变量
            Constant* init_value = nullptr;

            /* ===== 浮点支持 ===== */
            if (node->varType == "float") {
                float val = node->initValue.empty() ? 0.0f : stof(node->initValue);
                init_value = ConstantFloat::get(val, module_);
            } else {   // int
                int val = node->initValue.empty() ? 0 : stoi(node->initValue);
                init_value = ConstantInt::get(val, module_);
            }

            GlobalVariable* global_var = GlobalVariable::create(
                node->varName, module_, var_type, false, init_value
            );
            symbol_table_->put(node->varName, global_var);
            cout << indent << "[IRGEN]  Created global variable: " << node->varName << endl;
        } else {
            // cout << indent << "[IRGEN] Creating local variable" << endl;
            // 局部变量 - 需要在函数上下文中创建
            if (current_function_ && current_block_) {
                AllocaInst* alloca = builder_->create_alloca(var_type);
                alloca->set_name(node->varName);
                symbol_table_->put(node->varName, alloca);

                // 初始化 - 优先使用semanticChildren表达式，fallback才用initValue字符串
                if (!node->semanticChildren.empty()) {
                    // cout << indent << "[IRGEN] Using semanticChildren for initialization" << endl;
                    for (auto* child : node->semanticChildren) {
                        if (child && !child->semanticType.empty()) {
                            Value* init_expr = visitParseTreeExpr(child);
                            if (init_expr) {
                                builder_->create_store(init_expr, alloca);
                                break;
                            }
                        }
                    }
                } else if (!node->initValue.empty()) {
                    // Fallback：简单的常量初始化（当semanticChildren为空时）
                    // cout << indent << "[IRGEN] Using initValue string for initialization" << endl;

                    /* ===== 浮点支持 ===== */
                    Constant* init_val = nullptr;
                    if (node->varType == "float") {
                        init_val = ConstantFloat::get(stof(node->initValue), module_);
                    } else {
                        init_val = ConstantInt::get(stoi(node->initValue), module_);
                    }
                    builder_->create_store(init_val, alloca);
                }
            }
        }
    }
    else if (node->semanticType == "FunctionDef") {
        // 原函数定义逻辑无改动，省略大量注释节省篇幅
        Type* return_type = convert_type(node->varType);
        vector<Type*> param_types;
        vector<string> param_names;
        for (auto* child : node->semanticChildren) {
            if (child && child->semanticType == "FuncParam") {
                param_types.push_back(convert_type(child->varType));
                param_names.push_back(child->varName);
            }
        }
        FunctionType* func_type = FunctionType::get(return_type, param_types);
        Function* function = Function::create(func_type, node->varName, module_);
        current_function_ = function;
        SymbolTable* parent_table = symbol_table_;
        symbol_table_ = new SymbolTable(symbol_table_);

        BasicBlock* entry_block = BasicBlock::create(module_, node->varName + "_ENTRY", function, false);
        set_current_block(entry_block);
        builder_ = new IRBuilder(current_block_, module_);
        builder_->set_curFunc(function);

        auto& args = function->get_args();
        size_t i = 0;
        for (auto* arg : args) {
            if (i < param_names.size()) {
                arg->set_name(param_names[i]);
                AllocaInst* alloca = builder_->create_alloca(param_types[i]);
                // 为alloca变量添加前缀以避免与参数名冲突
                alloca->set_name("_" + param_names[i]);
                builder_->create_store(arg, alloca);
                symbol_table_->put(param_names[i], alloca);
                ++i;
            }
        }
        for (auto* child : node->semanticChildren) {
            if (child && child->semanticType != "FuncParam") {
                cout << "[IRGEN] Function body processing child with type: " << child->semanticType << endl;
                visitParseTreeNode(child);
            }
        }

        // 确保只有在当前块还没有终止指令时才添加默认返回语句
        if (current_block_ && !current_block_->get_terminator()) {
            if (return_type->is_void_type()) {
                builder_->create_void_ret();
            } else {
                // 对于非void函数，我们应该返回result变量的值，而不是硬编码的0
                // 查找result变量并返回它
                Value* result_var = symbol_table_->get("result");
                if (result_var) {
                    Value* result_value = builder_->create_load(result_var);
                    builder_->create_ret(result_value);
                } else {
                    // 如果找不到result变量，则回退到默认返回0
                    ConstantInt* zero = ConstantInt::get(0, module_);
                    builder_->create_ret(zero);
                }
            }
        }
        delete symbol_table_;
        symbol_table_ = parent_table;
        delete builder_;
        builder_ = nullptr;
        current_function_ = nullptr;
        current_block_ = nullptr;
    }
    else if (node->semanticType == "Assignment") {
        Value* left_var = symbol_table_->get(node->varName);
        if (!left_var) {
            cout << "[IRGEN] Error: Undefined variable '" << node->varName << "'" << endl;
            return;
        }
        if (!node->semanticChildren.empty()) {
            Value* right_value = visitParseTreeExpr(node->semanticChildren[0]);
            if (right_value && builder_)
                builder_->create_store(right_value, left_var);
        }
    }
    else if (node->semanticType == "ReturnStmt") {
        if (node->semanticChildren.empty()) {
            builder_->create_void_ret();
        } else {
            Value* ret_val = visitParseTreeExpr(node->semanticChildren[0]);
            if (ret_val)
                builder_->create_ret(ret_val);
            else
                builder_->create_void_ret();
        }
    }
    else if (node->semanticType == "IfStmt") {
        handleIfStatement(node);
    }
    else if (node->semanticType == "FunctionCall") {
        // 处理函数调用语句
        cout << "[IRGEN] Processing FunctionCall: " << node->varName << endl;
        Value* callValue = visitParseTreeExpr(node);
        if (callValue) {
            cout << "[IRGEN] FunctionCall generated value of type: " << callValue->get_type()->print() << endl;
        } else {
            cout << "[IRGEN] FunctionCall returned nullptr" << endl;
        }
        // 对于void返回类型的函数调用，不需要做任何额外处理
        // 对于有返回值的函数调用，返回值会被忽略（作为表达式语句）
    }
    else {
        // 检查是否是表达式语句（如函数调用）
        // if (!node->semanticType.empty()) {
        //     cout << "[IRGEN] Processing unknown node type: " << node->semanticType << endl;
        //     // 尝试作为表达式处理
        //     Value* exprValue = visitParseTreeExpr(node);
        //     if (exprValue) {
        //         cout << "[IRGEN] Expression returned value of type: " << exprValue->get_type()->print() << endl;
        //     } else {
        //         cout << "[IRGEN] Expression returned nullptr" << endl;
        //     }
        //     // 如果是函数调用且有返回值，可能需要处理返回值
        //     // 但如果函数返回void，则不需要做任何事情
        // } else {
        //     cout << "[IRGEN] Processing node with empty semanticType" << endl;
        // }
        
        // 递归处理子节点
        for (auto* child : node->semanticChildren) {
            visitParseTreeNode(child);
        }
    }
    } catch (...) {
        // 异常处理逻辑
        cout << "[IRGEN] Exception in visitParseTreeNode" << endl;
    }
}  

Value* IRGenerator::visitParseTreeExpr(ParseTreeNode* node) {
    if (!node) return nullptr;

    if (node->semanticType == "Number") {
        /* ===== 浮点支持 ===== */
        // 首先检查varType字段
        if (node->varType == "float") {          // 根据语义分析后的类型
            float val = stof(node->value);
            return ConstantFloat::get(val, module_);
        } 
        // 如果varType不是float，尝试通过值的内容判断类型
        else if (node->value.find('.') != string::npos || 
                 node->value.find('e') != string::npos || 
                 node->value.find('E') != string::npos) {  // 包含小数点或科学计数法
            float val = stof(node->value);
            return ConstantFloat::get(val, module_);
        } 
        // 默认为整数
        else {
            int val = stoi(node->value);
            return ConstantInt::get(val, module_);
        }
    }
    if (node->semanticType == "Variable") {
        Value* var = symbol_table_->get(node->varName);
        if (!var) return nullptr;
        return builder_->create_load(var);
    }
    if (node->semanticType == "BinaryExpr") {
        if (node->semanticChildren.size() >= 2) {
            Value* left  = visitParseTreeExpr(node->semanticChildren[0]);
            Value* right = visitParseTreeExpr(node->semanticChildren[1]);
            if (!left || !right) return nullptr;
            string op = node->operatorType;
            bool isFloat = left->get_type()->is_float_type() ||
                           right->get_type()->is_float_type();

            Value* res = nullptr;
            if (op == "+") {
                res = isFloat ? static_cast<Value*>(builder_->create_fadd(left, right))
                            : static_cast<Value*>(builder_->create_iadd(left, right));
                return res;
            }
            if (op == "-") {
                res = isFloat ? static_cast<Value*>(builder_->create_fsub(left, right))
                            : static_cast<Value*>(builder_->create_isub(left, right));
                return res;
            }
            if (op == "*") {
                res = isFloat ? static_cast<Value*>(builder_->create_fmul(left, right))
                            : static_cast<Value*>(builder_->create_imul(left, right));
                return res;
            }
            if (op == "/") {
                res = isFloat ? static_cast<Value*>(builder_->create_fdiv(left, right))
                            : static_cast<Value*>(builder_->create_isdiv(left, right));
                return res;
            }

            if (op == ">") {
                res = isFloat ? static_cast<Value*>(builder_->create_fcmp_ogt(left, right))
                            : static_cast<Value*>(builder_->create_icmp_gt(left, right));
                return res;
            }
            if (op == "<") {
                res = isFloat ? static_cast<Value*>(builder_->create_fcmp_olt(left, right))
                            : static_cast<Value*>(builder_->create_icmp_lt(left, right));
                return res;
            }
            if (op == ">=") {
                res = isFloat ? static_cast<Value*>(builder_->create_fcmp_oge(left, right))
                            : static_cast<Value*>(builder_->create_icmp_ge(left, right));
                return res;
            }
            if (op == "<=") {
                res = isFloat ? static_cast<Value*>(builder_->create_fcmp_ole(left, right))
                            : static_cast<Value*>(builder_->create_icmp_le(left, right));
                return res;
            }
            if (op == "==") {
                res = isFloat ? static_cast<Value*>(builder_->create_fcmp_oeq(left, right))
                            : static_cast<Value*>(builder_->create_icmp_eq(left, right));
                return res;
            }
            if (op == "!=") {
                res = isFloat ? static_cast<Value*>(builder_->create_fcmp_one(left, right))
                            : static_cast<Value*>(builder_->create_icmp_ne(left, right));
                return res;
            }
            Value* operand = visitParseTreeExpr(node->semanticChildren[0]);
            if (!operand) return nullptr;
            Value* zero  = isFloat ? static_cast<Value*>(ConstantFloat::get(0.0f, module_))
                                : static_cast<Value*>(ConstantInt::get(0, module_));
            return isFloat ? static_cast<Value*>(builder_->create_fsub(zero, operand))
                        : static_cast<Value*>(builder_->create_isub(zero, operand));
       
        }
        return nullptr;
    }
    if (node->semanticType == "UnaryExp" && node->operatorType == "-") {        Value* operand = visitParseTreeExpr(node->semanticChildren[0]);
        if (!operand) return nullptr;
        bool isFloat = operand->get_type()->is_float_type();
        Value* zero = isFloat ? static_cast<Value*>(ConstantFloat::get(0.0f, module_))
                              : static_cast<Value*>(ConstantInt::get(0, module_));
   
        if (isFloat) return static_cast<Value*>(builder_->create_fsub(zero, operand));
        else{
            return static_cast<Value*>(builder_->create_isub(zero, operand));
        }         
        
    }
    if (node->semanticType == "FunctionCall") {
        cout << "[IRGEN] visitParseTreeExpr processing FunctionCall: " << node->varName << endl;
        /* 原函数调用逻辑无改动 */
        string funcName = node->varName;
        Function* func = nullptr;
        for (auto* f : module_->get_functions())
            if (f->get_name() == funcName) { func = f; break; }
        if (!func) {
            cout << "[IRGEN] Function not found: " << funcName << endl;
            return nullptr;
        }
        
        // 获取函数参数类型
        auto funcType = func->get_function_type();
        unsigned numParams = funcType->get_num_of_args();
        
        cout << "[IRGEN] Function " << funcName << " has " << numParams << " parameters" << endl;
        
        vector<Value*> args;
        for (size_t i = 0; i < node->semanticChildren.size() && i < numParams; i++) {
            cout << "[IRGEN] Processing argument " << i << endl;
            Value* v = visitParseTreeExpr(node->semanticChildren[i]);
            if (v) {
                cout << "[IRGEN] Argument " << i << " has type: " << v->get_type()->print() << endl;
                // 检查是否需要类型转换
                Type* paramType = funcType->get_param_type(i);
                Type* argType = v->get_type();
                
                cout << "[IRGEN] Parameter " << i << " expected type: " << paramType->print() << endl;
                
                // 整数到浮点数的转换
                if (argType->is_int32_type() && paramType->is_float_type()) {
                    // 创建sitofp指令进行类型转换
                    cout << "[IRGEN] Converting int to float" << endl;
                    Value* converted = builder_->create_sitofp(v, paramType);
                    args.push_back(converted);
                } else {
                    args.push_back(v);
                }
            } else {
                cout << "[IRGEN] Argument " << i << " is null" << endl;
            }
        }
        cout << "[IRGEN] Creating call to " << funcName << " with " << args.size() << " arguments" << endl;
        return builder_->create_call(func, args);
    }
    /* 其余表达式节点按原逻辑处理 */
    for (auto* child : node->semanticChildren) {
        Value* v = visitParseTreeExpr(child);
        if (v) return v;
    }
    return nullptr;
}

void IRGenerator::handleIfStatement(ParseTreeNode* node) {
    // IfStmt结构: semanticChildren[0]=Condition, [1]=ThenBranch, [2]=ElseBranch(可选)
    if (!builder_ || node->semanticChildren.size() < 2) return;

    // 获取Condition节点的子节点作为条件表达式
    ParseTreeNode* conditionNode = node->semanticChildren[0];
    Value* condition = visitParseTreeExpr(conditionNode->semanticChildren.empty() ?
                                          nullptr : conditionNode->semanticChildren[0]);
    if (!condition) return;

    // 【修改点4：修复条件判断类型问题】
    // 将条件值转换为i1类型（如果当前不是i1类型）
    Value* bool_condition = condition;
    if (condition->get_type()->is_int32_type()) {
        // 将i32转换为i1：condition != 0
        Value* zero = ConstantInt::get(0, module_);
        bool_condition = builder_->create_icmp_ne(condition, zero);
    }

    // 【修改点5：解决嵌套if语句标签重复问题】
    // 使用唯一标识符确保基本块名称唯一
    static int if_stmt_counter = 0;
    int current_if_id = ++if_stmt_counter;
    
    // 创建基本块 - 使用唯一标识符避免命名冲突
    BasicBlock* then_block = BasicBlock::create(module_, "if_then_" + to_string(current_if_id), current_function_);
    BasicBlock* else_block = nullptr;
    BasicBlock* merge_block = BasicBlock::create(module_, "if_end_" + to_string(current_if_id), current_function_);

    // 检查是否有外层合并点需要跳转
    BasicBlock* outer_merge_block = nullptr;
    if (!merge_block_stack_.empty()) {
        outer_merge_block = merge_block_stack_.back();
    }

    // 将当前合并点压入栈
    merge_block_stack_.push_back(merge_block);

    // 检查是否有else分支
    bool has_else = (node->semanticChildren.size() >= 3);

    if (has_else) {
        else_block = BasicBlock::create(module_, "if_else_" + to_string(current_if_id), current_function_);
    } else {
        else_block = merge_block;
    }

    // 创建条件分支 - 使用转换后的bool条件
    builder_->create_cond_br(bool_condition, then_block, else_block);

    // 处理then分支
    set_current_block(then_block);
    ParseTreeNode* thenBranch = node->semanticChildren[1];
    for (auto* stmt : thenBranch->semanticChildren) {
        visitParseTreeNode(stmt);
    }

    // 只有在当前块还没有终止指令时才添加跳转到merge块的指令
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

        // 只有在当前块还没有终止指令时才添加跳转到merge块的指令
        if (!else_block->get_terminator()) {
            builder_->create_br(merge_block);
        }
    }

    // 从栈中弹出合并点
    merge_block_stack_.pop_back();

    // 如果存在外层合并点，则在当前合并点添加跳转到外层合并点的指令
    if (outer_merge_block) {
        set_current_block(merge_block);
        if (!merge_block->get_terminator()) {
            builder_->create_br(outer_merge_block);
        }
    }

    // 设置插入点到merge块，确保后续指令在此处继续执行
    set_current_block(merge_block);
}


string IRGenerator::new_temp() {
    return "op" + to_string(temp_counter_++);
}

void IRGenerator::set_current_block(BasicBlock* bb) {
    current_block_ = bb;
    if (builder_) {
        builder_->set_insert_point(bb);
    }
}

Type* IRGenerator::convert_type(const string& type_name) {
    try {
        if (!module_) {
            cout << "[IRGEN] ERROR: Module is null in convert_type!" << endl;
            return nullptr;
        }

        // cout << "[IRGEN] Converting type: '" << type_name << "'" << endl;

        if (type_name == "int") {
            auto* type = module_->get_int32_type();
            // cout << "[IRGEN]   -> int32_type: " << (type ? "OK" : "NULL") << endl;
            return type;
        } else if (type_name == "float") {
            auto* type = module_->get_float_type();
            // cout << "[IRGEN]   -> float_type: " << (type ? "OK" : "NULL") << endl;
            return type;
        } else if (type_name == "void") {
            auto* type = module_->get_void_type();
            // cout << "[IRGEN]   -> void_type: " << (type ? "OK" : "NULL") << endl;
            return type;
        }

        // cout << "[IRGEN] WARNING: Unknown type '" << type_name << "', defaulting to int32" << endl;
        return module_->get_int32_type();
    } catch (const exception& e) {
        cout << "[IRGEN] Exception in convert_type: " << e.what() << endl;
        return nullptr;
    } catch (...) {
        cout << "[IRGEN] Unknown exception in convert_type" << endl;
        return nullptr;
    }
}

void IRGenerator::declare_builtin_functions() {
    // getint(): i32
    FunctionType* getint_type = FunctionType::get(module_->get_int32_type(), {});
    Function::create(getint_type, "getint", module_);

    // getch(): i32
    FunctionType* getch_type = FunctionType::get(module_->get_int32_type(), {});
    Function::create(getch_type, "getch", module_);

    // getarray(i32*): i32
    vector<Type*> getarray_params = {module_->get_int32_ptr_type()};
    FunctionType* getarray_type = FunctionType::get(module_->get_int32_type(), getarray_params);
    Function::create(getarray_type, "getarray", module_);

    // putint(i32): void
    vector<Type*> putint_params = {module_->get_int32_type()};
    FunctionType* putint_type = FunctionType::get(module_->get_void_type(), putint_params);
    Function::create(putint_type, "putint", module_);

    

    // putch(i32): void
    vector<Type*> putch_params = {module_->get_int32_type()};
    FunctionType* putch_type = FunctionType::get(module_->get_void_type(), putch_params);
    Function::create(putch_type, "putch", module_);

    

    // putarray(i32, i32*): void
    vector<Type*> putarray_params = {module_->get_int32_type(), module_->get_int32_ptr_type()};
    FunctionType* putarray_type = FunctionType::get(module_->get_void_type(), putarray_params);
    Function::create(putarray_type, "putarray", module_);

    // starttime(): void
    FunctionType* starttime_type = FunctionType::get(module_->get_void_type(), {});
    Function::create(starttime_type, "starttime", module_);

    // stoptime(): void
    FunctionType* stoptime_type = FunctionType::get(module_->get_void_type(), {});
    Function::create(stoptime_type, "stoptime", module_);
}