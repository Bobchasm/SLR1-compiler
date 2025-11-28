#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>

// AST节点基类
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(int indent, std::ofstream& out) const = 0;
};

// 具体AST节点类型
class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> declarations;
    void print(int indent, std::ofstream& out) const override;
};

class FunctionDefNode : public ASTNode {
public:
    std::string return_type;
    std::string name;
    std::vector<std::pair<std::string, std::string>> params; // (type, name)
    std::vector<std::unique_ptr<ASTNode>> body;
    void print(int indent, std::ofstream& out) const override;
};

class VarDeclNode : public ASTNode {
public:
    std::string type;
    std::string name;
    std::unique_ptr<ASTNode> init_value;
    bool is_global = false;
    void print(int indent, std::ofstream& out) const override;
};

class AssignmentNode : public ASTNode {
public:
    std::string var_name;
    std::unique_ptr<ASTNode> value;
    void print(int indent, std::ofstream& out) const override;
};

class BinaryExprNode : public ASTNode {
public:
    std::string op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    void print(int indent, std::ofstream& out) const override;
};

class IfStmtNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> then_branch;
    std::unique_ptr<ASTNode> else_branch;
    void print(int indent, std::ofstream& out) const override;
};

class WhileStmtNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> body;
    void print(int indent, std::ofstream& out) const override;
};

class ReturnStmtNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> value;
    void print(int indent, std::ofstream& out) const override;
};

class VariableRefNode : public ASTNode {
public:
    std::string name;
    VariableRefNode() = default;
    VariableRefNode(const std::string& n) : name(n) {}
    void print(int indent, std::ofstream& out) const override;
};

class NumberNode : public ASTNode {
public:
    int value;
    NumberNode() : value(0) {}
    NumberNode(int v) : value(v) {}
    void print(int indent, std::ofstream& out) const override;
};

class FunctionCallNode : public ASTNode {
public:
    std::string func_name;
    std::vector<std::unique_ptr<ASTNode>> arguments;
    void print(int indent, std::ofstream& out) const override;
};

// 单目表达式节点
class UnaryExprNode : public ASTNode {
public:
    std::string op;  // 运算符: +, -, !
    std::unique_ptr<ASTNode> operand;  // 操作数
    void print(int indent, std::ofstream& out) const override;
};

#endif