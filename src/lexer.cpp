#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <tuple>
#include <cstring>

#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

using namespace std;

const char EPSILON = '\0';

const string TRANSITION_MATRIX_PATH = "process/";
const char* TEST_CASE_PATH = "case/";
const string S_LEXER_RESULT_EXT = "_lexer_s.txt";

map<string, pair<string, int>> keywords = {
    {"int", {"KW", 1}}, {"void", {"KW", 2}}, {"return", {"KW", 3}}, {"const", {"KW", 4}}, {"float", {"KW", 6}}, {"if", {"KW", 7}}, {"else", {"KW", 8}}};

map<string, pair<string, int>> operators = {
    {"+", {"OP", 9}}, {"-", {"OP", 10}}, {"*", {"OP", 11}}, {"/", {"OP", 12}}, {"%", {"OP", 13}}, {"=", {"OP", 14}}, {">", {"OP", 15}}, {"<", {"OP", 16}}, {"==", {"OP", 17}}, {"<=", {"OP", 18}}, {">=", {"OP", 19}}, {"!=", {"OP", 20}}, {"&&", {"OP", 21}}, {"||", {"OP", 22}}, {"!", {"OP", 29}}};

map<string, pair<string, int>> separators = {
    {"(", {"SE", 23}}, {")", {"SE", 24}}, {"{", {"SE", 25}}, {"}", {"SE", 26}}, {";", {"SE", 27}}, {",", {"SE", 28}}};

struct NFATransition
{
    int from;
    char symbol;
    int to;
};

struct NFA
{
    int startState;
    set<int> acceptStates;
    vector<NFATransition> transitions;
    int maxState;

    NFA() : startState(0), maxState(0) {}
};

struct DFATransition
{
    int from;
    char symbol;
    int to;
};

struct DFA
{
    int startState;
    set<int> acceptStates;
    vector<DFATransition> transitions;
    map<int, string> tokenType;
    map<int, string> tokenValue;
    int maxState;

    DFA() : startState(0), maxState(0) {}
};


void exportDFATransitionMatrix(const DFA &dfa, const string &filename);


// ==================== 正则表达式 -> NFA ====================

/**
 * 单个字符的NFA
 */
NFA createCharNFA(char c)
{
    NFA nfa;
    nfa.startState = 0;
    nfa.maxState = 1;
    nfa.acceptStates.insert(1);
    nfa.transitions.push_back({0, c, 1});
    return nfa;
}

/**
 * epsilon转移的NFA
 */
NFA createEpsilonNFA()
{
    NFA nfa;
    nfa.startState = 0;
    nfa.maxState = 1;
    nfa.acceptStates.insert(1);
    nfa.transitions.push_back({0, EPSILON, 1});
    return nfa;
}

/**
 * NFA连接
 */
NFA concatenateNFA(const NFA &nfa1, const NFA &nfa2)
{
    NFA result;
    int offset = nfa1.maxState + 1;

    result.startState = nfa1.startState;

    for (const auto &trans : nfa1.transitions)
        result.transitions.push_back(trans);

    for (int accept : nfa1.acceptStates)
        result.transitions.push_back({accept, EPSILON, nfa2.startState + offset});

    for (const auto &trans : nfa2.transitions)
        result.transitions.push_back({trans.from + offset, trans.symbol, trans.to + offset});

    for (int accept : nfa2.acceptStates)
        result.acceptStates.insert(accept + offset);

    result.maxState = nfa2.maxState + offset;
    return result;
}

/**
 * NFA选择
 */
NFA unionNFA(const NFA &nfa1, const NFA &nfa2)
{
    NFA result;
    int offset1 = 1;
    int offset2 = nfa1.maxState + 2;

    result.startState = 0;

    result.transitions.push_back({0, EPSILON, nfa1.startState + offset1});
    result.transitions.push_back({0, EPSILON, nfa2.startState + offset2});

    for (const auto &trans : nfa1.transitions)
        result.transitions.push_back({trans.from + offset1, trans.symbol, trans.to + offset1});

    for (const auto &trans : nfa2.transitions)
        result.transitions.push_back({trans.from + offset2, trans.symbol, trans.to + offset2});

    int newAccept = nfa2.maxState + offset2 + 1;
    result.maxState = newAccept;

    for (int accept : nfa1.acceptStates)
        result.transitions.push_back({accept + offset1, EPSILON, newAccept});

    for (int accept : nfa2.acceptStates)
        result.transitions.push_back({accept + offset2, EPSILON, newAccept});

    result.acceptStates.insert(newAccept);
    return result;
}

