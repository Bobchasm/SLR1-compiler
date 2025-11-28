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




// ==================== 语法分析树 ====================

// 语法树节点类型
enum NodeType 
{
    NODE_TERMINAL,      // 终结符节点（叶子节点）
    NODE_NONTERMINAL    // 非终结符节点（内部节点）
};

// 语法树节点（扩展为带语义属性的AST节点）
struct ParseTreeNode 
{
    NodeType type;           // 节点类型
    string symbol;           // 符号名称（终结符或非终结符）
    string value;            // 终结符的值（对于终结符节点）
    int prodIndex;           // 使用的产生式索引（对于非终结符节点）
    
    // 语义属性字段
    string semanticType;     // 语义节点类型（VarDecl, FunctionDef, BinaryExpr, Assignment 等）
    string varType;          // 变量/函数返回类型（int, float, void）
    string varName;          // 变量名/函数名
    bool isGlobal;           // 是否全局变量
    bool isConst;            // 是否常量
    string initValue;        // 初始化值
    string operatorType;     // 运算符类型（+, -, *, / 等）
    
    vector<ParseTreeNode*> children;  // 子节点列表
    
    ParseTreeNode(NodeType t, const string& sym, const string& val = "", int prod = -1)
        : type(t), symbol(sym), value(val), prodIndex(prod), 
          isGlobal(false), isConst(false) {}
    
    ~ParseTreeNode()
    {
        for (ParseTreeNode* child : children)
            delete child;
    }
    
    bool isLeaf() const 
    {
        return type == NODE_TERMINAL;
    }
    
    // 获取节点文本（用于visitor）
    string getText() const 
    {
        if (type == NODE_TERMINAL)
            return value.empty() ? symbol : value;
        return symbol;
    }
    
    string toSemanticString(int indent = 0, bool isLast = true, const string& prefix = "") const 
    {
        string result;
        
        if (!semanticType.empty()) 
        {
            // 当前节点的连接符
            if (indent > 0)
            {
                result += prefix;
                result += (isLast ? "└── " : "├── ");
            }
            
            result += semanticType;
            
            if (semanticType == "VarDecl") 
            {
                result += ": ";
                if (isGlobal) result += "global ";
                else result += "local ";
                if (isConst) result += "const ";
                result += varType + " " + varName;
                if (!initValue.empty())
                    result += " = " + initValue;
            }
            else if (semanticType == "FunctionDef") 
            {
                result += ": " + varType + " " + varName + "()";
            }
            else if (semanticType == "Assignment") 
            {
                result += ": " + varName + " = ";
                if (!children.empty())
                    result += children[0]->semanticType.empty() ? "" : children[0]->semanticType;
            }
            else if (semanticType == "BinaryExpr") 
            {
                result += ": " + operatorType;
            }
            else if (semanticType == "Variable") 
            {
                result += ": " + varName;
            }
            else if (semanticType == "Number") 
            {
                result += ": " + value;
            }
            else if (semanticType == "ReturnStmt") 
            {
                result += ": ";
                if (!children.empty())
                    result += children[0]->semanticType.empty() ? "" : children[0]->semanticType;
            }
            
            result += "\n";
            
            // 准备子节点的前缀
            string childPrefix = prefix;
            if (indent > 0)
                childPrefix += (isLast ? "    " : "│   ");
            
            // 递归输出子节点（只输出有语义类型的节点）
            vector<ParseTreeNode*> semanticChildren;
            for (const ParseTreeNode* child : children) 
            {
                if (child && !child->semanticType.empty())
                    semanticChildren.push_back(const_cast<ParseTreeNode*>(child));
            }
            
            for (size_t i = 0; i < semanticChildren.size(); i++) 
            {
                bool childIsLast = (i == semanticChildren.size() - 1);
                result += semanticChildren[i]->toSemanticString(indent + 1, childIsLast, childPrefix);
            }
        }
        else 
        {
            // 如果没有语义类型，只递归处理子节点
            for (const ParseTreeNode* child : children) 
            {
                if (child)
                    result += child->toSemanticString(indent, isLast, prefix);
            }
        }
        
        return result;
    }
    
    // 使用Markdown的Mermaid 可视化语法分析树
    string toMermaidMarkdown() const 
    {
        string result = "```mermaid\n";
        result += "flowchart TD\n";
        
        int nodeCounter = 0;
        toMermaidHelper(result, nodeCounter, -1);
        
        result += "```\n";
        return result;
    }
    
private:
    string escapeMermaid(const string& str) const 
    {
        string result;
        for (char c : str) 
        {
            if (isalnum(c) || c == '_')
                result += c;
            else if (c == ' ')
                result += '_';
        }
        return result;
    }
    
