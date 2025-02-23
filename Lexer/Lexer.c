#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

// Define token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_VARIABLE,
    TOKEN_CLASSED_VARIABLE,
    TOKEN_FUNCTION,
    TOKEN_CLASS,
    TOKEN_ARRAY,
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
    int line;
    int column;
} Token;

// List of keywords in Sanskrit
const char *keywords[] = {
    "पूर्ण",    // Integer
    "यदि",     // If
    "अन्यथा",  // Else
    "चक्र",    // Loop
    "से",      // From
    "तक",      // To
    "लेख",     // Print
    "प्रवे",    // Input
    "कक्षा",    // Class
    NULL
};

// List of operators
const char *operators[] = {
    "+", "-", "*", "/", "=", ">", "<", ">=", "<=", "==", "!=", "&&", "||", "!"
};

// List of special symbols
const char *special_symbols[] = {
    "(", ")", "{", "}", "[", "]", ";", ","
};

// Function prototypes
int isKeyword(const char *word);
int isOperator(const char *op);
int isSpecialSymbol(const char *symbol);
Token createToken(TokenType type, const char *value, int line, int column);
void tokenize(const char *input);
void handleComment(const char *input, int *i, int *line, int *column, int isMultiLine);
void handleOperator(const char *input, int *i, int *line, int *column);
void handleString(const char *input, int *i, int *line, int *column);
void handleNumber(const char *input, int *i, int *line, int *column);
void handleIdentifier(const char *input, int *i, int *line, int *column, int *isClassContext);
void handleSpecialSymbol(const char *input, int *i, int *line, int *column);

