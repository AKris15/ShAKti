#ifndef LEXER_H
#define LEXER_H

#include "Tokens.h"  // For Token and TokenType definitions
#include <wchar.h>   // For wide character support

// Function prototypes
void tokenize(const wchar_t *input);

// Helper function prototypes (used internally in lexer.c)
void handleComment(const wchar_t *input, int *i, int isMultiLine);
void handleOperator(const wchar_t *input, int *i);
void handleSpecialSymbol(const wchar_t *input, int *i);
void handleString(const wchar_t *input, int *i);
void handleCharLiteral(const wchar_t *input, int *i);
void handleNumber(const wchar_t *input, int *i);
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction);

// Variable and function tracking (extern declarations)
extern wchar_t variables[100][100];
extern int variable_count;
extern wchar_t functions[100][100];
extern int function_count;

// Declaration of utility functions used in lexer.c
int isFunctionDeclared(const wchar_t *word);
int isVariableDeclared(const wchar_t *word);

#endif // LEXER_H