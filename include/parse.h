#include <vector>
#include <map>
#include <set>
#include <string>

#define ACC 0
#define MOVE 1
#define REDUCTION 2


using namespace std;

const string EPSILON = "$";
const string END_MARKER = "EOF";

// 序号用于分析表终结符唯一标识，注意这里与词法分析器那里的标号无关
map<string, pair<string,int> > tokenTypeToTerminal = {
    {"int", {"int",1}}, {"void", {"void",2}}, {"return", {"return",3}}, {"const", {"const",4}}, {"float", {"float",5}}, {"if", {"if",6}}, {"else", {"else",7}}, 
    
    {"+", {"+",8}}, {"-", {"-",9}}, {"*", {"*",10}}, {"/", {"/",11}}, {"%", {"%",12}},
    
    {"=", {"=",13}}, {">", {">",14}}, {"<", {"<",15}}, {"==", {"==",16}}, {"<=", {"<=",17}}, 
    {">=", {">=",18}}, {"!=", {"!=",19}}, {"&&", {"&&",20}}, {"||", {"||",21}}, {"!", {"!",22}},
    
    {"(", {"(",23}}, {")", {")",24}}, {"{", {"{",25}}, {"}", {"}",26}}, {";", {";",27}}, {",", {",",28}},
    
    {"IDN", {"IDN",29}}, {"INT", {"INT",30}}, {"FLOAT",{"FLOAT",31}}
};

// 项目
struct Item
{
    string left;
    vector<string> right;
    int flag;   // 那个项目的点所在vector的位置，如在index为0元素前则其值为0
    int prodIndex;  // 对应的产生式索引（用于规约时确定使用哪个产生式）
    
    Item() : flag(0), prodIndex(-1) {}
    Item(string l, vector<string> r, int f, int pi = -1) 
        : left(l), right(r), flag(f), prodIndex(pi) {}
    
    // 比较运算符（用于set和map）
    bool operator<(const Item& other) const 
    {
        if (left != other.left) return left < other.left;
        if (right != other.right) return right < other.right;
        if (flag != other.flag) return flag < other.flag;
        return prodIndex < other.prodIndex;
    }
    
    bool operator==(const Item& other) const 
    {
        return left == other.left && right == other.right && 
               flag == other.flag && prodIndex == other.prodIndex;
    }
    
    // 判断点是否在末尾（可归约项目）
    bool isComplete() const 
    {
        return flag >= (int)right.size() || 
               (right.size() == 1 && right[0] == EPSILON);
    }
    
    // 获取点后面的符号（如果有）
    string getNextSymbol() const 
    {
        if (isComplete()) return "";
        return right[flag];
    }
    
    string toString() const 
    {
        string result = left + " -> ";
        
        if (right.empty() || (right.size() == 1 && right[0] == EPSILON))
        {
            result += "." + EPSILON;
            return result;
        }
        
        for (size_t i = 0; i < right.size(); i++) 
        {
            if (i == (size_t)flag)
                result += ".";
            
            if (i > 0)
                result += " ";
            result += right[i];
        }
        
        if ((size_t)flag == right.size())
            result += ".";
        
        return result;
    }
};

// 项目集(DFA的单个状态)
struct DFAStatus
{
    int statusNum;              // 状态编号
    vector<Item> items;         // 项目集
    
    DFAStatus() : statusNum(-1) {}
    DFAStatus(int num) : statusNum(num) {}
    
    bool operator<(const DFAStatus& other) const 
    {
        return items < other.items;
    }
    
    bool operator==(const DFAStatus& other) const 
    {
        return items == other.items;
    }
    
    string toString() const 
    {
        string result = "I" + to_string(statusNum) + ":\n";
        for (const Item& item : items) 
            result += "  " + item.toString() + "\n";
        return result;
    }
};

struct DFATransition
{
    int from;           // 源状态编号
    string symbol;      // 转换符号
    int to;             // 目标状态编号
    
    DFATransition(int f, string s, int t) : from(f), symbol(s), to(t) {}
};

struct DFA
{
    vector<DFAStatus> states;           // 项目集族
    int startState;                     // 起始状态编号
    vector<DFATransition> transitions;  // 状态转换
    
    // 转换表：[状态编号][符号] -> 目标状态编号
    map<int, map<string, int>> transitionTable;
    
    DFA() : startState(0) {}
    
    int addState(const DFAStatus& state) 
    {
        states.push_back(state);
        return states.size() - 1;
    }
    
    void addTransition(int from, const string& symbol, int to) 
    {
        transitions.push_back(DFATransition(from, symbol, to));
        transitionTable[from][symbol] = to;
    }
    