/**
 * NFA闭包
 */
NFA kleeneStarNFA(const NFA &nfa)
{
    NFA result;
    int offset = 1;

    result.startState = 0;
    result.transitions.push_back({0, EPSILON, nfa.startState + offset});

    for (const auto &trans : nfa.transitions)
        result.transitions.push_back({trans.from + offset, trans.symbol, trans.to + offset});

    int newAccept = nfa.maxState + offset + 1;
    result.maxState = newAccept;

    for (int accept : nfa.acceptStates)
        result.transitions.push_back({accept + offset, EPSILON, newAccept});

    for (int accept : nfa.acceptStates)
        result.transitions.push_back({accept + offset, EPSILON, nfa.startState + offset});

    result.transitions.push_back({0, EPSILON, newAccept});
    result.acceptStates.insert(newAccept);
    return result;
}

/**
 * NFA正闭包
 */
NFA kleenePlusNFA(const NFA &nfa)
{
    return concatenateNFA(nfa, kleeneStarNFA(nfa));
}

/**
 * 字符类NFA
 */
NFA createCharClassNFA(const string &chars)
{
    NFA result;
    if (chars.empty())
        return createEpsilonNFA();

    result = createCharNFA(chars[0]);

    for (size_t i = 1; i < chars.size(); i++)
        result = unionNFA(result, createCharNFA(chars[i]));

    return result;
}

/**
 * 关键字NFA（不区分大小写）
 */
NFA buildKeywordNFA(const string &keyword, const string &tokenType, int tokenCode)
{
    NFA result;
    int state = 0;
    result.startState = 0;

    for (size_t i = 0; i < keyword.length(); i++)
    {
        char c = tolower(keyword[i]);
        result.transitions.push_back({state, c, state + 1});
        if (isalpha(c))
            result.transitions.push_back({state, (char)toupper(c), state + 1});
        state++;
    }

    result.maxState = state;
    result.acceptStates.insert(state);
    return result;
}

/**
 * 标识符NFA: [a-zA-Z_][a-zA-Z0-9_]*
 */
NFA buildIdentifierNFA()
{
    NFA firstChar = createCharClassNFA("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");

    NFA restChars = createCharClassNFA("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
    NFA restCharsStar = kleeneStarNFA(restChars);

    return concatenateNFA(firstChar, restCharsStar);
}

/**
 * 整数NFA: [0-9]+
 */
NFA buildIntegerNFA()
{
    NFA digit = createCharClassNFA("0123456789");
    return kleenePlusNFA(digit);
}

/**
 * 浮点数NFA: [0-9]+\.[0-9]+
 */
NFA buildFloatNFA()
{
    NFA digit = createCharClassNFA("0123456789");
    NFA digits = kleenePlusNFA(digit);
    NFA dot = createCharNFA('.');
    NFA digits2 = kleenePlusNFA(digit);
    return concatenateNFA(concatenateNFA(digits, dot), digits2);
}

/**
 * 运算符NFA
 */
vector<NFA> buildOperatorNFAs()
{
    vector<NFA> nfas;

    vector<string> longOps = {"==", "<=", ">=", "!=", "&&", "||"};
    for (const string &op : longOps)
    {
        NFA nfa;
        nfa.startState = 0;
        nfa.transitions.push_back({0, op[0], 1});
        nfa.transitions.push_back({1, op[1], 2});
        nfa.maxState = 2;
        nfa.acceptStates.insert(2);
        nfas.push_back(nfa);
    }

    vector<string> singleOps = {"+", "-", "*", "/", "%", "=", ">", "<", "!"};
    for (const string &op : singleOps)
        nfas.push_back(createCharNFA(op[0]));

    return nfas;
}

/**
 * 界符NFA
 */
vector<NFA> buildSeparatorNFAs()
{
    vector<NFA> nfas;
    vector<string> seps = {"(", ")", "{", "}", ";", ","};
    for (const string &sep : seps)
        nfas.push_back(createCharNFA(sep[0]));

    return nfas;
}

// ==================== NFA -> DFA转换 ====================

/**
 * 计算epsilon闭包
 */
set<int> epsilonClosure(const NFA &nfa, const set<int> &states)
{
    set<int> closure = states;
    queue<int> q;

    for (int state : states)
        q.push(state);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        for (const auto &trans : nfa.transitions)
        {
            if (trans.from == current && trans.symbol == EPSILON)
            {
                if (closure.find(trans.to) == closure.end())
                {
                    closure.insert(trans.to);
                    q.push(trans.to);
                }
            }
        }
    }
    return closure;
}