    int toMermaidHelper(string& result, int& nodeCounter, int parentId) const 
    {
        int currentId = nodeCounter++;
        
        string nodeLabel;
        string nodeShape;
        
        if (type == NODE_TERMINAL) 
        {
            nodeLabel = escapeMermaid(value.empty() ? symbol : value);
            if (nodeLabel.empty()) nodeLabel = "T" + to_string(currentId);
            nodeShape = "((" + nodeLabel + "))";
        } 
        else 
        {
            nodeLabel = escapeMermaid(symbol);
            if (nodeLabel.empty())
                nodeLabel = "N" + to_string(currentId);
            nodeShape = "[" + nodeLabel + "]";
        }
        
        result += "    N" + to_string(currentId) + nodeShape + "\n";
        
        if (parentId >= 0)
            result += "    N" + to_string(parentId) + " --> N" + to_string(currentId) + "\n";
        
        for (const ParseTreeNode* child : children)
            child->toMermaidHelper(result, nodeCounter, currentId);
        
        return currentId;
    }

public:


};

// 序号用于分析表终结符唯一标识，注意这里与词法分析器那里的标号无关
map<string, pair<string,int> > tokenTypeToTerminal = {
    {"int", {"int",1}}, {"void", {"void",2}}, {"return", {"return",3}}, {"const", {"const",4}}, {"float", {"float",5}}, {"if", {"if",6}}, {"else", {"else",7}}, 
    
    {"+", {"+",8}}, {"-", {"-",9}}, {"*", {"*",10}}, {"/", {"/",11}}, {"%", {"%",12}},
    
    {"=", {"=",13}}, {">", {">",14}}, {"<", {"<",15}}, {"==", {"==",16}}, {"<=", {"<=",17}}, 
    {">=", {">=",18}}, {"!=", {"!=",19}}, {"&&", {"&&",20}}, {"||", {"||",21}}, {"!", {"!",22}},
    
    {"(", {"(",23}}, {")", {")",24}}, {"{", {"{",25}}, {"}", {"}",26}}, {";", {";",27}}, {",", {",",28}},
    
    {"IDN", {"Ident",29}}, {"INT", {"IntConst",30}}, {"FLOAT",{"floatConst",31}}
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
struct ParserDFAStatus
{
    int statusNum;              // 状态编号
    vector<Item> items;         // 项目集
    
    ParserDFAStatus() : statusNum(-1) {}
    ParserDFAStatus(int num) : statusNum(num) {}
    
    bool operator<(const ParserDFAStatus& other) const 
    {
        return items < other.items;
    }
    
    bool operator==(const ParserDFAStatus& other) const 
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

struct ParserDFATransition
{
    int from;           // 源状态编号
    string symbol;      // 转换符号
    int to;             // 目标状态编号
    
    ParserDFATransition(int f, string s, int t) : from(f), symbol(s), to(t) {}
};

struct ParserDFA
{
    vector<ParserDFAStatus> states;           // 项目集族
    int startState;                     // 起始状态编号
    vector<ParserDFATransition> transitions;  // 状态转换
    
    // 转换表：[状态编号][符号] -> 目标状态编号
    map<int, map<string, int>> transitionTable;
    
    ParserDFA() : startState(0) {}
    
    int addState(const ParserDFAStatus& state) 
    {
        states.push_back(state);
        return states.size() - 1;
    }
    
    void addTransition(int from, const string& symbol, int to) 
    {
        transitions.push_back(ParserDFATransition(from, symbol, to));
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
        for (const ParserDFAStatus& state : states) 
        {
            result += state.toString() + "\n";
        }
        
        result += "\nTransitions:\n";
        for (const ParserDFATransition& trans : transitions) 
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
    int type;  // 类型 规约还是移进，值由MOVE和REDUCTION定义
    int num;   // 移进则表示目标状态的序号，规约则表示所用产生式的编号

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

    ParserDFA parseDFA;

    // 分析表，行号为状态序号，列号为终结符/非终结符的唯一编号
    vector<vector<Action>> parseTable;
};

Grammar grammar;

static vector<pair<int, pair<string, vector<string> > > > originalProductions = {
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
    {18, {"varDef_list", {",", "varDef", "varDef_list"}}},
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
    {27, {"funcFParam_list", {",", "funcFParam", "funcFParam_list"}}},
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
    {61, {"funcRParam_list", {",", "funcRParam", "funcRParam_list"}}},
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



void initGrammar();

void buildParseDFA();

void buildAnalysisTable();

void exportAnalysisTable(vector<vector<vector<Action> > > tableWithConflicts);

void exportFirstSets();

void exportFollowSets();