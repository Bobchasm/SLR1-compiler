#include <iostream>
#include <string>
#include <fstream>
#include "include/parse.h"
#include "include/ir_generator.h"

using namespace std;

int main() {
    cout << "=== 手动测试 IR 生成 ===" << endl;
    
    // 创建一个简单的程序树:
    // int main() {
    //     return 0;
    // }
    
    // 1. 创建 Program 节点
    ParseTreeNode* program = new ParseTreeNode(NODE_NONTERMINAL, "Program");
    program->semanticType = "Program";
    
    // 2. 创建 FunctionDef 节点
    ParseTreeNode* funcDef = new ParseTreeNode(NODE_NONTERMINAL, "funcDef");
    funcDef->semanticType = "FunctionDef";
    funcDef->varType = "int";
    funcDef->varName = "main";
    
    // 3. 创建 ReturnStmt 节点
    ParseTreeNode* returnStmt = new ParseTreeNode(NODE_NONTERMINAL, "stmt");
    returnStmt->semanticType = "ReturnStmt";
    
    // 4. 创建 Number 节点
    ParseTreeNode* number = new ParseTreeNode(NODE_TERMINAL, "IntConst", "0");
    number->semanticType = "Number";
    number->value = "0";
    
    // 建立树结构
    program->children.push_back(funcDef);
    funcDef->children.push_back(returnStmt);
    returnStmt->children.push_back(number);
    
    // 测试 IR 生成
    IRGenerator generator;
    string ir_code = generator.generateFromParseTree(program, "test_manual.sy");
    
    cout << "\n生成的 LLVM IR:\n" << ir_code << endl;
    
    // 保存到文件
    string output_file = "case/manual_test_output.ll";
    ofstream out(output_file);
    if (out.is_open()) {
        out << ir_code;
        out.close();
        cout << "\nIR 代码已保存到: " << output_file << endl;
    }
    
    // 清理内存
    delete program;
    
    return 0;
}