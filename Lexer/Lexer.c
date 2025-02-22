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

// Function prototypes
int isKeyword(const char *word);
Token createToken(TokenType type, const char *value);
void tokenize(const char *input);
void handleComment(const char *input, int *i, int isMultiLine);
void handleOperator(const char *input, int *i);
void handleSpecialSymbol(const char *input, int *i);
void handleString(const char *input, int *i);
void handleNumber(const char *input, int *i);
void handleIdentifier(const char *input, int *i, int *isVariable, int *isClassVariable);
int isVariableDeclared(const char *word);

// Check if a string is a keyword
int isKeyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a variable is declared
int isVariableDeclared(const char *word) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Variable tracking
char variables[100][100];
int variable_count = 0;

// Create a token
Token createToken(TokenType type, const char *value) {
    Token token;
    token.type = type;
    strncpy(token.value, value, sizeof(token.value) - 1);
    token.value[sizeof(token.value) - 1] = '\0';
    return token;
}

// Tokenize input
void tokenize(const char *input) {
    int i = 0;
    int isVariable = 0, isClassVariable = 0;
    printf("\nLexical Analysis:\n");

    while (input[i] != '\0') {
        char c = input[i];

        if (isspace(c)) {
            i++;
            continue;
        }

        if (c == '/' && input[i + 1] == '/') {
            handleComment(input, &i, 0);
            continue;
        }

        if (c == '/' && input[i + 1] == '*') {
            handleComment(input, &i, 1);
            continue;
        }

        if (strchr("+-*/=><!&|?", c)) {
            handleOperator(input, &i);
            continue;
        }

        if (strchr("(){}[],;:", c)) {
            handleSpecialSymbol(input, &i);
            continue;
        }

        if (isdigit(c)) {
            handleNumber(input, &i);
            continue;
        }

        if (c == '"') {
            handleString(input, &i);
            continue;
        }
    }
}

// Handle comments
void handleComment(const char *input, int *i, int isMultiLine) {
    char buffer[1000];
    int bufferIndex = 0;
    *i += 2;

    while (1) {
        char c = input[*i];
        if (isMultiLine && c == '*' && input[*i + 1] == '/') {
            *i += 2;
            break;
        } else if (!isMultiLine && (c == '\n' || c == '\0')) {
            break;
        } else if (c == '\0') {
            printf("Error: Unterminated comment\n");
            return;
        }
        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_COMMENT, buffer);
    printf("%s Comment: %s\n", isMultiLine ? "Multi-line" : "Single-line", token.value);
}

// Handle operators
void handleOperator(const char *input, int *i) {
    char operatorStr[3] = {input[*i], '\0', '\0'};
    if (input[*i + 1] == '=' || input[*i + 1] == operatorStr[0]) {
        operatorStr[1] = input[++(*i)];
    }
    Token token = createToken(TOKEN_OPERATOR, operatorStr);
    printf("Operator: %s\n", token.value);
    (*i)++;
}
