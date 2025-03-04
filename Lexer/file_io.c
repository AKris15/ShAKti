#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>

// Read file into a wide character buffer
wchar_t *readFile(const char *filename) {
    // Open file in binary mode to avoid text transformations
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (fileSize <= 0) {
        fprintf(stderr, "Error: Empty or invalid file\n");
        fclose(file);
        return NULL;
    }

    // Read file into a char buffer
    char *buffer = (char *)malloc(fileSize + 1);
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed!\n");
        fclose(file);
        return NULL;
    }
    
    size_t bytesRead = fread(buffer, 1, fileSize, file);
    buffer[bytesRead] = '\0';
    fclose(file);
    
    // Check for UTF-8 BOM and skip if present
    unsigned char *ubuffer = (unsigned char *)buffer;
    int offset = 0;
    if (bytesRead >= 3 && ubuffer[0] == 0xEF && ubuffer[1] == 0xBB && ubuffer[2] == 0xBF) {
        offset = 3; // Skip UTF-8 BOM
    }
    
    // Convert UTF-8 to wide characters
    // First calculate required buffer size
    size_t wideLength = mbstowcs(NULL, buffer + offset, 0);
    if (wideLength == (size_t)-1) {
        fprintf(stderr, "Error: Invalid multibyte sequence in file\n");
        free(buffer);
        return NULL;
    }
    
    wchar_t *wideBuffer = (wchar_t *)malloc((wideLength + 1) * sizeof(wchar_t));
    if (!wideBuffer) {
        fprintf(stderr, "Memory allocation failed for wide buffer!\n");
        free(buffer);
        return NULL;
    }
    
    // Perform the actual conversion
    mbstowcs(wideBuffer, buffer + offset, wideLength + 1);
    free(buffer);
    
    return wideBuffer;
}