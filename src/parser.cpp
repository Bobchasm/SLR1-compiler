#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <algorithm>
#include <stack>
#include <cstring>

#include "parse.h"
#include "lexer.h"


using namespace std;


string PARSE_ANALYSIS_TABLE_PATH = "process/";

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
    
    // 用于判断项目集是否已经存在
    // map: 项目集内容 -> 状态编号
    map<vector<Item>, int> stateMap;
    stateMap[I0.items] = 0;
    
    // 待处理状态编号队列
    queue<int> workQueue;
    workQueue.push(0);
    
    // 获取所有可能的符号
    set<string> allSymbols = getAllSymbols();
    
    // 2&3.迭代构建状态集
    while (!workQueue.empty()) 
    {
        int currentStateNum = workQueue.front();
        workQueue.pop();
        
        const ParserDFAStatus& currentState = dfa.states[currentStateNum];
        
        // 对每个符号计算 GOTO
        for (const string& symbol : allSymbols) 
        {
            // 计算 GOTO(currentState, symbol)
            vector<Item> gotoItems = gotoSet(currentState.items, symbol);
            
            // 如果 GOTO 结果为空，跳过
            if (gotoItems.empty())
                continue;
            
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
                     << " with " << gotoItems.size() << " items" << endl;
            }
            
            // 添加转换：currentState --[symbol]--> targetState
            dfa.addTransition(currentStateNum, symbol, targetStateNum);
        }
    }
    
    cout << "[PARSER] DFA construction completed:" << endl;
    cout << "[PARSER]   Total states: " << dfa.states.size() << endl;
    cout << "[PARSER]   Total transitions: " << dfa.transitions.size() << endl;
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
    {
        grammar.firstSets[term.first].insert(term.first);
    }
    
    // 对非终结符，迭代计算直到不再变化
    bool changed = true;
    while (changed) 
    {
        changed = false;
        
        // 遍历所有产生式
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
    
    // 初始化分析表（所有动作默认为错误）
    // 注意：我们用一个特殊的结构来存储可能的冲突
    vector<vector<vector<Action>>> tableWithConflicts(stateCount, vector<vector<Action>>(symbolCount));
    
    // SLR(1)分析表构造规则：
    // 对于每个状态 I
    for (int stateNum = 0; stateNum < stateCount; stateNum++) 
    {
        const ParserDFAStatus& state = dfa.states[stateNum];
        
        // 遍历状态中的每个项目
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
                        int symbolIndex = grammar.terminals.at(nextSymbol);
                        tableWithConflicts[stateNum][symbolIndex].push_back(Action(MOVE, nextState));
                    }
                }
                // 如果是非终结符，添加GOTO
                else if (isNonterminal(nextSymbol)) 
                {
                    int nextState = dfa.getTransition(stateNum, nextSymbol);
                    if (nextState != -1) 
                    {
                        int symbolIndex = grammar.nonterminals.at(nextSymbol);
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
                    int eofIndex = grammar.terminals.at(END_MARKER);
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
                            int symbolIndex = it->second;
                            tableWithConflicts[stateNum][symbolIndex].push_back(
                                Action(REDUCTION, item.prodIndex));
                        }
                    }
                }
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
            {
                grammar.parseTable[i][j] = tableWithConflicts[i][j][0];
            }
        }
    }
    
    cout << "[PARSER] Analysis table built: " << stateCount << " states, "
         << symbolCount << " symbols" << endl;
    if (conflictCount > 0)
        cout << "[PARSER] WARNING: Found " << conflictCount << " conflicts" << endl;
    
    // 导出分析表到CSV文件
    #ifdef _WIN32
        system("if not exist process mkdir process");
    #else
        mkdir("process", 0755);
    #endif
    
    ofstream csvFile("process/parse_analysis_table.csv");
    if (!csvFile.is_open()) 
    {
        cerr << "[PARSER] Error: Cannot create parse_analysis_table.csv" << endl;
        return;
    }
    
    // 写入CSV头部
    csvFile << "State";
    
    // 终结符列
    for (const auto& term : grammar.terminals)
        csvFile << "," << term.first;
    
    // 非终结符列  
    for (const auto& nonterm : grammar.nonterminals)
        csvFile << "," << nonterm.first;
    
    csvFile << endl;
    
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
                // 有冲突，用逗号分隔多个动作
                for (size_t k = 0; k < tableWithConflicts[i][j].size(); k++) 
                {
                    if (k > 0) csvFile << ",";
                    
                    const Action& action = tableWithConflicts[i][j][k];
                    if (action.type == ACC)
                        csvFile << "acc";
                    else if (action.type == MOVE)
                        csvFile << "s" << action.num;
                    else if (action.type == REDUCTION)
                        csvFile << "r" << action.num;
                }
            }
            else if (tableWithConflicts[i][j].size() == 1) 
            {
                const Action& action = tableWithConflicts[i][j][0];
                if (action.type == ACC)
                    csvFile << "acc";
                else if (action.type == MOVE)
                    csvFile << "s" << action.num;
                else if (action.type == REDUCTION)
                    csvFile << "r" << action.num;
            }
            // 否则留空
        }
        csvFile << endl;
    }
    
    csvFile.close();
    cout << "[PARSER] Analysis table exported to process/parse_analysis_table.csv" << endl;
}



