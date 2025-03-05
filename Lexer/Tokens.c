#include "Tokens.h"

// Create a token with the specified type and value
// Parameters:
//   - type: The token type from TokenType enum
//   - value: Wide character string containing the token's text
// Returns: A new Token structure with the provided values
Token createToken(TokenType type, const wchar_t *value) {
    Token token;
    token.type = type;
    
    // Copy the value to the token, ensuring it doesn't exceed buffer size
    // wcsncpy is the wide character version of strncpy
    wcsncpy(token.value, value, sizeof(token.value) / sizeof(wchar_t) - 1);
    
    // Ensure null termination (wchar_t version of string terminator)
    token.value[sizeof(token.value) / sizeof(wchar_t) - 1] = L'\0';
    
    return token;
}