// Function to check if a string is a keyword
int isKeyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a string is an operator
int isOperator(const char *op) {
    for (int i = 0; operators[i] != NULL; i++) {
        if (strcmp(op, operators[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a string is a special symbol
int isSpecialSymbol(const char *symbol) {
    for (int i = 0; special_symbols[i] != NULL; i++) {
        if (strcmp(symbol, special_symbols[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to create a token
Token createToken(TokenType type, const char *value, int line, int column) {
    Token token;
    token.type = type;
    strcpy(token.value, value);
    token.line = line;
    token.column = column;
    return token;
}

// Function to tokenize the input
void tokenize(const char *input) {
    int i = 0;      // Current position in input
    int line = 1;   // Current line number
    int column = 1; // Current column number
    int isClassContext = 0; // Context flag for handling classed variables

    printf("Lexical Analysis:\n");
    printf("=================\n");

    while (input[i] != '\0') {
        char c = input[i];

        // Skip whitespace and track line/column
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            i++;
            continue;
        }

        // Handle single-line comments
        if (c == '/' && input[i + 1] == '/') {
            handleComment(input, &i, &line, &column, 0);
            continue;
        }

        // Handle multi-line comments
        if (c == '/' && input[i + 1] == '*') {
            handleComment(input, &i, &line, &column, 1);
            continue;
        }

        // Handle special symbols
        if (strchr("(){}[];,", c)) {
            handleSpecialSymbol(input, &i, &line, &column);
            continue;
        }

        // Handle operators
        if (strchr("=><!&|+-*/", c)) {
            handleOperator(input, &i, &line, &column);
            continue;
        }

        // Handle numbers
        if (isdigit(c)) {
            handleNumber(input, &i, &line, &column);
            continue;
        }

        // Handle strings
        if (c == '"') {
            handleString(input, &i, &line, &column);
            continue;
        }

        // Handle identifiers or keywords
        if (isalpha(c) || (c & 0x80)) { // Unicode support for Sanskrit
            handleIdentifier(input, &i, &line, &column, &isClassContext);
            continue;
        }

        // Handle unknown characters
        char unknown[2] = {c, '\0'};
        Token token = createToken(TOKEN_UNKNOWN, unknown, line, column);
        printf("Unknown: %s at Line: %d, Column: %d\n", token.value, token.line, token.column);
        i++;
        column++;
    }

    // End of file token
    Token eofToken = createToken(TOKEN_EOF, "EOF", line, column);
    printf("End of Input: %s\n", eofToken.value);
}

// Function to handle comments
void handleComment(const char *input, int *i, int *line, int *column, int isMultiLine) {
    int bufferIndex = 0;
    char buffer[1000];
    *i += isMultiLine ? 2 : 2; // Skip // or /*
    *column += 2;

    while (1) {
        char c = input[*i];
        if (isMultiLine && c == '*' && input[*i + 1] == '/') {
            *i += 2; // End of multi-line comment
            *column += 2;
            break;
        } else if (!isMultiLine && (c == '\n' || c == '\0')) {
            break; // End of single-line comment
        } else if (c == '\0') {
            printf("Error: Unterminated comment at Line: %d\n", *line);
            return;
        }
        buffer[bufferIndex++] = input[(*i)++];
        if (c == '\n') {
            (*line)++;
            *column = 1;
        } else {
            (*column)++;
        }
    }
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_COMMENT, buffer, *line, *column);
    printf("%s Comment: %s\n", isMultiLine ? "Multi-line" : "Single-line", token.value);
}

// Function to handle operators
void handleOperator(const char *input, int *i, int *line, int *column) {
    char operatorStr[3] = {input[*i], '\0', '\0'};
    if (input[*i + 1] == '=' || input[*i + 1] == operatorStr[0]) {
        operatorStr[1] = input[++(*i)];
    }
    Token token = createToken(TOKEN_OPERATOR, operatorStr, *line, *column);
    printf("Operator: %s at Line: %d, Column: %d\n", token.value, token.line, token.column);
    (*i)++;
    (*column)++;
}

// Function to handle strings
void handleString(const char *input, int *i, int *line, int *column) {
    char buffer[1000];
    int bufferIndex = 0;
    (*i)++; // Skip opening quote
    (*column)++;
    while (input[*i] != '"' && input[*i] != '\0') {
        if (input[*i] == '\\' && input[*i + 1] != '\0') { // Handle escape characters
            buffer[bufferIndex++] = input[(*i)++];
        }
        buffer[bufferIndex++] = input[(*i)++];
        (*column)++;
    }
    if (input[*i] == '\0') {
        printf("Error: Unterminated string at Line: %d, Column: %d\n", *line, *column);
        return;
    }
    (*i)++; // Skip closing quote
    (*column)++;
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_STRING, buffer, *line, *column);
    printf("String: \"%s\" at Line: %d, Column: %d\n", token.value, token.line, token.column);
}

// Function to handle numbers
void handleNumber(const char *input, int *i, int *line, int *column) {
    char buffer[100];
    int bufferIndex = 0;
    while (isdigit(input[*i]) || input[*i] == '.') {
        buffer[bufferIndex++] = input[(*i)++];
        (*column)++;
    }
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_NUMBER, buffer, *line, *column);
    printf("Number: %s at Line: %d, Column: %d\n", token.value, token.line, token.column);
}

// Function to handle identifiers or keywords
void handleIdentifier(const char *input, int *i, int *line, int *column, int *isClassContext) {
    char buffer[100];
    int bufferIndex = 0;
    while (isalpha(input[*i]) || (input[*i] & 0x80)) {
        buffer[bufferIndex++] = input[(*i)++];
        (*column)++;
    }
    buffer[bufferIndex] = '\0';

    if (*isClassContext) {
        Token token = createToken(TOKEN_CLASSED_VARIABLE, buffer, *line, *column);
        printf("Classed Variable: %s at Line: %d, Column: %d\n", token.value, token.line, token.column);
        *isClassContext = 0; // Reset class context
    } else if (isKeyword(buffer)) {
        Token token = createToken(TOKEN_KEYWORD, buffer, *line, *column);
        printf("Keyword: %s at Line: %d, Column: %d\n", token.value, token.line, token.column);
        if (strcmp(buffer, "कक्षा") == 0) {
            *isClassContext = 1; // Set class context
        }
    } else {
        Token token = createToken(TOKEN_VARIABLE, buffer, *line, *column);
        printf("Variable: %s at Line: %d, Column: %d\n", token.value, token.line, token.column);
    }
}

// Function to handle special symbols
void handleSpecialSymbol(const char *input, int *i, int *line, int *column) {
    char symbol[2] = {input[*i], '\0'};
    Token token = createToken(TOKEN_SPECIAL_SYMBOL, symbol, *line, *column);
    printf("Special Symbol: %s at Line: %d, Column: %d\n", token.value, token.line, token.column);
    (*i)++;
    (*column)++;
}

// Main function
int main() {
    setlocale(LC_CTYPE, ""); // Enable UTF-8 support
    char program[1000] = ""; // Buffer for input program
    char line[256];

    printf("Enter your Sanskrit program (type '|' to stop):\n");
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        if (strcmp(line, "|\n") == 0) break;
        strcat(program, line);
    }

    tokenize(program);
    return 0;
}
