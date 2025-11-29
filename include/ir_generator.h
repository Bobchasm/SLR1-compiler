#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "../ir_lib/include/Module.h"
#include "../ir_lib/include/Function.h"
#include "../ir_lib/include/BasicBlock.h"
#include "../ir_lib/include/IRbuilder.h"
#include "../ir_lib/include/IRprinter.h"
#include "../ir_lib/include/Constant.h"
#include "symbol_table.h"

// 前向声明ParseTreeNode，避免包含parse.h导致全局变量重复定义
struct ParseTreeNode;

class IRGenerator {
private:
    Module* module_;
    Function* current_function_;
    BasicBlock* current_block_;
    IRBuilder* builder_;
    SymbolTable* symbol_table_;
    int temp_counter_;

    // RAII深度管理类
    class DepthGuard {
    private:
        int& depth_;
    public:
        DepthGuard(int& depth) : depth_(depth) { depth_++; }
        ~DepthGuard() { depth_--; }
    };

public:
    IRGenerator();
    ~IRGenerator();

    // 直接从ParseTreeNode生成IR
    std::string generateFromParseTree(ParseTreeNode* parseTree, const std::string& source_file = "");

private:
    std::string new_temp();
    void set_current_block(BasicBlock* bb);
    Type* convert_type(const std::string& type_name);
    void declare_builtin_functions();

    // ParseTreeNode访问方法
    void visitParseTreeNode(ParseTreeNode* node);
    Value* visitParseTreeExpr(ParseTreeNode* node);

    // 辅助方法
    void handleAssignment(ParseTreeNode* node);
    void handleIfStatement(ParseTreeNode* node);
    Value* handleLVal(ParseTreeNode* node);
    void collectFunctionParams(ParseTreeNode* node, std::vector<Type*>& param_types, std::vector<std::string>& param_names);
    void collectFuncFParamList(ParseTreeNode* node, std::vector<Type*>& param_types, std::vector<std::string>& param_names);
    Value* handleBinaryExpr(ParseTreeNode* node);
    Value* handleRelExpr(ParseTreeNode* node);
    Value* handleEqExpr(ParseTreeNode* node);
    Value* handleLAndExpr(ParseTreeNode* node);
    Value* handleLOrExpr(ParseTreeNode* node);
};

#endif