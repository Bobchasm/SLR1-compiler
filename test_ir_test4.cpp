#include <iostream>
#include <string>
#include <fstream>
#include "include/parse.h"
#include "include/ir_generator.h"

using namespace std;

int main() {
    cout << "=== Test4 SysY IR Generation Test ===" << endl;

    try {
        // 读取test4.sy文件
        string filename = "case/test4.sy";
        ifstream input_file(filename);
        if (!input_file.is_open()) {
            cerr << "Error: Cannot open file " << filename << endl;
            return 1;
        }

        string source_code((istreambuf_iterator<char>(input_file)),
                          istreambuf_iterator<char>());
        input_file.close();

        cout << "Source code from " << filename << ":" << endl;
        cout << source_code << endl;
        cout << "========================================" << endl;

        // 使用现有的getParseTree函数进行完整的词法和语法分析
        cout << "\n=== Starting Lexical and Syntax Analysis ===" << endl;
        ParseTreeNode* parseTree = getParseTree(filename);

        if (!parseTree) {
            cerr << "Error: Parse failed!" << endl;
            return 1;
        }

        cout << "Parse tree created successfully" << endl;
        cout << "ParseTree semanticType: " << parseTree->semanticType << endl;
        cout << "ParseTree semanticChildren count: " << parseTree->semanticChildren.size() << endl;

        // 打印前几个子节点的信息
        for (size_t i = 0; i < parseTree->semanticChildren.size() && i < 5; i++) {
            auto* child = parseTree->semanticChildren[i];
            if (child) {
                cout << "Child " << i << ": semanticType='" << child->semanticType
                     << "', varName='" << child->varName << "', varType='" << child->varType << "'" << endl;
            }
        }

        // IR生成测试
        cout << "\n=== Starting IR Generation ===" << endl;
        IRGenerator generator;
        string ir_code = generator.generateFromParseTree(parseTree, filename);

        cout << "\n=== IR Generation Completed ===" << endl;
        cout << "Generated IR length: " << ir_code.length() << " characters" << endl;

        // 保存IR代码到文件
        string output_file = "test_output/test4_ir_output.ll";
        ofstream out(output_file);
        if (out.is_open()) {
            out << ir_code;
            out.close();
            cout << "IR code saved to: " << output_file << endl;
        } else {
            cerr << "Error: Cannot open output file " << output_file << endl;
        }

        // 显示部分IR代码（前500个字符）
        cout << "\nFirst 500 characters of generated IR:" << endl;
        cout << string(ir_code.begin(), ir_code.begin() + min(500, (int)ir_code.length())) << endl;
        if (ir_code.length() > 500) {
            cout << "... (truncated, total " << ir_code.length() << " characters)" << endl;
        }

        cout << "\n=== Test Completed Successfully! ===" << endl;
        return 0;

    } catch (const exception& e) {
        cerr << "Exception caught: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown exception caught!" << endl;
        return 1;
    }
}
