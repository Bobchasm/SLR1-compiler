#ifndef LEXER_INTERFACE_H
#define LEXER_INTERFACE_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        char text[256];
        char type[64];
        char value[256];
        int lineNumber;      // 源代码行号
        int valid;
    } Token;

    void initLexer(const char *input);
    Token getNextToken();
    void cleanupLexer();
    void setTestCase(const char *path, const string fileName);

#ifdef __cplusplus
}
#endif

#endif
