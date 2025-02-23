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
    "पूर्ण", "यदि", "अन्यथा", "चक्र", "से", "तक", "लेख", "प्रवे", "कक्ष","वा यदि", "न", NULL
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
char *getInput();  

// Check if a string is a keyword
int isKeyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

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

        if (isalpha(c) || (c & 0x80)) { // Unicode support
            handleIdentifier(input, &i, &isVariable, &isClassVariable);
            continue;
        }

        char unknown[2] = {c, '\0'};
        Token token = createToken(TOKEN_UNKNOWN, unknown);
        printf("Unknown: %s\n", token.value);
        i++;
    }

    Token eofToken = createToken(TOKEN_EOF, "EOF");
    printf("End of Input: %s\n", eofToken.value);
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

// Handle special symbols
void handleSpecialSymbol(const char *input, int *i) {
    char symbol[2] = {input[*i], '\0'};
    Token token = createToken(TOKEN_SPECIAL_SYMBOL, symbol);
    printf("Special Symbol: %s\n", token.value);
    (*i)++;
}

// Handle strings
void handleString(const char *input, int *i) {
    char buffer[1000];
    int bufferIndex = 0;
    (*i)++;

    while (input[*i] != '"' && input[*i] != '\0') {
        buffer[bufferIndex++] = input[(*i)++];
    }

    if (input[*i] == '\0') {
        printf("Error: Unterminated string\n");
        return;
    }

    (*i)++;
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_STRING, buffer);
    printf("String: \"%s\"\n", token.value);
}

// Handle numbers
void handleNumber(const char *input, int *i) {
    char buffer[100];
    int bufferIndex = 0;
    while (isdigit(input[*i]) || input[*i] == '.') {
        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_NUMBER, buffer);
    printf("Number: %s\n", token.value);
}

// Handle identifiers and variable classification
void handleIdentifier(const char *input, int *i, int *isVariable, int *isClassVariable) {
    char buffer[100];
    int bufferIndex = 0;
    while (isalpha(input[*i]) || (input[*i] & 0x80)) {
        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = '\0';

    if (isKeyword(buffer)) {
        Token token = createToken(TOKEN_KEYWORD, buffer);
        printf("Keyword: %s\n", token.value);

        if (strcmp(buffer, "पूर्ण") == 0) {
            *isVariable = 1;
        } else if (strcmp(buffer, "कक्षा") == 0) {
            *isClassVariable = 1;
        }
    } else if (*isVariable) {
        Token token = createToken(TOKEN_VARIABLE, buffer);
        printf("Variable: %s\n", token.value);
        *isVariable = 0;
    } else if (*isClassVariable) {
        Token token = createToken(TOKEN_CLASSED_VARIABLE, buffer);
        printf("Class Variable: %s\n", token.value);
        *isClassVariable = 0;
    } else {
        Token token = createToken(TOKEN_UNKNOWN, buffer);
        printf("Unknown: %s\n", token.value);
    }
}
// Function to get input dynamically and optimize memory
char *getInput() {
    size_t bufferSize = 256;
    char *input = malloc(bufferSize);
    if (!input) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    size_t len = 0;
    printf("Enter your Sanskrit program (type '|' alone to stop):\n");

    while (1) {
        char buffer[256];
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            break; // Stop reading if input fails
        }

        // Check if the input is just "|"
        if (strcmp(buffer, "|\n") == 0) {
            break;
        }

        size_t bufferLen = strlen(buffer);

        // Ensure enough space in input buffer
        char *newInput = realloc(input, len + bufferLen + 1);
        if (!newInput) {
            printf("Memory reallocation failed!\n");
            free(input);
            return NULL;
        }
        input = newInput;

        // Copy buffer into input
        strcpy(input + len, buffer);
        len += bufferLen;
    }

    return input;
}

// Main function
int main() {
    setlocale(LC_CTYPE, "");
    char *program = getInput();

    if (program) {
        tokenize(program);
        free(program);
    } else {
        printf("Error in reading input!\n");
    }

    return 0;
}
