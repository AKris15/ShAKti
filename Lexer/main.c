#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "file_io.h"
#include "Lexer.h"

int main(int argc, char *argv[]) {
    // Try multiple locales
    if (setlocale(LC_ALL, "hi_IN.UTF-8") == NULL) {
        if (setlocale(LC_ALL, "en_US.UTF-8") == NULL) {
            if (setlocale(LC_ALL, "C.UTF-8") == NULL) {
                // Fall back to system default
                setlocale(LC_ALL, "");
            }
        }
    }
    
    printf("Current locale: %s\n", setlocale(LC_ALL, NULL));
    
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    wchar_t *program = readFile(argv[1]);
    if (program) {
        for (int i = 0; i < 10 && program[i] != L'\0'; i++) {
            printf("%lc", program[i]);
        }
        printf("\n");
        
        tokenize(program);
        free(program);
    } else {
        printf("Error in reading file!\n");
    }
    return 0;
}