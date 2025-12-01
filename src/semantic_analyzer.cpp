#include "semantic_analyzer.h"
#include <sstream>
#include <climits>
#include <cstdlib>

// 进入新作用域
void SemanticAnalyzer::enterScope() 
{
    currentScope_ = new SemanticSymbolTable(currentScope_);
}

// 退出当前作用域
void SemanticAnalyzer::exitScope() 
{
    if (currentScope_) 
    {
        SemanticSymbolTable* parent = currentScope_->getParent();
        delete currentScope_;
        currentScope_ = parent;
    }
}

// 报告错误
void SemanticAnalyzer::reportError(const std::string& message, int line) 
{
    std::ostringstream oss;
    if (line > 0)
        oss << "[Line " << line << "] ";

    oss << "Semantic Error: " << message;
    errors_.push_back(oss.str());
    hasErrors_ = true;
}

// 执行语义分析
bool SemanticAnalyzer::check(ParseTreeNode* root) 
{
    if (!root) 
    {
        reportError("Parse tree root is null");
        return false;
    }
    
    std::cout << "[SEMANTIC] Starting semantic analysis..." << std::endl;
    errors_.clear();
    hasErrors_ = false;
    
    // 分析整个程序
    analyze(root);
    
    return !hasErrors_;
}

void SemanticAnalyzer::analyze(ParseTreeNode* node, const std::string& currentFunctionReturnType) 
{
    if (!node) return;
    
    std::cout << "[SEMANTIC-CHECK] Analyzing node: " << node->semanticType 
              << " (varName: " << node->varName << ")" << std::endl;
    
    if (node->semanticType == "Program") 
        checkProgram(node);

    else if (node->semanticType == "FunctionDef") 
        checkFunctionDef(node);

    else if (node->semanticType == "VarDecl") 
        checkVarDecl(node, currentFunctionReturnType);

    else if (node->semanticType == "Assignment") 
        checkAssignment(node, currentFunctionReturnType);

    else if (node->semanticType == "FunctionCall") 
        checkFunctionCall(node);

    else if (node->semanticType == "ReturnStmt")
        checkReturnStmt(node, currentFunctionReturnType);

    else if (node->semanticType == "IfStmt")
        checkIfStmt(node, currentFunctionReturnType);

    else 
    {
        for (auto* child : node->semanticChildren)
            analyze(child, currentFunctionReturnType);
    }
}

// 检查程序
void SemanticAnalyzer::checkProgram(ParseTreeNode* node) 
{
    std::cout << "[SEMANTIC-CHECK] Checking Program with " 
              << node->semanticChildren.size() << " children" << std::endl;
    
    // 第一遍：收集所有函数定义（用于前向引用）
    for (auto* child : node->semanticChildren) 
    {
        if (child && child->semanticType == "FunctionDef") 
        {
            Symbol funcSymbol;
            funcSymbol.name = child->varName;
            funcSymbol.kind = SYM_FUNCTION;
            funcSymbol.type = child->varType;
            funcSymbol.returnType = child->varType;
            funcSymbol.isGlobal = true;
            
            // 收集参数类型
            for (auto* param : child->semanticChildren) 
            {
                if (param && param->semanticType == "FuncParam")
                    funcSymbol.paramTypes.push_back(param->varType);
            }
            
            if (!currentScope_->add(funcSymbol))
                reportError("Function '" + funcSymbol.name + "' is already defined", child->lineNumber);

            std::cout << "[SEMANTIC-CHECK] Registered function: " << funcSymbol.name 
                      << " with " << funcSymbol.paramTypes.size() << " parameters" << std::endl;
        }
    }
    
    // 第二遍：检查所有节点
    for (auto* child : node->semanticChildren)
        analyze(child);
}

// 检查函数定义
void SemanticAnalyzer::checkFunctionDef(ParseTreeNode* node) 
{
    std::cout << "[SEMANTIC-CHECK] Checking FunctionDef: " << node->varName << std::endl;
    
    // 进入函数作用域
    enterScope();
    
    // 添加参数到符号表
    for (auto* child : node->semanticChildren) 
    {
        if (child && child->semanticType == "FuncParam") 
        {
            Symbol paramSymbol;
            paramSymbol.name = child->varName;
            paramSymbol.kind = SYM_PARAMETER;
            paramSymbol.type = child->varType;
            paramSymbol.isConst = false;
            
            if (!currentScope_->add(paramSymbol))
                reportError("Parameter '" + paramSymbol.name + "' is already defined", child->lineNumber);
        }
    }
    
    // 检查函数体
    std::string returnType = node->varType;
    for (auto* child : node->semanticChildren) 
    {
        if (child && child->semanticType != "FuncParam")
            analyze(child, returnType);
    }
    
    // 非void函数必须包含至少一个ReturnStmt
    if (returnType != "void")
    {
        bool hasReturn = false;
        for (auto* child : node->semanticChildren)
        {
            if (child && containsReturn(child)) {
                hasReturn = true;
                break;
            }
        }
        if (!hasReturn)
            reportError("Non-void function '" + node->varName + "' missing return statement", node->lineNumber);
    }
    
    // 退出函数作用域
    exitScope();
}

