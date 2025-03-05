#include "Lexer.h"
#include "Tokens.h"
#include "utils.h"
#include <wctype.h>
#include <stdio.h>
#include <string.h>

// Global arrays to track declared identifiers
// These help determine token types for identifiers based on previous declarations
wchar_t variables[100][100];       // Regular variables
int variable_count = 0;            // Count of declared variables
wchar_t functions[100][100];       // Function names
int function_count = 0;            // Count of declared functions
wchar_t class_variables[100][100]; // Class variables
int class_variable_count = 0;      // Count of declared class variables

// Function prototypes for internal helper functions
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

// Check if a function name has been previously declared
// Returns 1 if found, 0 otherwise
int isFunctionDeclared(const wchar_t *word) {
    for (int i = 0; i < function_count; i++) {
        // wcscmp is the wide character version of strcmp
        if (wcscmp(functions[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a variable name has been previously declared
// Returns 1 if found, 0 otherwise
int isVariableDeclared(const wchar_t *word) {
    for (int i = 0; i < variable_count; i++) {
        if (wcscmp(variables[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a class variable has been previously declared
// Returns 1 if found, 0 otherwise
int isClassVariableDeclared(const wchar_t *word) {
    for (int i = 0; i < class_variable_count; i++) {
        if (wcscmp(class_variables[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Main tokenization function - processes the input text and identifies tokens
// This is the core of the lexical analyzer
void tokenize(const wchar_t *input) {
    int i = 0;                                  // Current position in input
    long int line = 1, column = 1;              // Track line and column for error reporting
    int isVariable = 0, isClassVariable = 0, isFunction = 0;  // Context flags
    
    printf("\nLexical Analysis:\n");
    while (input[i] != L'\0') {                 // Process until end of input
        wchar_t c = input[i];                   // Current character

        // Track line and column numbers for error reporting
        if (c == L'\n') {
            line++;
            column = 1;
        } else {
            column++;
        }

        // Skip whitespace characters
        if (iswspace(c)) {
            i++;
            continue;
        }

        // Handle single-line comments (// style)
        if (c == L'/' && input[i + 1] == L'/') {
            handleComment(input, &i, 0);
            continue;
        }

        // Handle multi-line comments (/* */ style)
        if (c == L'/' && input[i + 1] == L'*') {
            handleComment(input, &i, 1);
            continue;
        }

        // Handle operators (+, -, *, /, etc.)
        if (wcschr(L"+-*/=><!&;?", c)) {
            handleOperator(input, &i);
            continue;
        }

        // Handle special symbols (parentheses, brackets, etc.)
        if (wcschr(L"(){}[],:|", c)) {
            handleSpecialSymbol(input, &i);
            continue;
        }

        // Handle identifiers starting with underscore
        if (c == L'_') {
            handleIdentifier(input, &i, &isVariable, &isClassVariable, &isFunction);
            continue;
        }

        // Handle numeric literals (including Devanagari digits)
        if (iswdigit(c) || isDevanagariDigit(c)) {
            handleNumber(input, &i);
            continue;
        }

        // Handle string literals (enclosed in double quotes)
        if (c == L'"') {
            handleString(input, &i);
            continue;
        }

        // Handle character literals (enclosed in single quotes)
        if (c == L'\'') {
            handleCharLiteral(input, &i);
            continue;
        }

        // Handle identifiers (including Sanskrit/Devanagari characters)
        // The high bit check (c & 0x80) helps catch Unicode characters
        if (isSanskritAlpha(c) || (c & 0x80)) {
            handleIdentifier(input, &i, &isVariable, &isClassVariable, &isFunction);
            continue;
        }

        // Handle unknown characters that don't match any pattern
        wchar_t unknown[2] = {c, L'\0'};
        Token token = createToken(TOKEN_UNKNOWN, unknown);
        printf("Unknown: %ls\n", token.value);
        i++;
    }

    // Create and output the end-of-file token
    Token eofToken = createToken(TOKEN_EOF, L"EOF");
    printf("End of Input: %ls\n", eofToken.value);
}

// Process character literals (single characters in single quotes)
void handleCharLiteral(const wchar_t *input, int *i) {
    wchar_t buffer[2] = {input[*i], L'\0'};
    Token token = createToken(TOKEN_wchar_t, buffer);
    printf("Character Literal: '%ls'\n", token.value);
    (*i)++;
}

// Process comments (both single-line and multi-line)
// Parameters:
//   - input: The input text
//   - i: Pointer to current position (will be updated)
//   - isMultiLine: 0 for single-line comments, 1 for multi-line comments
void handleComment(const wchar_t *input, int *i, int isMultiLine) {
    wchar_t buffer[1000];              // Buffer to store comment text
    int bufferIndex = 0;               // Current position in buffer
    int nestedCommentCount = 0;        // Track nested comments (for multi-line)
    *i += 2;                           // Skip the comment start characters (// or /*)
    
    while (1) {
        wchar_t c = input[*i];
        
        // Check for buffer overflow
        if (bufferIndex >= 999) {
            wprintf(L"Error: Comment too long!\n");
            break;
        }
        
        if (isMultiLine) {
            // Handle nested comments (/* inside another */)
            if (c == L'/' && input[*i + 1] == L'*') {
                nestedCommentCount++;
                *i += 2;
                continue;
            }
            
            // Check for unterminated comment
            if (c == L'\0') {
                wprintf(L"Error: Unterminated multi-line comment!\n");
                break;
            } 
            // Check for comment end
            else if (c == L'*' && input[*i + 1] == L'/') {
                if (nestedCommentCount > 0) {
                    // Close a nested comment
                    nestedCommentCount--;
                    *i += 2;
                    continue;
                } else {
                    // Close the main comment
                    *i += 2;
                    break;
                }
            }
        } 
        // For single-line comments, end at newline or EOF
        else if (c == L'\n' || c == L'\0') {
            break;
        }

        // Add character to buffer and advance
        buffer[bufferIndex++] = input[(*i)++];
    }
    
    // Null-terminate the comment text
    buffer[bufferIndex] = L'\0';
    
    // Create and output the comment token
    Token token = createToken(TOKEN_COMMENT, buffer);
    printf("%s Comment: %ls\n", isMultiLine ? "Multi-line" : "Single-line", token.value);
}

// Process operators (+, -, *, /, ==, !=, etc.)
// Handles both single-character and two-character operators
void handleOperator(const wchar_t *input, int *i) {
    // Initialize with the first character
    wchar_t operatorStr[3] = {input[*i], L'\0', L'\0'};
    
    // Check for two-character operators (==, +=, etc.)
    if (input[*i + 1] == L'=' || input[*i + 1] == operatorStr[0]) {
        operatorStr[1] = input[++(*i)];
    }
    
    // Create and output the operator token
    Token token = createToken(TOKEN_OPERATOR, operatorStr);
    printf("Operator: %ls\n", token.value);
    (*i)++;
}

// Process special symbols (parentheses, brackets, etc.)
void handleSpecialSymbol(const wchar_t *input, int *i) {
    wchar_t symbol[2] = {input[*i], L'\0'};
    Token token;

    // Special case: | is treated as end-of-line
    if (wcscmp(symbol, L"|") == 0) {
        token = createToken(TOKEN_EOL, symbol);
        printf("End of Line: %ls\n", token.value);
    } else {
        token = createToken(TOKEN_SPECIAL_SYMBOL, symbol);
        printf("Special Symbol: %ls\n", token.value);
    }

    (*i)++;
}

// Process string literals (text enclosed in double quotes)
void handleString(const wchar_t *input, int *i) {
    wchar_t buffer[1000];
    int bufferIndex = 0;
    (*i)++;  // Skip the opening quote

    // Process characters until closing quote or end of input
    while (input[*i] != L'"' && input[*i] != L'\0') {
        // Handle escape sequence for newline (Sanskrit-specific)
        if (wcsncmp(&input[*i], L"\\नव", 6) == 0) {
            buffer[bufferIndex++] = L'\n';
            *i += 6;
        } else {
            // Regular character
            buffer[bufferIndex++] = input[(*i)++];
        }
    }

    // Check for unterminated string
    if (input[*i] == L'\0') {
        printf("Error: Unterminated string\n");
        return;
    }

    (*i)++;  // Skip the closing quote
    buffer[bufferIndex] = L'\0';  // Null-terminate the string
    
    // Create and output the string token
    Token token = createToken(TOKEN_STRING, buffer);
    printf("String: \"%ls\"\n", token.value);
}

// Process numeric literals (including Devanagari digits)
// Also handles the case where a number is followed by letters (treated as unknown)
void handleNumber(const wchar_t *input, int *i) {
    wchar_t buffer[100];
    int bufferIndex = 0;
    int isIdentifier = 0;  // Flag for number-letter combinations
    int hasDigits = 0;     // Flag to confirm we have at least one digit
    
    // First, collect all digits (including Devanagari digits)
    while (iswdigit(input[*i]) || isDevanagariDigit(input[*i])) {
        buffer[bufferIndex++] = input[(*i)++];
        hasDigits = 1;
    }
    
    // Check if this is actually an identifier (number followed by letters)
    // This handles cases like "123abc" which are not valid numbers
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
    
    // Null-terminate the collected text
    buffer[bufferIndex] = L'\0';
    
    if (isIdentifier) {
        // Treat as an unknown identifier (not a valid number)
        Token token = createToken(TOKEN_UNKNOWN, buffer);
        printf("Unknown: %ls\n", token.value);
    } else if (hasDigits) {
        // It's a valid number
        Token token = createToken(TOKEN_NUMBER, buffer);
        printf("Number: %ls\n", token.value);
    }
}

// Process identifiers (variable names, function names, keywords, etc.)
// This is the most complex handler as it needs to track context
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction) {
    wchar_t buffer[100];
    int bufferIndex = 0;

    // Collect characters that can be part of an identifier
    // This includes Sanskrit characters, digits, Devanagari digits, underscores,
    // and any Unicode character (high bit set)
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

    // Check if it's a keyword (like पूर्ण, यदि, etc.)
    if (isKeyword(buffer)) {
        Token token = createToken(TOKEN_KEYWORD, buffer);
        printf("Keyword: %ls\n", token.value);

        // Set context flags based on specific keywords
        // These flags affect how subsequent identifiers are processed
        if (wcscmp(buffer, L"पूर्ण") == 0) {
            *isVariable = 1;  // Next identifier will be a variable
        } else if (wcscmp(buffer, L"कक्षा") == 0) {
            *isClassVariable = 1;  // Next identifier will be a class variable
        } else if (wcscmp(buffer, L"कर्म") == 0) {
            *isFunction = 1;  // Next identifier will be a function
        }
        return;
    }

    // Check if it's a boolean literal (सत्य or असत्य)
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
        // Add to function list and create token
        wcscpy(functions[function_count++], buffer);
        Token token = createToken(TOKEN_FUNCTION, buffer);
        printf("Function: %ls\n", token.value);
        *isFunction = 0;  // Reset the context flag
        return;
    }

    // Handle class variable name after कक्षा keyword
    if (*isClassVariable) {
        if (class_variable_count >= 100) {
            wprintf(L"Error: Too many class variables declared!\n");
            return;
        }
        // Add to class variable list and create token
        wcscpy(class_variables[class_variable_count++], buffer);
        Token token = createToken(TOKEN_CLASSED_VARIABLE, buffer);
        printf("Class Variable: %ls\n", token.value);
        *isClassVariable = 0;  // Reset the context flag
        return;
    }

    // Handle variable name after पूर्ण keyword
    if (*isVariable) {
        if (variable_count >= 100) {
            wprintf(L"Error: Too many variables!\n");
            *isVariable = 0;
            return;
        }
        // Add to variable list and create token
        wcscpy(variables[variable_count++], buffer);
        Token token = createToken(TOKEN_VARIABLE, buffer);
        printf("Variable: %ls\n", token.value);
        *isVariable = 0;  // Reset the context flag
        return;
    }

    // If nothing matched, treat it as an unknown identifier
    Token token = createToken(TOKEN_UNKNOWN, buffer);
    printf("Unknown: %ls\n", token.value);
}