#ifndef LEXER_H
#define LEXER_H

#include "Tokens.h"  // For Token and TokenType definitions
#include <wchar.h>   // For wide character support

// Main tokenization function - processes input text and identifies tokens
void tokenize(const wchar_t *input);

// Helper function prototypes (used internally in lexer.c)
// These handle specific token types during lexical analysis
void handleComment(const wchar_t *input, int *i, int isMultiLine);
void handleOperator(const wchar_t *input, int *i);
void handleSpecialSymbol(const wchar_t *input, int *i);
void handleString(const wchar_t *input, int *i);
void handleCharLiteral(const wchar_t *input, int *i);
void handleNumber(const wchar_t *input, int *i);
void handleIdentifier(const wchar_t *input, int *i, int *isVariable, int *isClassVariable, int *isFunction);

// Global arrays to track declared identifiers (extern declarations)
// These are defined in lexer.c and used to determine token types
extern wchar_t variables[100][100];       // Regular variables
extern int variable_count;                // Count of declared variables
extern wchar_t functions[100][100];       // Function names
extern int function_count;                // Count of declared functions
extern wchar_t class_variables[100][100];       // Class variables
extern int class_variable_count;                // Count of declared Class variables              // Count of declared functions

// Declaration of utility functions used in lexer.c
int isFunctionDeclared(const wchar_t *word);
int isVariableDeclared(const wchar_t *word);

#endif // LEXER_H