void initGrammar() 
{
    cout << "[DEBUG] initGrammar() started" << endl;
    grammar.startSymbol = "Start";

    // 添加所有词法终结符
    for (auto &t : tokenTypeToTerminal)
        grammar.terminals.insert(t.second);
    
    // 添加结束标记符号 EOF
    int eofIndex = grammar.terminals.size() + 1;
    grammar.terminals.insert({END_MARKER, eofIndex});
    
    cout << "[DEBUG] Terminals added: " << grammar.terminals.size() << endl;
    
    // 终结符标号按打表的来，非终结符从终结符序号后接着标
    int nonterminal_count = grammar.terminals.size();

    // 拓广文法
    Production exprod;
    exprod.original_index = 0;
    exprod.left = "Start";
    exprod.right = vector<string>{"Program"};
    grammar.productions.push_back(exprod);

    for (auto &p : originalProductions) 
    {
        Production prod;
        prod.original_index = p.first;
        prod.left = p.second.first;
        prod.right = p.second.second;
        grammar.productions.push_back(prod);
        if (grammar.nonterminals.find(prod.left) == grammar.nonterminals.end())
            grammar.nonterminals.insert(pair<string,int>(prod.left, ++nonterminal_count));
    }

    cout << "[DEBUG] Productions: " << grammar.productions.size() << ", Nonterminals: " << grammar.nonterminals.size() << endl;

    buildParseDFA();
    buildAnalysisTable();

    cout << "[DEBUG] initGrammar() completed" << endl;



    
}






class SLR1Parser 
{
private:
    stack<int> statusStack;    // 状态栈 
    stack<string> symbolStack;    // 符号栈
    ostream &output;
    int stepCount;
    
    string getCurrentToken(Token &token) 
    {
        if (!token.valid) 
            return END_MARKER;
        
        string tokenText = token.text;
        string tokenType = token.type;
        
        if (tokenType == "IDN") return "Ident";
        if (tokenType == "INT") return "IntConst";
        if (tokenType == "FLOAT") return "floatConst";
        
        if (tokenType == "KW")
            transform(tokenText.begin(), tokenText.end(), tokenText.begin(), ::tolower);
        
        if (tokenTypeToTerminal.count(tokenText))
            return tokenTypeToTerminal[tokenText].first;

        return tokenText;
    }
    
public:
    SLR1Parser(ostream &out) : output(out), stepCount(1) 
    {
        initGrammar();

        
    }
    
    bool parse() 
    {
        Token currentToken = getNextToken();
        
        

        
        
        return true;
    }
};




int main(int argc, char *argv[]) 
{
    cout << "[DEBUG] main() started, argc=" << argc << endl;
    bool fileInput = (argc == 2);
    string inputFilename = fileInput ? argv[1] : "";
    

    
    if (fileInput) 
    {
        #ifdef _WIN32
            system("if not exist analysis mkdir analysis");
        #else
            mkdir("process", 0755);
        #endif
        


    }
    
    char *input = nullptr;
    
    if (fileInput) 
    {
        cout << "[DEBUG] Opening file: " << argv[1] << endl;
        FILE *fp = fopen(argv[1], "r");
        if (!fp) 
        {
            cerr << "Cannot open file: " << argv[1] << endl;
            return 1;
        }
        
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        input = (char*)malloc(size + 1);
        fread(input, 1, size, fp);
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
    // 临时注释掉 initLexer 以测试 parser 的 DFA 和分析表构建
    initLexer(input);
    cout << "[DEBUG] initLexer() skipped for testing" << endl;
    
    if (fileInput) 
    {
        string baseFilename = inputFilename;
        size_t dotPos = baseFilename.find_last_of('.');
        if (dotPos != string::npos)
            baseFilename = baseFilename.substr(0, dotPos);
        
        string outputFilename = baseFilename + ".ref";
        cout << "[DEBUG] Output file: " << outputFilename << endl;
        ofstream outputFile(outputFilename);
        
        cout << "[DEBUG] Creating SLR1Parser" << endl;
        SLR1Parser parser(outputFile);
        cout << "[DEBUG] SLR1Parser created, calling parse()" << endl;
        parser.parse();
        
        outputFile.close();
    } 
    else 
    {
        SLR1Parser parser(cout);
        parser.parse();
    }
    
    cleanupLexer();
    free(input);
    
    cout << "[DEBUG] main() completed" << endl;
    return 0;
}
