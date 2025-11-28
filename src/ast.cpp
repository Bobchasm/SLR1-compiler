#include "ast.h"
#include <iostream>

// 辅助函数：输出缩进
static void print_indent(int indent, std::ofstream& out) {
    for (int i = 0; i < indent; i++) {
        out << "  ";
    }
}

// ProgramNode
void ProgramNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "Program" << std::endl;
    for (const auto& decl : declarations) {
        if (decl) decl->print(indent + 1, out);
    }
}

// FunctionDefNode
void FunctionDefNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "FunctionDef: " << return_type << " " << name << "(";
    for (size_t i = 0; i < params.size(); i++) {
        if (i > 0) out << ", ";
        out << params[i].first << " " << params[i].second;
    }
    out << ")" << std::endl;
    for (const auto& stmt : body) {
        if (stmt) stmt->print(indent + 1, out);
    }
}

// VarDeclNode
void VarDeclNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "VarDecl: " << type << " " << name;
    if (is_global) out << " (global)";
    out << std::endl;
    if (init_value) {
        print_indent(indent + 1, out);
        out << "InitValue:" << std::endl;
        init_value->print(indent + 2, out);
    }
}

// AssignmentNode
void AssignmentNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "Assignment: " << var_name << " =" << std::endl;
    if (value) value->print(indent + 1, out);
}

// BinaryExprNode
void BinaryExprNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "BinaryExpr: " << op << std::endl;
    if (left) left->print(indent + 1, out);
    if (right) right->print(indent + 1, out);
}

// IfStmtNode
void IfStmtNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "IfStmt:" << std::endl;
    print_indent(indent + 1, out);
    out << "Condition:" << std::endl;
    if (condition) condition->print(indent + 2, out);
    print_indent(indent + 1, out);
    out << "Then:" << std::endl;
    if (then_branch) then_branch->print(indent + 2, out);
    if (else_branch) {
        print_indent(indent + 1, out);
        out << "Else:" << std::endl;
        else_branch->print(indent + 2, out);
    }
}

// WhileStmtNode
void WhileStmtNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "WhileStmt:" << std::endl;
    print_indent(indent + 1, out);
    out << "Condition:" << std::endl;
    if (condition) condition->print(indent + 2, out);
    print_indent(indent + 1, out);
    out << "Body:" << std::endl;
    if (body) body->print(indent + 2, out);
}

// ReturnStmtNode
void ReturnStmtNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "ReturnStmt:" << std::endl;
    if (value) value->print(indent + 1, out);
}

// VariableRefNode
void VariableRefNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "VariableRef: " << name << std::endl;
}

// NumberNode
void NumberNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "Number: " << value << std::endl;
}

// FunctionCallNode
void FunctionCallNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "FunctionCall: " << func_name << "()" << std::endl;
    for (const auto& arg : arguments) {
        if (arg) arg->print(indent + 1, out);
    }
}

// UnaryExprNode
void UnaryExprNode::print(int indent, std::ofstream& out) const {
    print_indent(indent, out);
    out << "UnaryExpr: " << op << std::endl;
    if (operand) operand->print(indent + 1, out);
}
