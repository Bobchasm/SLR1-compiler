#include <vector>
#include <map>
#include <set>
#include <string>

using namespace std;

const string EPSILON = "$";
const string END_MARKER = "EOF";

map<string, string> tokenTypeToTerminal = {
    {"int", "int"}, {"void", "void"}, {"const", "const"}, {"float", "float"},
    {"if", "if"}, {"else", "else"}, {"return", "return"},
    {"+", "+"}, {"-", "-"}, {"*", "*"}, {"/", "/"}, {"%", "%"},
    {"=", "="}, {">", ">"}, {"<", "<"}, {"==", "=="}, {"<=", "<="}, 
    {">=", ">="}, {"!=", "!="}, {"&&", "&&"}, {"||", "||"}, {"!", "!"},
    {"(", "("}, {")", ")"}, {"{", "{"}, {"}", "}"}, {";", ";"}, {",", ","},
    {"IDN", "IDN"}, {"INT", "INT"}, {"FLOAT", "FLOAT"}
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
    vector<Production> productions;
    set<string> nonterminals;
    set<string> terminals;
    string startSymbol;
    
    map<string, set<string>> firstSets;
    map<string, set<string>> followSets;
    map<pair<string, string>, Production> parseTable;
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