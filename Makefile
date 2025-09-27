.PHONY: all lexer namo parser clean

all: lexer namo parser

lexer:
	$(MAKE) -C Lexer

namo:
	$(MAKE) -C "Namo (Text-Editor)"

PARSER_DIR := Parser
PARSER_SRCS := $(PARSER_DIR)/file_io.c $(PARSER_DIR)/utils.c $(PARSER_DIR)/Tokens.c
PARSER_OBJS := $(PARSER_SRCS:.c=.o)
PARSER_LIB := $(PARSER_DIR)/libparser.a

parser: $(PARSER_LIB)

$(PARSER_LIB): $(PARSER_OBJS)
	ar rcs $@ $^

$(PARSER_DIR)/%.o: $(PARSER_DIR)/%.c
	$(CC) -Wall -Wextra -std=c11 -O2 -c $< -o $@

clean:
	$(MAKE) -C Lexer clean
	$(MAKE) -C "Namo (Text-Editor)" clean
	$(RM) $(PARSER_OBJS) $(PARSER_LIB)