    int getTransition(int from, const string& symbol) const 
    {
        auto it1 = transitionTable.find(from);
        if (it1 == transitionTable.end()) return -1;
        
        auto it2 = it1->second.find(symbol);
        if (it2 == it1->second.end()) return -1;
        
        return it2->second;
    }
    
    string toString() const 
    {
        string result = "DFA States:\n";
        for (const DFAStatus& state : states) 
        {
            result += state.toString() + "\n";
        }
        
        result += "\nTransitions:\n";
        for (const DFATransition& trans : transitions) 
        {
            result += "I" + to_string(trans.from) + " --[" + 
                     trans.symbol + "]--> I" + to_string(trans.to) + "\n";
        }
        
        return result;
    }
};

// 指分析表里每个格里的一个动作
struct Action
{
    int type;
    int num;

    Action(int typein, int numin) : type(typein), num(numin) {}
};

struct Production 
{
    string left;
    vector<string> right;
    int original_index;
    
    string toString() const 
    {
        string result = left + " -> ";
        if (right.empty() || (right.size() == 1 && right[0] == EPSILON))
            result += EPSILON;
        else 
        {
            for (size_t i = 0; i < right.size(); i++) 
            {
                if (i > 0) 
                    result += " ";
                result += right[i];
            }
        }
        return result;
    }
};

struct Grammar 
{
    // 产生式
    vector<Production> productions;
    // 开始符号
    string startSymbol;

    // 所有非终结符
    map<string,int> nonterminals;
    // 所有终结符
    map<string,int> terminals;
    
    map<string, set<string>> firstSets;
    map<string, set<string>> followSets;

    DFA parseDFA;

    // 分析表，行号为状态序号，列号为终结符/非终结符的唯一编号
    vector<vector<Action>> parseTable;
};

Grammar grammar;

