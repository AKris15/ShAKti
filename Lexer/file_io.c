#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>

// Read file into a wide character buffer
wchar_t *readFile(const char *filename) {
    FILE *file = fopen(filename, "r, ccs=UTF-8");
    if (!file) {
        wprintf(L"Error: Unable to open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    wchar_t *buffer = (wchar_t *)malloc((length + 1) * sizeof(wchar_t));
    if (!buffer) {
        wprintf(L"Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }

    size_t index = 0;
    wint_t ch;
    while ((ch = fgetwc(file)) != WEOF) {
        buffer[index++] = (wchar_t)ch;
    }
    buffer[index] = L'\0';

    fclose(file);
    return buffer;
}