/**
 * 计算move
 */
set<int> move(const NFA &nfa, const set<int> &states, char c)
{
    set<int> result;

    for (int state : states)
    {
        for (const auto &trans : nfa.transitions)
        {
            if (trans.from == state && trans.symbol == c)
                result.insert(trans.to);
        }
    }

    return result;
}

/**
 * 合并多个NFA
 */
struct CombinedNFA
{
    vector<NFA> nfas;
    map<int, pair<string, string>> acceptMap;
    int startState;
    set<int> allStates;
    vector<NFATransition> allTransitions;
    CombinedNFA() : startState(0) {}
};

/**
 * 合并NFA
 */
CombinedNFA combineNFAs(const vector<NFA> &nfas, const vector<pair<string, string>> &tokenInfos)
{
    CombinedNFA combined;
    combined.nfas = nfas;
    combined.startState = 0;

    int stateOffset = 1;
    map<int, int> nfaStateOffset;

    for (size_t i = 0; i < nfas.size(); i++)
    {
        nfaStateOffset[i] = stateOffset;
        combined.allTransitions.push_back({0, EPSILON, stateOffset});

        for (const auto &trans : nfas[i].transitions)
        {
            combined.allTransitions.push_back({trans.from + stateOffset,
                                               trans.symbol,
                                               trans.to + stateOffset});
        }

        for (int accept : nfas[i].acceptStates)
        {
            int globalAccept = accept + stateOffset;
            combined.acceptMap[globalAccept] = tokenInfos[i];
        }
        stateOffset += nfas[i].maxState + 1;
    }
    return combined;
}

/**
 * NFA -> DFA转换
 */
DFA nfaToDFA(const CombinedNFA &combined)
{
    DFA dfa;

    set<char> alphabet;
    for (const auto &trans : combined.allTransitions)
    {
        if (trans.symbol != EPSILON)
            alphabet.insert(trans.symbol);
    }

    set<int> startSet;
    startSet.insert(combined.startState);
    set<int> startClosure = epsilonClosure(combined.nfas[0], startSet);

    startClosure.clear();
    startClosure.insert(0);
    for (const auto &trans : combined.allTransitions)
    {
        if (trans.from == 0 && trans.symbol == EPSILON)
        {
            startClosure.insert(trans.to);
            queue<int> q;
            q.push(trans.to);
            while (!q.empty())
            {
                int s = q.front();
                q.pop();
                for (const auto &t : combined.allTransitions)
                {
                    if (t.from == s && t.symbol == EPSILON && startClosure.find(t.to) == startClosure.end())
                    {
                        startClosure.insert(t.to);
                        q.push(t.to);
                    }
                }
            }
        }
    }

    map<set<int>, int> stateSetToDFAState;
    int dfaStateCounter = 0;

    stateSetToDFAState[startClosure] = dfaStateCounter;
    dfa.startState = dfaStateCounter++;

    int bestAcceptState = -1;
    for (int state : startClosure)
    {
        if (combined.acceptMap.find(state) != combined.acceptMap.end())
        {
            if (bestAcceptState == -1 || state < bestAcceptState)
                bestAcceptState = state;
        }
    }
    if (bestAcceptState != -1)
    {
        dfa.acceptStates.insert(dfa.startState);
        auto it = combined.acceptMap.find(bestAcceptState);
        if (it != combined.acceptMap.end())
        {
            dfa.tokenType[dfa.startState] = it->second.first;
            dfa.tokenValue[dfa.startState] = it->second.second;
        }
    }

    queue<set<int>> workQueue;
    workQueue.push(startClosure);

    while (!workQueue.empty())
    {
        set<int> currentSet = workQueue.front();
        workQueue.pop();
        int currentDFAState = stateSetToDFAState[currentSet];

        for (char c : alphabet)
        {
            set<int> nextSet;
            for (int state : currentSet)
            {
                for (const auto &trans : combined.allTransitions)
                {
                    if (trans.from == state && trans.symbol == c)
                        nextSet.insert(trans.to);
                }
            }

            if (nextSet.empty())
                continue;

            set<int> nextClosure = nextSet;
            queue<int> q;
            for (int s : nextSet)
                q.push(s);

            while (!q.empty())
            {
                int s = q.front();
                q.pop();
                for (const auto &trans : combined.allTransitions)
                {
                    if (trans.from == s && trans.symbol == EPSILON)
                    {
                        if (nextClosure.find(trans.to) == nextClosure.end())
                        {
                            nextClosure.insert(trans.to);
                            q.push(trans.to);
                        }
                    }
                }
            }

            if (stateSetToDFAState.find(nextClosure) == stateSetToDFAState.end())
            {
                int newDFAState = dfaStateCounter++;
                stateSetToDFAState[nextClosure] = newDFAState;

                int bestAcceptState = -1;
                for (int state : nextClosure)
                {
                    if (combined.acceptMap.find(state) != combined.acceptMap.end())
                    {
                        if (bestAcceptState == -1 || state < bestAcceptState)
                            bestAcceptState = state;
                    }
                }
                if (bestAcceptState != -1)
                {
                    dfa.acceptStates.insert(newDFAState);
                    auto it = combined.acceptMap.find(bestAcceptState);
                    if (it != combined.acceptMap.end())
                    {
                        dfa.tokenType[newDFAState] = it->second.first;
                        dfa.tokenValue[newDFAState] = it->second.second;
                    }
                }

                workQueue.push(nextClosure);
            }

            int nextDFAState = stateSetToDFAState[nextClosure];
            dfa.transitions.push_back({currentDFAState, c, nextDFAState});
        }
    }

    dfa.maxState = dfaStateCounter - 1;
    return dfa;
}

