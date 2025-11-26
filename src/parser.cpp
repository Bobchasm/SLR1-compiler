#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <stack>
#include <cstring>

#include "parse.h"
#include "lexer.h"


using namespace std;






class SLR1Parser 
{
private:
    // stack<string> parseStack;
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
            return tokenTypeToTerminal[tokenText];

        return tokenText;
    }
    
public:
    SLR1Parser(ostream &out) : output(out), stepCount(1) 
    {
        // parseStack.push(END_MARKER);
        // parseStack.push(grammar.startSymbol);
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
