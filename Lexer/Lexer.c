#include "Lexer.h"
#include "Tokens.h"
#include "utils.h"
#include <wctype.h>
#include <stdio.h>
#include <string.h>

// Variable and function tracking
wchar_t variables[100][100];
int variable_count = 0;
wchar_t functions[100][100];
int function_count = 0;
wchar_t class_variables[100][100];
int class_variable_count = 0;

// Function prototypes
void handleComment(const wchar_t *input, int *i, int isMultiLine);
void handleOperator(const wchar_t *input, int *i);
void handleSpecialSymbol(const wchar_t *input, int *i);
void handleString(const wchar_t *input, int *i);
void handleCharLiteral(const wchar_t *input, int *i);
void handleNumber(const wchar_t *input, int *i);
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction);
int isVariableDeclared(const wchar_t *word);
int isFunctionDeclared(const wchar_t *word);
int isClassVariableDeclared(const wchar_t *word);

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

// Check if a class variable is declared
int isClassVariableDeclared(const wchar_t *word) {
    for (int i = 0; i < class_variable_count; i++) {
        if (wcscmp(class_variables[i], word) == 0) {
            return 1;
        }
    }
    return 0;
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

        if (wcschr(L"+-*/=><!&;?", c)) {
            handleOperator(input, &i);
            continue;
        }

        if (wcschr(L"(){}[],:|", c)) {
            handleSpecialSymbol(input, &i);
            continue;
        }

        // Check for underscore at the beginning of an identifier
        if (c == L'_') {
            handleIdentifier(input, &i, &isVariable, &isClassVariable, &isFunction);
            continue;
        }

        // Check for digits & Devanagari digits
        if (iswdigit(c) || isDevanagariDigit(c)) {
            handleNumber(input, &i );
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

        // Handle unknown characters
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
    Token token;

    if (wcscmp(symbol, L"|") == 0) {
        token = createToken(TOKEN_EOL, symbol);
        printf("End of Line: %ls\n", token.value);
    } else {
        token = createToken(TOKEN_SPECIAL_SYMBOL, symbol);
        printf("Special Symbol: %ls\n", token.value);
    }

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

// the handleNumber function to recognize Devanagari digits
void handleNumber(const wchar_t *input, int *i) {
    wchar_t buffer[100];
    int bufferIndex = 0;
    int isIdentifier = 0;
    int hasDigits = 0;
    
    // First, collect all digits (including Devanagari digits)
    while (iswdigit(input[*i]) || isDevanagariDigit(input[*i])) {
        buffer[bufferIndex++] = input[(*i)++];
        hasDigits = 1;
    }
    
    // Check if the next character indicates this is actually an identifier
    if (isSanskritAlpha(input[*i]) || input[*i] == L'_' || (input[*i] & 0x80)) {
        isIdentifier = 1;
        
        // Continue collecting the rest of the identifier
        while (isSanskritAlpha(input[*i]) || iswdigit(input[*i]) || 
               isDevanagariDigit(input[*i]) || input[*i] == L'_' || (input[*i] & 0x80)) {
            if (bufferIndex >= 99) {
                wprintf(L"Error: Identifier too long!\n");
                break;
            }
            buffer[bufferIndex++] = input[(*i)++];
        }
    }
    
    buffer[bufferIndex] = L'\0';
    
    if (isIdentifier) {
        // Treat as an unknown identifier
        Token token = createToken(TOKEN_UNKNOWN, buffer);
        printf("Unknown: %ls\n", token.value);
    } else if (hasDigits) {
        // It's a pure number
        Token token = createToken(TOKEN_NUMBER, buffer);
        printf("Number: %ls\n", token.value);
    }
}

// the handleIdentifier function to better handle mixed identifiers
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction) {
    wchar_t buffer[100];
    int bufferIndex = 0;

    // Handle identifiers that can include Sanskrit characters, digits, Devanagari digits, and underscores
    while (isSanskritAlpha(input[*i]) || iswdigit(input[*i]) || 
           isDevanagariDigit(input[*i]) || input[*i] == L'_' || (input[*i] & 0x80)) {
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
        } else if (wcscmp(buffer, L"कर्म") == 0) {
            *isFunction = 1;
        }
        return;
    }

    // Check if it's a boolean literal
    if (isBooleanLiteral(buffer)) {
        Token token = createToken(TOKEN_BOOLEAN, buffer);
        printf("Boolean: %ls\n", token.value);
        return;
    }

    // Check if it's a previously declared variable
    if (isVariableDeclared(buffer)) {
        Token token = createToken(TOKEN_VARIABLE, buffer);
        printf("Variable: %ls\n", token.value);
        return;
    }

    // Check if it's a previously declared class variable
    if (isClassVariableDeclared(buffer)) {
        Token token = createToken(TOKEN_CLASSED_VARIABLE, buffer);
        printf("Class Variable: %ls\n", token.value);
        return;
    }

    // Check if it's a previously declared function
    if (isFunctionDeclared(buffer)) {
        Token token = createToken(TOKEN_FUNCTION, buffer);
        printf("Function: %ls\n", token.value);
        return;
    }

    // Handle function name after कर्म keyword
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

    // Handle class variable name after कक्षा keyword
    if (*isClassVariable) {
        if (class_variable_count >= 100) {
            wprintf(L"Error: Too many class variables declared!\n");
            return;
        }
        wcscpy(class_variables[class_variable_count++], buffer);
        Token token = createToken(TOKEN_CLASSED_VARIABLE, buffer);
        printf("Class Variable: %ls\n", token.value);
        *isClassVariable = 0;
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

    // If nothing matched, treat it as an unknown identifier
    Token token = createToken(TOKEN_UNKNOWN, buffer);
    printf("Unknown: %ls\n", token.value);
}