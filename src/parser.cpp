#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <stack>
#include <cstring>
#include <ctime>

#ifdef _WIN32
    #include <direct.h>
    #include <sys/stat.h>
#else
    #include <sys/stat.h>
#endif

#include "parse.h"
#include "lexer.h"
#include "semantic_analyzer.h"


using namespace std;



const string PARSE_ANALYSIS_TABLE_PATH = "process/";
const string CASE_PATH = "case/";

const string S_PARSER_RESULT_EXT = "_parse_s.txt";




// 全局变量：保存原始的cout buffer
// 在main.cpp中初始化，在parser.cpp中使用
// 当parser作为独立可执行文件时，这里需要定义它
#ifndef NO_MAIN
streambuf* g_originalCoutBuffer = nullptr;
#endif

// 输出到终端的辅助函数（绕过日志重定向）
void printToConsoleParse(const string& message) 
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




// 全局变量定义
Grammar grammar;
ParseTreeNode* parseTree = nullptr;

// 序号用于分析表终结符唯一标识，注意这里与词法分析器那里的标号无关
map<string, pair<string,int> > tokenTypeToTerminal = {
    {"int", {"int",1}}, {"void", {"void",2}}, {"return", {"return",3}}, {"const", {"const",4}}, {"float", {"float",5}}, {"if", {"if",6}}, {"else", {"else",7}}, 
    
    {"+", {"+",8}}, {"-", {"-",9}}, {"*", {"*",10}}, {"/", {"/",11}}, {"%", {"%",12}},
    
    {"=", {"=",13}}, {">", {">",14}}, {"<", {"<",15}}, {"==", {"==",16}}, {"<=", {"<=",17}}, 
    {">=", {">=",18}}, {"!=", {"!=",19}}, {"&&", {"&&",20}}, {"||", {"||",21}}, {"!", {"!",22}},
    
    {"(", {"(",23}}, {")", {")",24}}, {"{", {"{",25}}, {"}", {"}",26}}, {";", {";",27}}, {",", {",",28}},
    
    {"IDN", {"Ident",29}}, {"INT", {"IntConst",30}}, {"FLOAT",{"floatConst",31}}
};


vector<pair<int, pair<string, vector<string> > > > originalProductions = {
    // 1. Program -> compUnit
    {1, {"Program", {"compUnit"}}},
    
    // 2. compUnit -> (decl | funcDef)* EOF
    {2, {"compUnit", {"compUnit_list", "EOF"}}},
    {3, {"compUnit_list", {"compUnit_list", "compUnit_item"}}},
    {4, {"compUnit_list", {"compUnit_item"}}},
    {5, {"compUnit_item", {"decl"}}},
    {6, {"compUnit_item", {"funcDef"}}},
    
    // 3. decl -> constDecl | varDecl
    {7, {"decl", {"constDecl"}}},
    {8, {"decl", {"varDecl"}}},
    
    // 4. constDecl -> 'const' bType constDef (',' constDef)* ';'
    {9, {"constDecl", {"const", "bType", "constDef", "constDef_list", ";"}}},
    {10, {"constDef_list", {"constDef_list", ",", "constDef"}}},
    {11, {"constDef_list", {EPSILON}}},
    
    // 5. bType -> 'int' | 'float' | 'void'
    {12, {"bType", {"int"}}},
    {13, {"bType", {"float"}}},
    {14, {"bType", {"void"}}},
    
    // 6. constDef -> Ident '=' constInitVal
    {15, {"constDef", {"Ident", "=", "constInitVal"}}},
    
    // 7. constInitVal -> constExp
    {16, {"constInitVal", {"constExp"}}},
    
    // 8. varDecl -> bType varDef (',' varDef)* ';'
    {17, {"varDecl", {"bType", "varDef", "varDef_list", ";"}}},
    {18, {"varDef_list", {"varDef_list", ",", "varDef"}}},
    {19, {"varDef_list", {EPSILON}}},
    
    // 9. varDef -> Ident | Ident '=' initVal
    {20, {"varDef", {"Ident"}}},
    {21, {"varDef", {"Ident", "=", "initVal"}}},
    
    // 10. initVal -> exp
    {22, {"initVal", {"exp"}}},
    
    // 11. funcDef -> bType Ident '(' (funcFParams)? ')' block  (修改：funcType改为bType)
    {23, {"funcDef", {"bType", "Ident", "(", "funcFParams_opt", ")", "block"}}},
    {24, {"funcFParams_opt", {"funcFParams"}}},
    {25, {"funcFParams_opt", {EPSILON}}},
    
    // 12. funcFParams -> funcFParam (',' funcFParam)*  (修改：序号调整)
    {26, {"funcFParams", {"funcFParam", "funcFParam_list"}}},
    {27, {"funcFParam_list", {"funcFParam_list", ",", "funcFParam"}}},
    {28, {"funcFParam_list", {EPSILON}}},
    
    // 13. funcFParam -> bType Ident
    {29, {"funcFParam", {"bType", "Ident"}}},
    
    // 14. block -> '{' (blockItem)* '}'
    {30, {"block", {"{", "blockItem_list", "}"}}},
    {31, {"blockItem_list", {"blockItem_list", "blockItem"}}},
    {32, {"blockItem_list", {EPSILON}}},
    
    // 15. blockItem -> decl | stmt
    {33, {"blockItem", {"decl"}}},
    {34, {"blockItem", {"stmt"}}},
    
    // 16. stmt -> lVal '=' exp ';' | (exp)? ';' | block | 'if' '(' cond ')' stmt ('else' stmt)? | 'return' (exp)? ';'
    {35, {"stmt", {"lVal", "=", "exp", ";"}}},
    {36, {"stmt", {"exp_opt", ";"}}},
    {37, {"stmt", {"block"}}},
    {38, {"stmt", {"if", "(", "cond", ")", "stmt", "else_opt"}}},
    {39, {"stmt", {"return", "exp_opt", ";"}}},
    {40, {"exp_opt", {"exp"}}},
    {41, {"exp_opt", {EPSILON}}},
    {42, {"else_opt", {"else", "stmt"}}},
    {43, {"else_opt", {EPSILON}}},
    
    // 17. exp -> addExp
    {44, {"exp", {"addExp"}}},
    
    // 18. cond -> lOrExp
    {45, {"cond", {"lOrExp"}}},
    
    // 19. lVal -> Ident
    {46, {"lVal", {"Ident"}}},
    
    // 20. primaryExp -> '(' exp ')' | lVal | number
    {47, {"primaryExp", {"(", "exp", ")"}}},
    {48, {"primaryExp", {"lVal"}}},
    {49, {"primaryExp", {"number"}}},
    
    // 21. number -> IntConst | floatConst
    {50, {"number", {"IntConst"}}},
    {51, {"number", {"floatConst"}}},
    
    // 22. unaryExp -> primaryExp | Ident '(' (funcRParams)? ')' | unaryOp unaryExp
    {52, {"unaryExp", {"primaryExp"}}},
    {53, {"unaryExp", {"Ident", "(", "funcRParams_opt", ")"}}},
    {54, {"unaryExp", {"unaryOp", "unaryExp"}}},
    {55, {"funcRParams_opt", {"funcRParams"}}},
    {56, {"funcRParams_opt", {EPSILON}}},
    
    // 23. unaryOp -> '+' | '-' | '!'
    {57, {"unaryOp", {"+"}}},
    {58, {"unaryOp", {"-"}}},
    {59, {"unaryOp", {"!"}}},
    
    // 24. funcRParams -> funcRParam (',' funcRParam)*
    {60, {"funcRParams", {"funcRParam", "funcRParam_list"}}},
    {61, {"funcRParam_list", {"funcRParam_list", ",", "funcRParam"}}},
    {62, {"funcRParam_list", {EPSILON}}},
    
    // 25. funcRParam -> exp
    {63, {"funcRParam", {"exp"}}},
    
    // 26. mulExp -> unaryExp | mulExp ('*' | '/' | '%') unaryExp
    {64, {"mulExp", {"unaryExp"}}},
    {65, {"mulExp", {"mulExp", "*", "unaryExp"}}},
    {66, {"mulExp", {"mulExp", "/", "unaryExp"}}},
    {67, {"mulExp", {"mulExp", "%", "unaryExp"}}},
    
    // 27. addExp -> mulExp | addExp ('+' | '-') mulExp
    {68, {"addExp", {"mulExp"}}},
    {69, {"addExp", {"addExp", "+", "mulExp"}}},
    {70, {"addExp", {"addExp", "-", "mulExp"}}},
    
    // 28. relExp -> addExp | relExp ('<' | '>' | '<=' | '>=') addExp
    {71, {"relExp", {"addExp"}}},
    {72, {"relExp", {"relExp", "<", "addExp"}}},
    {73, {"relExp", {"relExp", ">", "addExp"}}},
    {74, {"relExp", {"relExp", "<=", "addExp"}}},
    {75, {"relExp", {"relExp", ">=", "addExp"}}},
    
    // 29. eqExp -> relExp | eqExp ('==' | '!=') relExp
    {76, {"eqExp", {"relExp"}}},
    {77, {"eqExp", {"eqExp", "==", "relExp"}}},
    {78, {"eqExp", {"eqExp", "!=", "relExp"}}},
    
    // 30. lAndExp -> eqExp | lAndExp '&&' eqExp
    {79, {"lAndExp", {"eqExp"}}},
    {80, {"lAndExp", {"lAndExp", "&&", "eqExp"}}},
    
    // 31. lOrExp -> lAndExp | lOrExp '||' lAndExp
    {81, {"lOrExp", {"lAndExp"}}},
    {82, {"lOrExp", {"lOrExp", "||", "lAndExp"}}},
    
    // 32. constExp -> addExp
    {83, {"constExp", {"addExp"}}}
};



// ==================== SLR(1) DFA构造 ====================

bool isNonterminal(const string& symbol) 
{
    return grammar.nonterminals.find(symbol) != grammar.nonterminals.end();
}

/**
 * 获取所有符号（终结符+非终结符）
 */
set<string> getAllSymbols() 
{
    set<string> symbols;
    
    for (const auto& term : grammar.terminals)
        symbols.insert(term.first);
    
    for (const auto& nonterm : grammar.nonterminals)
        symbols.insert(nonterm.first);
    
    return symbols;
}

/**
 * CLOSURE(I)
 */
