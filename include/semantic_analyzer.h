#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "parse.h"

using namespace std;

enum SymbolKind 
{
    SYM_VARIABLE,    // 变量
    SYM_CONSTANT,    // 常量
    SYM_FUNCTION,    // 函数
    SYM_PARAMETER    // 函数参数
};

struct Symbol 
{
    string name;
    SymbolKind kind;
    string type;        // "int", "float", "void"
    bool isConst;            // 是否为常量
    bool isGlobal;           // 是否为全局作用域
    int lineNumber;          // 定义位置
    
    // 函数专用
    vector<string> paramTypes;  // 参数类型列表
    string returnType;               // 返回类型
    
    Symbol() : kind(SYM_VARIABLE), isConst(false), isGlobal(false), lineNumber(0) {}
};

// 语义分析符号表
class SemanticSymbolTable 
{
private:
    unordered_map<string, Symbol> table_;
    SemanticSymbolTable* parent_;  // 父作用域
    
public:
    SemanticSymbolTable(SemanticSymbolTable* parent = nullptr) : parent_(parent) {}
    bool add(const Symbol& symbol) 
    {
        // 检查当前作用域是否已定义（不检查父作用域）
        if (table_.find(symbol.name) != table_.end())
            return false;  // 重复定义

        table_[symbol.name] = symbol;
        return true;
    }
    
    // 查找符号（向上查找父作用域）
    Symbol* lookup(const string& name) 
    {
        auto it = table_.find(name);
        if (it != table_.end())
            return &it->second;
        if (parent_)
            return parent_->lookup(name);
        return nullptr;
    }
    
    // 仅在当前作用域查找
    Symbol* lookupLocal(const string& name) 
    {
        auto it = table_.find(name);
        if (it != table_.end())
            return &it->second;

        return nullptr;
    }
    
    // 获取父作用域
    SemanticSymbolTable* getParent() const 
    {
        return parent_;
    }
};

class SemanticAnalyzer 
{
private:
    SemanticSymbolTable* currentScope_;  // 当前作用域
    vector<string> errors_;    // 错误列表
    bool hasErrors_;
    
    void enterScope();
    void exitScope();
    void reportError(const string& message, int line = 0);
    
    void checkProgram(ParseTreeNode* node);
    void checkFunctionDef(ParseTreeNode* node);
    void checkVarDecl(ParseTreeNode* node, const string& currentFunctionReturnType = "");
    void checkAssignment(ParseTreeNode* node, const string& currentFunctionReturnType = "");
    void checkFunctionCall(ParseTreeNode* node);
    void checkReturnStmt(ParseTreeNode* node, const string& expectedReturnType);
    void checkIfStmt(ParseTreeNode* node, const string& currentFunctionReturnType);
    bool containsReturn(ParseTreeNode* node);
    
    string getExprType(ParseTreeNode* node);
    bool isTypeCompatible(const string& type1, const string& type2);
    bool checkIntOverflow(const string& value);
    
    void analyze(ParseTreeNode* node, const string& currentFunctionReturnType = "");
    
public:
    SemanticAnalyzer() : currentScope_(nullptr), hasErrors_(false) 
    {
        currentScope_ = new SemanticSymbolTable();  // 全局作用域
    }
    
    ~SemanticAnalyzer() 
    {
        while (currentScope_) {
            SemanticSymbolTable* parent = currentScope_->getParent();
            delete currentScope_;
            currentScope_ = parent;
        }
    }
    
    // 执行语义分析
    bool check(ParseTreeNode* root);
    
    // 获取错误信息
    const vector<string>& getErrors() const 
    { 
        return errors_; 
    }
    bool hasError() const 
    { 
        return hasErrors_; 
    }
    
    // 打印错误
    void printErrors() const 
    {
        if (errors_.empty())
            cout << "[SEMANTIC] No semantic errors found." << endl;
        else 
        {
            cout << "[SEMANTIC] Found " << errors_.size() << " semantic error(s):" << endl;
            for (const auto& err : errors_)
                cerr << err << endl;
        }
    }
};

#endif
