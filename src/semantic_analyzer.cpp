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
void SemanticAnalyzer::reportError(const string& message, int line) 
{
    ostringstream oss;
    if (line > 0)
        oss << "[Line " << line << "] ";

    oss << "Semantic Error: " << message;
    errors_.push_back(oss.str());
    hasErrors_ = true;
}

// 报告警告
void SemanticAnalyzer::reportWarning(const string& message, int line) 
{
    ostringstream oss;
    if (line > 0)
        oss << "[Line " << line << "] ";

    oss << "Warning: " << message;
    cout << oss.str() << endl;  // 警告输出到log，不阻止编译
}

bool SemanticAnalyzer::check(ParseTreeNode* root) 
{
    if (!root) 
    {
        reportError("Parse tree root is null");
        return false;
    }
    
    cout << "[SEMANTIC] Starting semantic analysis..." << endl;
    errors_.clear();
    hasErrors_ = false;
    
    analyze(root);
    
    return !hasErrors_;
}

void SemanticAnalyzer::analyze(ParseTreeNode* node, const string& currentFunctionReturnType) 
{
    if (!node) return;
    
    cout << "[SEMANTIC-CHECK] Analyzing node: " << node->semanticType 
              << " (varName: " << node->varName << ")" << endl;
    
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

    else if (node->semanticType == "Variable" || node->semanticType == "LVal")
    {
        // 检查变量是否已定义
        if (!node->varName.empty())
        {
            Symbol* symbol = currentScope_->lookup(node->varName);
            if (!symbol)
                reportError("Variable '" + node->varName + "' is not defined", node->lineNumber);
        }
        // Variable节点通常没有子节点，但为了保险还是递归检查
        for (auto* child : node->semanticChildren)
            analyze(child, currentFunctionReturnType);
    }

    else if (node->semanticType == "BinaryExpr" || node->semanticType == "UnaryExpr")
    {
        // 递归检查表达式的子节点（可能包含未定义的变量）
        for (auto* child : node->semanticChildren)
            analyze(child, currentFunctionReturnType);
    }

    else 
    {
        for (auto* child : node->semanticChildren)
            analyze(child, currentFunctionReturnType);
    }
}

// 检查程序
void SemanticAnalyzer::checkProgram(ParseTreeNode* node) 
{
    cout << "[SEMANTIC-CHECK] Checking Program with " 
              << node->semanticChildren.size() << " children" << endl;
    
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

            cout << "[SEMANTIC-CHECK] Registered function: " << funcSymbol.name 
                      << " with " << funcSymbol.paramTypes.size() << " parameters" << endl;
        }
    }
    
    // 第二遍：检查所有节点
    for (auto* child : node->semanticChildren)
        analyze(child);
}

// 检查函数定义
void SemanticAnalyzer::checkFunctionDef(ParseTreeNode* node) 
{
    cout << "[SEMANTIC-CHECK] Checking FunctionDef: " << node->varName << endl;
    
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
    string returnType = node->varType;
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
void SemanticAnalyzer::checkVarDecl(ParseTreeNode* node, const string& currentFunctionReturnType) 
{
    cout << "[SEMANTIC-CHECK] Checking VarDecl: " << node->varName 
              << " (isConst: " << node->isConst << ")" << endl;
    
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
    if (!node->semanticChildren.empty()) 
    {
        string initType = getExprType(node->semanticChildren[0]);
        // float 可以接受 int 字面量初始化（隐式提升）
        // int 可以接受 float 初始化（截断小数部分）
        bool compatible = isTypeCompatible(node->varType, initType);
        if (!compatible) 
        {
            if (node->varType == "float" && initType == "int")
            {
                // int → float
            }
            else if (node->varType == "int" && initType == "float") 
            {
                // float → int: 截断
                reportWarning("Implicit conversion from float to int in initialization of '" 
                             + node->varName + "' will truncate decimal part", node->lineNumber);
                // 截断初始化表达式中的 float 值
                if (!node->semanticChildren.empty())
                    truncateFloatToInt(node->semanticChildren[0]);
            }
            else 
            {
                reportError("Type mismatch in initialization of '" + node->varName 
                           + "': expected " + node->varType + ", got " + initType, node->lineNumber);
            }
        }
    }
    
    // 递归检查初始化表达式中的语义（如FunctionCall）
    for (auto* child : node->semanticChildren) {
        // 在分析子节点之前，确保Number节点的类型与变量声明一致
        setNumberNodeType(child, node->varType);
        analyze(child, currentFunctionReturnType);
    }
}

// 检查赋值语句
void SemanticAnalyzer::checkAssignment(ParseTreeNode* node, const string& currentFunctionReturnType) 
{
    cout << "[SEMANTIC-CHECK] Checking Assignment to: " << node->varName << endl;
    
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
        string exprType = getExprType(node->semanticChildren[0]);
        // float 可以接受 int 值赋值（隐式提升）
        // int 可以接受 float 值赋值（截断小数部分，发出警告）
        bool compatible = isTypeCompatible(symbol->type, exprType);
        if (!compatible) 
        {
            if (symbol->type == "float" && exprType == "int") 
            {
                // int → float
            }
            else if (symbol->type == "int" && exprType == "float") 
            {
                // float → int
                reportWarning("Implicit conversion from float to int in assignment to '" 
                             + node->varName + "' will truncate decimal part", node->lineNumber);
                // 截断赋值表达式中的 float 值
                if (!node->semanticChildren.empty())
                    truncateFloatToInt(node->semanticChildren[0]);
            }
            else 
            {
                reportError("Type mismatch in assignment to '" + node->varName 
                           + "': expected " + symbol->type + ", got " + exprType, node->lineNumber);
            }
        }
        
        // 递归检查赋值表达式中的语义（如FunctionCall）
        for (auto* child : node->semanticChildren) {
            // 在分析子节点之前，确保Number节点的类型与变量声明一致
            setNumberNodeType(child, symbol->type);
            analyze(child, currentFunctionReturnType);
        }
    }
}

