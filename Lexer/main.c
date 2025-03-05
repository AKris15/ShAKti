#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include "file_io.h"
#include "Lexer.h"

int main(int argc, char *argv[]) {
    // Try to set locale for Unicode/Devanagari support
    // We try multiple locales in case some aren't available on the system
    if (setlocale(LC_ALL, "hi_IN.UTF-8") == NULL) {
        if (setlocale(LC_ALL, "en_US.UTF-8") == NULL) {
            if (setlocale(LC_ALL, "C.UTF-8") == NULL) {
                // Fall back to system default if specific locales aren't available
                setlocale(LC_ALL, "");
            }
        }
    }
    
    // Display the current locale for debugging
    printf("Current locale: %s\n", setlocale(LC_ALL, NULL));
    
    // Check for command line arguments
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Read the input file into a wide character buffer
    wchar_t *program = readFile(argv[1]);
    if (program) {
        printf("\n");
        
        // Process the input and identify tokens
        tokenize(program);
        
        // Clean up
        free(program);
    } else {
        printf("Error in reading file!\n");
    }
    return 0;
}