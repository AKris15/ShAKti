#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>

#define LINE_INIT_CAPACITY 1024

typedef struct {
    char *chars;
    size_t size;
    size_t capacity;
} Line;

static void line_grow(Line *line, size_t n)
{
    size_t new_capacity = line->capacity;

    assert(new_capacity >= line->size);
    while (new_capacity - line->size < n) {
        if (new_capacity == 0) {
            new_capacity = LINE_INIT_CAPACITY;
        } else {
            new_capacity *= 2;
        }
    }

    if (new_capacity != line->capacity) {
        line->chars = realloc(line->chars, new_capacity);
        line->capacity = new_capacity;
    }
}

void line_append_text(Line *line, const char *text)
{
    line_append_text_sized(line, text, strlen(text));
}

void line_append_text_sized(Line *line, const char *text, size_t text_size)
{
    size_t col = line->size;
    line_insert_text_sized_before(line, text, text_size, &col);
}

void line_insert_text_sized_before(Line *line, const char *text, size_t text_size, size_t *col)
{
    if (*col > line->size) {
        *col = line->size;
    }

    line_grow(line, text_size);

    memmove(line->chars + *col + text_size,
            line->chars + *col,
            line->size - *col);
    memcpy(line->chars + *col, text, text_size);
    line->size += text_size;
    *col += text_size;
}

void line_insert_text_before(Line *line, const char *text, size_t *col)
{
    line_insert_text_sized_before(line, text, strlen(text), col);
}

void line_backspace(Line *line, size_t *col)
{
    if (*col > line->size) {
        *col = line->size;
    }

    if (*col > 0 && line->size > 0) {
        memmove(line->chars + *col - 1,
                line->chars + *col,
                line->size - *col);
        line->size -= 1;
        *col -= 1;
    }
}

void line_delete(Line *line, size_t *col)
{
    if (*col > line->size) {
        *col = line->size;
    }

    if (*col < line->size && line->size > 0) {
        memmove(line->chars + *col,
                line->chars + *col + 1,
                line->size - *col);
        line->size -= 1;
    }
}