vector<Item> closure(const vector<Item>& items) 
{
    vector<Item> result = items;
    
    set<string> addedItems;  // "left->right,flag" 格式存储，用于去重
    for (const Item& item : result) 
    {
        string key = item.left + "->";
        for (const string& s : item.right)
            key += s + " ";
        key += "," + to_string(item.flag);
        addedItems.insert(key);
    }
    
    for (size_t i = 0; i < result.size(); i++) 
    {
        const Item& item = result[i];
        
        // 如果项目已经完成(点在末尾)
        if (item.isComplete())
            continue;
        
        // 预期符号
        string nextSymbol = item.getNextSymbol();
        
        // 预期符号是非终结符，添加相关产生式
        if (isNonterminal(nextSymbol)) 
        {
            // 找到所有形如 A -> α 的产生式
            for (size_t prodIdx = 0; prodIdx < grammar.productions.size(); prodIdx++) 
            {
                const Production& prod = grammar.productions[prodIdx];
                
                if (prod.left == nextSymbol) 
                {
                    // 创建新项目 A -> ·α
                    Item newItem(prod.left, prod.right, 0, prodIdx);
                    
                    // 判重
                    string key = newItem.left + "->";
                    for (const string& s : newItem.right)
                        key += s + " ";
                    key += ",0";
                    
                    if (addedItems.find(key) == addedItems.end()) 
                    {
                        result.push_back(newItem);
                        addedItems.insert(key);
                    }
                }
            }
        }
    }
    
    return result;
}

/**
 * GOTO：计算从一个项目集经过某个符号转换后的项目集
 * 输入：项目集 I 和符号 X
 * 输出：GOTO(I, X)
 */
vector<Item> gotoSet(const vector<Item>& items, const string& symbol) 
{
    vector<Item> movedItems;
    
    for (const Item& item : items) 
    {   
        // 项目形如 A -> α·Xβ，则将 A -> αX·β 加入结果
        if (!item.isComplete() && item.getNextSymbol() == symbol) 
        {
            Item newItem = item;
            newItem.flag++;
            movedItems.push_back(newItem);
        }
    }
    
    return closure(movedItems);
}

/**
 * 构造 SLR(1) 分析用 DFA
 * 步骤：
 * 1.创建初始项目集 I0 = CLOSURE({S' -> ·S})
 * 2.对每个项目集和每个符号，计算 GOTO
 * 3.重复直到不再产生新的项目集
 */
void buildParseDFA()
{
    cout << "[PARSER] Building SLR(1) DFA..." << endl;
    
    ParserDFA& dfa = grammar.parseDFA;
    dfa.states.clear();
    dfa.transitions.clear();
    dfa.transitionTable.clear();
    
    // 1.创建初始项目集 I0 = CLOSURE({Start -> ·Program})
    Item initialItem;
    initialItem.left = grammar.startSymbol;  // "Start"
    initialItem.right = grammar.productions[0].right;  // {"Program"}
    initialItem.flag = 0;
    initialItem.prodIndex = 0;
    
    vector<Item> I0_items = closure({initialItem});
    
    ParserDFAStatus I0(0);
    I0.items = I0_items;
    dfa.states.push_back(I0);
    dfa.startState = 0;
    
    cout << "[PARSER] Initial state I0 created with " << I0.items.size() << " items" << endl;
    cout << "[DEBUG] I0 items:" << endl;
    for (const Item& item : I0.items) 
    {
        cout << "  " << item.toString();
        if (!item.isComplete())
            cout << " [next: \"" << item.getNextSymbol() << "\"]";      
        else
            cout << " [COMPLETE]";
        cout << endl;
    }
    
    // 用于判断项目集是否已经存在
    // map: 项目集内容 -> 状态编号
    map<vector<Item>, int> stateMap;
    stateMap[I0.items] = 0;
    
    // 待处理状态编号队列
    queue<int> workQueue;
    workQueue.push(0);
    
    // 获取所有可能的符号
    set<string> allSymbols = getAllSymbols();
    
    cout << "[DEBUG] Total symbols: " << allSymbols.size() 
         << " (Terminals: " << grammar.terminals.size() 
         << ", Nonterminals: " << grammar.nonterminals.size() << ")" << endl;
    
    // 检查是否有 EPSILON
    if (allSymbols.find(EPSILON) != allSymbols.end())
        cout << "[WARNING] EPSILON \"" << EPSILON << "\" found in allSymbols" << endl;
    
    // 2&3.迭代构建状态集
    int processedStates = 0;
    while (!workQueue.empty()) 
    {
        int currentStateNum = workQueue.front();
        workQueue.pop();
        processedStates++;
        
        vector<Item> currentItems = dfa.states[currentStateNum].items;
        
        cout << "[DEBUG] Processing state I" << currentStateNum 
             << " (" << currentItems.size() << " items)" << endl;
        
        // 对每个符号计算 GOTO
        int transitionsFromThisState = 0;
        for (const string& symbol : allSymbols) 
        {
            // 计算 GOTO(currentState, symbol)
            vector<Item> gotoItems = gotoSet(currentItems, symbol);
            
            // 如果 GOTO 结果为空，跳过
            if (gotoItems.empty())
                continue;
            
            transitionsFromThisState++;
            
            // 检查这个项目集是否已经存在
            int targetStateNum;
            
            if (stateMap.find(gotoItems) != stateMap.end()) 
            {
                // 项目集已存在，复用状态编号
                targetStateNum = stateMap[gotoItems];
            } 
            else 
            {
                // 新项目集，创建新状态
                targetStateNum = dfa.states.size();
                
                ParserDFAStatus newState(targetStateNum);
                newState.items = gotoItems;
                
                dfa.states.push_back(newState);
                stateMap[gotoItems] = targetStateNum;
                workQueue.push(targetStateNum);
                
                cout << "[PARSER] Created new state I" << targetStateNum 
                     << " with " << gotoItems.size() << " items via symbol '" << symbol << "'" << endl;
            }
            
            // 添加转换：currentState --[symbol]--> targetState
            dfa.addTransition(currentStateNum, symbol, targetStateNum);
        }
        
        if (transitionsFromThisState == 0) {
            cout << "[DEBUG] State I" << currentStateNum << " has no outgoing transitions!" << endl;
        }
    }
    
    cout << "[PARSER] DFA construction completed:" << endl;
    cout << "[PARSER]   Total states: " << dfa.states.size() << endl;
    cout << "[PARSER]   Total transitions: " << dfa.transitions.size() << endl;
    cout << "[PARSER]   Processed states: " << processedStates << endl;
}



// ==================== FIRST & FOLLOW集计算 ====================

/**
 * 计算FIRST集
 * 规则：
 * 1.如果X是终结符，FIRST(X) = {X}
 * 2.如果X是非终结符且有产生式X→ε，则ε∈FIRST(X)
 * 3.如果X→Y1Y2...Yk，将FIRST(Y1)-{ε}加入FIRST(X)
 * 4.如果ε∈FIRST(Y1)...FIRST(Yi-1)，将FIRST(Yi)-{ε}加入FIRST(X)
 */
void computeFirstSets() 
{
    cout << "[PARSER] Computing FIRST sets..." << endl;
    
    // 初始化：终结符的FIRST集就是它自己
    for (const auto& term : grammar.terminals) 
        grammar.firstSets[term.first].insert(term.first);
    
    // 对非终结符，迭代计算直到不再变化
    bool changed = true;
    while (changed) 
    {
        changed = false;
        
        for (const Production& prod : grammar.productions) 
        {
            const string& A = prod.left;
            const vector<string>& right = prod.right;
            
            // 空产生式：A → ε
            if (right.empty() || (right.size() == 1 && right[0] == EPSILON)) 
            {
                if (grammar.firstSets[A].find(EPSILON) == grammar.firstSets[A].end()) 
                {
                    grammar.firstSets[A].insert(EPSILON);
                    changed = true;
                }
                continue;
            }
            
            // A → Y1 Y2 ... Yk
            for (size_t i = 0; i < right.size(); i++) 
            {
                const string& Yi = right[i];
                
                // 将FIRST(Yi) - {ε} 加入 FIRST(A)
                for (const string& symbol : grammar.firstSets[Yi]) 
                {
                    if (symbol != EPSILON) 
                    {
                        if (grammar.firstSets[A].find(symbol) == grammar.firstSets[A].end()) 
                        {
                            grammar.firstSets[A].insert(symbol);
                            changed = true;
                        }
                    }
                }
                
                // 如果Yi不能推导出ε，停止
                if (grammar.firstSets[Yi].find(EPSILON) == grammar.firstSets[Yi].end())
                    break;
                
                // 如果所有符号都能推导出ε，A也能推导出ε
                if (i == right.size() - 1) 
                {
                    if (grammar.firstSets[A].find(EPSILON) == grammar.firstSets[A].end()) 
                    {
                        grammar.firstSets[A].insert(EPSILON);
                        changed = true;
                    }
                }
            }
        }
    }
    
    cout << "[PARSER] FIRST sets computed for " << grammar.nonterminals.size() << " nonterminals" << endl;
}

/**
 * 计算FOLLOW集
 * 规则：
 * 1.将$加入FOLLOW(S)，S是开始符号
 * 2.如果A→αBβ，将FIRST(β)-{ε}加入FOLLOW(B)
 * 3.如果A→αB，或A→αBβ且ε∈FIRST(β)，将FOLLOW(A)加入FOLLOW(B)
 */
