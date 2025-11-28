#ifndef PARSER_CLASS_H
#define PARSER_CLASS_H

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include "parse.h"

class SLR1Parser 
{
public:
    SLR1Parser(std::ostream& output, const std::string& filename = "");
    
    ~SLR1Parser();
    
    bool parse();
    
    ParseTreeNode* getParseTree() const;
    
    void outputParseTree();

private:
    std::stack<int> statusStack;
    std::stack<std::string> symbolStack;
    std::stack<ParseTreeNode*> treeStack;
    ParseTreeNode* parseTree;
    std::ostream& output;
    int stepCount;
    std::string inputFilename;
    bool isInGlobalScope;
    
    ParseTreeNode* findChild(const std::vector<ParseTreeNode*>& children, const std::string& symbol);
    std::string extractExpressionValue(ParseTreeNode* node);
    void collectSemanticChildren(ParseTreeNode* node, std::vector<ParseTreeNode*>& result);
    void setLocalScope(ParseTreeNode* node);
    void fillSemanticAttributes(ParseTreeNode* node, int originalIndex, const std::vector<ParseTreeNode*>& children);
    std::string getCurrentToken(const Token& token);
    std::string getActionString(const Action& action);
};

#endif
