/**
 * 
 *   make compiler
 * 
 *   ./build/simple case/test.sy
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include "parse.h"
#include "ast.h"
#include "ir_generator.h"
#include "symbol_table.h"

// #include "ir_generator.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }
    
    // 获取语法分析只需要下面，然后还需要引入 parser.h就行
    ParseTreeNode* parseTree = getParseTree(argv[1]);
    
    if (!parseTree) {
        cerr << "Failed to get parse tree!" << endl;
        return 1;
    }
    
    // 6.中间代码部分
    cout << "\n=== 开始生成中间代码 ===" << endl;
    
    // 直接使用ParseTreeNode生成IR
    IRGenerator generator;
    string ir_code = generator.generateFromParseTree(parseTree, argv[1]);
    
    // 输出IR代码
    cout << "\n生成的LLVM IR:\n" << endl;
    cout << ir_code << endl;
    
    // 保存到文件
    string output_file = "case/output.ll";
    ofstream out(output_file);
    if (out.is_open()) {
        out << ir_code;
        out.close();
        cout << "\nIR代码已保存到: " << output_file << endl;
    }
    
    cout << "\nDone!" << endl;
    return 0;
}