void computeFollowSets() 
{
    cout << "[PARSER] Computing FOLLOW sets..." << endl;
    
    // 规则1：$加入FOLLOW(S)
    grammar.followSets[grammar.startSymbol].insert(END_MARKER);
    
    // 迭代计算直到不再变化
    bool changed = true;
    while (changed) 
    {
        changed = false;
        
        // 遍历所有产生式 A → α
        for (const Production& prod : grammar.productions) 
        {
            const string& A = prod.left;
            const vector<string>& right = prod.right;
            
            // 跳过空产生式
            if (right.empty() || (right.size() == 1 && right[0] == EPSILON))
                continue;
            
            // 对于产生式右部的每个符号
            for (size_t i = 0; i < right.size(); i++) 
            {
                const string& B = right[i];
                
                // 只处理非终结符
                if (!isNonterminal(B))
                    continue;
                
                // 规则2：A → αBβ，将FIRST(β)-{ε}加入FOLLOW(B)
                bool allEpsilon = true;
                for (size_t j = i + 1; j < right.size(); j++) 
                {
                    const string& symbol = right[j];
                    
                    // 将FIRST(symbol) - {ε} 加入 FOLLOW(B)
                    for (const string& first : grammar.firstSets[symbol]) 
                    {
                        if (first != EPSILON) 
                        {
                            if (grammar.followSets[B].find(first) == grammar.followSets[B].end()) 
                            {
                                grammar.followSets[B].insert(first);
                                changed = true;
                            }
                        }
                    }
                    
                    // 如果symbol不能推导出ε，停止
                    if (grammar.firstSets[symbol].find(EPSILON) == grammar.firstSets[symbol].end()) 
                    {
                        allEpsilon = false;
                        break;
                    }
                }
                
                // 规则3：如果B是最后一个符号，或后面所有符号都能推导出ε
                if (i == right.size() - 1 || allEpsilon) 
                {
                    // 将FOLLOW(A)加入FOLLOW(B)
                    for (const string& follow : grammar.followSets[A]) 
                    {
                        if (grammar.followSets[B].find(follow) == grammar.followSets[B].end()) 
                        {
                            grammar.followSets[B].insert(follow);
                            changed = true;
                        }
                    }
                }
            }
        }
    }
    
    cout << "[PARSER] FOLLOW sets computed" << endl;
}

// ==================== SLR(1)分析表构造 ====================

/**
 * CSV转义函数
 */
string escapeCSV(const string& field) 
{
    bool needsQuote = (field.find(',') != string::npos || 
                       field.find('"') != string::npos || 
                       field.find('\n') != string::npos);
    
    if (!needsQuote)
        return field;
    
    string result = "\"";
    for (char c : field) 
    {
        if (c == '"')
            result += "\"\";";
        else
            result += c;
    }
    result += "\"";
    return result;
}

void buildAnalysisTable()
{
    cout << "[PARSER] Building SLR(1) analysis table..." << endl;
    
    // 先计算FIRST和FOLLOW集
    computeFirstSets();
    computeFollowSets();
    
    const ParserDFA& dfa = grammar.parseDFA;
    int stateCount = dfa.states.size();
    
    // 获取所有符号的总数（终结符+非终结符）
    int symbolCount = grammar.terminals.size() + grammar.nonterminals.size();
    
    // 初始化分析表（所有动作默认为错误，该结构还存在冲突）
    vector<vector<vector<Action>>> tableWithConflicts(stateCount, vector<vector<Action>>(symbolCount));
    
    // 对于每个状态 I
    for (int stateNum = 0; stateNum < stateCount; stateNum++) 
    {
        const ParserDFAStatus& state = dfa.states[stateNum];
        
        for (const Item& item : state.items) 
        {
            // 规则1：移进项目 A → α·aβ (a是终结符)
            if (!item.isComplete()) 
            {
                string nextSymbol = item.getNextSymbol();
                
                // 如果是终结符，添加移进动作
                if (grammar.terminals.find(nextSymbol) != grammar.terminals.end()) 
                {
                    int nextState = dfa.getTransition(stateNum, nextSymbol);
                    if (nextState != -1) 
                    {
                        int symbolIndex = grammar.terminals.at(nextSymbol) - 1;   // 这里是为了数组下标起始为0，而符号的标号从1开始，为了直接从第0列填 [写的时候混乱了，就这样吧]
                        tableWithConflicts[stateNum][symbolIndex].push_back(Action(MOVE, nextState));
                    }
                }
                // 如果是非终结符，添加GOTO
                else if (isNonterminal(nextSymbol)) 
                {
                    int nextState = dfa.getTransition(stateNum, nextSymbol);
                    if (nextState != -1) 
                    {
                        int nonterminalIndex = grammar.nonterminals.at(nextSymbol) - 1;
                        // 非终结符的索引需要加上终结符的数量，因为分析表是终结符在前，非终结符在后
                        int symbolIndex = nonterminalIndex + grammar.terminals.size();
                        tableWithConflicts[stateNum][symbolIndex].push_back(Action(MOVE, nextState));
                    }
                }
            }
            // 规则2：归约项目 A → α· (α≠S')
            else 
            {
                // 接受项目：Start → Program·
                if (item.left == grammar.startSymbol && item.prodIndex == 0) 
                {
                    // 在EOF上设置接受动作
                    int eofIndex = grammar.terminals.at(END_MARKER) - 1;
                    tableWithConflicts[stateNum][eofIndex].push_back(Action(ACC, 0));
                }
                // 普通归约项目
                else 
                {
                    // 对于FOLLOW(A)中的每个终结符a，添加"归约A→α"
                    const string& A = item.left;
                    for (const string& followSymbol : grammar.followSets[A]) 
                    {
                        auto it = grammar.terminals.find(followSymbol);
                        if (it != grammar.terminals.end()) 
                        {
                            int symbolIndex = it->second - 1;
                            tableWithConflicts[stateNum][symbolIndex].push_back(
                                Action(REDUCTION, item.prodIndex));
                        }
                    }
                }
            }
        }
    }
    
    // 去重：每个单元的动作去重，避免重复
    for (int i = 0; i < stateCount; i++) 
    {
        for (int j = 0; j < symbolCount; j++) 
        {
            if (tableWithConflicts[i][j].size() > 1) 
            {
                vector<Action> dedup;
                set<pair<int,int>> seen;
                for (const Action& a : tableWithConflicts[i][j]) 
                {
                    pair<int,int> key = {a.type, a.num};
                    if (seen.insert(key).second)
                        dedup.push_back(a);
                }
                tableWithConflicts[i][j].swap(dedup);
            }
        }
    }

    // 检测冲突并构建最终的分析表
    grammar.parseTable.resize(stateCount, vector<Action>(symbolCount, Action(-1, -1)));
    
    int conflictCount = 0;
    for (int i = 0; i < stateCount; i++) 
    {
        for (int j = 0; j < symbolCount; j++) 
        {
            if (tableWithConflicts[i][j].size() > 1) 
            {
                conflictCount++;
                cout << "[PARSER] WARNING: Conflict in state " << i << ", symbol " << j 
                     << " (" << tableWithConflicts[i][j].size() << " actions)" << endl;
                // 使用第一个动作（简化处理）
                grammar.parseTable[i][j] = tableWithConflicts[i][j][0];
            }
            else if (tableWithConflicts[i][j].size() == 1) 
                grammar.parseTable[i][j] = tableWithConflicts[i][j][0];
        }
    }
    
    cout << "[PARSER] Analysis table built: " << stateCount << " states, "
         << symbolCount << " symbols" << endl;
    if (conflictCount > 0)
        cout << "[PARSER] WARNING: Found " << conflictCount << " conflicts" << endl;
    
    // #EXPORT_DEBUG# 输出分析表
    // exportAnalysisTable(tableWithConflicts);
   
}

void exportAnalysisTable(vector<vector<vector<Action> > > tableWithConflicts)
{
    ofstream csvFile(PARSE_ANALYSIS_TABLE_PATH + "parse_analysis_table.csv");
    if (!csvFile.is_open())
    {
        cout << "[PARSER] Error: Cannot create parse_analysis_table.csv" << endl;
        return;
    }

    // 写入CSV头部
    csvFile << "State";

    // 终结符列
    for (const auto &term : grammar.terminals)
        csvFile << "," << escapeCSV(term.first);

    // 非终结符列
    for (const auto &nonterm : grammar.nonterminals)
        csvFile << "," << escapeCSV(nonterm.first);

    csvFile << endl;

    int stateCount = grammar.parseTable.size();
    int symbolCount = 0;
    if (stateCount > 0) {
        symbolCount = grammar.parseTable[0].size();
    }

    // 写入每个状态的动作
    for (int i = 0; i < stateCount; i++)
    {
        csvFile << i;

        for (int j = 0; j < symbolCount; j++)
        {
            csvFile << ",";

            // 检查是否有冲突
            if (tableWithConflicts[i][j].size() > 1)
            {
                // 有冲突，将多个动作用分号分隔
                string actions;
                for (size_t k = 0; k < tableWithConflicts[i][j].size(); k++)
                {
                    if (k > 0)
                        actions += ";";

                    const Action &action = tableWithConflicts[i][j][k];
                    if (action.type == ACC)
                        actions += "acc";
                    else if (action.type == MOVE)
                        actions += "s" + to_string(action.num);
                    else if (action.type == REDUCTION)
                        actions += "r" + to_string(action.num);
                }
                csvFile << escapeCSV(actions); // 转义（如果包含特殊字符）
            }
            else if (tableWithConflicts[i][j].size() == 1)
            {
                const Action &action = tableWithConflicts[i][j][0];
                if (action.type == ACC)
                    csvFile << "acc";
                else if (action.type == MOVE)
                    csvFile << "s" << action.num;
                else if (action.type == REDUCTION)
                    csvFile << "r" << action.num;
            }
        }
        csvFile << endl;
    }

    csvFile.close();
    cout << "[PARSER] Analysis table exported to " + PARSE_ANALYSIS_TABLE_PATH + "parse_analysis_table.csv" << endl;
}

void exportFirstSets()
{   
    ofstream outFile(PARSE_ANALYSIS_TABLE_PATH + "first_sets.txt");
    if (!outFile.is_open())
    {
        cout << "[PARSER] Error: Cannot create first_sets.txt" << endl;
        return;
    }
    
    size_t maxSymbolLen = 0;
    for (const auto& pair : grammar.firstSets)
        maxSymbolLen = max(maxSymbolLen, pair.first.length());
    
    outFile << left << setw(maxSymbolLen + 2) << "Symbol" << "FIRST Set" << endl;
    outFile << string(70, '-') << endl;
    
    for (const auto& pair : grammar.firstSets)
    {
        outFile << left << setw(maxSymbolLen + 2) << pair.first << "{ ";
        
        bool first = true;
        for (const string& symbol : pair.second)
        {
            if (!first) outFile << ", ";
            outFile << symbol;
            first = false;
        }
        
        outFile << " }" << endl;
    }
    
    outFile << endl << "=" << string(70, '=') << endl;
    outFile << "Total: " << grammar.firstSets.size() << " symbols" << endl;
    outFile << "=" << string(70, '=') << endl;
    
    outFile.close();

    printToConsoleParse("[PARSER] FIRST sets exported to " + PARSE_ANALYSIS_TABLE_PATH + "first_sets.txt\n");
    // cout << "[PARSER] FIRST sets exported to " + PARSE_ANALYSIS_TABLE_PATH + "first_sets.txt" << endl;
}

