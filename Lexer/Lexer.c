#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>
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
    TOKEN_wchar_t,
    TOKEN_COMMENT,
    TOKEN_SPECIAL_SYMBOL,
    TOKEN_BOOLEAN,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    wchar_t value[100];
} Token;

// Keywords in Sanskrit
const wchar_t *keywords[] = {
    L"पूर्ण", L"यदि", L"अन्यथा", 
    L"चक्र", L"से", L"तक", L"लेख", 
    L"प्रवे", L"कक्ष", L"वा यदि", 
    L"न", L"कर्म" , NULL
};

// Boolean literals
const wchar_t *boolean_literals[] = {
    L"सत्य", L"असत्य", NULL
};

// Operators
const wchar_t *operators[] = {
    L"+", L"-", L"*", L"/", L"=", L">", L"<", L">=", L"<=", L"==", L"!=", L"&&", L"||", L"!",
    L"?", L"+=", L"-=", L"*=", L"/="
};

// Special symbols
const wchar_t *special_symbols[] = {
    L"(", L")", L"{", L"}", L"[", L"]", L";", L",", L":"
};

// Variable and function tracking
wchar_t variables[100][100];
int variable_count = 0;
wchar_t functions[100][100];
int function_count = 0;

// Function prototypes
int isKeyword(const wchar_t *word);
int isBooleanLiteral(const wchar_t *word);
Token createToken(TokenType type, const wchar_t *value);
void tokenize(const wchar_t *input);
void handleComment(const wchar_t *input, int *i, int isMultiLine);
void handleOperator(const wchar_t *input, int *i);
void handleSpecialSymbol(const wchar_t *input, int *i);
void handleString(const wchar_t *input, int *i);
void handleCharLiteral(const wchar_t *input, int *i);
void handleNumber(const wchar_t *input, int *i);
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction);
wchar_t *readFile(const char *filename);
int isVariableDeclared(const wchar_t *word);
int isFunctionDeclared(const wchar_t *word);

