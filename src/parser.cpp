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
    
    DFA& dfa = grammar.parseDFA;
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
    
    DFAStatus I0(0);
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
        
        const DFAStatus& currentState = dfa.states[currentStateNum];
        
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
                
                DFAStatus newState(targetStateNum);
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







void initGrammar() 
{
    grammar.startSymbol = "Start";

    for (auto &t : tokenTypeToTerminal)
        grammar.terminals.insert(t.second);
    
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
    
    initLexer(input);
    
    if (fileInput) 
    {
        string baseFilename = inputFilename;
        size_t dotPos = baseFilename.find_last_of('.');
        if (dotPos != string::npos)
            baseFilename = baseFilename.substr(0, dotPos);
        
        string outputFilename = baseFilename + ".ref";
        ofstream outputFile(outputFilename);
        
        SLR1Parser parser(outputFile);
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
    
    return 0;
}
