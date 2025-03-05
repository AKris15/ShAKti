#include "utils.h"

// Keywords in Sanskrit - These are reserved words in the language
const wchar_t *keywords[] = {
    L"पूर्ण", L"यदि", L"अन्यथा", 
    L"चक्र", L"से", L"तक", L"लेख", 
    L"प्रवे", L"कक्षा", L"वा यदि", 
    L"न", L"कर्म" , NULL
};

// Boolean literals in Sanskrit (true and false)
const wchar_t *boolean_literals[] = {
    L"सत्य", L"असत्य", NULL
};

// Operators supported by the language
const wchar_t *operators[] = {
    L"+", L"-", L"*", L"/", L"=", L">", L"<", L">=", L"<=", L"==", L"!=", L"&&", L";", L"!",
    L"?", L"+=", L"-=", L"*=", L"/="
};

// Special symbols used in the language
const wchar_t *special_symbols[] = {
    L"(", L")", L"{", L"}", L"[", L"]", L",", L":", L"|", NULL
};

// Check if a string is a keyword
// Returns 1 if it's a keyword, 0 otherwise
int isKeyword(const wchar_t *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (wcscmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a string is a boolean literal (सत्य or असत्य)
// Returns 1 if it's a boolean literal, 0 otherwise
int isBooleanLiteral(const wchar_t *word) {
    for (int i = 0; boolean_literals[i] != NULL; i++) {
        if (wcscmp(word, boolean_literals[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Check if a character is a Sanskrit alphabet character
// This checks if the character is in the Devanagari Unicode range
// Returns 1 if it's a Sanskrit character, 0 otherwise
int isSanskritAlpha(wchar_t c) {
    // Unicode ranges for Devanagari script
    return (c >= L'\u0900' && c <= L'\u097F') || (c >= L'\uA8E0' && c <= L'\uA8FF');
}

// Check if a character is a Devanagari digit (०-९)
// Returns 1 if it's a Devanagari digit, 0 otherwise
int isDevanagariDigit(wchar_t c) {
    // Unicode range for Devanagari digits (० to ९)
    return (c >= L'\u0966' && c <= L'\u096F');
}