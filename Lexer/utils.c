#include "utils.h"

// Keywords in Sanskrit
const wchar_t *keywords[] = {
    L"पूर्ण", L"यदि", L"अन्यथा", 
    L"चक्र", L"से", L"तक", L"लेख", 
    L"प्रवे", L"कक्षा", L"वा यदि", 
    L"न", L"कर्म" , NULL
};

// Boolean literals
const wchar_t *boolean_literals[] = {
    L"सत्य", L"असत्य", NULL
};

// Operators
const wchar_t *operators[] = {
    L"+", L"-", L"*", L"/", L"=", L">", L"<", L">=", L"<=", L"==", L"!=", L"&&", L";", L"!",
    L"?", L"+=", L"-=", L"*=", L"/="
};

// Special symbols
const wchar_t *special_symbols[] = {
    L"(", L")", L"{", L"}", L"[", L"]", L",", L":", L"|", NULL
};

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

// to check if a character is a Devanagari digit
int isDevanagariDigit(wchar_t c) {
    return (c >= L'\u0966' && c <= L'\u096F'); // ० to ९
}