#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <ctime>
#include "parse.h"
#include "ast.h"
#include "ir_generator.h"
#include "symbol_table.h"
#include "semantic_analyzer.h"


using namespace std;

// 全局变量：保存原始的cout buffer（去除static，在parse.h中声明为extern）
streambuf* g_originalCoutBuffer = nullptr;

// 输出到终端的辅助函数（绕过日志重定向，与语法分析那里一样的，只是懒得把它变成公共的，直接搬了一遍）
void printToConsole(const string& message) 
{
    if (g_originalCoutBuffer) 
    {
        streambuf* currentBuffer = cout.rdbuf();
        cout.rdbuf(g_originalCoutBuffer);  // 临时恢复到终端
        cout << message;
        cout.rdbuf(currentBuffer);  // 恢复日志重定向
    } 
    else
        cout << message;  // 如果还没重定向，直接输出
}

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
            //printTreeDetailed(node->semanticChildren[i], 0);
        }
    }

    // 注释掉循环，以避免unused variable警告
    // for (auto* child : node->children) {
    //     printTreeDetailed(child, depth + 1);
    // }
}

int main(int argc, char* argv[])
{
    // ===================== 一些准备 =============================
    printToConsole("\n[STEP 1/4] Preparing...\n");

    if (argc != 2)
    {
        printToConsole("[PRE] Usage: " + string(argv[0]) + " <source_file>\n");
        return 1;
    }

    // 生成时间戳用于日志文件名
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeinfo);
    string logFilename = "logs/log_" + string(timestamp) + ".txt";
    
    // 保存原始cout buffer
    g_originalCoutBuffer = cout.rdbuf();
    
    // 打开日志文件
    ofstream logFile(logFilename, ios::out | ios::trunc);
    
    if (logFile.is_open()) 
    {
        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        logFile << "==============================================" << endl;
        logFile << "Compiler Log" << endl;
        logFile << "Time: " << timeStr << endl;
        logFile << "Source File: " << argv[1] << endl;
        logFile << "==============================================" << endl;
        logFile << endl;
        
        // 重定向 cout 到日志文件
        cout.rdbuf(logFile.rdbuf());

        printToConsole("[DEBUG] Logs saved to " + logFilename + "\n");
    }

    printToConsole("\n[STEP 1/4] Prepared completed!\n");


    // ===================== 获取语法树 (语法分析) =============================
    
    printToConsole("[STEP 2/4] Starting lexical and syntax analysis...\n");

    ParseTreeNode* parseTree = getParseTree(argv[1]);

    if (!parseTree) 
    {
        printToConsole("[ERROR] Parse Error\n");
        return 1;
    }
    printToConsole("[STEP 2/4] Lexical and syntax analysis passed!\n\n");


    
    // ==================== 语义检查 ====================

    printToConsole("[STEP 3/4] Starting semantic analysis...\n");
    
    SemanticAnalyzer semanticAnalyzer;
    bool semanticCheckPassed = semanticAnalyzer.check(parseTree);
    
    if (!semanticCheckPassed) 
    {
        // 语义错误信息输出到终端
        printToConsole("[ERROR] Semantic analysis failed!\n");
        
        // 输出错误列表到终端
        if (g_originalCoutBuffer) {
            streambuf* currentBuffer = cout.rdbuf();
            cout.rdbuf(g_originalCoutBuffer);
            semanticAnalyzer.printErrors();
            cout.rdbuf(currentBuffer);
        } else {
            semanticAnalyzer.printErrors();
        }
        
        printToConsole("[SEMANTIC] Compilation aborted due to semantic errors.\n\n");
        return 1;
    }
    
    printToConsole("[STEP 3/4] Semantic analysis passed!\n\n");



    // ===================== 中间代码部分 =============================

    printToConsole("[STEP 4/4] Starting IR generation...\n");

    // 直接使用ParseTreeNode生成IR
    IRGenerator generator;
    string ir_code = generator.generateFromParseTree(parseTree, argv[1]);

    
    printToConsole("[STEP 4/4] IR generation completed\n\n");


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


    // string output_file = "case/" + pureFilename + "_output.ll";
    string output_file = "output/" + pureFilename + "_output.ll";
    printToConsole("[CONPLETES] Generated " + output_file + " successfully!\n\n");
    ofstream out(output_file);
    if (out.is_open()) {
        out << ir_code;
        out.close();
        cout << "\nIR 代码已保存到: " << output_file << endl;
    } else {
        cout << "无法打开输出文件: " << output_file << endl;
    }

    return 0;
}
