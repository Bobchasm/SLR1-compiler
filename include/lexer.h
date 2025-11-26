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
        int valid;
    } Token;

    void initLexer(const char *input);
    Token getNextToken();
    void cleanupLexer();

#ifdef __cplusplus
}
#endif

#endif
