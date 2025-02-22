#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

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
    TOKEN_COMMENT,
    TOKEN_SPECIAL_SYMBOL,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[100];
} Token;

// Keywords in Sanskrit
const char *keywords[] = {
    "पूर्ण", "यदि", "अन्यथा", "चक्र", "से", "तक", "लेख", "प्रवे", "कक्ष", "वा यदि", "न", NULL
};

// Operators
const char *operators[] = {
    "+", "-", "*", "/", "=", ">", "<", ">=", "<=", "==", "!=", "&&", "||", "!",
    "?", "+=", "-=", "*=", "/="
};

// Special symbols
const char *special_symbols[] = {
    "(", ")", "{", "}", "[", "]", ";", ",", ":"
};

// Check if a string is a keyword
int isKeyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}
