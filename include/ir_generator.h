#ifndef IR_GENERATOR_H
#define IR_GENERATOR_H

#include "ast.h"
#include "symbol_table.h"
#include "Module.h"
#include "Function.h"
#include "BasicBlock.h"
#include "Instruction.h"
#include "Constant.h"

// 前向声明ParseTreeNode，避免包含parse.h导致全局变量重复定义
struct ParseTreeNode;

class IRGenerator {
private:
    Module* module_;
    Function* current_function_;
    BasicBlock* current_block_;
    SymbolTable* symbol_table_;
    int temp_counter_;

public:
    IRGenerator();
    ~IRGenerator();
    
    // 生成IR的主入口
    std::string generate(ASTNode* ast, const std::string& source_file = "");
    
    // 新增：直接从ParseTreeNode生成IR
    std::string generateFromParseTree(ParseTreeNode* parseTree, const std::string& source_file = "");
    
    // AST访问方法
    void visitProgram(ProgramNode* node);
    void visitFunctionDef(FunctionDefNode* node);
    void visitVarDecl(VarDeclNode* node);
    void visitAssignment(AssignmentNode* node);
    Value* visitBinaryExpr(BinaryExprNode* node);
    void visitIfStmt(IfStmtNode* node);
    void visitWhileStmt(WhileStmtNode* node);
    void visitReturnStmt(ReturnStmtNode* node);
    Value* visitVariableRef(VariableRefNode* node);
    Value* visitNumber(NumberNode* node);
    Value* visitFunctionCall(FunctionCallNode* node);
    Value* visitUnaryExpr(UnaryExprNode* node);

private:
    std::string new_temp();
    void set_current_block(BasicBlock* bb);
    Type* convert_type(const std::string& type_name);
    void declare_builtin_functions();
    Value* visitNode(ASTNode* node);
    Value* generate_logical_and(BinaryExprNode* node);
    Value* generate_logical_or(BinaryExprNode* node);
    
    // ParseTreeNode访问方法
    void visitParseTreeNode(ParseTreeNode* node);
    Value* visitParseTreeExpr(ParseTreeNode* node);
};

#endif