vector<pair<int, pair<string, vector<string> > > > originalProductions = {
    // 1. Program -> compUnit
    {1, {"Program", {"compUnit"}}},
    
    // 2. compUnit -> (decl | funcDef)* EOF
    {2, {"compUnit", {"compUnit_list", "EOF"}}},
    {3, {"compUnit_list", {"compUnit_list", "compUnit_item"}}},
    {4, {"compUnit_list", {EPSILON}}},
    {5, {"compUnit_item", {"decl"}}},
    {6, {"compUnit_item", {"funcDef"}}},
    
    // 3. decl -> constDecl | varDecl
    {7, {"decl", {"constDecl"}}},
    {8, {"decl", {"varDecl"}}},
    
    // 4. constDecl -> 'const' bType constDef (',' constDef)* ';'
    {9, {"constDecl", {"const", "bType", "constDef", "constDef_list", ";"}}},
    {10, {"constDef_list", {",", "constDef", "constDef_list"}}},
    {11, {"constDef_list", {EPSILON}}},
    
    // 5. bType -> 'int' | 'float'
    {12, {"bType", {"int"}}},
    {13, {"bType", {"float"}}},
    
    // 6. constDef -> Ident '=' constInitVal
    {14, {"constDef", {"Ident", "=", "constInitVal"}}},
    
    // 7. constInitVal -> constExp
    {15, {"constInitVal", {"constExp"}}},
    
    // 8. varDecl -> bType varDef (',' varDef)* ';'
    {16, {"varDecl", {"bType", "varDef", "varDef_list", ";"}}},
    {17, {"varDef_list", {",", "varDef", "varDef_list"}}},
    {18, {"varDef_list", {EPSILON}}},
    
    // 9. varDef -> Ident | Ident '=' initVal
    {19, {"varDef", {"Ident"}}},
    {20, {"varDef", {"Ident", "=", "initVal"}}},
    
    // 10. initVal -> exp
    {21, {"initVal", {"exp"}}},
    
    // 11. funcDef -> funcType Ident '(' (funcFParams)? ')' block
    {22, {"funcDef", {"funcType", "Ident", "(", "funcFParams_opt", ")", "block"}}},
    {23, {"funcFParams_opt", {"funcFParams"}}},
    {24, {"funcFParams_opt", {EPSILON}}},
    
    // 12. funcType -> 'void' | 'int'
    {25, {"funcType", {"void"}}},
    {26, {"funcType", {"int"}}},
    
    // 13. funcFParams -> funcFParam (',' funcFParam)*
    {27, {"funcFParams", {"funcFParam", "funcFParam_list"}}},
    {28, {"funcFParam_list", {",", "funcFParam", "funcFParam_list"}}},
    {29, {"funcFParam_list", {EPSILON}}},
    
    // 14. funcFParam -> bType Ident
    {30, {"funcFParam", {"bType", "Ident"}}},
    
    // 15. block -> '{' (blockItem)* '}'
    {31, {"block", {"{", "blockItem_list", "}"}}},
    {32, {"blockItem_list", {"blockItem_list", "blockItem"}}},
    {33, {"blockItem_list", {EPSILON}}},
    
    // 16. blockItem -> decl | stmt
    {34, {"blockItem", {"decl"}}},
    {35, {"blockItem", {"stmt"}}},
    
    // 17. stmt -> lVal '=' exp ';' | (exp)? ';' | block | 'if' '(' cond ')' stmt ('else' stmt)? | 'return' (exp)? ';'
    {36, {"stmt", {"lVal", "=", "exp", ";"}}},
    {37, {"stmt", {"exp_opt", ";"}}},
    {38, {"stmt", {"block"}}},
    {39, {"stmt", {"if", "(", "cond", ")", "stmt", "else_opt"}}},
    {40, {"stmt", {"return", "exp_opt", ";"}}},
    {41, {"exp_opt", {"exp"}}},
    {42, {"exp_opt", {EPSILON}}},
    {43, {"else_opt", {"else", "stmt"}}},
    {44, {"else_opt", {EPSILON}}},
    
    // 18. exp -> addExp
    {45, {"exp", {"addExp"}}},
    
    // 19. cond -> lOrExp
    {46, {"cond", {"lOrExp"}}},
    
    // 20. lVal -> Ident
    {47, {"lVal", {"Ident"}}},
    
    // 21. primaryExp -> '(' exp ')' | lVal | number
    {48, {"primaryExp", {"(", "exp", ")"}}},
    {49, {"primaryExp", {"lVal"}}},
    {50, {"primaryExp", {"number"}}},
    
    // 22. number -> IntConst | floatConst
    {51, {"number", {"IntConst"}}},
    {52, {"number", {"floatConst"}}},
    
    // 23. unaryExp -> primaryExp | Ident '(' (funcRParams)? ')' | unaryOp unaryExp
    {53, {"unaryExp", {"primaryExp"}}},
    {54, {"unaryExp", {"Ident", "(", "funcRParams_opt", ")"}}},
    {55, {"unaryExp", {"unaryOp", "unaryExp"}}},
    {56, {"funcRParams_opt", {"funcRParams"}}},
    {57, {"funcRParams_opt", {EPSILON}}},
    
    // 24. unaryOp -> '+' | '-' | '!'
    {58, {"unaryOp", {"+"}}},
    {59, {"unaryOp", {"-"}}},
    {60, {"unaryOp", {"!"}}},
    
    // 25. funcRParams -> funcRParam (',' funcRParam)*
    {61, {"funcRParams", {"funcRParam", "funcRParam_list"}}},
    {62, {"funcRParam_list", {",", "funcRParam", "funcRParam_list"}}},
    {63, {"funcRParam_list", {EPSILON}}},
    
    // 26. funcRParam -> exp
    {64, {"funcRParam", {"exp"}}},
    
    // 27. mulExp -> unaryExp | mulExp ('*' | '/' | '%') unaryExp
    {65, {"mulExp", {"unaryExp"}}},
    {66, {"mulExp", {"mulExp", "*", "unaryExp"}}},
    {67, {"mulExp", {"mulExp", "/", "unaryExp"}}},
    {68, {"mulExp", {"mulExp", "%", "unaryExp"}}},
    
    // 28. addExp -> mulExp | addExp ('+' | '-') mulExp
    {69, {"addExp", {"mulExp"}}},
    {70, {"addExp", {"addExp", "+", "mulExp"}}},
    {71, {"addExp", {"addExp", "-", "mulExp"}}},
    
    // 29. relExp -> addExp | relExp ('<' | '>' | '<=' | '>=') addExp
    {72, {"relExp", {"addExp"}}},
    {73, {"relExp", {"relExp", "<", "addExp"}}},
    {74, {"relExp", {"relExp", ">", "addExp"}}},
    {75, {"relExp", {"relExp", "<=", "addExp"}}},
    {76, {"relExp", {"relExp", ">=", "addExp"}}},
    
    // 30. eqExp -> relExp | eqExp ('==' | '!=') relExp
    {77, {"eqExp", {"relExp"}}},
    {78, {"eqExp", {"eqExp", "==", "relExp"}}},
    {79, {"eqExp", {"eqExp", "!=", "relExp"}}},
    
    // 31. lAndExp -> eqExp | lAndExp '&&' eqExp
    {80, {"lAndExp", {"eqExp"}}},
    {81, {"lAndExp", {"lAndExp", "&&", "eqExp"}}},
    
    // 32. lOrExp -> lAndExp | lOrExp '||' lAndExp
    {82, {"lOrExp", {"lAndExp"}}},
    {83, {"lOrExp", {"lOrExp", "||", "lAndExp"}}},
    
    // 33. constExp -> addExp
    {84, {"constExp", {"addExp"}}}
};



void initGrammar();

void buildParseDFA();