// Check if a string is a keyword
int isKeyword(const wchar_t *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (wcscmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a string is a boolean literal
int isBooleanLiteral(const wchar_t *word) {
    for (int i = 0; boolean_literals[i] != NULL; i++) {
        if (wcscmp(word, boolean_literals[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a character is a Sanskrit alphabet
int isSanskritAlpha(wchar_t c) {
    return (c >= L'\u0900' && c <= L'\u097F') || (c >= L'\uA8E0' && c <= L'\uA8FF');
}

// Check if a function is declared
int isFunctionDeclared(const wchar_t *word) {
    for (int i = 0; i < function_count; i++) {
        if (wcscmp(functions[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a variable is declared
int isVariableDeclared(const wchar_t *word) {
    for (int i = 0; i < variable_count; i++) {
        if (wcscmp(variables[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Create a token
Token createToken(TokenType type, const wchar_t *value) {
    Token token;
    token.type = type;
    wcsncpy(token.value, value, sizeof(token.value) / sizeof(wchar_t) - 1);
    token.value[sizeof(token.value) / sizeof(wchar_t) - 1] = L'\0';
    return token;
}

// Tokenize input
void tokenize(const wchar_t *input) {
    int i = 0;
    long int line = 1, column = 1;
    int isVariable = 0, isClassVariable = 0, isFunction = 0;

    printf("\nLexical Analysis:\n");
    while (input[i] != L'\0') {
        wchar_t c = input[i];

        if (c == L'\n') {
            line++;
            column = 1;
        } else {
            column++;
        }

        if (iswspace(c)) {
            i++;
            continue;
        }

        if (c == L'/' && input[i + 1] == L'/') {
            handleComment(input, &i, 0);
            continue;
        }

        if (c == L'/' && input[i + 1] == L'*') {
            handleComment(input, &i, 1);
            continue;
        }

        if (wcschr(L"+-*/=><!&|?", c)) {
            handleOperator(input, &i);
            continue;
        }

        if (wcschr(L"(){}[],;:", c)) {
            handleSpecialSymbol(input, &i);
            continue;
        }

        if (iswdigit(c)) {
            handleNumber(input, &i);
            continue;
        }

        if (c == L'"') {
            handleString(input, &i);
            continue;
        }

        if (c == L'\'') {
            handleCharLiteral(input, &i);
            continue;
        }

        if (isSanskritAlpha(c) || (c & 0x80)) {
            handleIdentifier(input, &i, &isVariable, &isClassVariable, &isFunction);
            continue;
        }

        wchar_t unknown[2] = {c, L'\0'};
        Token token = createToken(TOKEN_UNKNOWN, unknown);
        printf("Unknown: %ls\n", token.value);
        i++;
    }

    Token eofToken = createToken(TOKEN_EOF, L"EOF");
    printf("End of Input: %ls\n", eofToken.value);
}

// Handle CharLiteral
void handleCharLiteral(const wchar_t *input, int *i) {
    wchar_t buffer[2] = {input[*i], L'\0'};
    Token token = createToken(TOKEN_wchar_t, buffer);
    printf("Character Literal: '%ls'\n", token.value);
    (*i)++;
}

// Handle comments
void handleComment(const wchar_t *input, int *i, int isMultiLine) {
    wchar_t buffer[1000];
    int bufferIndex = 0;
    int nestedCommentCount = 0;
    *i += 2;
    while (1) {
        wchar_t c = input[*i];
        if (bufferIndex >= 999) {
            wprintf(L"Error: Comment too long!\n");
            break;
        }
        if (isMultiLine) {
            if (c == L'/' && input[*i + 1] == L'*') {
                nestedCommentCount++;
                *i += 2;
                continue;
            }
            if (c == L'\0') {
                wprintf(L"Error: Unterminated multi-line comment!\n");
                break;
            } else if (c == L'*' && input[*i + 1] == L'/') {
                if (nestedCommentCount > 0) {
                    nestedCommentCount--;
                    *i += 2;
                    continue;
                } else {
                    *i += 2;
                    break;
                }
            }
        } else if (c == L'\n' || c == L'\0') {
            break;
        }

        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = L'\0';
    Token token = createToken(TOKEN_COMMENT, buffer);
    printf("%s Comment: %ls\n", isMultiLine ? "Multi-line" : "Single-line", token.value);
}

// Handle operators
void handleOperator(const wchar_t *input, int *i) {
    wchar_t operatorStr[3] = {input[*i], L'\0', L'\0'};
    if (input[*i + 1] == L'=' || input[*i + 1] == operatorStr[0]) {
        operatorStr[1] = input[++(*i)];
    }
    Token token = createToken(TOKEN_OPERATOR, operatorStr);
    printf("Operator: %ls\n", token.value);
    (*i)++;
}

// Handle special symbols
void handleSpecialSymbol(const wchar_t *input, int *i) {
    wchar_t symbol[2] = {input[*i], L'\0'};
    Token token = createToken(TOKEN_SPECIAL_SYMBOL, symbol);
    printf("Special Symbol: %ls\n", token.value);
    (*i)++;
}

// Handle strings
void handleString(const wchar_t *input, int *i) {
    wchar_t buffer[1000];
    int bufferIndex = 0;
    (*i)++;

    while (input[*i] != L'"' && input[*i] != L'\0') {
        if (wcsncmp(&input[*i], L"\\नव", 6) == 0) {
            buffer[bufferIndex++] = L'\n';
            *i += 6;
        } else {
            buffer[bufferIndex++] = input[(*i)++];
        }
    }

    if (input[*i] == L'\0') {
        printf("Error: Unterminated string\n");
        return;
    }

    (*i)++;
    buffer[bufferIndex] = L'\0';
    Token token = createToken(TOKEN_STRING, buffer);
    printf("String: \"%ls\"\n", token.value);
}

// Handle numbers
void handleNumber(const wchar_t *input, int *i) {
    wchar_t buffer[100];
    int bufferIndex = 0;
    while (iswdigit(input[*i])) {
        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = L'\0';
    Token token = createToken(TOKEN_NUMBER, buffer);
    printf("Number: %ls\n", token.value);
}

// Handle identifiers and variable classification
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction) {
    wchar_t buffer[100];
    int bufferIndex = 0;

    while (isSanskritAlpha(input[*i]) || iswdigit(input[*i]) || input[*i] == L'_') {
        if (bufferIndex >= 99) {
            wprintf(L"Error: Identifier too long!\n");
            buffer[bufferIndex] = L'\0';
            break;
        }
        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = L'\0';

    // Check if it's a keyword
    if (isKeyword(buffer)) {
        Token token = createToken(TOKEN_KEYWORD, buffer);
        printf("Keyword: %ls\n", token.value);

        if (wcscmp(buffer, L"पूर्ण") == 0) {
            *isVariable = 1;
        } else if (wcscmp(buffer, L"कक्षा") == 0) {
            *isClassVariable = 1;
        }
        return;
    }

    // Check if it's a boolean literal
    if (isBooleanLiteral(buffer)) {
        Token token = createToken(TOKEN_BOOLEAN, buffer);
        printf("Boolean: %ls\n", token.value);
        return;
    }

    // Check if it’s a function declaration keyword
    if (wcscmp(buffer, L"कर्म") == 0) {
        *isFunction = 1;
        return;
    }

    // If the previous token was "कर्म", mark this as a function
    if (*isFunction) {
        if (function_count >= 100) {
            wprintf(L"Error: Too many functions declared!\n");
            return;
        }
        wcscpy(functions[function_count++], buffer);
        Token token = createToken(TOKEN_FUNCTION, buffer);
        printf("Function: %ls\n", token.value);
        *isFunction = 0;
        return;
    }

    // Check if it's a previously declared variable
    if (isVariableDeclared(buffer)) {
        Token token = createToken(TOKEN_VARIABLE, buffer);
        printf("Variable: %ls\n", token.value);
        return;
    }

    // Check if it's a previously declared function
    if (isFunctionDeclared(buffer)) {
        Token token = createToken(TOKEN_FUNCTION, buffer);
        printf("Function: %ls\n", token.value);
        return;
    }

    // If it's a new variable declaration
    if (*isVariable) {
        if (variable_count >= 100) {
            wprintf(L"Error: Too many variables!\n");
            *isVariable = 0;
            return;
        }
        wcscpy(variables[variable_count++], buffer);
        Token token = createToken(TOKEN_VARIABLE, buffer);
        printf("Variable: %ls\n", token.value);
        *isVariable = 0;
        return;
    }

    // If it's a class variable
    if (*isClassVariable) {
        Token token = createToken(TOKEN_CLASSED_VARIABLE, buffer);
        printf("Class Variable: %ls\n", token.value);
        *isClassVariable = 0;
        return;
    }

    // If nothing matched, treat it as an unknown identifier
    Token token = createToken(TOKEN_UNKNOWN, buffer);
    wprintf(L"Unknown: %ls\n", token.value);
}

// Read file into a wide character buffer
wchar_t *readFile(const char *filename) {
    FILE *file = fopen(filename, "r, ccs=UTF-8");
    if (!file) {
        wprintf(L"Error: Unable to open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    wchar_t *buffer = (wchar_t *)malloc((length + 1) * sizeof(wchar_t));
    if (!buffer) {
        wprintf(L"Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }

    size_t index = 0;
    wint_t ch;
    while ((ch = fgetwc(file)) != WEOF) {
        buffer[index++] = (wchar_t)ch;
    }
    buffer[index] = L'\0';

    fclose(file);
    return buffer;
}

// Main function
int main(int argc, char *argv[]) {
    setlocale(LC_CTYPE, "hi_IN.UTF-8");
    if (argc < 2) {
        wprintf(L"Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    wchar_t *program = readFile(argv[1]);
    if (program) {
        tokenize(program);
        free(program);
    } else {
        wprintf(L"Error in reading file!\n");
    }
    return 0;
}