// ==================== DFA最小化 ====================

/**
 * 将DFA的状态划分为等价类，合并等价状态
 */
DFA minimizeDFA(const DFA &dfa)
{
    DFA minDFA;

    if (dfa.maxState < 0)
        return minDFA;

    set<int> allStates;
    for (int i = 0; i <= dfa.maxState; i++)
        allStates.insert(i);

    set<char> alphabet;
    for (const auto &trans : dfa.transitions)
        alphabet.insert(trans.symbol);

    map<int, map<char, int>> transitionTable;
    for (const auto &trans : dfa.transitions)
        transitionTable[trans.from][trans.symbol] = trans.to;

    vector<set<int>> partition;
    map<string, set<int>> acceptGroups;

    for (int state : allStates)
    {
        if (dfa.acceptStates.find(state) != dfa.acceptStates.end())
        {
            auto typeIt = dfa.tokenType.find(state);
            auto valueIt = dfa.tokenValue.find(state);
            string typeStr = (typeIt != dfa.tokenType.end()) ? typeIt->second : "";
            string valueStr = (valueIt != dfa.tokenValue.end()) ? valueIt->second : "";
            string key = typeStr + "," + valueStr;
            acceptGroups[key].insert(state);
        }
    }

    set<int> nonAcceptSet;
    for (int state : allStates)
    {
        if (dfa.acceptStates.find(state) == dfa.acceptStates.end())
            nonAcceptSet.insert(state);
    }
    if (!nonAcceptSet.empty())
        partition.push_back(nonAcceptSet);

    for (const auto &group : acceptGroups)
        partition.push_back(group.second);

    if (partition.size() <= 1)
        return dfa;

    bool changed = true;
    while (changed)
    {
        changed = false;
        vector<set<int>> newPartition;

        for (const auto &group : partition)
        {
            map<string, set<int>> subgroups;

            for (int state : group)
            {
                string key = "";
                for (char c : alphabet)
                {
                    int nextState = -1;
                    if (transitionTable[state].find(c) != transitionTable[state].end())
                        nextState = transitionTable[state][c];

                    int groupIndex = -1;
                    for (size_t i = 0; i < partition.size(); i++)
                    {
                        if (partition[i].find(nextState) != partition[i].end())
                        {
                            groupIndex = i;
                            break;
                        }
                    }
                    key += to_string(groupIndex) + ",";
                }

                subgroups[key].insert(state);
            }

            if (subgroups.size() > 1)
                changed = true;

            for (const auto &subgroup : subgroups)
                newPartition.push_back(subgroup.second);
        }

        partition = newPartition;
    }

    map<int, int> stateToGroup;
    for (size_t i = 0; i < partition.size(); i++)
        for (int state : partition[i])
            stateToGroup[state] = i;

    minDFA.startState = stateToGroup[dfa.startState];
    minDFA.maxState = partition.size() - 1;

    for (size_t i = 0; i < partition.size(); i++)
    {
        bool isAccept = false;
        string tokenType, tokenValue;
        for (int state : partition[i])
        {
            if (dfa.acceptStates.find(state) != dfa.acceptStates.end())
            {
                isAccept = true;
                auto typeIt = dfa.tokenType.find(state);
                auto valueIt = dfa.tokenValue.find(state);
                if (typeIt != dfa.tokenType.end())
                    tokenType = typeIt->second;
                if (valueIt != dfa.tokenValue.end())
                    tokenValue = valueIt->second;
                break;
            }
        }
        if (isAccept)
        {
            minDFA.acceptStates.insert(i);
            minDFA.tokenType[i] = tokenType;
            minDFA.tokenValue[i] = tokenValue;
        }
    }

    set<pair<int, char>> addedTransitions;
    for (const auto &trans : dfa.transitions)
    {
        int fromGroup = stateToGroup[trans.from];
        int toGroup = stateToGroup[trans.to];
        pair<int, char> key = {fromGroup, trans.symbol};

        if (addedTransitions.find(key) == addedTransitions.end())
        {
            minDFA.transitions.push_back({fromGroup, trans.symbol, toGroup});
            addedTransitions.insert(key);
        }
    }

    return minDFA;
}

