#ifndef FILEIO_H
#define FILEIO_H

#include <wchar.h>

// Function prototypes
// readFile - Reads a file and converts its contents to wide characters (wchar_t)
// This allows handling of Unicode text including Devanagari script
wchar_t *readFile(const char *filename);

#endif // FILE_IO_H