void exportFollowSets()
{   
    ofstream outFile(PARSE_ANALYSIS_TABLE_PATH + "follow_sets.txt");
    if (!outFile.is_open())
    {
        cout << "[PARSER] Error: Cannot create follow_sets.txt" << endl;
        return;
    }
    
    size_t maxSymbolLen = 0;
    for (const auto& pair : grammar.followSets)
        maxSymbolLen = max(maxSymbolLen, pair.first.length());
    
    outFile << left << setw(maxSymbolLen + 2) << "Symbol" << "FOLLOW Set" << endl;
    outFile << string(70, '-') << endl;
    
    for (const auto& pair : grammar.followSets)
    {
        outFile << left << setw(maxSymbolLen + 2) << pair.first << "{ ";
        
        bool first = true;
        for (const string& symbol : pair.second)
        {
            if (!first) outFile << ", ";
            outFile << symbol;
            first = false;
        }
        
        outFile << " }" << endl;
    }
    
    outFile << endl << "=" << string(70, '=') << endl;
    outFile << "Total: " << grammar.followSets.size() << " symbols" << endl;
    outFile << "=" << string(70, '=') << endl;
    
    outFile.close();

    printToConsoleParse("[PARSER] FOLLOW sets exported to " + PARSE_ANALYSIS_TABLE_PATH + "follow_sets.txt\n");
    // cout << "[PARSER] FOLLOW sets exported to " + PARSE_ANALYSIS_TABLE_PATH + "follow_sets.txt" << endl;
}

void initGrammar() 
{
    cout << "[DEBUG] initGrammar() started" << endl;
    grammar.startSymbol = "Start";

    // 添加所有终结符
    for (auto &t : tokenTypeToTerminal)
        grammar.terminals.insert(t.second);
    
    // 添加结束标记符号 EOF
    int eofIndex = grammar.terminals.size() + 1;
    grammar.terminals.insert({END_MARKER, eofIndex});
    
    cout << "[DEBUG] Terminals added: " << grammar.terminals.size() << endl;
    
    // 终结符标号按打表的来，非终结符从1开始独立编号
    int nonterminal_count = 0;

    // 拓广文法
    Production exprod;
    exprod.original_index = 0;
    exprod.left = "Start";
    exprod.right = vector<string>{"Program"};
    grammar.productions.push_back(exprod);

    // 先收集所有非终结符
    set<string> nonterminalSymbols;
    nonterminalSymbols.insert("Start");
    for (auto &p : originalProductions) 
    {
        nonterminalSymbols.insert(p.second.first);
        // 收集右部出现的所有可能的非终结符
        for (const string& symbol : p.second.second)
        {
            // 跳过epsilon和终结符
            if (symbol != EPSILON && grammar.terminals.find(symbol) == grammar.terminals.end())
                nonterminalSymbols.insert(symbol);
        }
    }
    
    // 为所有非终结符分配索引
    for (const string& symbol : nonterminalSymbols)
    {
        if (grammar.nonterminals.find(symbol) == grammar.nonterminals.end())
            grammar.nonterminals.insert(pair<string,int>(symbol, ++nonterminal_count));
    }
    
    // 添加产生式
    for (auto &p : originalProductions) 
    {
        Production prod;
        prod.original_index = p.first;
        prod.left = p.second.first;
        prod.right = p.second.second;
        grammar.productions.push_back(prod);
    }

    cout << "[DEBUG] Productions: " << grammar.productions.size() << ", Nonterminals: " << grammar.nonterminals.size() << endl;
    
    cout << "[DEBUG] Nonterminals:" << endl;
    int count = 0;
    for (const auto& nt : grammar.nonterminals) {
        cout << nt.second << "  " << nt.first << endl;
        ++count;
    }

    buildParseDFA();
    buildAnalysisTable();
    
    // #EXPORT_DEBUG# 输出FIRST&FOLLOW集合
    // exportFirstSets();
    // exportFollowSets();

    cout << "[DEBUG] initGrammar() completed" << endl;



    
}






class SLR1Parser 
{
private:
    stack<int> statusStack;         // 状态栈
    stack<string> symbolStack;      // 符号栈

    stack<ParseTreeNode*> treeStack;  // 语法树节点栈


    ostream &output;

    int stepCount;    // 步骤(从1开始)
    string inputFilename;  // 输入文件名


    
    bool isInGlobalScope;  // 当前是否在全局作用域
    
    // 从子节点中查找特定 symbol 的节点
    ParseTreeNode* findChild(const vector<ParseTreeNode*>& children, const string& symbol) 
    {
        for (ParseTreeNode* child : children) 
        {
            if (child && child->symbol == symbol)
                return child;
        }
        return nullptr;
    }
    
    // 递归提取表达式的值（用于常量/初始化值）
    string extractExpressionValue(ParseTreeNode* node) 
    {
        if (!node) return "";
        
        if (node->symbol == "IntConst" || node->symbol == "floatConst")
            return node->value;
        
        for (ParseTreeNode* child : node->children) 
        {
            string val = extractExpressionValue(child);
            if (!val.empty()) return val;
        }
        
        return "";
    }
    
    // 递归收集有语义类型的子节点
    void collectSemanticChildren(ParseTreeNode* node, vector<ParseTreeNode*>& result)
    {
        if (!node) return;
        
        cout << "[COLLECT] Node: " << node->symbol << ", semanticType: '" 
             << node->semanticType << "', children: " << node->children.size() << endl;
        
        // 如果当前节点有语义类型，检查是否可以优化
        if (!node->semanticType.empty())
        {
            // Constant folding: UnaryExp with +/- and single Number operand
            if (node->semanticType == "UnaryExp" && 
                (node->operatorType == "+" || node->operatorType == "-") &&
                node->semanticChildren.size() == 1 &&
                node->semanticChildren[0]->semanticType == "Number")
            {
                // Create a new Number node with folded value
                ParseTreeNode* foldedNode = new ParseTreeNode(NODE_NONTERMINAL, "Number", "", -1);
                foldedNode->semanticType = "Number";
                foldedNode->lineNumber = node->lineNumber;  // 继承行号
                if (node->operatorType == "-")
                {
                    foldedNode->value = "-" + node->semanticChildren[0]->value;
                }
                else  // "+"
                {
                    foldedNode->value = node->semanticChildren[0]->value;
                }
                foldedNode->semanticChildren.clear();
                
                cout << "[COLLECT] Folded UnaryExp " << node->operatorType 
                     << " into Number: " << foldedNode->value << endl;
                result.push_back(foldedNode);
            }
            else
            {
                // Normal case: add node as-is
                cout << "[COLLECT] Adding node with semanticType: " << node->semanticType 
                     << ", varName: " << node->varName << ", ptr: " << (void*)node << endl;
                result.push_back(node);
            }
        }
        else
        {            // 否则递归处理所有子节点
            for (ParseTreeNode* child : node->children)
            {
                if (child)
                    collectSemanticChildren(child, result);
            }
        }
    }
    
    // 递归设置节点及其子节点为局部作用域
    void setLocalScope(ParseTreeNode* node)
    {
        if (!node) return;
        node->isGlobal = false;
        for (ParseTreeNode* child : node->children)
            setLocalScope(child);
    }
    
