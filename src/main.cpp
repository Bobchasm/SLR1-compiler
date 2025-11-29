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
#include <ctime>
#include "parse.h"
#include "ast.h"
#include "ir_generator.h"
#include "symbol_table.h"

// #include "ir_generator.h"

using namespace std;

void printTreeDetailed(ParseTreeNode* node, int depth = 0) {
    if (!node) return;

    string indent(depth * 2, ' ');

    cout << indent << "Node: " << node->symbol
         << ", semanticType: '" << node->semanticType << "'"
         << ", varName: '" << node->varName << "'"
         << ", value: '" << node->value << "'"
         << ", children: " << node->children.size()
         << ", semanticChildren: " << node->semanticChildren.size() << endl;

    if (!node->semanticChildren.empty()) {
        cout << indent << "  Semantic children:" << endl;
        for (size_t i = 0; i < node->semanticChildren.size(); i++) {
            cout << indent << "    [" << i << "] ";
            printTreeDetailed(node->semanticChildren[i], 0);
        }
    }

    for (auto* child : node->children) {
        printTreeDetailed(child, depth + 1);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }

    // 生成时间戳用于日志文件名
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeinfo);
    string logFilename = "logs/log_" + string(timestamp) + ".txt";
    
    // 保存原始的 cout 和 cerr streambuf
    std::streambuf* originalCoutBuffer = std::cout.rdbuf();
    std::streambuf* originalCerrBuffer = std::cerr.rdbuf();
    
    // 打开日志文件
    std::ofstream logFile(logFilename, ios::out | ios::trunc);
    
    if (logFile.is_open()) {
        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        logFile << "==============================================" << endl;
        logFile << "Compiler Log" << endl;
        logFile << "Time: " << timeStr << endl;
        logFile << "Source File: " << argv[1] << endl;
        logFile << "==============================================" << endl;
        logFile << endl;
        
        logFile << "==============================================" << endl;
        logFile << "Parser Log" << endl;
        logFile << "==============================================" << endl;
        logFile << endl;
        
        // 重定向 cout 到日志文件
        std::cout.rdbuf(logFile.rdbuf());
    }
    
    cout << "[STEP 1/4] Starting lexical and syntax analysis..." << endl;

    // 获取语法分析只需要下面，然后还需要引入 parser.h就行
    ParseTreeNode* parseTree = getParseTree(argv[1]);

    cout << "[STEP 2/4] Parse tree obtained, semanticType: " << (parseTree ? parseTree->semanticType : "null") << endl;

    if (!parseTree) {
        // 错误信息输出到终端
        std::cerr.rdbuf(originalCerrBuffer);
        cerr << "Failed to get parse tree!" << endl;
        return 1;
    }

    // Debug: print detailed tree structure
    // cout << "Detailed parse tree:" << endl;
    // printTreeDetailed(parseTree, 0);

    // 将 IR 生成的调试输出重定向到日志文件
    // if (logFile.is_open()) {
    //     logFile << "\n=============================================="<< endl;
    //     logFile << "IR Generation Log" << endl;
    //     logFile << "==============================================" << endl;
    //     logFile << endl;
        
    //     // 重定向 cerr 到日志文件(因为 IR 生成器使用 cerr)
    //     std::cerr.rdbuf(logFile.rdbuf());
    // }
    
    // 6.中间代码部分
    cout << "[STEP 3/4] Starting IR generation..." << endl;

    // 直接使用ParseTreeNode生成IR
    IRGenerator generator;
    cout << "[STEP 3.1] IRGenerator created" << endl;
    string ir_code = generator.generateFromParseTree(parseTree, argv[1]);
    
    // 恢复 cout 和 cerr 到终端
    // std::cout.rdbuf(originalCoutBuffer);
    // std::cerr.rdbuf(originalCerrBuffer);
    
    cout << "[STEP 4/4] IR generation completed" << endl;


    string inputFilename = argv[1];
        size_t last_slash = inputFilename.find_last_of("/\\");
        string filename_only = (last_slash != string::npos) ? inputFilename.substr(last_slash + 1) : inputFilename;
    
    string pureFilename = "";
        string baseFilename = inputFilename;
        size_t dotPos = baseFilename.find_last_of('.');
        if (dotPos != string::npos)
            baseFilename = baseFilename.substr(0, dotPos);
        
        size_t slashPos = baseFilename.find_last_of("\\/");
        pureFilename = (slashPos != string::npos) ? baseFilename.substr(slashPos + 1) : baseFilename;




    string output_file = "case/" + pureFilename + "_output.ll";
    ofstream out(output_file);
    if (out.is_open()) {
        out << ir_code;
        out.close();
        cout << "\nIR 代码已保存到: " << output_file << endl;
    } else {
        cerr << "无法打开输出文件: " << output_file << endl;
    }

    // 输出IR代码到stderr（不受cout重定向影响）
    // cerr << ir_code << endl;
    return 0;
}