// 检查函数调用
void SemanticAnalyzer::checkFunctionCall(ParseTreeNode* node) 
{
    string funcName = node->varName;
    cout << "[SEMANTIC-CHECK] Checking FunctionCall: " << funcName << endl;
    
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
        ostringstream oss;
        oss << "Function '" << funcName << "' expects " << expectedParams 
            << " argument(s), but " << actualParams << " provided";
        reportError(oss.str(), node->lineNumber);
        return;
    }
    
    // 参数类型
    for (size_t i = 0; i < actualParams; i++) 
    {
        // 在检查参数类型之前，确保Number节点的类型与参数类型一致
        setNumberNodeType(node->semanticChildren[i], funcSymbol->paramTypes[i]);
        string actualType = getExprType(node->semanticChildren[i]);
        string expectedType = funcSymbol->paramTypes[i];
        
        cout << "[SEMANTIC-CHECK] Function '" << funcName << "' param " << (i+1) 
                  << ": expected=" << expectedType << ", actual=" << actualType << endl;
        
        // float 参数可以接受 int 实参（隐式提升）
        // int 参数可以接受 float 实参（截断，警告）
        bool compatible = isTypeCompatible(expectedType, actualType);
        if (!compatible) 
        {
            if (expectedType == "float" && actualType == "int") 
            {
                // int → float
            }
            else if (expectedType == "int" && actualType == "float") 
            {
                // float → int
                reportWarning("Implicit conversion from float to int in argument " 
                             + to_string(i+1) + " of function '" + funcName 
                             + "' will truncate decimal part", node->lineNumber);
                // 截断函数参数中的 float 值
                truncateFloatToInt(node->semanticChildren[i]);
            }
            else 
            {
                ostringstream oss;
                oss << "Function '" << funcName << "' parameter " << (i + 1) 
                    << " type mismatch: expected " << expectedType << ", got " << actualType;
                reportError(oss.str(), node->lineNumber);
            }
        }
    }
}

// 检查返回语句
void SemanticAnalyzer::checkReturnStmt(ParseTreeNode* node, const string& expectedReturnType) 
{
    cout << "[SEMANTIC-CHECK] Checking ReturnStmt (expected: " 
              << expectedReturnType << ")" << endl;
    
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
            string actualType = getExprType(node->semanticChildren[0]);
            // float 返回值可以接受 int 表达式（隐式提升）
            // int 返回值可以接受 float 表达式（截断，警告）
            bool compatible = isTypeCompatible(expectedReturnType, actualType);
            if (!compatible) 
            {
                if (expectedReturnType == "float" && actualType == "int") 
                {
                    // int → float
                }
                else if (expectedReturnType == "int" && actualType == "float") 
                {
                    // float → int
                    reportWarning("Implicit conversion from float to int in return statement will truncate decimal part", 
                                 node->lineNumber);
                    // 截断返回表达式中的 float 值
                    if (!node->semanticChildren.empty())
                        truncateFloatToInt(node->semanticChildren[0]);
                }
                else 
                {
                    reportError("Return type mismatch: expected " + expectedReturnType 
                               + ", got " + actualType, node->lineNumber);
                }
            }
        }
    }
    
    // 递归检查返回表达式中的语义（如FunctionCall）
    for (auto* child : node->semanticChildren) {
        // 在分析子节点之前，确保Number节点的类型与返回类型一致
        setNumberNodeType(child, expectedReturnType);
        analyze(child, expectedReturnType);
    }
}

