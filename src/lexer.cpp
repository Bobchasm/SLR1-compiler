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

using namespace std;

const char EPSILON = '\0';

map<string, pair<string, int>> keywords = {
    {"int", {"KW", 1}}, {"void", {"KW", 2}}, {"return", {"KW", 3}}, {"const", {"KW", 4}}, {"float", {"KW", 6}}, {"if", {"KW", 7}}, {"else", {"KW", 8}}};

map<string, pair<string, int>> operators = {
    {"+", {"OP", 9}}, {"-", {"OP", 10}}, {"*", {"OP", 11}}, {"/", {"OP", 12}}, {"%", {"OP", 13}}, {"=", {"OP", 14}}, {">", {"OP", 15}}, {"<", {"OP", 16}}, {"==", {"OP", 17}}, {"<=", {"OP", 18}}, {">=", {"OP", 19}}, {"!=", {"OP", 20}}, {"&&", {"OP", 21}}, {"||", {"OP", 22}}};

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

    vector<string> singleOps = {"+", "-", "*", "/", "%", "=", ">", "<"};
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

    map<int, map<char, int>> transitionTable;

public:
    Lexer(const DFA &dfa, const string &input)
        : dfa(dfa), input(input), pos(0), line(1), column(1)
    {
        for (const auto &trans : dfa.transitions)
            transitionTable[trans.from][trans.symbol] = trans.to;
    }

    tuple<string, string, string, bool> nextToken()
    {
        while (pos < input.length())
        {
            if (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\r')
            {
                column++;
                pos++;
            }
            else if (input[pos] == '\n')
            {
                line++;
                column = 1;
                pos++;
            }
            else
                break;
        }

        if (pos >= input.length())
            return make_tuple("", "", "", false);

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

            return make_tuple(tokenText, tokenType, tokenValue, true);
        }
        else
        {
            string errorChar = string(1, input[startPos]);
            pos++;
            column++;

            return make_tuple(errorChar, "ERROR", to_string(startLine) + "," + to_string(startColumn), true);
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

    vector<string> singleOps = {"+", "-", "*", "/", "%", "=", ">", "<"};
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

// ==================== 封装接口 ====================
#include "lexer.h"

static Lexer *global_lexer = nullptr;
static DFA global_dfa;

extern "C"
{

    void initLexer(const char *input)
    {
        if (global_lexer != nullptr)
            delete global_lexer;

        global_dfa = buildLexerDFA();
        global_lexer = new Lexer(global_dfa, string(input));
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
        bool success = get<3>(result);

        if (!success)
            return token;

        strncpy(token.text, tokenText.c_str(), 255);
        token.text[255] = '\0';
        strncpy(token.type, tokenType.c_str(), 63);
        token.type[63] = '\0';
        strncpy(token.value, tokenValue.c_str(), 255);
        token.value[255] = '\0';
        token.valid = 1;

        return token;
    }

    void cleanupLexer()
    {
        if (global_lexer != nullptr)
        {
            delete global_lexer;
            global_lexer = nullptr;
        }
    }
}

#ifndef NO_MAIN
int main(int argc, char *argv[])
{
    DFA lexerDFA = buildLexerDFA();
    string input;

    if (argc == 2)
    {
        ifstream file(argv[1]);
        if (!file.is_open())
        {
            cerr << "Error: Cannot open file '" << argv[1] << "'" << endl;
            return 1;
        }
        string line;
        while (getline(file, line))
            input += line + "\n";
        file.close();
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
    }
    else
    {
        cerr << "Usage: " << argv[0] << " [filename]" << endl;
        return 1;
    }

    Lexer lexer(lexerDFA, input);

    while (true)
    {
        tuple<string, string, string, bool> result = lexer.nextToken();
        string tokenText = get<0>(result);
        string tokenType = get<1>(result);
        string tokenValue = get<2>(result);
        bool success = get<3>(result);

        if (!success)
            break;

        if (tokenType == "ERROR")
            cout << tokenText << "\t<ERROR," << tokenValue << ">" << endl;
        else
            cout << tokenText << "\t<" << tokenType << "," << tokenValue << ">" << endl;
    }

    return 0;
}
#endif