    // 语义分析
    void fillSemanticAttributes(ParseTreeNode* node, int originalIndex, const vector<ParseTreeNode*>& children) 
    {
        if (!node) return;
        
        // 自动从第一个子节点继承行号
        if (!children.empty() && children[0])
            node->lineNumber = children[0]->lineNumber;
        
        cout << "[SEMANTIC] Filling attributes for production " << originalIndex 
             << " (" << node->symbol << ")" << endl;
        
        switch (originalIndex) 
        {
            // 1.Program -> compUnit
            case 1:
            {
                node->semanticType = "Program";
                cout << "[SEMANTIC] Set Program semanticType" << endl;
                
                // 从compUnit中收集所有语义子节点
                vector<ParseTreeNode*> semanticChildren;
                for (ParseTreeNode* child : children)
                {
                    if (child)
                        collectSemanticChildren(child, semanticChildren);
                }
                node->semanticChildren = semanticChildren;
                cout << "[SEMANTIC] Program collected " << semanticChildren.size() << " semantic children" << endl;
                break;
            }
            
            // 2-6.compUnit 相关：收集所有声明和函数定义
            case 2:
            {
                cout << "[SEMANTIC] Processing compUnit, children count: " << children.size() << endl;
                // compUnit_list
                ParseTreeNode* listNode = findChild(children, "compUnit_list");
                if (listNode)
                {
                    cout << "[SEMANTIC] Found compUnit_list, collecting semantic children" << endl;
                    // 递归收集 compUnit_list 中所有有语义的节点
                    vector<ParseTreeNode*> semanticChildren;
                    collectSemanticChildren(listNode, semanticChildren);
                    cout << "[SEMANTIC] Collected " << semanticChildren.size() << " semantic children" << endl;
                    node->semanticChildren = semanticChildren;  // 使用semanticChildren，不修改children
                }
                else
                    cout << "[SEMANTIC] WARNING: compUnit_list not found!" << endl;
                break;
            }
            case 3:  // compUnit_list -> compUnit_list compUnit_item
            case 5:  // compUnit_item -> decl
            case 6:  // compUnit_item -> funcDef
                {
                    vector<ParseTreeNode*> semanticChildren;
                    for (ParseTreeNode* child : children)
                    {
                        if (child)
                            collectSemanticChildren(child, semanticChildren);
                    }
                    node->semanticChildren = semanticChildren;  // 使用semanticChildren
                }
                break;
            case 4:  // compUnit_list -> epsilon
                // epsilon 产生式，children 为空
                node->semanticChildren.clear();
                break;
            
            // 7.decl -> constDecl
            case 7:
                // decl 只是中间节点，不设置 semanticType，直接保留 children 以便 collectSemanticChildren 穿透
                break;
            
            // 8.decl -> varDecl
            case 8:
                break;
            
            // 9.constDecl -> 'const' bType constDef (',' constDef)* ';'
            case 9:
            {
                ParseTreeNode* bTypeNode = findChild(children, "bType");
                
                if (bTypeNode)
                {
                    // 收集所有constDef节点
                    vector<ParseTreeNode*> constDefNodes;
                    for (ParseTreeNode* child : children)
                    {
                        if (child && child->symbol == "constDef")
                            constDefNodes.push_back(child);
                    }
                    
                    if (!constDefNodes.empty())
                    {
                        // 如果只有一个变量，当前节点就是VarDecl
                        if (constDefNodes.size() == 1)
                        {
                            node->semanticType = "VarDecl";
                            node->varType = bTypeNode->varType;
                            node->varName = constDefNodes[0]->varName;
                            node->isConst = true;
                            node->isGlobal = isInGlobalScope;
                            node->initValue = constDefNodes[0]->initValue;
                            
                            // 收集初始化表达式的语义子节点
                            if (!constDefNodes[0]->semanticChildren.empty())
                            {
                                node->semanticChildren = constDefNodes[0]->semanticChildren;
                            }
                            else
                            {
                                node->semanticChildren.clear();
                            }
                        }
                        else
                        {
                            // 多个变量，创建多个VarDecl子节点
                            for (ParseTreeNode* constDefNode : constDefNodes)
                            {
                                ParseTreeNode* varDeclNode = new ParseTreeNode(NODE_NONTERMINAL, "VarDecl", "", -1);
                                varDeclNode->semanticType = "VarDecl";
                                varDeclNode->varType = bTypeNode->varType;
                                varDeclNode->varName = constDefNode->varName;
                                varDeclNode->isConst = true;
                                varDeclNode->isGlobal = isInGlobalScope;
                                varDeclNode->initValue = constDefNode->initValue;
                                varDeclNode->lineNumber = constDefNode->lineNumber;  // 继承行号
                                
                                // 复制初始化表达式的语义子节点
                                varDeclNode->semanticChildren = constDefNode->semanticChildren;
                                
                                node->semanticChildren.push_back(varDeclNode);
                            }
                        }
                    }
                }
                break;
            }
            
            // 12-14.bType -> 'int' | 'float' | 'void'
            case 12:
                node->varType = "int";
                break;
            case 13:
                node->varType = "float";
                break;
            case 14:
                node->varType = "void";
                break;
            
            // 15.constDef -> Ident '=' constInitVal
            case 15:
            {
                ParseTreeNode* identNode = findChild(children, "Ident");
                ParseTreeNode* initValNode = findChild(children, "constInitVal");
                
                if (identNode)
                    node->varName = identNode->value;
                if (initValNode)
                {
                    node->initValue = extractExpressionValue(initValNode);
                    // 收集语义子节点
                    collectSemanticChildren(initValNode, node->semanticChildren);
                }
                break;
            }
            
            // 17.varDecl -> bType varDef (',' varDef)* ';'
            case 17:
            {
                cout << "[SEMANTIC] Processing varDecl" << endl;
                ParseTreeNode* bTypeNode = findChild(children, "bType");
                
                if (bTypeNode)
                {
                    // 收集所有varDef节点
                    vector<ParseTreeNode*> varDefNodes;
                    for (ParseTreeNode* child : children)
                    {
                        if (child && child->symbol == "varDef")
                            varDefNodes.push_back(child);
                    }
                    
                    cout << "[SEMANTIC] Found " << varDefNodes.size() << " varDef nodes" << endl;
                    
                    if (!varDefNodes.empty())
                    {
                        // 如果只有一个变量，当前节点就是VarDecl
                        if (varDefNodes.size() == 1)
                        {
                            node->semanticType = "VarDecl";
                            node->varType = bTypeNode->varType;
                            node->varName = varDefNodes[0]->varName;
                            node->isConst = false;
                            node->isGlobal = isInGlobalScope;  // 使用当前作用域状态
                            node->initValue = varDefNodes[0]->initValue;
                            
                            // 收集初始化表达式的语义子节点
                            if (!varDefNodes[0]->semanticChildren.empty())
                            {
                                node->semanticChildren = varDefNodes[0]->semanticChildren;
                            }
                            else
                            {
                                node->semanticChildren.clear();
                            }
                            
                            cout << "[SEMANTIC] Created VarDecl: " << node->varType << " " 
                                 << node->varName << " = " << node->initValue 
                                 << " (isGlobal=" << node->isGlobal 
                                 << ", semanticChildren.size()=" << node->semanticChildren.size() << ")" << endl;
                        }
                        else
                        {
                            // 多个变量，创建多个VarDecl子节点
                            for (ParseTreeNode* varDefNode : varDefNodes)
                            {
                                ParseTreeNode* varDeclNode = new ParseTreeNode(NODE_NONTERMINAL, "VarDecl", "", -1);
                                varDeclNode->semanticType = "VarDecl";
                                varDeclNode->varType = bTypeNode->varType;
                                varDeclNode->varName = varDefNode->varName;
                                varDeclNode->isConst = false;
                                varDeclNode->isGlobal = isInGlobalScope;  // 使用当前作用域状态
                                varDeclNode->initValue = varDefNode->initValue;
                                varDeclNode->lineNumber = varDefNode->lineNumber;
                                
                                // 复制初始化表达式的语义子节点
                                varDeclNode->semanticChildren = varDefNode->semanticChildren;
                                
                                node->semanticChildren.push_back(varDeclNode);
                                
                                cout << "[SEMANTIC] Created VarDecl: " << varDeclNode->varType << " " 
                                     << varDeclNode->varName << " = " << varDeclNode->initValue 
                                     << " (isGlobal=" << varDeclNode->isGlobal << ")" << endl;
                            }
                        }
                    }
                }
                break;
            }
            
            // 20.varDef -> Ident
            case 20:
            {
                ParseTreeNode* identNode = findChild(children, "Ident");
                if (identNode)
                    node->varName = identNode->value;
                break;
            }
            
            // 21.varDef -> Ident '=' initVal
            case 21:
            {
                ParseTreeNode* identNode = findChild(children, "Ident");
                ParseTreeNode* initValNode = findChild(children, "initVal");
                
                if (identNode)
                    node->varName = identNode->value;
                    
                // 不再只提取简单值，而是收集完整的表达式语义节点
                if (initValNode)
                {
                    // 尝试提取简单值用于显示
                    node->initValue = extractExpressionValue(initValNode);
                    
                    // 收集语义子节点（用于复杂表达式）
                    collectSemanticChildren(initValNode, node->semanticChildren);
                    
                    cout << "[SEMANTIC] varDef (case 21): varName='" << node->varName 
                         << "', initValue='" << node->initValue 
                         << "', semanticChildren.size()=" << node->semanticChildren.size() << endl;
                }
                break;
            }
            
            // 23.funcDef -> bType Ident '(' (funcFParams)? ')' block
            case 23:
            {
                ParseTreeNode* bTypeNode = findChild(children, "bType");
                ParseTreeNode* identNode = findChild(children, "Ident");
                ParseTreeNode* funcFParamsOpt = findChild(children, "funcFParams_opt");
                ParseTreeNode* blockNode = findChild(children, "block");
                
                if (bTypeNode && identNode)
                {
                    node->semanticType = "FunctionDef";
                    node->varType = bTypeNode->varType;
                    node->varName = identNode->value;
                    
                    // 收集函数参数
                    if (funcFParamsOpt)
                    {
                        vector<ParseTreeNode*> paramNodes;
                        collectSemanticChildren(funcFParamsOpt, paramNodes);
                        node->semanticChildren.insert(node->semanticChildren.end(), 
                                                      paramNodes.begin(), paramNodes.end());
                        cout << "[SEMANTIC] FunctionDef collected " << paramNodes.size() << " parameters" << endl;
                    }
                    
                    // 将 block 的语义子节点添加为当前节点的语义子节点
                    if (blockNode)
                    {
                        vector<ParseTreeNode*> semanticChildren;
                        collectSemanticChildren(blockNode, semanticChildren);
                        node->semanticChildren.insert(node->semanticChildren.end(), 
                                                      semanticChildren.begin(), semanticChildren.end());
                        cout << "[SEMANTIC] FunctionDef collected " << semanticChildren.size() << " semantic children from block" << endl;
                    }
                    
                    // 将所有从block收集的VarDecl设置为局部变量
                    for (ParseTreeNode* child : node->semanticChildren)
                    {
                        if (child && child->semanticType == "VarDecl")
                        {
                            child->isGlobal = false;
                            cout << "[SEMANTIC] Set VarDecl '" << child->varName << "' to local" << endl;
                        }
                    }
                    
                    // 设置全局作用域标志（用于后续可能的嵌套函数，虽然SysY不支持）
                    isInGlobalScope = false;
                }
                break;
            }
            
            // 29.funcFParam -> bType Ident
            case 29:
            {
                ParseTreeNode* bTypeNode = findChild(children, "bType");
                ParseTreeNode* identNode = findChild(children, "Ident");
                
                if (bTypeNode && identNode)
                {
                    node->semanticType = "FuncParam";
                    node->varType = bTypeNode->varType;
                    node->varName = identNode->value;
                    node->semanticChildren.clear();
                    
                    cout << "[SEMANTIC] Created FuncParam: " << node->varType << " " << node->varName << endl;
                }
                break;
            }
            
            // 30-32.block -> '{' blockItem_list '}'
            case 30:  // block -> '{' blockItem_list '}'
            {
                // blockItem_list
                ParseTreeNode* listNode = findChild(children, "blockItem_list");
                if (listNode)
                {
                    vector<ParseTreeNode*> semanticChildren;
                    collectSemanticChildren(listNode, semanticChildren);
                    node->semanticChildren = semanticChildren;  // 使用semanticChildren
                }
                break;
            }
            case 31:  // blockItem_list -> blockItem_list blockItem
                {
                    vector<ParseTreeNode*> semanticChildren;
                    for (ParseTreeNode* child : children)
                    {
                        if (child)
                            collectSemanticChildren(child, semanticChildren);
                    }
                    node->semanticChildren = semanticChildren;  // 使用semanticChildren
                }
                break;
            case 32:  // blockItem_list -> epsilon
                node->semanticChildren.clear();
                break;
            
            // 33-34.blockItem -> decl | stmt
            case 33:  // blockItem -> decl
                // blockItem 只是中间节点，不设置 semanticType
                // isGlobal 已经在 FunctionDef 处理时通过 isInGlobalScope=false 设置
                // 不需要再次调用 setLocalScope，因为 VarDecl 已经正确设置了 isGlobal
                break;
            case 34:  // blockItem -> stmt
                break;
            
            // 35.stmt -> lVal '=' exp ';'
            case 35:
            {
                ParseTreeNode* lValNode = findChild(children, "lVal");
                ParseTreeNode* expNode = findChild(children, "exp");
                
                if (lValNode && expNode)
                {
                    node->semanticType = "Assignment";
                    node->varName = lValNode->varName;
                    
                    // 从 exp 中收集语义子节点
                    collectSemanticChildren(expNode, node->semanticChildren);
                }
                break;
            }
            
            // 36-38.stmt -> exp_opt ';' | block | if
            case 36:  // stmt -> exp_opt ';'
            case 37:  // stmt -> block
                // 这些类型暂不处理或直接传递
                break;
            
            // 38.stmt -> 'if' '(' cond ')' stmt else_opt
            case 38:
            {
                node->semanticType = "IfStmt";
                
                ParseTreeNode* condNode = findChild(children, "cond");
                ParseTreeNode* thenStmt = nullptr;
                ParseTreeNode* elseOpt = findChild(children, "else_opt");
                
                // 找到then分支的stmt (第5个child)
                if (children.size() >= 5)
                    thenStmt = children[4];  // if ( cond ) stmt
                
                // 创建Condition节点
                if (condNode)
                {
                    ParseTreeNode* conditionNode = new ParseTreeNode(NODE_NONTERMINAL, "Condition", "", -1);
                    conditionNode->semanticType = "Condition";
                    conditionNode->lineNumber = condNode->lineNumber;
                    collectSemanticChildren(condNode, conditionNode->semanticChildren);
                    node->semanticChildren.push_back(conditionNode);
                }
                
                // 创建ThenBranch节点
                if (thenStmt)
                {
                    ParseTreeNode* thenNode = new ParseTreeNode(NODE_NONTERMINAL, "ThenBranch", "", -1);
                    thenNode->semanticType = "ThenBranch";
                    thenNode->lineNumber = thenStmt->lineNumber;
                    collectSemanticChildren(thenStmt, thenNode->semanticChildren);
                    node->semanticChildren.push_back(thenNode);
                }
                
                // 创建ElseBranch节点（如果有）
                if (elseOpt)
                {
                    vector<ParseTreeNode*> elseChildren;
                    collectSemanticChildren(elseOpt, elseChildren);
                    
                    if (!elseChildren.empty())
                    {
                        ParseTreeNode* elseNode = new ParseTreeNode(NODE_NONTERMINAL, "ElseBranch", "", -1);
                        elseNode->semanticType = "ElseBranch";
                        elseNode->lineNumber = elseOpt->lineNumber;
                        elseNode->semanticChildren = elseChildren;
                        node->semanticChildren.push_back(elseNode);
                    }
                }
                
                cout << "[SEMANTIC] Created IfStmt with " << node->semanticChildren.size() << " branches" << endl;
                break;
            }
            
            // 39.stmt -> 'return' (exp)? ';'
            case 39:
            {
                node->semanticType = "ReturnStmt";
                
                ParseTreeNode* expOptNode = findChild(children, "exp_opt");
                if (expOptNode)
                {
                    // 从 exp_opt 中收集语义子节点
                    collectSemanticChildren(expOptNode, node->semanticChildren);
                }
                break;
            }
            
            // 40-43.可选表达式和 else 分支
            case 40:  // exp_opt -> exp
            case 41:  // exp_opt -> epsilon
            case 42:  // else_opt -> else stmt
            case 43:  // else_opt -> epsilon
                break;
            
            // 46.lVal -> Ident
            case 46:
            {
                ParseTreeNode* identNode = findChild(children, "Ident");
                if (identNode)
                {
                    node->semanticType = "Variable";
                    node->varName = identNode->value;
                    node->semanticChildren.clear();
                }
                break;
            }
            
            // 50-51.number -> IntConst | floatConst
            case 50:
            case 51:
            {
                if (!children.empty() && children[0])
                {
                    node->semanticType = "Number";
                    node->value = children[0]->value;
                    node->semanticChildren.clear();
                }
                break;
            }
            
            // 表达式传递：不设置semanticType，让语义信息穿透
            // 44.exp -> addExp
            case 44:
            // 45.cond -> lOrExp
            case 45:
            // 48.primaryExp -> lVal
            case 48:
            // 49.primaryExp -> number
            case 49:
            // 52.unaryExp -> primaryExp
            case 52:
            // 64.mulExp -> unaryExp
            case 64:
            // 68.addExp -> mulExp
            case 68:
            // 71.relExp -> addExp
            case 71:
            // 76.eqExp -> relExp
            case 76:
            // 79.lAndExp -> eqExp
            case 79:
            // 81.lOrExp -> lAndExp
            case 81:
            {
                // 不设置semanticType，保持节点透明，让collectSemanticChildren穿透
                break;
            }
            
            // 53.unaryExp -> Ident '(' funcRParams_opt ')' - 函数调用
            case 53:
            {
                node->semanticType = "FunctionCall";
                
                // 获取函数名（children[0]应该是Ident）
                if (!children.empty() && children[0] && children[0]->type == NODE_TERMINAL)
                {
                    node->varName = children[0]->value;
                }
                
                // 收集参数（从funcRParams_opt中）
                ParseTreeNode* funcRParamsOpt = findChild(children, "funcRParams_opt");
                if (funcRParamsOpt)
                {
                    collectSemanticChildren(funcRParamsOpt, node->semanticChildren);
                }
                
                cout << "[SEMANTIC] Created FunctionCall: " << node->varName 
                     << " with " << node->semanticChildren.size() << " arguments" << endl;
                break;
            }
            
            // 54.unaryExp -> unaryOp unaryExp - 一元运算
            case 54:
            {
                // 获取运算符
                ParseTreeNode* unaryOpNode = findChild(children, "unaryOp");
                string op;
                if (unaryOpNode && !unaryOpNode->children.empty())
                {
                    op = unaryOpNode->children[0]->symbol;  // +, -, !
                }
                
                // 收集操作数（保持语义树结构：unaryExp 的语义子节点就是内部的表达式）
                ParseTreeNode* unaryExpNode = findChild(children, "unaryExp");
                if (unaryExpNode)
                {
                    vector<ParseTreeNode*> operands;
                    collectSemanticChildren(unaryExpNode, operands);
                    
                    // Optimization: if operand is a single Number constant and operator is + or -,
                    // fold the sign into the number value directly
                    if (!operands.empty() && operands.size() == 1 && 
                        operands[0]->semanticType == "Number" && 
                        (op == "+" || op == "-"))
                    {
                        // Fold constant: create a Number node with signed value
                        node->semanticType = "Number";
                        if (op == "-")
                        {
                            node->value = "-" + operands[0]->value;
                        }
                        else  // op == "+"
                        {
                            node->value = operands[0]->value;
                        }
                        node->semanticChildren.clear();
                        cout << "[SEMANTIC] Folded UnaryExp " << op << " into Number: " << node->value << endl;
                    }
                    else
                    {
                        // General case: create UnaryExp node
                        node->semanticType = "UnaryExp";
                        node->operatorType = op;
                        node->semanticChildren = operands;
                        cout << "[SEMANTIC] Created UnaryExp: " << node->operatorType << endl;
                    }
                }
                break;
            }
            
            // 二元表达式：创建 BinaryExpr 节点
            // 65-67.mulExp -> mulExp ('*' | '/' | '%') unaryExp
            case 65:  // *
            case 66:  // /
            case 67:  // %
            // 69-70.addExp -> addExp ('+' | '-') mulExp
            case 69:  // +
            case 70:  // -
            // 72-75.relExp -> relExp ('<' | '>' | '<=' | '>=') addExp
            case 72:  // <
            case 73:  // >
            case 74:  // <=
            case 75:  // >=
            // 77-78.eqExp -> eqExp ('==' | '!=') relExp
            case 77:  // ==
            case 78:  // !=
            // 80.lAndExp -> lAndExp '&&' eqExp
            case 80:  // &&
            // 82.lOrExp -> lOrExp '||' lAndExp
            case 82:  // ||
            {
                node->semanticType = "BinaryExpr";
                
                // 确定运算符
                string op;
                if (originalIndex == 65) op = "*";
                else if (originalIndex == 66) op = "/";
                else if (originalIndex == 67) op = "%";
                else if (originalIndex == 69) op = "+";
                else if (originalIndex == 70) op = "-";
                else if (originalIndex == 72) op = "<";
                else if (originalIndex == 73) op = ">";
                else if (originalIndex == 74) op = "<=";
                else if (originalIndex == 75) op = ">=";
                else if (originalIndex == 77) op = "==";
                else if (originalIndex == 78) op = "!=";
                else if (originalIndex == 80) op = "&&";
                else if (originalIndex == 82) op = "||";
                
                node->operatorType = op;
                
                // 收集左右操作数：children[0] 和 children[2]
                if (children.size() >= 3)
                {
                    if (children[0])
                        collectSemanticChildren(children[0], node->semanticChildren);
                    if (children[2])
                        collectSemanticChildren(children[2], node->semanticChildren);
                }
                
                cout << "[SEMANTIC] Created BinaryExpr: " << op << " with " 
                     << node->semanticChildren.size() << " operands" << endl;
                break;
            }
            
            default:
                break;
        }
    }
    