bool SemanticAnalyzer::containsReturn(ParseTreeNode* node) 
{
    if (!node) return false;
    if (node->semanticType == "ReturnStmt") return true;
    for (auto* child : node->semanticChildren) 
    {
        if (containsReturn(child)) 
            return true;
    }
    return false;
}

// 检查变量声明
void SemanticAnalyzer::checkVarDecl(ParseTreeNode* node, const std::string& currentFunctionReturnType) 
{
    std::cout << "[SEMANTIC-CHECK] Checking VarDecl: " << node->varName 
              << " (isConst: " << node->isConst << ")" << std::endl;
    
    Symbol varSymbol;
    varSymbol.name = node->varName;
    varSymbol.kind = node->isConst ? SYM_CONSTANT : SYM_VARIABLE;
    varSymbol.type = node->varType;
    varSymbol.isConst = node->isConst;
    varSymbol.isGlobal = node->isGlobal;
    varSymbol.lineNumber = node->lineNumber;
    
    // 常量必须初始化
    if (node->isConst && node->semanticChildren.empty() && node->initValue.empty())
        reportError("Constant '" + node->varName + "' must be initialized", node->lineNumber);
    
    // 整数溢出
    if (varSymbol.type == "int" && !node->initValue.empty()) 
    {
        if (!checkIntOverflow(node->initValue))
            reportError("Integer overflow in initialization of '" + node->varName + "'", node->lineNumber);
    }
    
    // 重复定义
    if (!currentScope_->add(varSymbol))
        reportError("Variable '" + varSymbol.name + "' is already defined in this scope", node->lineNumber);
    
    // 初始化值的类型兼容性
    if (!node->semanticChildren.empty()) {
        std::string initType = getExprType(node->semanticChildren[0]);
        if (!isTypeCompatible(node->varType, initType)) {
            reportError("Type mismatch in initialization of '" + node->varName 
                       + "': expected " + node->varType + ", got " + initType, node->lineNumber);
        }
    }
    
    // 递归检查初始化表达式中的语义（如FunctionCall）
    for (auto* child : node->semanticChildren)
        analyze(child, currentFunctionReturnType);
}

// 检查赋值语句
void SemanticAnalyzer::checkAssignment(ParseTreeNode* node, const std::string& currentFunctionReturnType) 
{
    std::cout << "[SEMANTIC-CHECK] Checking Assignment to: " << node->varName << std::endl;
    
    // 变量是否已定义
    Symbol* symbol = currentScope_->lookup(node->varName);
    if (!symbol) 
    {
        reportError("Variable '" + node->varName + "' is not defined", node->lineNumber);
        return;
    }
    
    // 不能修改常量
    if (symbol->isConst)
        reportError("Cannot assign to constant '" + node->varName + "'", node->lineNumber);
    
    // 类型兼容性
    if (!node->semanticChildren.empty()) 
    {
        std::string exprType = getExprType(node->semanticChildren[0]);
        if (!isTypeCompatible(symbol->type, exprType)) 
    {
            reportError("Type mismatch in assignment to '" + node->varName 
                       + "': expected " + symbol->type + ", got " + exprType, node->lineNumber);
        }
        
        // 递归检查赋值表达式中的语义（如FunctionCall）
        for (auto* child : node->semanticChildren)
            analyze(child, currentFunctionReturnType);
    }
}

