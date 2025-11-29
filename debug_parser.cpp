#include <iostream>
#include <string>
#include <fstream>
#include "include/parse.h"

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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <source_file>" << endl;
        return 1;
    }

    cout << "Parsing file: " << argv[1] << endl;

    ParseTreeNode* parseTree = getParseTree(argv[1]);

    if (!parseTree) {
        cerr << "Failed to get parse tree!" << endl;
        return 1;
    }

    cout << "Parse tree obtained successfully" << endl;
    cout << "Detailed tree structure:" << endl;
    printTreeDetailed(parseTree);

    // Clean up
    delete parseTree;

    return 0;
}
