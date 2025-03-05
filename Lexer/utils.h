#ifndef UTILS_H
#define UTILS_H

#include <wchar.h>

// Function prototypes
int isKeyword(const wchar_t *word);
int isBooleanLiteral(const wchar_t *word);
int isSanskritAlpha(wchar_t c);
int isDevanagariDigit(wchar_t c);

#endif // UTILS_H