// ==================== 词法分析器 ====================

/**
 * 词法分析
 */
class Lexer
{
private:
    DFA dfa;
    string input;
    size_t pos;
    int line;
    int column;
    bool hasError;

    map<int, map<char, int>> transitionTable;

public:
    Lexer(const DFA &dfa, const string &input)
        : dfa(dfa), input(input), pos(0), line(1), column(1)
    {
        for (const auto &trans : dfa.transitions)
            transitionTable[trans.from][trans.symbol] = trans.to;
        hasError = false;
    }
    
    ~Lexer() 
    {
        pos = 0;
        line = 1;
        column = 1;
        transitionTable.clear();
    }

    void setHasErrorOption()
    {
        hasError = true;
    }

    bool getHasErrorOption()
    {
        return hasError;
    }

    tuple<string, string, string, int, bool> nextToken()
    {
        // 跳过空白字符和注释
        while (pos < input.length())
        {
            // 跳过空白字符
            if (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\r')
            {
                column++;
                pos++;
                continue;
            }
            else if (input[pos] == '\n')
            {
                line++;
                column = 1;
                pos++;
                continue;
            }
            
            // 处理注释
            if (input[pos] == '/' && pos + 1 < input.length())
            {
                // 单行注释 //
                if (input[pos + 1] == '/')
                {
                    pos += 2;
                    column += 2;
                    // 跳过直到行尾
                    while (pos < input.length() && input[pos] != '\n')
                    {
                        pos++;
                        column++;
                    }
                    // 跳过换行符
                    if (pos < input.length() && input[pos] == '\n')
                    {
                        pos++;
                        line++;
                        column = 1;
                    }
                    continue;
                }
                // 多行注释 /* */
                else if (input[pos + 1] == '*')
                {
                    pos += 2;
                    column += 2;
                    // 寻找注释结束 */
                    while (pos + 1 < input.length())
                    {
                        if (input[pos] == '*' && input[pos + 1] == '/')
                        {
                            pos += 2;
                            column += 2;
                            break;
                        }
                        else if (input[pos] == '\n')
                        {
                            pos++;
                            line++;
                            column = 1;
                        }
                        else
                        {
                            pos++;
                            column++;
                        }
                    }
                    continue;
                }
            }
            
            // 不是空白或注释，退出循环
            break;
        }

        if (pos >= input.length())
            return make_tuple("", "", "", 0, false);

        int currentState = dfa.startState;
        size_t startPos = pos;
        size_t lastAcceptPos = pos;
        int lastAcceptState = -1;
        int startLine = line;
        int startColumn = column;

        while (pos < input.length())
        {
            char c = input[pos];

            if (transitionTable[currentState].find(c) != transitionTable[currentState].end())
            {
                currentState = transitionTable[currentState][c];
                pos++;
                column++;

                if (dfa.acceptStates.find(currentState) != dfa.acceptStates.end())
                {
                    lastAcceptPos = pos;
                    lastAcceptState = currentState;
                }
            }
            else
                break;
        }

        if (lastAcceptState != -1)
        {
            string tokenText = input.substr(startPos, lastAcceptPos - startPos);
            pos = lastAcceptPos;

            string tokenType = dfa.tokenType[lastAcceptState];
            string tokenValue = dfa.tokenValue[lastAcceptState];

            if ((tokenType == "INT" || tokenType == "FLOAT") && pos < input.length())
            {
                char nextChar = input[pos];
                if (isalpha(nextChar) || nextChar == '_')
                {
                    size_t errorEnd = pos;
                    while (errorEnd < input.length() && 
                           (isalnum(input[errorEnd]) || input[errorEnd] == '_' || input[errorEnd] == '.'))
                        errorEnd++;
                    
                    string errorText = input.substr(startPos, errorEnd - startPos);
                    column += (errorEnd - pos);
                    pos = errorEnd;
                    
                    return make_tuple(errorText, "ERROR", to_string(startLine) + "," + to_string(startColumn), startLine, true);
                }
            }

            if (tokenType == "IDN")
            {
                string lowerToken = tokenText;
                transform(lowerToken.begin(), lowerToken.end(), lowerToken.begin(), ::tolower);
                if (keywords.find(lowerToken) != keywords.end())
                {
                    tokenType = "KW";
                    tokenValue = to_string(keywords[lowerToken].second);
                }
                else
                    tokenValue = tokenText;
            }
            else if (tokenType == "INT" || tokenType == "FLOAT")
                tokenValue = tokenText;
            else if (tokenType == "OP")
            {
                if (operators.find(tokenText) != operators.end())
                    tokenValue = to_string(operators[tokenText].second);
            }
            else if (tokenType == "SE")
            {
                if (separators.find(tokenText) != separators.end())
                    tokenValue = to_string(separators[tokenText].second);
            }

            return make_tuple(tokenText, tokenType, tokenValue, startLine, true);
        }
        else
        {
            string errorChar = string(1, input[startPos]);
            pos++;
            column++;

            return make_tuple(errorChar, "ERROR", to_string(startLine) + "," + to_string(startColumn), startLine, true);
        }
    }
};

DFA buildLexerDFA()
{
    vector<NFA> nfas;
    vector<pair<string, string>> tokenInfos;

    // 关键字NFA
    vector<string> keywordList = {"return", "const", "float", "void", "else", "int", "if"};
    for (const string &keyword : keywordList)
    {
        string lowerKeyword = keyword;
        transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
        NFA nfa = buildKeywordNFA(keyword, "KW", keywords[lowerKeyword].second);
        nfas.push_back(nfa);
        tokenInfos.push_back({"KW", to_string(keywords[lowerKeyword].second)});
    }

    // 运算符NFA
    vector<string> longOps = {"==", "<=", ">=", "!=", "&&", "||"};
    for (const string &op : longOps)
    {
        NFA nfa;
        nfa.startState = 0;
        nfa.transitions.push_back({0, op[0], 1});
        nfa.transitions.push_back({1, op[1], 2});
        nfa.maxState = 2;
        nfa.acceptStates.insert(2);
        nfas.push_back(nfa);
        tokenInfos.push_back({"OP", to_string(operators[op].second)});
    }

    vector<string> singleOps = {"+", "-", "*", "/", "%", "=", ">", "<", "!"};
    for (const string &op : singleOps)
    {
        nfas.push_back(createCharNFA(op[0]));
        tokenInfos.push_back({"OP", to_string(operators[op].second)});
    }

    // 界符NFA
    vector<string> seps = {"(", ")", "{", "}", ";", ","};
    for (const string &sep : seps)
    {
        nfas.push_back(createCharNFA(sep[0]));
        tokenInfos.push_back({"SE", to_string(separators[sep].second)});
    }

    // 标识符NFA,关键字优先
    NFA idnNFA = buildIdentifierNFA();
    nfas.push_back(idnNFA);
    tokenInfos.push_back({"IDN", ""});

    // 整数NFA
    NFA intNFA = buildIntegerNFA();
    nfas.push_back(intNFA);
    tokenInfos.push_back({"INT", ""});

    // 浮点数NFA
    NFA floatNFA = buildFloatNFA();
    nfas.push_back(floatNFA);
    tokenInfos.push_back({"FLOAT", ""});

    // 合并所有NFA
    CombinedNFA combined = combineNFAs(nfas, tokenInfos);

    // NFA -> DFA
    DFA dfa = nfaToDFA(combined);

    // DFA最小化
    DFA minDFA = minimizeDFA(dfa);

    return minDFA;
}

// ==================== 导出状态转移矩阵 ====================
void createDirectoryIfNotExists(const string &path)
{
#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), 0755);
#endif
}

/**
 * 导出DFA状态转移矩阵到CSV文件
 */
void exportDFATransitionMatrix(const DFA &dfa, const string &filename)
{
    // createDirectoryIfNotExists(TRANSITION_MATRIX_PATH);

    ofstream csvFile(filename);
    if (!csvFile.is_open())
    {
        cout << "Error: Cannot create file '" << filename << "'" << endl;
        return;
    }

    set<char> alphabet;
    for (const auto &trans : dfa.transitions)
        alphabet.insert(trans.symbol);

    map<int, map<char, int>> transitionTable;
    for (const auto &trans : dfa.transitions)
        transitionTable[trans.from][trans.symbol] = trans.to;

    csvFile << "State";
    for (char c : alphabet)
    {
        csvFile << ",";
        if (c == ' ')
            csvFile << "SPACE";
        else if (c == '\t')
            csvFile << "TAB";
        else if (c == '\n')
            csvFile << "NEWLINE";
        else if (c == '\r')
            csvFile << "CR";
        else
            csvFile << c;
    }
    csvFile << ",Accept,TokenType,TokenValue" << endl;

    for (int state = 0; state <= dfa.maxState; state++)
    {
        csvFile << state;
        for (char c : alphabet)
        {
            csvFile << ",";
            if (transitionTable[state].find(c) != transitionTable[state].end())
                csvFile << transitionTable[state][c];
            else
                csvFile << "-";
        }

        csvFile << ",";
        if (dfa.acceptStates.find(state) != dfa.acceptStates.end())
            csvFile << "YES";
        else
            csvFile << "NO";

        csvFile << ",";
        auto typeIt = dfa.tokenType.find(state);
        if (typeIt != dfa.tokenType.end())
            csvFile << typeIt->second;

        csvFile << ",";
        auto valueIt = dfa.tokenValue.find(state);
        if (valueIt != dfa.tokenValue.end())
            csvFile << valueIt->second;

        csvFile << endl;
    }

    csvFile.close();
    cout << "[LEXER] DFA state transition matrix has exported to " << filename << endl;
}


// ==================== 封装接口 ====================
#include "lexer.h"

static Lexer *global_lexer = nullptr;
static DFA global_dfa;
static string test_case_path = "";
static string test_file_name = "";
static ofstream token_output_file;

extern "C"
{

    void initLexer(const char *input)
    {
        test_case_path = TEST_CASE_PATH;

        if (global_lexer != nullptr)
            delete global_lexer;

        global_dfa = buildLexerDFA();

        // #EXPORT_DEBUG# 输出词法分析状态转移矩阵
        // exportDFATransitionMatrix(global_dfa, TRANSITION_MATRIX_PATH + "lexer_state_transition_matrix.csv");
        
        if (input == nullptr)
            global_lexer = new Lexer(global_dfa, string(""));
        else 
        {
            // 计算输入长度，避免读取到意外的内存数据
            size_t input_length = strlen(input);
            global_lexer = new Lexer(global_dfa, string(input, input_length));
        }
        
        if (!test_case_path.empty() && !test_file_name.empty())
        {
            // createDirectoryIfNotExists(test_case_path);
            
            string base_name = test_file_name;
            size_t dot_pos = base_name.find_last_of('.');
            if (dot_pos != string::npos)
                base_name = base_name.substr(0, dot_pos);
            
            string output_path = "output/" +  base_name + S_LEXER_RESULT_EXT;
            token_output_file.open(output_path, ios::out | ios::trunc);
            if (!token_output_file.is_open())
                cout << "Warning: Cannot create token output file: " << output_path << endl;
            else
                cout << "[LEXER] Token output file saved to: " << output_path << endl;
        }
    }

    Token getNextToken()
    {
        Token token;
        token.valid = 0;

        if (global_lexer == nullptr)
            return token;

        auto result = global_lexer->nextToken();
        string tokenText = get<0>(result);
        string tokenType = get<1>(result);
        string tokenValue = get<2>(result);
        int lineNumber = get<3>(result);
        bool success = get<4>(result);

        if (!success)
        {
            // cout<< "[LEXER] " << endl;
            //token_output_file << "ACC" <<endl;
            if (token_output_file.is_open())
                token_output_file.close();
            return token;
        }

        // 词法分析输出要求main被当成关键字，但是语法分析器还是当成表示符好一点，只能这样了哈哈哈
        bool isMain = tokenText == "main";

        strncpy(token.text, tokenText.c_str(), 255);
        token.text[255] = '\0';
        strncpy(token.type, tokenType.c_str(), 63);
        token.type[63] = '\0';
        strncpy(token.value, tokenValue.c_str(), 255);
        token.value[255] = '\0';
        token.lineNumber = lineNumber;
        token.valid = 1;

        if (token_output_file.is_open())
        {
            if (tokenType == "ERROR")
            {
                token_output_file << tokenText << "\t<ERROR," << tokenValue << ">" << endl;
                global_lexer->setHasErrorOption();
                cout << "[Line " << token.lineNumber << "] Bad token " << tokenText << endl;
            }
            else
                token_output_file << tokenText << "\t<" << (isMain ? "KW" : tokenType) << "," << (isMain ? "5" : tokenValue) << ">" << endl;
        }

        return token;
    }

    void cleanupLexer()
    {
        if (global_lexer != nullptr)
        {
            delete global_lexer;
            global_lexer = nullptr;
        }
        
        // 清理DFA数据
        global_dfa = DFA();
        
        // 关闭并清理文件流
        if (token_output_file.is_open())
        {
            token_output_file.flush();
            token_output_file.close();
        }
        
        // 重置位置计数器和路径信息
        test_case_path = "";
        test_file_name = "";
    }
    
    void setTestCase(const char* path, const string fileName)
    {
        test_case_path = string(path);
        test_file_name = fileName;
    }
}

#ifndef NO_MAIN
int main(int argc, char *argv[])
{
    string input;
    string input_filename = "";

    cout<< "[LEXER] Lexer running..."<<endl;

    if (argc == 2)
    {
        input_filename = string(argv[1]);
        ifstream file(input_filename);
        if (!file.is_open())
        {
            cout << "Error: Cannot open file '" << input_filename << "'" << endl;
            return 1;
        }
        string line;
        while (getline(file, line))
            input += line + "\n";
        file.close();
        
        size_t last_slash = input_filename.find_last_of("/\\");
        string filename_only = (last_slash != string::npos) ? input_filename.substr(last_slash + 1) : input_filename;
        
        setTestCase(TEST_CASE_PATH, filename_only);
        initLexer(input.c_str());
        
        cout << "[LEXER] Tokenizing file: " << test_file_name << endl;
        // cout << endl;
        
        while (true)
        {
            Token token = getNextToken();
            if (!token.valid)
                break;
        }

        if(global_lexer->getHasErrorOption())
            cout << "\n[ERROR] Some tokens error appealed!\n"<< endl;
        else
            cout<< "[LEXER] ACC\n"<<endl;
        
        cleanupLexer();
    }
    else if (argc == 1)
    {
        cout << "Enter text (type 'END' on a new line to finish, or press Ctrl+Z then Enter (Windows) / Ctrl+D (Linux/Mac)):" << endl;
        string line;
        while (getline(cin, line))
        {
            if (line == "END" || line == "end")
                break;
            input += line + "\n";
        }
        
        DFA lexerDFA = buildLexerDFA();
        Lexer lexer(lexerDFA, input);
        
        while (true)
        {
            tuple<string, string, string, int, bool> result = lexer.nextToken();
            string tokenText = get<0>(result);
            string tokenType = get<1>(result);
            string tokenValue = get<2>(result);
            int lineNumber = get<3>(result);
            bool success = get<4>(result);

            if (!success)
                break;

            if (tokenType == "ERROR")
                cout << tokenText << "\t<ERROR," << tokenValue << ">" << endl;
            else
                cout << tokenText << "\t<" << tokenType << "," << tokenValue << ">" << endl;
        }
        if(global_lexer->getHasErrorOption())
            cout<< "[LEXER] Some tokens error appealed!\n"<<endl;
        else
            cout<< "[LEXER] ACC\n"<<endl;
    }
    else
    {
        cout << "Usage: " << argv[0] << " [filename]" << endl;
        return 1;
    }

    return 0;
}
#endif