#ifndef TOKENS_H
#define TOKENS_H

#include <wchar.h>

// Token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_VARIABLE,
    TOKEN_CLASSED_VARIABLE,
    TOKEN_FUNCTION,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_STRING,
    TOKEN_wchar_t,
    TOKEN_COMMENT,
    TOKEN_SPECIAL_SYMBOL,
    TOKEN_BOOLEAN,
    TOKEN_EOF,
    TOKEN_EOL,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    wchar_t value[100];
} Token;

// Function prototypes
Token createToken(TokenType type, const wchar_t *value);

#endif // TOKENS_H