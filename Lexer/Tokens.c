#include "Tokens.h"

// Create a token
Token createToken(TokenType type, const wchar_t *value) {
    Token token;
    token.type = type;
    wcsncpy(token.value, value, sizeof(token.value) / sizeof(wchar_t) - 1);
    token.value[sizeof(token.value) / sizeof(wchar_t) - 1] = L'\0';
    return token;
}