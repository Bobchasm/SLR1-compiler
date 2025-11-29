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

    cout << "[STEP 1/4] Starting lexical and syntax analysis..." << endl;

    // 获取语法分析只需要下面，然后还需要引入 parser.h就行
    ParseTreeNode* parseTree = getParseTree(argv[1]);

    cout << "[STEP 2/4] Parse tree obtained, semanticType: " << (parseTree ? parseTree->semanticType : "null") << endl;

    if (!parseTree) {
        cerr << "Failed to get parse tree!" << endl;
        return 1;
    }

    // Debug: print detailed tree structure
    cout << "Detailed parse tree:" << endl;
    printTreeDetailed(parseTree, 0);

    // 6.中间代码部分
    cout << "[STEP 3/4] Starting IR generation..." << endl;

    // 直接使用ParseTreeNode生成IR
    IRGenerator generator;
    cout << "[STEP 3.1] IRGenerator created" << endl;
    string ir_code = generator.generateFromParseTree(parseTree, argv[1]);
    cout << "[STEP 4/4] IR generation completed" << endl;

    // 输出IR代码到stderr（不受cout重定向影响）
    cerr << ir_code << endl;
    return 0;
}