    // 将token转换为文法终结符名
    string getCurrentToken(Token &token) 
    {
        if (!token.valid) 
            return END_MARKER;
        
        string tokenText = token.text;
        string tokenType = token.type;
        
        if (tokenType == "IDN") return "Ident";
        if (tokenType == "INT") return "IntConst";
        if (tokenType == "FLOAT") return "floatConst";
        
        // 关键字转小写
        if (tokenType == "KW")
            transform(tokenText.begin(), tokenText.end(), tokenText.begin(), ::tolower);
        
        if (tokenTypeToTerminal.count(tokenText))
            return tokenTypeToTerminal[tokenText].first;

        return tokenText;
    }
    
    // 获取符号在分析表中的列索引（0-based）
    int getSymbolIndex(const string& symbol) 
    {
        auto it_term = grammar.terminals.find(symbol);
        if (it_term != grammar.terminals.end())
            return it_term->second - 1;
        
        auto it_nonterm = grammar.nonterminals.find(symbol);
        if (it_nonterm != grammar.nonterminals.end())
            return it_nonterm->second - 1 + grammar.terminals.size();
        
        return -1;
    }
    
    // 获取动作的字符串表示
    string getActionString(const Action& action) 
    {
        if (action.type == ACC)
            return "acc";
        else if (action.type == MOVE)
            return "s" + to_string(action.num);
        else if (action.type == REDUCTION)
            return "r" + to_string(action.num);
        else
            return "error";
    }
    
public:
    SLR1Parser(ostream &out, const string& filename = "") 
        : output(out), stepCount(1), inputFilename(filename), isInGlobalScope(true)
    {
        initGrammar();
    }
    
