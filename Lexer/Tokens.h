#ifndef TOKENS_H
#define TOKENS_H

#include <wchar.h>

// Token types - Enumeration of all possible token categories our lexer can identify
typedef enum {
    TOKEN_KEYWORD,         // Reserved words in the language (like पूर्ण, यदि, etc.)
    TOKEN_IDENTIFIER,      // Variable/function names that aren't keywords
    TOKEN_VARIABLE,        // Identified variables
    TOKEN_CLASSED_VARIABLE, // Class variables (after कक्षा keyword)
    TOKEN_FUNCTION,        // Function names (after कर्म keyword)
    TOKEN_NUMBER,          // Numeric literals (including Devanagari digits)
    TOKEN_OPERATOR,        // Mathematical and logical operators (+, -, *, etc.)
    TOKEN_STRING,          // String literals enclosed in double quotes
    TOKEN_wchar_t,         // Character literals (single characters)
    TOKEN_COMMENT,         // Comments (both single and multi-line)
    TOKEN_SPECIAL_SYMBOL,  // Special symbols like brackets, parentheses
    TOKEN_BOOLEAN,         // Boolean literals (सत्य, असत्य)
    TOKEN_EOF,             // End of file marker
    TOKEN_EOL,             // End of line marker
    TOKEN_UNKNOWN          // Unrecognized tokens
} TokenType;

// Token structure - Stores the type and value of each token
// Uses wchar_t array to support Unicode/Devanagari characters
typedef struct {
    TokenType type;        // The category of this token
    wchar_t value[100];    // The actual text content (using wide chars for Unicode support)
} Token;

// Function prototypes
Token createToken(TokenType type, const wchar_t *value);

#endif // TOKENS_H