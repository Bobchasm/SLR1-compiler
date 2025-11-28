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
#include "lexer.h"
#include "ast.h"
#include "ir_generator.h"
#include "parser_class.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    if (argc != 2) 
    {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }
    
    // 1.读文件
    FILE *fp = fopen(argv[1], "r");
    if (!fp) 
    {
        cerr << "Cannot open file: " << argv[1] << endl;
        return 1;
    }
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    char* input = (char*)malloc(size + 1);
    fread(input, 1, size, fp);
    input[size] = '\0';
    fclose(fp);
    
    // 2.提取文件名
    string filepath = argv[1];
    size_t pos = filepath.find_last_of("/\\");
    string filename = (pos != string::npos) ? filepath.substr(pos + 1) : filepath;
    
    string pureFilename = filename;
    pos = pureFilename.find_last_of('.');
    if (pos != string::npos)
        pureFilename = pureFilename.substr(0, pos);
    
    // 3.初始化词法分析器
    setTestCase("case/", filename.c_str());
    initLexer(input);
    
    // 4.创建 Parser 并解析
    SLR1Parser parser(cout, pureFilename);
    bool success = parser.parse();
    
    if (!success) 
    {
        cerr << "Parse failed!" << endl;
        cleanupLexer();
        free(input);
        return 1;
    }
    
    // 5.获取语法树
    ParseTreeNode* parseTree = parser.getParseTree();
    
    if (!parseTree) {
        cerr << "Failed to get parse tree!" << endl;
        cleanupLexer();
        free(input);
        return 1;
    }
    
    // 6.中间代码部分
    cout << "\n=== 开始生成中间代码 ===" << endl;
    
    // 直接使用ParseTreeNode生成IR
    IRGenerator generator;
    string source_file = "../input/" + filename;
    string ir_code = generator.generateFromParseTree(parseTree, source_file);
    
    // 输出IR代码
    cout << "\n生成的LLVM IR:\n" << endl;
    cout << ir_code << endl;
    
    // 保存到文件
    string output_file = "case/" + pureFilename + "_output.ll";
    ofstream out(output_file);
    if (out.is_open()) {
        out << ir_code;
        out.close();
        cout << "\nIR代码已保存到: " << output_file << endl;
    }




    // 7.清理
    cleanupLexer();
    free(input);
    
    cout << "\nDone!" << endl;
    return 0;
}
