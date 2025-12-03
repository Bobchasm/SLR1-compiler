#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>
#include <fstream>

using namespace std;

// AST节点基类
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print(int indent, ofstream& out) const = 0;
};

// 具体AST节点类型
class ProgramNode : public ASTNode {
public:
    vector<unique_ptr<ASTNode>> declarations;
    void print(int indent, ofstream& out) const override;
};

class FunctionDefNode : public ASTNode {
public:
    string return_type;
    string name;
    vector<pair<string, string>> params; // (type, name)
    vector<unique_ptr<ASTNode>> body;
    void print(int indent, ofstream& out) const override;
};

class VarDeclNode : public ASTNode {
public:
    string type;
    string name;
    unique_ptr<ASTNode> init_value;
    bool is_global = false;
    void print(int indent, ofstream& out) const override;
};

class AssignmentNode : public ASTNode {
public:
    string var_name;
    unique_ptr<ASTNode> value;
    void print(int indent, ofstream& out) const override;
};

class BinaryExprNode : public ASTNode {
public:
    string op;
    unique_ptr<ASTNode> left;
    unique_ptr<ASTNode> right;
    void print(int indent, ofstream& out) const override;
};

class IfStmtNode : public ASTNode {
public:
    unique_ptr<ASTNode> condition;
    unique_ptr<ASTNode> then_branch;
    unique_ptr<ASTNode> else_branch;
    void print(int indent, ofstream& out) const override;
};

class WhileStmtNode : public ASTNode {
public:
    unique_ptr<ASTNode> condition;
    unique_ptr<ASTNode> body;
    void print(int indent, ofstream& out) const override;
};

class ReturnStmtNode : public ASTNode {
public:
    unique_ptr<ASTNode> value;
    void print(int indent, ofstream& out) const override;
};

class VariableRefNode : public ASTNode {
public:
    string name;
    VariableRefNode() = default;
    VariableRefNode(const string& n) : name(n) {}
    void print(int indent, ofstream& out) const override;
};

class NumberNode : public ASTNode {
public:
    int value;
    NumberNode() : value(0) {}
    NumberNode(int v) : value(v) {}
    void print(int indent, ofstream& out) const override;
};

class FunctionCallNode : public ASTNode {
public:
    string func_name;
    vector<unique_ptr<ASTNode>> arguments;
    void print(int indent, ofstream& out) const override;
};

// 单目表达式节点
class UnaryExprNode : public ASTNode {
public:
    string op;  // 运算符: +, -, !
    unique_ptr<ASTNode> operand;  // 操作数
    void print(int indent, ofstream& out) const override;
};

#endif