#include <iostream>
#include <string>
#include <fstream>
#include "include/parse.h"
#include "include/ir_generator.h"

using namespace std;

int main() {
    cout << "=== 复杂语法树 IR 生成测试 ===" << endl;
    
    // 创建一个复杂的程序树:
    // int main() {
    //     int a = 1;
    //     int b = 2;
    //     int c = a + b * 3;
    //     if (c > 5) {
    //         return 1;
    //     } else {
    //         return 0;
    //     }
    // }
    
    // 1. 创建 Program 节点
    ParseTreeNode* program = new ParseTreeNode(NODE_NONTERMINAL, "Program");
    program->semanticType = "Program";
    
    // 2. 创建 FunctionDef 节点 (int main())
    ParseTreeNode* funcDef = new ParseTreeNode(NODE_NONTERMINAL, "FunctionDef");
    funcDef->semanticType = "FunctionDef";
    funcDef->varType = "int";
    funcDef->varName = "main";
    
    // 3. 创建函数体内的变量声明
    // int a = 1;
    ParseTreeNode* varDeclA = new ParseTreeNode(NODE_NONTERMINAL, "VarDecl");
    varDeclA->semanticType = "VarDecl";
    varDeclA->varType = "int";
    varDeclA->varName = "a";
    varDeclA->isGlobal = false;
    varDeclA->initValue = "1";
    
    // int b = 2;
    ParseTreeNode* varDeclB = new ParseTreeNode(NODE_NONTERMINAL, "VarDecl");
    varDeclB->semanticType = "VarDecl";
    varDeclB->varType = "int";
    varDeclB->varName = "b";
    varDeclB->isGlobal = false;
    varDeclB->initValue = "2";
    
    // int c = a + b * 3;
    ParseTreeNode* varDeclC = new ParseTreeNode(NODE_NONTERMINAL, "VarDecl");
    varDeclC->semanticType = "VarDecl";
    varDeclC->varType = "int";
    varDeclC->varName = "c";
    varDeclC->isGlobal = false;
    
    // 创建复杂的初始化表达式: a + b * 3
    ParseTreeNode* addExpr = new ParseTreeNode(NODE_NONTERMINAL, "BinaryExpr");
    addExpr->semanticType = "BinaryExpr";
    addExpr->operatorType = "+";
    
    // 左边: a
    ParseTreeNode* varA = new ParseTreeNode(NODE_NONTERMINAL, "Variable");
    varA->semanticType = "Variable";
    varA->varName = "a";
    
    // 右边: b * 3
    ParseTreeNode* mulExpr = new ParseTreeNode(NODE_NONTERMINAL, "BinaryExpr");
    mulExpr->semanticType = "BinaryExpr";
    mulExpr->operatorType = "*";
    
    // b
    ParseTreeNode* varB = new ParseTreeNode(NODE_NONTERMINAL, "Variable");
    varB->semanticType = "Variable";
    varB->varName = "b";
    
    // 3
    ParseTreeNode* num3 = new ParseTreeNode(NODE_TERMINAL, "Number", "3");
    num3->semanticType = "Number";
    num3->value = "3";
    
    // 构建乘法表达式
    mulExpr->children.push_back(varB);
    mulExpr->children.push_back(num3);
    
    // 构建加法表达式
    addExpr->children.push_back(varA);
    addExpr->children.push_back(mulExpr);
    
    // 设置变量c的初始化表达式
    varDeclC->children.push_back(addExpr);
    
    // 4. 创建 if-else 语句
    ParseTreeNode* ifStmt = new ParseTreeNode(NODE_NONTERMINAL, "IfStmt");
    ifStmt->semanticType = "IfStmt";
    
    // 条件表达式: c > 5
    ParseTreeNode* condition = new ParseTreeNode(NODE_NONTERMINAL, "BinaryExpr");
    condition->semanticType = "BinaryExpr";
    condition->operatorType = ">";
    
    // c
    ParseTreeNode* varC = new ParseTreeNode(NODE_NONTERMINAL, "Variable");
    varC->semanticType = "Variable";
    varC->varName = "c";
    
    // 5
    ParseTreeNode* num5 = new ParseTreeNode(NODE_TERMINAL, "Number", "5");
    num5->semanticType = "Number";
    num5->value = "5";
    
    condition->children.push_back(varC);
    condition->children.push_back(num5);
    
    // then 分支: return 1;
    ParseTreeNode* thenReturn = new ParseTreeNode(NODE_NONTERMINAL, "ReturnStmt");
    thenReturn->semanticType = "ReturnStmt";
    
    ParseTreeNode* num1 = new ParseTreeNode(NODE_TERMINAL, "Number", "1");
    num1->semanticType = "Number";
    num1->value = "1";
    
    thenReturn->children.push_back(num1);
    
    // else 分支: return 0;
    ParseTreeNode* elseReturn = new ParseTreeNode(NODE_NONTERMINAL, "ReturnStmt");
    elseReturn->semanticType = "ReturnStmt";
    
    ParseTreeNode* num0 = new ParseTreeNode(NODE_TERMINAL, "Number", "0");
    num0->semanticType = "Number";
    num0->value = "0";
    
    elseReturn->children.push_back(num0);
    
    // 构建 if 语句
    ifStmt->children.push_back(condition);
    ifStmt->children.push_back(thenReturn);
    ifStmt->children.push_back(elseReturn);
    
    // 5. 建立完整的树结构
    program->children.push_back(funcDef);
    
    // 函数体中的语句
    funcDef->children.push_back(varDeclA);
    funcDef->children.push_back(varDeclB);
    funcDef->children.push_back(varDeclC);
    funcDef->children.push_back(ifStmt);
    
    // 打印树结构用于调试
    cout << "\n构建的语法树结构:" << endl;
    cout << program->toSemanticString() << endl;
    
    // 测试 IR 生成
    IRGenerator generator;
    string ir_code = generator.generateFromParseTree(program, "complex_test.sy");
    
    cout << "\n生成的 LLVM IR:\n" << ir_code << endl;
    
    // 保存到文件
    string output_file = "case/complex_test_output.ll";
    ofstream out(output_file);
    if (out.is_open()) {
        out << ir_code;
        out.close();
        cout << "\nIR 代码已保存到: " << output_file << endl;
    } else {
        cerr << "无法打开输出文件: " << output_file << endl;
    }
    
    // 清理内存
    delete program;
    
    cout << "\n测试完成!" << endl;
    return 0;
}