    ~SLR1Parser() {
        // BUGFIX: Do not delete parseTree or treeStack nodes
        // These nodes are still referenced via semanticChildren pointers
        // Deleting them causes dangling pointers
        
        // if (parseTree)
        //     delete parseTree;

        // while (!treeStack.empty()) 
        // {
        //     delete treeStack.top();
        //     treeStack.pop();
        // }
    }
    
    ParseTreeNode* getParseTree() const 
    {
        return parseTree;
    }
    
    // 输出语法分析树 (md格式)
    void outputParseTree() 
    {
        if (!parseTree) 
        {
            cout << "[PARSER] No parse tree available." << endl;
            return;
        }
        
        if (!inputFilename.empty()) 
        {
            // 输出Mermaid格式
            string mdFilePath = CASE_PATH + inputFilename + "_parse_tree.md";
            ofstream mdFile(mdFilePath);
            if (mdFile.is_open()) 
            {
                mdFile << parseTree->toMermaidMarkdown();
                mdFile.close();
                cout << "[PARSER] Parse tree (Mermaid) saved to: " << mdFilePath << endl;
            }
            
            // 输出语义树简化格式
            string semanticFilePath = CASE_PATH + inputFilename + "_semantic_tree.txt";
            ofstream semanticFile(semanticFilePath);
            if (semanticFile.is_open()) 
            {
                semanticFile << parseTree->toSemanticString();
                semanticFile.close();

                printToConsoleParse("[PARSER] Semantic tree saved to: " + semanticFilePath + "\n");
                // cout << "[PARSER] Semantic tree saved to: " << semanticFilePath << endl;
            }
            
            // cout << "\n[PARSER] Semantic Tree:\n" << endl;
        }
    }
    
    bool parse() 
    {
        cout << "\n\n ==============================================\n\n";
        cout << "[PARSER] Starting SLR(1) parsing..." << endl;
        
        ofstream parseLog;
        ofstream s_parseResult;
        if (!inputFilename.empty()) 
        {
            string logPath = CASE_PATH + inputFilename + "_parse_analysis.txt";
            string s_parsePath = "output/" + inputFilename + S_PARSER_RESULT_EXT;
            parseLog.open(logPath);
            s_parseResult.open(s_parsePath);
            if (!s_parseResult.is_open())
                cout << "[PARSER] Error: Cannot create parse analysis results: " << s_parsePath << endl;
            if (!parseLog.is_open())
                cout << "[PARSER] Error: Cannot create parse analysis log: " << logPath << endl;
            else 
            {
                cout << "[PARSER] Parse analysis log: " << logPath << endl;

                // TODO 为方便调试先输出的详细一点，而老师文档里给的不像是SLR(1)版本的输出格式，后续要交前再改一下就行
                parseLog << left << setw(8) << "Step" 
                         << setw(12) << "State" 
                         << setw(20) << "Symbol Stack" 
                         << setw(20) << "Input" 
                         << setw(15) << "Action" << endl;
                parseLog << string(75, '-') << endl;
            }
        }
        
        

        // ============ SLR(1) 分析步骤 ============
        
        // 1.状态栈压入初始状态0，符号栈为空
        statusStack.push(0);
        
        // 2.读取第一个输入符号
        Token currentToken = getNextToken();
        string currentSymbol = getCurrentToken(currentToken);
        
        cout << "[PARSER] First input symbol: " << currentSymbol << endl;
        
        int step = 1;
        bool success = false;
        
        // 3.分析，直到接受或出错
        while (true) 
        {
            int currentState = statusStack.top();
            string topSymbol = symbolStack.empty() ? "#" : symbolStack.top();
            
            // 查询分析表 ACTION[currentState, currentSymbol]
            int symbolIndex = getSymbolIndex(currentSymbol);
            
            if (symbolIndex == -1) 
            {
                // 符号不在文法中，错误
                cout << "[PARSER] Error: Unknown symbol '" << currentSymbol << "'" << endl;
                // 输出到终端的错误信息
                std::ostringstream errorMsg;
                errorMsg << "[Line " << currentToken.lineNumber << "] Bad token " << currentSymbol;
                printToConsoleParse(errorMsg.str());
                
                if (parseLog.is_open()) 
                {
                    parseLog << left << setw(8) << step 
                             << setw(12) << currentState 
                             << setw(20) << topSymbol 
                             << setw(20) << currentSymbol 
                             << setw(15) << "Error: Unknown symbol" << endl;
                }


                if (s_parseResult.is_open())
                {
                    s_parseResult << step << "\t"
                            << topSymbol << "#"
                            << currentSymbol << "\t"
                            << "Error: Unknown symbol" << endl;
                }

                break;
            }
            
            // 检查边界
            if (currentState < 0 || currentState >= (int)grammar.parseTable.size() || 
                symbolIndex < 0 || symbolIndex >= (int)grammar.parseTable[currentState].size()) {
                cout << "[PARSER] Error: Index out of bounds. currentState=" << currentState 
                     << ", symbolIndex=" << symbolIndex 
                     << ", parseTable.size()=" << grammar.parseTable.size()
                     << ", parseTable[" << currentState << "].size()=" << grammar.parseTable[currentState].size() << endl;
                break;
            }
            
            Action action = grammar.parseTable[currentState][symbolIndex];
            
            string actionStr = getActionString(action);
            if (parseLog.is_open()) 
            {
                parseLog << left << setw(8) << step 
                         << setw(12) << currentState 
                         << setw(20) << topSymbol 
                         << setw(20) << currentSymbol 
                         << setw(15) << actionStr << endl;

                if (s_parseResult.is_open())
                {
                    s_parseResult << step << "\t"
                            << topSymbol << "#"
                            << currentSymbol << "\t"
                            << (action.type == MOVE ? "move" : "reduction")<< endl;
                }
            }
            
            // 4.根据动作类型执行操作
            if (action.type == ACC) 
            {
                // 接受
                cout << "[PARSER] Accept! Parsing completed successfully." << endl;
                
                // 保存语法树根节点
                if (!treeStack.empty()) 
                {
                    parseTree = treeStack.top();
                    treeStack.pop();
                }
                
                success = true;
                break;
            }
            else if (action.type == MOVE) 
            {
                // 移进
                int nextState = action.num;
                
                // (1)状态栈压入新状态
                statusStack.push(nextState);
                
                // (2)符号栈压入当前输入符号
                symbolStack.push(currentSymbol);
                
                // (3)创建终结符节点并压入树栈
                ParseTreeNode* terminalNode = new ParseTreeNode(
                    NODE_TERMINAL, 
                    currentSymbol, 
                    string(currentToken.text)  // 保存token的原始文本值
                );
                terminalNode->lineNumber = currentToken.lineNumber;
                treeStack.push(terminalNode);
                
                // (4)读取下一个输入符号
                currentToken = getNextToken();
                currentSymbol = getCurrentToken(currentToken);
                
                step++;
            }
            else if (action.type == REDUCTION) 
            {
                // 归约 (Reduce)
                int prodIndex = action.num;
                
                // 获取产生式 A -> β
                const Production& prod = grammar.productions[prodIndex];
                string A = prod.left;  // 左部非终结符
                const vector<string>& beta = prod.right;  // 右部符号串
                
                // 计算右部长度（跳过epsilon）
                int betaLen = 0;
                if (!(beta.size() == 1 && beta[0] == EPSILON))
                    betaLen = beta.size();
                
                // 创建非终结符节点
                ParseTreeNode* nonterminalNode = new ParseTreeNode(
                    NODE_NONTERMINAL,
                    A,
                    "",
                    prodIndex
                );
                
                // (1)从树栈中弹出 |β| 个子节点，添加为当前节点的子节点
                vector<ParseTreeNode*> childrenNodes;
                for (int i = 0; i < betaLen; i++) 
                {
                    if (!treeStack.empty()) 
                    {
                        childrenNodes.push_back(treeStack.top());
                        treeStack.pop();
                    }
                    if (!statusStack.empty()) statusStack.pop();
                    if (!symbolStack.empty()) symbolStack.pop();
                }
                
                // 反转子节点顺序（因为栈是后进先出）
                reverse(childrenNodes.begin(), childrenNodes.end());
                nonterminalNode->children = childrenNodes;
                
                // ===== 语义分析：根据产生式填充语义属性 =====
                fillSemanticAttributes(nonterminalNode, prod.original_index, childrenNodes);
                
                // (2)获取当前状态栈顶
                if (statusStack.empty()) 
                {
                    cout << "[PARSER] Error: State stack underflow during reduction" << endl;
                    delete nonterminalNode;
                    break;
                }
                int topState = statusStack.top();
                
                // (3)查询 GOTO[topState, A]
                int gotoIndex = getSymbolIndex(A);
                if (gotoIndex == -1) 
                {
                    cout << "[PARSER] Error: Nonterminal '" << A << "' not found" << endl;
                    delete nonterminalNode;
                    break;
                }
                
                // 检查边界
                if (topState < 0 || topState >= (int)grammar.parseTable.size() || 
                    gotoIndex < 0 || gotoIndex >= (int)grammar.parseTable[topState].size()) {
                    cout << "[PARSER] Error: GOTO index out of bounds. topState=" << topState 
                         << ", gotoIndex=" << gotoIndex 
                         << ", parseTable.size()=" << grammar.parseTable.size()
                         << ", parseTable[" << topState << "].size()=" << grammar.parseTable[topState].size() << endl;
                    delete nonterminalNode;
                    break;
                }
                
                Action gotoAction = grammar.parseTable[topState][gotoIndex];
                
                if (gotoAction.type != MOVE) 
                {
                    cout << "[PARSER] Error: GOTO[" << topState << ", " << A 
                         << "] is not defined (expected shift action)" << endl;
                    delete nonterminalNode;
                    break;
                }
                
                int gotoState = gotoAction.num;
                
                // (4)状态栈压入 GOTO 状态
                statusStack.push(gotoState);
                
                // (5)符号栈压入非终结符 A
                symbolStack.push(A);
                
                // (6)将新创建的节点压入树栈
                treeStack.push(nonterminalNode);
                
                step++;
            }
            else 
            {
                // 错误
                cout << "[PARSER] Error: No valid action for state " << currentState 
                     << ", symbol '" << currentSymbol << "'" << endl;
                // 输出到终端的错误信息
                std::ostringstream errorMsg;
                errorMsg << "[Line " << currentToken.lineNumber << "] Syntactic error.";
                printToConsoleParse(errorMsg.str());
                
                if (parseLog.is_open()) 
                {
                    // TODO 为方便调试先输出的详细一点，而老师文档里给的不像是SLR(1)版本的输出格式，后续要交前再改一下就行
                    parseLog << left << setw(8) << step 
                             << setw(12) << currentState 
                             << setw(20) << topSymbol 
                             << setw(20) << currentSymbol 
                             << setw(15) << "Error: No action" << endl;                    
                }

                if (s_parseResult.is_open())
                {
                    s_parseResult << step << "\t"
                            << topSymbol << "#"
                            << currentSymbol << "\t"
                            << "Error: No action" << endl;
                }

                break;
            }
        }
        
        if (parseLog.is_open())
            parseLog.close();

        if (s_parseResult.is_open())
            s_parseResult.close();
        
        cout << "[PARSER] Total steps: " << step << endl;

        // if (success && parseTree) {
        //     cout << "[DEBUG] Before outputParseTree - checking tree integrity..." << endl;
        //     if (!parseTree->semanticChildren.empty()) {
        //         auto* prog = parseTree;
        //         for (size_t i = 0; i < prog->semanticChildren.size(); i++) {
        //             auto* child = prog->semanticChildren[i];
        //             cout << "[DEBUG] Program child[" << i << "]: ptr=" << (void*)child;
        //             if (child) {
        //                 cout << ", type=" << child->semanticType << ", varName=" << child->varName;
        //                 if (child->semanticType == "FunctionDef" && !child->semanticChildren.empty()) {
        //                     cout << "\n[DEBUG]   FunctionDef semanticChildren.size()=" << child->semanticChildren.size();
        //                     for (size_t j = 0; j < child->semanticChildren.size(); j++) {
        //                         auto* fchild = child->semanticChildren[j];
        //                         cout << "\n[DEBUG]     [" << j << "]: ptr=" << (void*)fchild;
        //                         if (fchild) {
        //                             cout << ", type=" << fchild->semanticType << ", varName=" << fchild->varName;
        //                         }
        //                     }
        //                 }
        //             }
        //             cout << endl;
        //         }
        //     }
        // }
        
        if (success && parseTree)
            outputParseTree();
        
        return success;
    }
};


