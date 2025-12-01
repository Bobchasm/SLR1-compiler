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

    // 鍐呭瓨妫€鏌ワ細妫€鏌arseTree鐨勫叧閿睘鎬?
    // cout << "[IRGEN] ParseTree memory check:" << endl;
    // cout << "[IRGEN]   semanticChildren size: " << parseTree->semanticChildren.size() << endl;
    // cout << "[IRGEN]   varName: '" << parseTree->varName << "'" << endl;
    // cout << "[IRGEN]   varType: '" << parseTree->varType << "'" << endl;

    // 鍒涘缓妯″潡
    // cout << "[IRGEN] Creating module..." << endl;
    module_ = new Module("sysy2022_compiler");
    if (!module_) {
        cout << "[IRGEN] ERROR: Failed to create module!" << endl;
        return "";
    }
    // cout << "[IRGEN] Module created successfully" << endl;

    // 妫€鏌ユā鍧楃被鍨嬬郴缁?
    // cout << "[IRGEN] Module type system initialization check:" << endl;
    // cout << "[IRGEN]   int32_type: " << (module_->get_int32_type() ? "OK" : "NULL") << endl;
    // cout << "[IRGEN]   void_type: " << (module_->get_void_type() ? "OK" : "NULL") << endl;
    // cout << "[IRGEN]   float_type: " << (module_->get_float_type() ? "OK" : "NULL") << endl;
    // cout << "[IRGEN]   int32_ptr_type: " << (module_->get_int32_ptr_type() ? "OK" : "NULL") << endl;

    // 璁剧疆婧愭枃浠跺悕
    if (!source_file.empty()) {
        module_->set_source_file_name(source_file);
    }

    // 澹版槑鍐呯疆鍑芥暟
    // cout << "[IRGEN] Declaring builtin functions..." << endl;
    declare_builtin_functions();
    // cout << "[IRGEN] Builtin functions declared" << endl;

    // 閬嶅巻ParseTreeNode鐢熸垚IR
    // cout << "[IRGEN] Starting visitParseTreeNode..." << endl;
    visitParseTreeNode(parseTree);
    // cout << "[IRGEN] visitParseTreeNode completed" << endl;

    // 鐢熸垚IR鏂囨湰
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

    // 杈圭晫妫€鏌?
    if (node->semanticChildren.empty()) {
        cout << indent << "[IRGEN] Warning: Node has no semantic children, semanticType: " << node->semanticType << endl;
    }

    DepthGuard guard(depth);  // RAII绠＄悊娣卞害璁℃暟

    if (node->semanticType.empty()) {
        // 璺宠繃娌℃湁璇箟淇℃伅鐨勮妭鐐?
        cout << indent << "[IRGEN] Node has empty semanticType, processing " << node->semanticChildren.size() << " semantic children" << endl;
        for (auto* child : node->semanticChildren) {
            visitParseTreeNode(child);
        }
        return;
    }

    // 澶勭悊涓嶅悓绫诲瀷鐨勮妭鐐?
    if (node->semanticType == "Program") {
        // 澶勭悊Program鑺傜偣锛氬厛澶勭悊鍏ㄥ眬鍙橀噺锛屽啀澶勭悊鍑芥暟
        // cout << indent << "[IRGEN] === Processing Program ===" << endl;

        // 妫€鏌ユā鍧楃姸鎬?
        if (!module_) {
            // cout << indent << "[IRGEN] ERROR: Module is null!" << endl;
            return;
        }

        // cout << indent << "[IRGEN] Module check:" << endl;
        // cout << indent << "[IRGEN]   int32_type: " << (module_->get_int32_type() ? "OK" : "NULL") << endl;
        // cout << indent << "[IRGEN]   void_type: " << (module_->get_void_type() ? "OK" : "NULL") << endl;
        // cout << indent << "[IRGEN]   float_type: " << (module_->get_float_type() ? "OK" : "NULL") << endl;

        // cout << indent << "[IRGEN] Children count: " << node->semanticChildren.size() << endl;

    // 鍏抽敭璋冭瘯鐐?锛氭鏌emanticChildren鍚戦噺鏈韩
    // cout << indent << "[IRGEN] DEBUG: Checking semanticChildren vector..." << endl;
    if (node->semanticChildren.empty()) {
        cout << indent << "[IRGEN] semanticChildren is empty" << endl;
    } else {
        cout << indent << "[IRGEN] semanticChildren is not empty, size: " << node->semanticChildren.size() << endl;
    }

    // 璇︾粏杈撳嚭姣忎釜瀛愯妭鐐逛俊鎭?- 娣诲姞閫愪釜瀛愯妭鐐圭殑澶勭悊
    for (size_t i = 0; i < node->semanticChildren.size(); i++) {
        // cout << indent << "[IRGEN] DEBUG: Starting to process child " << i << endl;

        auto* child = node->semanticChildren[i];
        if (child) {
            // cout << indent << "[IRGEN] Child " << i << " is not null" << endl;

            // // 鍏抽敭璋冭瘯鐐?锛氶€愪釜杈撳嚭灞炴€э紝閬垮厤涓€娆℃€ц緭鍑哄彲鑳界殑闂
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

            // 妫€鏌ュ叧閿睘鎬?
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

    // // 鍏抽敭璋冭瘯鐐?锛氭鏌ョ涓€涓惊鐜紑濮?
    // cout << indent << "[IRGEN] DEBUG: Starting first loop (global variables)" << endl;

    // // 绗竴涓惊鐜細澶勭悊鍏ㄥ眬鍙橀噺
    // cout << indent << "[IRGEN] First loop: Processing global variables" << endl;
    
    // 銆愪慨鏀圭偣1锛氫慨澶嶅叏灞€鍙橀噺閲嶅鐢熸垚闂銆?
    // 浣跨敤闆嗗悎璁板綍宸插鐞嗙殑鍏ㄥ眬鍙橀噺锛岄伩鍏嶉噸澶?
    std::set<std::string> processed_globals;
    int globalVarCount = 0;
    for (auto* child : node->semanticChildren) {
        if (child) {
            cout << indent << "[IRGEN] Checking child for VarDecl: " << child->semanticType
                      << ", isGlobal=" << child->isGlobal << endl;
            if (child->semanticType == "VarDecl" && child->isGlobal) {
                // 妫€鏌ユ槸鍚﹀凡缁忓鐞嗚繃杩欎釜鍏ㄥ眬鍙橀噺
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

    // 鍏抽敭璋冭瘯鐐?锛氭鏌ョ浜屼釜寰幆寮€濮?
    // cout << indent << "[IRGEN] DEBUG: Starting second loop (FunctionDef)" << endl;

    // 绗簩涓惊鐜細澶勭悊鍑芥暟瀹氫箟
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
                // 鍙互閫夋嫨閫掑綊澶勭悊绌鸿妭鐐圭殑瀛愯妭鐐?
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
        
        // 銆愪慨鏀圭偣2锛氬湪VarDecl澶勭悊涓坊鍔犳ā鍧楃骇鍒叏灞€鍙橀噺妫€鏌ャ€?
        if (node->isGlobal) {
            // 妫€鏌ユā鍧椾腑鏄惁宸插瓨鍦ㄥ悓鍚嶅叏灞€鍙橀噺
            auto global_vars = module_->get_global_variable();
            for (auto* gv : global_vars) {
                if (gv->get_name() == node->varName) {
                    cout << indent << "[IRGEN] 鉂?ERROR: Global variable '" << node->varName 
                              << "' already exists in module! This should not happen!" << endl;
                    return;  // 鐩存帴杩斿洖锛屼笉閲嶅澶勭悊
                }
            }
            
            // 妫€鏌ョ鍙疯〃涓槸鍚﹀凡瀛樺湪鍚屽悕鍏ㄥ眬鍙橀噺锛堝弻閲嶄繚闄╋級
            Value* existing_var = symbol_table_->get(node->varName);
            if (existing_var) {
                cout << indent << "[IRGEN] Warning: Global variable '" << node->varName 
                          << "' already exists in symbol table, skipping duplicate" << endl;
                return;  // 鐩存帴杩斿洖锛屼笉閲嶅澶勭悊
            }
        }
        
        // 澶勭悊鍙橀噺澹版槑
        Type* var_type = convert_type(node->varType);

        if (node->isGlobal) {
            cout << indent << "[IRGEN] 馃殌 Creating global variable: " << node->varName << endl;
            // 鍏ㄥ眬鍙橀噺
            Constant* init_value = nullptr;
            if (!node->initValue.empty()) {
                int val = stoi(node->initValue);
                init_value = ConstantInt::get(val, module_);
            } else {
                init_value = ConstantZero::get(var_type, module_);
            }

            GlobalVariable* global_var = GlobalVariable::create(
                node->varName, module_, var_type, false, init_value
            );
            symbol_table_->put(node->varName, global_var);
            cout << indent << "[IRGEN] 鉁?Created global variable: " << node->varName << endl;
        } else {
            // cout << indent << "[IRGEN] Creating local variable" << endl;
            // 灞€閮ㄥ彉閲?- 闇€瑕佸湪鍑芥暟涓婁笅鏂囦腑鍒涘缓
            if (current_function_ && current_block_) {
                AllocaInst* alloca = builder_->create_alloca(var_type);
                alloca->set_name(node->varName);
                symbol_table_->put(node->varName, alloca);

                // 鍒濆鍖?- 浼樺厛浣跨敤semanticChildren琛ㄨ揪寮忥紝fallback鎵嶇敤initValue瀛楃涓?
                if (!node->semanticChildren.empty()) {
                    // 琛ㄨ揪寮忓垵濮嬪寲锛堝寘鎷父閲忔姌鍙犲悗鐨勮礋鏁帮級
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
                    // Fallback锛氱畝鍗曠殑甯搁噺鍒濆鍖栵紙褰搒emanticChildren涓虹┖鏃讹級
                    // cout << indent << "[IRGEN] Using initValue string for initialization" << endl;
                    int val = stoi(node->initValue);
                    ConstantInt* init_val = ConstantInt::get(val, module_);
                    builder_->create_store(init_val, alloca);
                }
            }
        }
    }
    else if (node->semanticType == "FunctionDef") {
        // cout << indent << "[IRGEN] Processing FunctionDef: " << node->varName << endl;

        // 妫€鏌ュ叧閿睘鎬?
        if (node->varName.empty()) {
            cout << indent << "[IRGEN] ERROR: FunctionDef has empty varName!" << endl;
            return;
        }
        if (node->varType.empty()) {
            cout << indent << "[IRGEN] ERROR: FunctionDef has empty varType!" << endl;
            return;
        }

        // cout << indent << "[IRGEN] FunctionDef has " << node->semanticChildren.size() << " semantic children" << endl;

        // 澶勭悊鍑芥暟瀹氫箟
        // cout << indent << "[IRGEN] Converting return type: " << node->varType << endl;
        Type* return_type = convert_type(node->varType);
        if (!return_type) {
            cout << indent << "[IRGEN] ERROR: Failed to convert return type!" << endl;
            return;
        }
        // cout << indent << "[IRGEN] Return type converted successfully" << endl;

        // 澶勭悊鍑芥暟鍙傛暟
        vector<Type*> param_types;
        vector<string> param_names;

        // 鏍规嵁瀵规帴鏂囨。锛孎unctionDef鐨剆emanticChildren鍖呭惈FuncParam鍜屽嚱鏁颁綋
        // cout << indent << "[IRGEN] Processing function parameters..." << endl;
        for (auto* child : node->semanticChildren) {
            if (child) {
                // cout << indent << "[IRGEN]   Child type: '" << child->semanticType << "', varName: '" << child->varName << "', varType: '" << child->varType << "'" << endl;
                if (child->semanticType == "FuncParam") {
                    // cout << indent << "[IRGEN] Found FuncParam: " << child->varName << " type: " << child->varType << endl;
                    // 鏀堕泦鍙傛暟绫诲瀷鍜屽悕绉?
                    param_types.push_back(convert_type(child->varType));
                    param_names.push_back(child->varName);
                }
            }
        }
        // cout << indent << "[IRGEN] Collected " << param_types.size() << " parameters" << endl;

        // cout << indent << "[IRGEN] Creating FunctionType with " << param_types.size() << " params..." << endl;
        FunctionType* func_type = FunctionType::get(return_type, param_types);
        // cout << indent << "[IRGEN] FunctionType created successfully" << endl;
        // cout << indent << "[IRGEN] Creating Function: " << node->varName << endl;
        Function* function = Function::create(func_type, node->varName, module_);
        // cout << indent << "[IRGEN] Function created successfully: " << node->varName << endl;

        current_function_ = function;
        // cout << indent << "[IRGEN] Set current_function_ to: " << node->varName << endl;
        // cout << indent << "[IRGEN] Creating new symbol table scope for function: " << node->varName << endl;
        SymbolTable* parent_table = symbol_table_; // 淇濆瓨鐖朵綔鐢ㄥ煙
        symbol_table_ = new SymbolTable(symbol_table_); // 鏂扮殑浣滅敤鍩?

        // 銆愪慨鏀圭偣3锛氫慨澶嶅熀鏈潡鍛藉悕闂 - 浣跨敤澶у啓entry銆?
        // 鍒涘缓鍏ュ彛鍩烘湰鍧?
        BasicBlock* entry_block = BasicBlock::create(module_, node->varName + "_ENTRY", function, false);
        set_current_block(entry_block);

        // 鍒涘缓IRBuilder
        builder_ = new IRBuilder(current_block_, module_);
        builder_->set_curFunc(function);

        // 澶勭悊鍑芥暟鍙傛暟 - 涓烘瘡涓弬鏁板垱寤篴lloca骞跺瓨鍌?
        auto& args = function->get_args();
        size_t i = 0;
        for (auto* arg : args) {
            if (i < param_names.size()) {
                arg->set_name(param_names[i]);

                // 鍒涘缓灞€閮ㄥ彉閲忓瓨鍌ㄥ弬鏁?
                AllocaInst* alloca = builder_->create_alloca(param_types[i]);
                alloca->set_name(param_names[i]);
                builder_->create_store(arg, alloca);

                // 灏嗗弬鏁版坊鍔犲埌绗﹀彿琛?
                symbol_table_->put(param_names[i], alloca);
                ++i;
            }
        }

        // 澶勭悊鍑芥暟浣?- 鏍规嵁瀵规帴鏂囨。锛宻emanticChildren鍖呭惈FuncParam鍜屽嚱鏁颁綋璇彞
        // cout << indent << "[IRGEN] Processing function body, children count: " << node->semanticChildren.size() << endl;
        int body_stmt_count = 0;
        for (auto* child : node->semanticChildren) {
            if (child && child->semanticType != "FuncParam") {
                // 璺宠繃鍙傛暟锛屽鐞嗗嚱鏁颁綋璇彞
                body_stmt_count++;
                // cout << indent << "[IRGEN] Processing function body child #" << body_stmt_count << ": " << child->semanticType << " (varName: " << child->varName << ")" << endl;
                visitParseTreeNode(child);
                // cout << indent << "[IRGEN] Finished processing function body child #" << body_stmt_count << ": " << child->semanticType << endl;
            }
        }
        // cout << indent << "[IRGEN] Function body processing completed, processed " << body_stmt_count << " statements" << endl;

        // 纭繚鍑芥暟鏈夌粓缁撴寚浠?
        if (current_block_ && !current_block_->get_terminator()) {
            if (return_type->is_void_type()) {
                builder_->create_void_ret();
            } else {
                ConstantInt* zero = ConstantInt::get(0, module_);
                builder_->create_ret(zero);
            }
        }

        // cout << indent << "[IRGEN] Restoring parent symbol table for function: " << node->varName << endl;
        delete symbol_table_;
        symbol_table_ = parent_table; // 鎭㈠鐖朵綔鐢ㄥ煙
        delete builder_;
        builder_ = nullptr;
        current_function_ = nullptr;
        current_block_ = nullptr;
        // cout << indent << "[IRGEN] FunctionDef processing completed for: " << node->varName << endl;
    }
    else if (node->semanticType == "Assignment") {
        // cout << indent << "[IRGEN] Processing Assignment for var '" << node->varName << "'" << endl;
        // 澶勭悊璧嬪€艰鍙?
        Value* left_var = symbol_table_->get(node->varName);
        if (!left_var) {
            cout << indent << "[IRGEN] Error: Undefined variable '" << node->varName << "'" << endl;
            return;
        }

        // 鑾峰彇鍙冲€?- 浣跨敤semanticChildren
        Value* right_value = nullptr;
        if (!node->semanticChildren.empty()) {
            // cout << indent << "[IRGEN] Using semanticChildren[0]" << endl;
            right_value = visitParseTreeExpr(node->semanticChildren[0]);
        }

        if (right_value && builder_) {
            // cout << indent << "[IRGEN] Creating store instruction" << endl;
            builder_->create_store(right_value, left_var);
        } else {
            cout << indent << "[IRGEN] Warning: Could not get right value for assignment" << endl;
        }
    }
    else if (node->semanticType == "ReturnStmt") {
        // cout << indent << "[IRGEN] Processing ReturnStmt" << endl;
        // 澶勭悊return璇彞 - 鐩存帴浣跨敤semanticChildren
        if (node->semanticChildren.empty()) {
            // cout << indent << "[IRGEN] Void return" << endl;
            builder_->create_void_ret();
        } else {
            // 娣诲姞瀹夊叏妫€鏌?
            if (!node->semanticChildren[0]) {
                cout << indent << "[IRGEN] Error: Return expression child is null" << endl;
                builder_->create_void_ret();
                return;
            }

            // cout << indent << "[IRGEN] Using semanticChildren[0] for return, type: "
                    //   << node->semanticChildren[0]->semanticType << endl;
            Value* ret_val = visitParseTreeExpr(node->semanticChildren[0]);
            if (ret_val) {
                // cout << indent << "[IRGEN] Creating return with value" << endl;
                builder_->create_ret(ret_val);
            } else {
                // cout << indent << "[IRGEN] Void return (no value found)" << endl;
                builder_->create_void_ret();
            }
        }
    }
    else if (node->semanticType == "IfStmt") {
        // cout << indent << "[IRGEN] Processing IfStmt" << endl;
        handleIfStatement(node);
    }
    else {
        // cout << indent << "[IRGEN] Unknown semanticType: '" << node->semanticType << "', processing semantic children" << endl;
        // 鍏朵粬鑺傜偣锛岄€掑綊澶勭悊瀛愯妭鐐?
        for (auto* child : node->semanticChildren) {
            visitParseTreeNode(child);
        }
    }

        // cout << indent << "[IRGEN] visitParseTreeNode completed for: " << node->semanticType << endl;
    } catch (const exception& e) {
        cout << "[IRGEN] Exception in visitParseTreeNode: " << e.what() << endl;
        cout << "[IRGEN] Node semanticType: " << (node ? node->semanticType : "null") << endl;
        cout << "[IRGEN] Current depth: " << depth << endl;
    } catch (...) {
        cout << "[IRGEN] Unknown exception in visitParseTreeNode" << endl;
        cout << "[IRGEN] Node semanticType: " << (node ? node->semanticType : "null") << endl;
        cout << "[IRGEN] Current depth: " << depth << endl;
    }
}

Value* IRGenerator::visitParseTreeExpr(ParseTreeNode* node) {
    if (!node) {
        std::cout << "[IRGEN] visitParseTreeExpr: node is null" << std::endl;
        return nullptr;
    }

    // std::cout << "[IRGEN] visitParseTreeExpr: semanticType='" << node->semanticType
    //           << "', value='" << node->value << "', varName='" << node->varName
    //           << "', operatorType='" << node->operatorType << "'" << std::endl;
    // std::cout << "[IRGEN]   has " << node->semanticChildren.size() << " children" << std::endl;

    if (node->semanticType == "Number") {
        // 鏁板瓧甯搁噺
        int val = std::stoi(node->value);
        return ConstantInt::get(val, module_);
    }
    else if (node->semanticType == "Variable") {
        // 鍙橀噺寮曠敤
        Value* var = symbol_table_->get(node->varName);
        if (!var) {
            std::cout << "Error: Undefined variable '" << node->varName << "'" << std::endl;
            return nullptr;
        }
        // 鍔犺浇鍙橀噺鍊?
        return builder_->create_load(var);
    }
    else if (node->semanticType == "BinaryExpr") {
        // 浜屽厓琛ㄨ揪寮?
        if (node->semanticChildren.size() >= 2) {
            Value* left = visitParseTreeExpr(node->semanticChildren[0]);
            Value* right = visitParseTreeExpr(node->semanticChildren[1]);
            if (!left || !right) return nullptr;

            std::string op = node->operatorType;
            if (op == "+") {
                return builder_->create_iadd(left, right);
            } else if (op == "-") {
                return builder_->create_isub(left, right);
            } else if (op == "*") {
                return builder_->create_imul(left, right);
            } else if (op == "/") {
                return builder_->create_isdiv(left, right);
            } else if (op == "%") {
                return builder_->create_irem(left, right);
            } else if (op == ">") {
                return builder_->create_icmp_gt(left, right);
            } else if (op == "<") {
                return builder_->create_icmp_lt(left, right);
            } else if (op == ">=") {
                return builder_->create_icmp_ge(left, right);
            } else if (op == "<=") {
                return builder_->create_icmp_le(left, right);
            } else if (op == "==") {
                return builder_->create_icmp_eq(left, right);
            } else if (op == "!=") {
                return builder_->create_icmp_ne(left, right);
            }
        }
    }
    else if (node->semanticType == "UnaryExpr") {
        // 涓€鍏冭〃杈惧紡
        if (!node->semanticChildren.empty()) {
            Value* operand = visitParseTreeExpr(node->semanticChildren[0]);
            if (!operand) return nullptr;

            std::string op = node->operatorType;
            if (op == "-") {
                // 璐熷彿锛? - operand
                Value* zero = ConstantInt::get(0, module_);
                return builder_->create_isub(zero, operand);
            } else if (op == "!") {
                // 閫昏緫闈烇細杞崲涓烘瘮杈?operand == 0
                Value* zero = ConstantInt::get(0, module_);
                return builder_->create_icmp_eq(operand, zero);
            } else if (op == "+") {
                // 姝ｅ彿锛氱洿鎺ヨ繑鍥炴搷浣滄暟
                return operand;
            }
        }
    }
    else if (node->semanticType == "FunctionCall") {
        // 鍑芥暟璋冪敤
        std::string funcName = node->varName;
        std::vector<Value*> args;

        // 璁＄畻鎵€鏈夊弬鏁?
        for (auto* arg : node->semanticChildren) {
            Value* argValue = visitParseTreeExpr(arg);
            if (argValue) {
                args.push_back(argValue);
            }
        }

        // 鑾峰彇鍑芥暟
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

        // 鐢熸垚call鎸囦护
        return builder_->create_call(func, args);
    }
    else if (node->semanticType == "Condition") {
        // 鏉′欢琛ㄨ揪寮?
        return visitParseTreeExpr(node->semanticChildren.empty() ? nullptr : node->semanticChildren[0]);
    }
    else {
        // 閫掑綊澶勭悊瀛愯〃杈惧紡
        for (auto* child : node->semanticChildren) {
            Value* result = visitParseTreeExpr(child);
            if (result) return result;
        }
    }

    return nullptr;
}

void IRGenerator::handleIfStatement(ParseTreeNode* node) {
    // IfStmt缁撴瀯: semanticChildren[0]=Condition, [1]=ThenBranch, [2]=ElseBranch(鍙€?
    if (!builder_ || node->semanticChildren.size() < 2) return;

    // 鑾峰彇Condition鑺傜偣鐨勫瓙鑺傜偣浣滀负鏉′欢琛ㄨ揪寮?
    ParseTreeNode* conditionNode = node->semanticChildren[0];
    Value* condition = visitParseTreeExpr(conditionNode->semanticChildren.empty() ?
                                          nullptr : conditionNode->semanticChildren[0]);
    if (!condition) return;

    // 銆愪慨鏀圭偣4锛氫慨澶嶆潯浠跺垽鏂被鍨嬮棶棰樸€?
    // 灏嗘潯浠跺€艰浆鎹负i1绫诲瀷锛堝鏋滃綋鍓嶄笉鏄痠1绫诲瀷锛?
    Value* bool_condition = condition;
    if (condition->get_type()->is_int32_type()) {
        // 灏唅32杞崲涓篿1锛歝ondition != 0
        Value* zero = ConstantInt::get(0, module_);
        bool_condition = builder_->create_icmp_ne(condition, zero);
    }

    // 鍒涘缓鍩烘湰鍧?- 浣跨敤鏇存竻鏅扮殑鍛藉悕
    BasicBlock* then_block = BasicBlock::create(module_, "if_then", current_function_);
    BasicBlock* else_block = nullptr;
    BasicBlock* merge_block = BasicBlock::create(module_, "if_end", current_function_);

    // 妫€鏌ユ槸鍚︽湁else鍒嗘敮
    bool has_else = (node->semanticChildren.size() >= 3);

    if (has_else) {
        else_block = BasicBlock::create(module_, "if_else", current_function_);
    } else {
        else_block = merge_block;
    }

    // 鍒涘缓鏉′欢鍒嗘敮 - 浣跨敤杞崲鍚庣殑bool鏉′欢
    builder_->create_cond_br(bool_condition, then_block, else_block);

    // 澶勭悊then鍒嗘敮
    set_current_block(then_block);
    ParseTreeNode* thenBranch = node->semanticChildren[1];
    for (auto* stmt : thenBranch->semanticChildren) {
        visitParseTreeNode(stmt);
    }

    if (!then_block->get_terminator()) {
        builder_->create_br(merge_block);
    }

    // 澶勭悊else鍒嗘敮
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

    // 璁剧疆鎻掑叆鐐瑰埌merge鍧?
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

    // putint(i32): void
    vector<Type*> putint_params = {module_->get_int32_type()};
    FunctionType* putint_type = FunctionType::get(module_->get_void_type(), putint_params);
    Function::create(putint_type, "putint", module_);

    // getch(): i32
    FunctionType* getch_type = FunctionType::get(module_->get_int32_type(), {});
    Function::create(getch_type, "getch", module_);

    // putch(i32): void
    vector<Type*> putch_params = {module_->get_int32_type()};
    FunctionType* putch_type = FunctionType::get(module_->get_void_type(), putch_params);
    Function::create(putch_type, "putch", module_);

    // getarray(i32*): i32
    vector<Type*> getarray_params = {module_->get_int32_ptr_type()};
    FunctionType* getarray_type = FunctionType::get(module_->get_int32_type(), getarray_params);
    Function::create(getarray_type, "getarray", module_);

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