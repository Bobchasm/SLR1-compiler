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
    
    vector<ParseTreeNode*> children;          // 子节点列表（完整的语法树结构）
    vector<ParseTreeNode*> semanticChildren;  // 语义子节点（简化的语义树结构）
    
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
            else if (semanticType == "FuncParam")
            {
                result += ": " + varType + " " + varName;
            }
            else if (semanticType == "Assignment") 
            {
                result += ": " + varName + " = ";
                if (!semanticChildren.empty())
                    result += semanticChildren[0]->semanticType.empty() ? "" : semanticChildren[0]->semanticType;
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
                if (!semanticChildren.empty())
                    result += semanticChildren[0]->semanticType.empty() ? "" : semanticChildren[0]->semanticType;
            }
            else if (semanticType == "IfStmt")
            {
                result += ": ";
                // If语句的子节点包括：条件 + then分支 + else分支
            }
            else if (semanticType == "Condition")
            {
                result += ": ";
            }
            else if (semanticType == "ThenBranch")
            {
                result += ": ";
            }
            else if (semanticType == "ElseBranch")
            {
                result += ": ";
            }
            else if (semanticType == "BinaryExpr")
            {
                // 显示运算符和操作数
                result += ": " + operatorType;
            }
            else if (semanticType == "UnaryExpr")
            {
                result += ": " + operatorType;
            }
            else if (semanticType == "FunctionCall")
            {
                result += ": " + varName + "()";
            }
            
            result += "\n";
            
            // 准备子节点的前缀
            string childPrefix = prefix;
            if (indent > 0)
                childPrefix += (isLast ? "    " : "│   ");
            
            // 递归输出语义子节点
            // 优先使用显式设置的semanticChildren
            if (!semanticChildren.empty())
            {
                for (size_t i = 0; i < semanticChildren.size(); i++) 
                {
                    bool childIsLast = (i == semanticChildren.size() - 1);
                    result += semanticChildren[i]->toSemanticString(indent + 1, childIsLast, childPrefix);
                }
            }
            // 如果semanticChildren为空，且当前节点不是叶子语义节点，才从children过滤
            // 叶子语义节点：Number, Variable (没有语义子节点)
            else if (semanticType != "Number" && semanticType != "Variable")
            {
                // 从children中收集有语义类型的节点
                vector<ParseTreeNode*> filteredChildren;
                for (const ParseTreeNode* child : children) 
                {
                    if (child && !child->semanticType.empty())
                        filteredChildren.push_back(const_cast<ParseTreeNode*>(child));
                }
                
                for (size_t i = 0; i < filteredChildren.size(); i++) 
                {
                    bool childIsLast = (i == filteredChildren.size() - 1);
                    result += filteredChildren[i]->toSemanticString(indent + 1, childIsLast, childPrefix);
                }
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
            else
                result += c;  // 保留其他字符（如括号、运算符等）
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
            nodeLabel = value.empty() ? symbol : value;
            if (nodeLabel.empty()) nodeLabel = "T" + to_string(currentId);
            // 使用引号包裹终结符，避免特殊字符问题
            nodeShape = "((\"" + nodeLabel + "\"))";
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

extern Grammar grammar;


extern ParseTreeNode* parseTree;       // 完整的语法分析树根节点

void initGrammar();

void buildParseDFA();

void buildAnalysisTable();

void exportAnalysisTable(vector<vector<vector<Action> > > tableWithConflicts);

void exportFirstSets();

void exportFollowSets();

ParseTreeNode* getParseTree(string inputFilename);