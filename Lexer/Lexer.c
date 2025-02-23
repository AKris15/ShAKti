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
wchar_t *getInput();  
int isVariableDeclared(const wchar_t *word);
int isFunctionDeclared(const wchar_t *word);

// Check if a string is a keyword
int isKeyword(const wchar_t *word){
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
// Alpha Check
int isSanskritAlpha(wchar_t c) {
    // Check if the character belongs to the Devanagari script range
    return (c >= L'\u0900' && c <= L'\u097F') ||  // Devanagari block
           (c >= L'\uA8E0' && c <= L'\uA8FF');   // Extended Devanagari block
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
    wcsncpy(token.value, value, sizeof(token.value) - 1);
    token.value[sizeof(token.value) - 1] = '\0';
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
        
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }

        if (iswspace(c)) {
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
        
        if (isSanskritAlpha(c) || (c & 0x80)) { // Unicode support
            handleIdentifier(input, &i, &isVariable, &isClassVariable, &isFunction);
            continue;
        }

        wchar_t unknown[2] = {c, '\0'};
        Token token = createToken(TOKEN_UNKNOWN, unknown);
        printf("Unknown: %s\n", token.value);
        i++;
    }

    Token eofToken = createToken(TOKEN_EOF, L"EOF");
    printf("End of Input: %ls\n", eofToken.value);
}
// Handle CharLiteral
void handleCharLiteral(const wchar_t *input, int *i) {
    wchar_t buffer[2] = {input[*i], L'\0'};
    Token token = createToken(TOKEN_wchar_t, buffer);
    printf("Character Literal: '%s'\n", token.value);
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
        if (isMultiLine) {
            if (c == '/' && input[*i + 1] == '*') {
                nestedCommentCount++; // Start of a new nested comment
                *i += 2;
                continue;
            } else if (c == '*' && input[*i + 1] == '/') {
                if (nestedCommentCount > 0) {
                    nestedCommentCount--; // End of a nested comment
                    *i += 2;
                    continue;
                } else {
                    *i += 2;
                    break;
                }
            }
        }
        // If single-line comment, just break at end of line or EOF
        else if (c == '\n' || c == '\0') {
            break;
        }

        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_COMMENT, buffer);
    printf("%s Comment: %s\n", isMultiLine ? "Multi-line" : "Single-line", token.value);
}

// Handle operators
void handleOperator(const wchar_t *input, int *i) {
    wchar_t operatorStr[3] = {input[*i], '\0', '\0'};
    if (input[*i + 1] == '=' || input[*i + 1] == operatorStr[0]) {
        operatorStr[1] = input[++(*i)];
    }
    Token token = createToken(TOKEN_OPERATOR, operatorStr);
    printf("Operator: %s\n", token.value);
    (*i)++;
}

// Handle special symbols
void handleSpecialSymbol(const wchar_t *input, int *i) {
    wchar_t symbol[2] = {input[*i], '\0'};
    Token token = createToken(TOKEN_SPECIAL_SYMBOL, symbol);
    printf("Special Symbol: %s\n", token.value);
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
    printf("String: \"%s\"\n", token.value);
}

// Handle numbers
void handleNumber(const wchar_t *input, int *i) {
    wchar_t buffer[100];
    int bufferIndex = 0;
    while (iswdigit(input[*i]) || input[*i] == '.') {
        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = '\0';
    Token token = createToken(TOKEN_NUMBER, buffer);
    printf("Number: %s\n", token.value);
}

// Handle identifiers and variable classification
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction) {
    wchar_t buffer[100];
    int bufferIndex = 0;
    while (isSanskritAlpha(input[*i]) || (input[*i] & 0x80)) {
        buffer[bufferIndex++] = input[(*i)++];
    }
    buffer[bufferIndex] = '\0';
    
    // Check if it's a function (e.g., using the 'कर्म' keyword)
    if (wcscmp(buffer, L"कर्म") == 0) {
        *isFunction = 1;
        return;
    }
    
    // Handle the function declaration
    if (*isFunction) {
        wcscat(functions[function_count++], buffer);
        Token token = createToken(TOKEN_FUNCTION, buffer);
        printf("Function: %s\n", token.value);
        *isFunction = 0;
    }
    
    // Check if it is a keyword first
    if (isKeyword(buffer)) {
        Token token = createToken(TOKEN_KEYWORD, buffer);
        printf("Keyword: %s\n", token.value);

        // Handle specific keywords
        if (wcscmp(buffer, L"पूर्ण") == 0) {
            *isVariable = 1;
        } else if (wcscmp(buffer, L"कक्षा") == 0) {
            *isClassVariable = 1;
        }
    }
    // Check if it is a previously declared variable
    else if (isVariableDeclared(buffer)) {
        Token token = createToken(TOKEN_VARIABLE, buffer);
        printf("Variable: %s\n", token.value);
    }
    // Check if it's a previously declared function
    else if (isFunctionDeclared(buffer)) {
        Token token = createToken(TOKEN_FUNCTION, buffer);
        printf("Function: %s\n", token.value);
    }
    // If it's a new variable
    else if (*isVariable) {
        wcsncpy(variables[variable_count++], buffer, sizeof(variables[0]) - 1);
        variables[variable_count - 1][sizeof(variables[0]) - 1] = L'\0';
        Token token = createToken(TOKEN_VARIABLE, buffer);
        printf("Variable: %s\n", token.value);
        *isVariable = 0;
    }
    // If it's a class variable
    else if (*isClassVariable) {
        Token token = createToken(TOKEN_CLASSED_VARIABLE, buffer);
        printf("Class Variable: %s\n", token.value);
        *isClassVariable = 0;
    } else {
        Token token = createToken(TOKEN_UNKNOWN, buffer);
        wprintf(L"Unknown: %ls\n", token.value);
    }
}

// Function to get input dynamically and optimize memory
wchar_t *getInput() {
    size_t bufferSize = 256;
    wchar_t *input = malloc(bufferSize * sizeof(wchar_t));  // Allocate memory
    if (!input) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    input[0] = L'\0';  // Initialize as an empty string

    size_t len = 0;
    printf("Enter your Sanskrit program (type '|' alone to stop):\n");

    while (1) {
        wchar_t buffer[256];
        if (!fgetws(buffer, sizeof(buffer) / sizeof(wchar_t), stdin)) {
            break; // Stop reading if input fails
        }

        // Check if the input is just "|"
        if (wcscmp(buffer, L"|\n") == 0) {
            break;
        }

        size_t bufferLen = wcslen(buffer);

        // Ensure enough space in input buffer
        wchar_t *newInput = realloc(input, (len + bufferLen + 1) * sizeof(wchar_t));  
        if (!newInput) {
            printf("Memory reallocation failed!\n");
            free(input);
            return NULL;
        }
        input = newInput;

        // Use `wcscpy` when `len == 0`, otherwise use `wcscat`
        if (len == 0) {
            wcscpy(input, buffer);
        } else {
            wcscat(input, buffer);
        }

        len += bufferLen;
    }

    return input;
}
// Main function
int main() {
    setlocale(LC_CTYPE, "en_US.UTF-8");
    wchar_t *program = getInput();

    if (program) {
        tokenize(program);
        free(program);
    } else {
        printf("Error in reading input!\n");
    }

    return 0;
}