// 检查函数调用
void SemanticAnalyzer::checkFunctionCall(ParseTreeNode* node) 
{
    std::string funcName = node->varName;
    std::cout << "[SEMANTIC-CHECK] Checking FunctionCall: " << funcName << std::endl;
    
    // 函数是否已定义
    Symbol* funcSymbol = currentScope_->lookup(funcName);
    if (!funcSymbol || funcSymbol->kind != SYM_FUNCTION) 
    {
        reportError("Function '" + funcName + "' is not defined", node->lineNumber);
        return;
    }
    
    // 参数数量
    size_t expectedParams = funcSymbol->paramTypes.size();
    size_t actualParams = node->semanticChildren.size();
    if (expectedParams != actualParams) 
    {
        std::ostringstream oss;
        oss << "Function '" << funcName << "' expects " << expectedParams 
            << " argument(s), but " << actualParams << " provided";
        reportError(oss.str(), node->lineNumber);
        return;
    }
    
    // 参数类型
    for (size_t i = 0; i < actualParams; i++) 
    {
        std::string actualType = getExprType(node->semanticChildren[i]);
        std::string expectedType = funcSymbol->paramTypes[i];
        
        std::cout << "[SEMANTIC-CHECK] Function '" << funcName << "' param " << (i+1) 
                  << ": expected=" << expectedType << ", actual=" << actualType << std::endl;
        
        if (!isTypeCompatible(expectedType, actualType)) 
        {
            std::ostringstream oss;
            oss << "Function '" << funcName << "' parameter " << (i + 1) 
                << " type mismatch: expected " << expectedType << ", got " << actualType;
            reportError(oss.str(), node->lineNumber);
        }
    }
}

// 检查返回语句
void SemanticAnalyzer::checkReturnStmt(ParseTreeNode* node, const std::string& expectedReturnType) 
{
    std::cout << "[SEMANTIC-CHECK] Checking ReturnStmt (expected: " 
              << expectedReturnType << ")" << std::endl;
    
    if (expectedReturnType.empty()) 
    {
        reportError("Return statement outside function", node->lineNumber);
        return;
    }
    
    // 检查返回值类型
    if (expectedReturnType == "void") 
    {
        if (!node->semanticChildren.empty())
            reportError("Void function should not return a value", node->lineNumber);
    } 
    else 
    {
        if (node->semanticChildren.empty())
            reportError("Non-void function must return a value", node->lineNumber);
        else 
        {
            std::string actualType = getExprType(node->semanticChildren[0]);
            if (!isTypeCompatible(expectedReturnType, actualType)) 
            {
                reportError("Return type mismatch: expected " + expectedReturnType 
                           + ", got " + actualType, node->lineNumber);
            }
        }
    }
    
    // 递归检查返回表达式中的语义（如FunctionCall）
    for (auto* child : node->semanticChildren)
        analyze(child, expectedReturnType);
}

// 检查if语句
void SemanticAnalyzer::checkIfStmt(ParseTreeNode* node, const std::string& currentFunctionReturnType) 
{
    std::cout << "[SEMANTIC-CHECK] Checking IfStmt" << std::endl;
    
    // 递归检查if语句的子节点，传递函数返回类型
    for (auto* child : node->semanticChildren)
        analyze(child, currentFunctionReturnType);
}

// 获取表达式类型
std::string SemanticAnalyzer::getExprType(ParseTreeNode* node) 
{
    if (!node) return "unknown";
    
    if (node->semanticType == "Number") 
    {
        if (node->value.find('.') != std::string::npos)
            return "float";
        return "int";
    }
    else if (node->semanticType == "Variable" || node->semanticType == "LVal") 
    {
        Symbol* symbol = currentScope_->lookup(node->varName);
        if (symbol)
            return symbol->type;
        return "unknown";
    }
    else if (node->semanticType == "BinaryExpr" || node->semanticType == "UnaryExp") 
    {
        if (!node->semanticChildren.empty())
            return getExprType(node->semanticChildren[0]);
    }
    else if (node->semanticType == "FunctionCall") 
    {
        Symbol* funcSymbol = currentScope_->lookup(node->varName);
        if (funcSymbol && funcSymbol->kind == SYM_FUNCTION)
            return funcSymbol->returnType;
    }
    
    return "int";
}

// 类型兼容性检查
bool SemanticAnalyzer::isTypeCompatible(const std::string& type1, const std::string& type2) 
{
    if (type1 == type2) return true;
    
    if ((type1 == "int" && type2 == "float") || (type1 == "float" && type2 == "int"))
        return false;  // 不允许隐式类型转换
    
    return false;
}

// 检查整数溢出
bool SemanticAnalyzer::checkIntOverflow(const std::string& value) 
{
    if (value.empty()) return true;
    
    try {
        std::string absValue = value;
        bool isNegative = false;
        if (absValue[0] == '-') 
        {
            isNegative = true;
            absValue = absValue.substr(1);
        } 
        else if (absValue[0] == '+')
            absValue = absValue.substr(1);
        
        long long num = std::stoll(absValue);
        if (isNegative)
            num = -num;
        
        if (num < INT_MIN || num > INT_MAX)
            return false;
        
        return true;
    } catch (const std::exception& e) {
        return false;  // 转换失败，说明溢出
    }
}
