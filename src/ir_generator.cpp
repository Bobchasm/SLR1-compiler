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
    if (!parseTree) {
        cout << "Error: ParseTree is null" << endl;
        return "";
    }

    module_ = new Module("sysy2022_compiler");
    if (!module_) {
        cout << "[IRGEN] ERROR: Failed to create module!" << endl;
        return "";
    }

    if (!source_file.empty()) {
        module_->set_source_file_name(source_file);
    }

    declare_builtin_functions();

    visitParseTreeNode(parseTree);

    string ir_code = module_->print();
    return ir_code;
}

void IRGenerator::visitParseTreeNode(ParseTreeNode* node) {
    static int depth = 0;
    const int MAX_DEPTH = 1000;

    try {
        if (depth > MAX_DEPTH) {
            return;
        }

        if (!node) {
            return;
        }

        DepthGuard guard(depth);

        // ✅ 检测当前块是否已经终结,避免在死代码区生成IR
        if (current_block_ && current_block_->get_terminator()) {
            // 当前块已经有终结指令(如 ret 或 br),不再处理后续语句
            // 否则会生成不合法的IR(在终结指令后添加指令)
            cout << "[IRGEN-SKIP] Skipping statement after terminator in block: " 
                 << current_block_->get_name() << endl;
            return;
        }

        if (node->semanticType.empty()) {
            for (auto* child : node->semanticChildren) {
                visitParseTreeNode(child);
            }
            return;
        }

        if (node->semanticType == "Program") {
            set<string> processed_globals;
            for (auto* child : node->semanticChildren) {
                if (child && child->semanticType == "VarDecl" && child->isGlobal) {
                    if (processed_globals.find(child->varName) == processed_globals.end()) {
                        visitParseTreeNode(child);
                        processed_globals.insert(child->varName);
                    }
                }
            }
            for (auto* child : node->semanticChildren) {
                if (child && child->semanticType == "FunctionDef") {
                    visitParseTreeNode(child);
                }
            }
        }
        else if (node->semanticType == "VarDecl") {
            Type* var_type = convert_type(node->varType);

            if (node->isGlobal) {
                Constant* init_value = nullptr;
                if (node->varType == "float") {
                    float val = node->initValue.empty() ? 0.0f : stof(node->initValue);
                    init_value = ConstantFloat::get(val, module_);
                } else {
                    int val = node->initValue.empty() ? 0 : stoi(node->initValue);
                    init_value = ConstantInt::get(val, module_);
                }

                GlobalVariable* global_var = GlobalVariable::create(
                    node->varName, module_, var_type, false, init_value
                );
                symbol_table_->put(node->varName, global_var);
            } else {
                if (current_function_ && current_block_) {
                    AllocaInst* alloca = builder_->create_alloca(var_type);
                    alloca->set_name(node->varName);
                    symbol_table_->put(node->varName, alloca);

                    if (!node->semanticChildren.empty()) {
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
            Type* return_type = convert_type(node->varType);
            if (node->varName == "main" && return_type->is_void_type()) {
                return_type = module_->get_int32_type();
            }

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
                    alloca->set_name("_" + param_names[i]);
                    builder_->create_store(arg, alloca);
                    symbol_table_->put(param_names[i], alloca);
                    ++i;
                }
            }

            for (auto* child : node->semanticChildren) {
                if (child && child->semanticType != "FuncParam") {
                    visitParseTreeNode(child);
                }
            }

            if (current_block_ && !current_block_->get_terminator()) {
                if (return_type->is_void_type()) {
                    builder_->create_void_ret();
                } else {
                    Value* result_var = symbol_table_->get("result");
                    if (result_var) {
                        Value* result_value = builder_->create_load(result_var);
                        builder_->create_ret(result_value);
                    } else {
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
            cout << "[RETURN] Processing return statement" << endl;
            
            if (node->semanticChildren.empty()) {
                builder_->create_void_ret();
            } else {
                Value* ret_val = visitParseTreeExpr(node->semanticChildren[0]);
                if (ret_val)
                    builder_->create_ret(ret_val);
                else
                    builder_->create_void_ret();
            }
            
            return;
        }
        else if (node->semanticType == "IfStmt") {
            handleIfStatement(node);
        }
        else if (node->semanticType == "FunctionCall") {
            Value* callValue = visitParseTreeExpr(node);
        }
        else {
            for (auto* child : node->semanticChildren) {
                visitParseTreeNode(child);
            }
        }
    } catch (...) {
        cout << "[IRGEN] Exception in visitParseTreeNode" << endl;
    }
}

Value* IRGenerator::visitParseTreeExpr(ParseTreeNode* node) {
    if (!node) return nullptr;

    if (node->semanticType == "Number") {
        if (node->varType == "float") {
            float val = stof(node->value);
            return ConstantFloat::get(val, module_);
        } 
        else if (node->value.find('.') != string::npos || 
                 node->value.find('e') != string::npos || 
                 node->value.find('E') != string::npos) {
            float val = stof(node->value);
            return ConstantFloat::get(val, module_);
        } 
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

            if (op == "+") {
                return isFloat ? static_cast<Value*>(builder_->create_fadd(left, right))
                            : static_cast<Value*>(builder_->create_iadd(left, right));
            }
            if (op == "-") {
                return isFloat ? static_cast<Value*>(builder_->create_fsub(left, right))
                            : static_cast<Value*>(builder_->create_isub(left, right));
            }
            if (op == "*") {
                return isFloat ? static_cast<Value*>(builder_->create_fmul(left, right))
                            : static_cast<Value*>(builder_->create_imul(left, right));
            }
            if (op == "/") {
                return isFloat ? static_cast<Value*>(builder_->create_fdiv(left, right))
                            : static_cast<Value*>(builder_->create_isdiv(left, right));
            }
            if (op == ">") {
                return isFloat ? static_cast<Value*>(builder_->create_fcmp_ogt(left, right))
                            : static_cast<Value*>(builder_->create_icmp_gt(left, right));
            }
            if (op == "<") {
                return isFloat ? static_cast<Value*>(builder_->create_fcmp_olt(left, right))
                            : static_cast<Value*>(builder_->create_icmp_lt(left, right));
            }
            if (op == ">=") {
                return isFloat ? static_cast<Value*>(builder_->create_fcmp_oge(left, right))
                            : static_cast<Value*>(builder_->create_icmp_ge(left, right));
            }
            if (op == "<=") {
                return isFloat ? static_cast<Value*>(builder_->create_fcmp_ole(left, right))
                            : static_cast<Value*>(builder_->create_icmp_le(left, right));
            }
            if (op == "==") {
                return isFloat ? static_cast<Value*>(builder_->create_fcmp_oeq(left, right))
                            : static_cast<Value*>(builder_->create_icmp_eq(left, right));
            }
            if (op == "!=") {
                return isFloat ? static_cast<Value*>(builder_->create_fcmp_one(left, right))
                            : static_cast<Value*>(builder_->create_icmp_ne(left, right));
            }
        }
        return nullptr;
    }
    if (node->semanticType == "UnaryExp" && node->operatorType == "-") {
        Value* operand = visitParseTreeExpr(node->semanticChildren[0]);
        if (!operand) return nullptr;
        bool isFloat = operand->get_type()->is_float_type();
        Value* zero = isFloat ? static_cast<Value*>(ConstantFloat::get(0.0f, module_))
                              : static_cast<Value*>(ConstantInt::get(0, module_));
        if (isFloat) return static_cast<Value*>(builder_->create_fsub(zero, operand));
        else return static_cast<Value*>(builder_->create_isub(zero, operand));
    }
    if (node->semanticType == "FunctionCall") {
        string funcName = node->varName;
        Function* func = nullptr;
        for (auto* f : module_->get_functions())
            if (f->get_name() == funcName) { func = f; break; }
        if (!func) {
            cout << "[IRGEN] Function not found: " << funcName << endl;
            return nullptr;
        }
        
        auto funcType = func->get_function_type();
        unsigned numParams = funcType->get_num_of_args();
        
        vector<Value*> args;
        for (size_t i = 0; i < node->semanticChildren.size() && i < numParams; i++) {
            Value* v = visitParseTreeExpr(node->semanticChildren[i]);
            if (v) {
                Type* paramType = funcType->get_param_type(i);
                Type* argType = v->get_type();
                
                if (argType->is_int32_type() && paramType->is_float_type()) {
                    Value* converted = builder_->create_sitofp(v, paramType);
                    args.push_back(converted);
                } else {
                    args.push_back(v);
                }
            }
        }
        return builder_->create_call(func, args);
    }
    
    for (auto* child : node->semanticChildren) {
        Value* v = visitParseTreeExpr(child);
        if (v) return v;
    }
    return nullptr;
}

void IRGenerator::handleIfStatement(ParseTreeNode* node) {
    cout << "[IF-STMT] Processing if statement" << endl;
    
    if (!builder_ || node->semanticChildren.size() < 2) {
        cout << "[IF-ERROR] Invalid if statement structure" << endl;
        return;
    }

    // 1. 获取条件表达式
    ParseTreeNode* condNode = node->semanticChildren[0];
    Value* condition = visitParseTreeExpr(condNode);
    
    if (!condition) {
        cout << "[IF-ERROR] Failed to evaluate condition" << endl;
        return;
    }

    // 确保条件是 i1 类型
    Value* bool_condition = condition;
    if (condition->get_type()->is_integer_type() && 
        static_cast<IntegerType*>(condition->get_type())->get_num_bits() != 1) {
        Value* zero = ConstantInt::get(0, module_);
        bool_condition = builder_->create_icmp_ne(condition, zero);
    }

    // 2. 创建唯一的基本块名称
    static int if_counter = 0;
    int current_id = ++if_counter;
    
    // 创建基本块
    BasicBlock* then_block = BasicBlock::create(module_, 
        "if_then_" + to_string(current_id), current_function_);
    
    BasicBlock* else_block = nullptr;
    BasicBlock* merge_block = BasicBlock::create(module_, 
        "if_end_" + to_string(current_id), current_function_);
    
    bool has_else = (node->semanticChildren.size() >= 3);
    
    if (has_else) {
        else_block = BasicBlock::create(module_, 
            "if_else_" + to_string(current_id), current_function_);
    } else {
        else_block = merge_block;
    }

    // 3. 创建条件分支
    builder_->create_cond_br(bool_condition, then_block, else_block);

    // 4. 处理 then 分支
    set_current_block(then_block);
    if (node->semanticChildren.size() >= 2) {
        ParseTreeNode* thenNode = node->semanticChildren[1];
        for (auto* stmt : thenNode->semanticChildren) {
            if (stmt) {
                visitParseTreeNode(stmt);
                // 如果遇到终结指令，停止处理后续语句
                if (current_block_ && current_block_->get_terminator()) {
                    break;
                }
            }
        }
    }
    
    // ✅ 关键修复：检查 current_block_ 而不是 then_block
    // 因为嵌套 if 处理后，current_block_ 可能已经变成内层的 merge 块
    if (current_block_ && !current_block_->get_terminator()) {
        builder_->create_br(merge_block);
        cout << "[IF-DEBUG] Added br from " << current_block_->get_name() 
             << " to " << merge_block->get_name() << endl;
    }

    // 5. 处理 else 分支（如果存在）
    if (has_else && else_block != merge_block) {
        set_current_block(else_block);
        ParseTreeNode* elseNode = node->semanticChildren[2];
        
        // 检查else分支是否又是一个IfStmt（else if）
        if (elseNode->semanticChildren.size() == 1 && 
            elseNode->semanticChildren[0]->semanticType == "IfStmt") {
            // 这是else if情况，递归处理嵌套的if语句
            cout << "[IF-DEBUG] Processing else-if (nested IfStmt)" << endl;
            visitParseTreeNode(elseNode->semanticChildren[0]);
        } else {
            // 普通的else分支，正常处理
            for (auto* stmt : elseNode->semanticChildren) {
                if (stmt) {
                    visitParseTreeNode(stmt);
                    // 如果遇到终结指令，停止处理后续语句
                    if (current_block_ && current_block_->get_terminator()) {
                        break;
                    }
                }
            }
        }
        
        // ✅ 关键修复：检查 current_block_ 而不是 else_block
        if (current_block_ && !current_block_->get_terminator()) {
            builder_->create_br(merge_block);
            cout << "[IF-DEBUG] Added br from " << current_block_->get_name() 
                 << " to " << merge_block->get_name() << endl;
        }
    }

    // 6. ✅ 关键修改:根据merge块的前驱情况决定插入点
    auto& merge_preds = merge_block->get_pre_basic_blocks();
    if (!merge_preds.empty()) {
        // merge 块有前驱,说明至少有一个分支会汇合到这里
        // 外层代码应该从 merge 块继续插入
        set_current_block(merge_block);
        cout << "[IF-DEBUG] Set insertion point to merge block with " 
             << merge_preds.size() << " predecessor(s)" << endl;
    } else {
        // 两个分支都有终结指令(return/无条件跳转到外层),merge块用不到
        cout << "[IF-DEBUG] Removing unused merge block (no predecessors)" << endl;
        merge_block->erase_from_parent();
        // 注意:此时当前块的插入点可能指向一个已终结的块,
        // 后续代码如果是死代码,应该不生成或者放到一个哨兵块中
    }
    
    cout << "[IF-END] If statement processed successfully" << endl;
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

        if (type_name == "int") {
            return module_->get_int32_type();
        } else if (type_name == "float") {
            return module_->get_float_type();
        } else if (type_name == "void") {
            return module_->get_void_type();
        }

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
    FunctionType* getint_type = FunctionType::get(module_->get_int32_type(), {});
    Function::create(getint_type, "getint", module_);

    FunctionType* getch_type = FunctionType::get(module_->get_int32_type(), {});
    Function::create(getch_type, "getch", module_);

    vector<Type*> getarray_params = {module_->get_int32_ptr_type()};
    FunctionType* getarray_type = FunctionType::get(module_->get_int32_type(), getarray_params);
    Function::create(getarray_type, "getarray", module_);

    vector<Type*> putint_params = {module_->get_int32_type()};
    FunctionType* putint_type = FunctionType::get(module_->get_void_type(), putint_params);
    Function::create(putint_type, "putint", module_);

    vector<Type*> putch_params = {module_->get_int32_type()};
    FunctionType* putch_type = FunctionType::get(module_->get_void_type(), putch_params);
    Function::create(putch_type, "putch", module_);

    vector<Type*> putarray_params = {module_->get_int32_type(), module_->get_int32_ptr_type()};
    FunctionType* putarray_type = FunctionType::get(module_->get_void_type(), putarray_params);
    Function::create(putarray_type, "putarray", module_);

    FunctionType* starttime_type = FunctionType::get(module_->get_void_type(), {});
    Function::create(starttime_type, "starttime", module_);

    FunctionType* stoptime_type = FunctionType::get(module_->get_void_type(), {});
    Function::create(stoptime_type, "stoptime", module_);
}