#ifndef UTILS_H
#define UTILS_H

#include <wchar.h>

// Function prototypes for utility functions

// Check if a string is a keyword in the language
int isKeyword(const wchar_t *word);

// Check if a string is a boolean literal (सत्य or असत्य)
int isBooleanLiteral(const wchar_t *word);

// Check if a character is a Sanskrit alphabet character
int isSanskritAlpha(wchar_t c);

// Check if a character is a Devanagari digit (०-९)
int isDevanagariDigit(wchar_t c);

#endif // UTILS_H