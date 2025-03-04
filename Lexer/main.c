#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "file_io.h"
#include "Lexer.h"

int main(int argc, char *argv[]) {
    setlocale(LC_CTYPE, "hi_IN.UTF-8");
    if (argc < 2) {
        wprintf(L"Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    wchar_t *program = readFile(argv[1]);
    if (program) {
        tokenize(program);
        free(program);
    } else {
        wprintf(L"Error in reading file!\n");
    }
    return 0;
}