// 检查if语句
void SemanticAnalyzer::checkIfStmt(ParseTreeNode* node, const string& currentFunctionReturnType) 
{
    cout << "[SEMANTIC-CHECK] Checking IfStmt" << endl;
    
    // 递归检查if语句的子节点，传递函数返回类型
    for (auto* child : node->semanticChildren)
        analyze(child, currentFunctionReturnType);
}

// 获取表达式类型
string SemanticAnalyzer::getExprType(ParseTreeNode* node) 
{
    if (!node) return "unknown";
    
    if (node->semanticType == "Number") 
    {
        if (node->value.find('.') != string::npos)
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
    else if (node->semanticType == "BinaryExpr") 
    {
        // 二元表达式：检查两个操作数的类型
        if (node->semanticChildren.size() >= 2)
        {
            string leftType = getExprType(node->semanticChildren[0]);
            string rightType = getExprType(node->semanticChildren[1]);
            
            // 混合运算统一返回 int
            if ((leftType == "float" && rightType == "int") || 
                (leftType == "int" && rightType == "float"))
                return "int";
            
            // 两个 float 返回 float
            if (leftType == "float" && rightType == "float")
                return "float";
            
            // 默认返回 int（包括 int + int）
            return "int";
        }
        else if (!node->semanticChildren.empty())
            return getExprType(node->semanticChildren[0]);
    }
    else if (node->semanticType == "UnaryExp") 
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
bool SemanticAnalyzer::isTypeCompatible(const string& type1, const string& type2) 
{
    if (type1 == type2) return true;
    
    if ((type1 == "int" && type2 == "float") || (type1 == "float" && type2 == "int"))
        return false;  // 不允许隐式类型转换
    
    return false;
}

// 检查整数溢出
bool SemanticAnalyzer::checkIntOverflow(const string& value) 
{
    if (value.empty()) return true;
    
    try {
        string absValue = value;
        bool isNegative = false;
        if (absValue[0] == '-') 
        {
            isNegative = true;
            absValue = absValue.substr(1);
        } 
        else if (absValue[0] == '+')
            absValue = absValue.substr(1);
        
        long long num = stoll(absValue);
        if (isNegative)
            num = -num;
        
        if (num < INT_MIN || num > INT_MAX)
            return false;
        
        return true;
    } catch (const exception& e) {
        return false;  // 转换失败，说明溢出
    }
}

// 截断 float 节点为 int
void SemanticAnalyzer::truncateFloatToInt(ParseTreeNode* node) 
{
    if (!node) return;
    
    // 如果是 Number 节点，直接截断 value
    if (node->semanticType == "Number" && node->value.find('.') != string::npos) 
    {
        try {
            float floatVal = stof(node->value);
            int intVal = static_cast<int>(floatVal);  // 截断
            node->value = to_string(intVal);
            cout << "[SEMANTIC-TRUNCATE] Truncated float value to int: " 
                     << floatVal << " -> " << intVal << endl;
        } catch (...) {
            // 转换失败，保持原值
        }
    }
    
    // 递归处理子节点（如 BinaryExpr 中的 Number）
    for (auto* child : node->semanticChildren) 
    {
        truncateFloatToInt(child);
    }
}

// 设置 Number 节点的类型
void SemanticAnalyzer::setNumberNodeType(ParseTreeNode* node, const std::string& targetType) 
{
    if (!node) return;
    
    // 如果是 Number 节点，设置其 varType 与目标类型一致
    if (node->semanticType == "Number") 
    {
        node->varType = targetType;
        std::cout << "[SEMANTIC-TYPE] Set Number node type to: " << targetType << std::endl;
    }
    
    // 递归处理子节点
    for (auto* child : node->semanticChildren) 
    {
        setNumberNodeType(child, targetType);
    }
}