ParseTreeNode* getParseTree(string inputFilename)
{
    // 确保有一个干净的开始状态
    cleanupLexer();
    parseTree = nullptr;
    
    // ======================== 获取源文件内容 =========================

    char *input = nullptr;

    cout << "[DEBUG] Opening file: " << inputFilename << endl;
    FILE *fp = fopen(inputFilename.c_str(), "rb");
    if (!fp)
    {
        cout << "Cannot open file: " << inputFilename << endl;
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    input = (char *)malloc(size + 1);
    if (input == nullptr) 
    {
        cout << "Error: Memory allocation failed" << endl;
        fclose(fp);
        return NULL;
    }
    
    size_t read_bytes = fread(input, 1, size, fp);
    if (read_bytes != (size_t)size) 
    {
        cout << "Error: Failed to read complete file, expected " << size << " bytes, got " << read_bytes << " bytes" << endl;
        free(input);
        fclose(fp);
        return NULL;
    }
    input[size] = '\0';
    fclose(fp);
    cout << "[DEBUG] File read, size=" << size << endl;


    // ======================== 初始化词法分析器 =========================

    cout << "[DEBUG] Calling initLexer()" << endl;

    size_t last_slash = inputFilename.find_last_of("/\\");
    string filename_only = (last_slash != string::npos) ? inputFilename.substr(last_slash + 1) : inputFilename;
    setTestCase("case/", filename_only);

    initLexer(input);
    
    string pureFilename = "";

    string baseFilename = inputFilename;
    size_t dotPos = baseFilename.find_last_of('.');
    if (dotPos != string::npos)
        baseFilename = baseFilename.substr(0, dotPos);

    size_t slashPos = baseFilename.find_last_of("\\/");
    pureFilename = (slashPos != string::npos) ? baseFilename.substr(slashPos + 1) : baseFilename;


    // ======================== 初始语法分析器 =========================

    cout << "[DEBUG] Creating SLR1Parser" << endl;
    SLR1Parser parser(cout, pureFilename);
    cout << "[DEBUG] SLR1Parser created, calling parse()" << endl;
    bool result = parser.parse();
    
    if (result)
        cout << "[DEBUG] Parsing succeeded" << endl;
    else
        cout << "[DEBUG] Parsing failed" << endl;
    
    cleanupLexer();
    free(input);
    
    cout << "[DEBUG] Completed" << endl;
    return parseTree;
}


#ifndef NO_MAIN
int main(int argc, char *argv[]) 
{

    printToConsoleParse("\n[FRONTEND] Frontend executing...\n\n");
    
    bool fileInput = (argc == 2);
    string inputFilename = fileInput ? argv[1] : "";
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y%m%d_%H%M%S", timeinfo);
    
    string logFilename = "logs/log_" + string(timestamp) + ".txt";
    
    // 保存原始cout以便输出Accept信息到终端
    g_originalCoutBuffer = cout.rdbuf();
    
    // 调试信息重定向至时间戳日志文件
    ofstream debugLog(logFilename, ios::out | ios::trunc);
    if (debugLog.is_open()) 
    {
        char timeStr[100];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        debugLog << "==============================================" << endl;
        debugLog << "Parser Log" << endl;
        debugLog << "Time: " << timeStr << endl;
        if (fileInput)
            debugLog << "Test File: " << inputFilename << endl;
        else
            debugLog << "Test File: <stdin>" << endl;

        debugLog << "==============================================" << endl;
        debugLog << endl;
        
        cout.rdbuf(debugLog.rdbuf());
    }

    cleanupLexer();
    parseTree = nullptr;


    cout << "[DEBUG] main() started, argc=" << argc << endl;
    
    char *input = nullptr;
    
    if (fileInput) 
    {
        cout << "[DEBUG] Opening file: " << argv[1] << endl;
        FILE *fp = fopen(argv[1], "rb");
        if (!fp) 
        {
            cout << "Cannot open file: " << argv[1] << endl;
            return 1;
        }
        
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        input = (char*)malloc(size + 1);
        if (input == nullptr) 
        {
            cout << "Error: Memory allocation failed" << endl;
            fclose(fp);
            return 1;
        }
        
        size_t read_bytes = fread(input, 1, size, fp);
        if (read_bytes != (size_t)size) 
        {
            cout << "Error: Failed to read complete file, expected " << size << " bytes, got " << read_bytes << " bytes" << endl;
            free(input);
            fclose(fp);
            return 1;
        }
        input[size] = '\0';
        fclose(fp);
        cout << "[DEBUG] File read, size=" << size << endl;
    } 
    else {
        size_t buffer_size = 1024;
        size_t content_size = 0;
        input = (char*)malloc(buffer_size);
        
        int ch;
        while ((ch = getchar()) != EOF) 
        {
            if (content_size + 1 >= buffer_size) 
            {
                buffer_size *= 2;
                input = (char*)realloc(input, buffer_size);
            }
            input[content_size++] = (char)ch;
        }
        input[content_size] = '\0';
    }
    
    cout << "[DEBUG] Calling initLexer()" << endl;

    if (fileInput) {
        size_t last_slash = inputFilename.find_last_of("/\\");
        string filename_only = (last_slash != string::npos) ? inputFilename.substr(last_slash + 1) : inputFilename;
        setTestCase("case/", filename_only);
    }
    
    initLexer(input);
    
    string pureFilename = "";
    if (fileInput) 
    {
        string baseFilename = inputFilename;
        size_t dotPos = baseFilename.find_last_of('.');
        if (dotPos != string::npos)
            baseFilename = baseFilename.substr(0, dotPos);
        
        size_t slashPos = baseFilename.find_last_of("\\/");
        pureFilename = (slashPos != string::npos) ? baseFilename.substr(slashPos + 1) : baseFilename;
    }
    
    cout << "[DEBUG] Creating SLR1Parser" << endl;
    SLR1Parser parser(cout, pureFilename);
    cout << "[DEBUG] SLR1Parser created, calling parse()" << endl;
    bool result = parser.parse();
    
    if (!result)
    {
        printToConsoleParse("\n[ERROR] Parse Error\n\n");
        cout << "[DEBUG] Parsing failed" << endl;
        cout << "Error" << endl;
        cleanupLexer();
        free(input);
        return 1;
    }
    else
    {
        printToConsoleParse("\n[PARSER] Lexical and syntax analysis passed!\n");
    }

    SemanticAnalyzer semanticAnalyzer;
    bool semanticCheckPassed = semanticAnalyzer.check(parseTree);
    
    if (!semanticCheckPassed) 
    {
        // 语义错误信息输出到终端
        printToConsoleParse("\n[ERROR] Semantic analysis failed!\n");
        
        // 输出错误列表到终端
        if (g_originalCoutBuffer) 
        {
            streambuf* currentBuffer = cout.rdbuf();
            cout.rdbuf(g_originalCoutBuffer);
            semanticAnalyzer.printErrors();
            cout.rdbuf(currentBuffer);
        } 
        else
            semanticAnalyzer.printErrors();
        
        printToConsoleParse("[ERROR] Compilation aborted due to semantic errors.\n\n");
        return 1;
    }

    printToConsoleParse("\n[SEMANTIC] Semantic analysis passed!\n");


    cout << "[DEBUG] Parsing succeeded" << endl;
    cout << "Accept" << endl;
    printToConsoleParse("\n[FRONTEND] Frontend passed!\n\n\n");


    cleanupLexer();
    free(input);

    return 0;

}
#endif