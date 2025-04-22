#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include <stdbool.h>
#include "utils/arr.h"
#include "utils/queue.h"
#include "token.h"

typedef struct token_t token_t;

typedef struct lexer_t {
    const char *source;
    size_t length;
    size_t position;
    size_t line;
    size_t column;
    arr_t *tokens;
} lexer_t;

lexer_t *new_lexer(const char *source);
void free_lexer(lexer_t *lexer);

bool lexer_is_end(lexer_t *lexer);
char lexer_peak(lexer_t *lexer);
char lexer_peak_next(lexer_t *lexer);
char lexer_advance(lexer_t *lexer);

token_t *get_next_token(lexer_t *lexer);
token_t *get_previouse_token(lexer_t *lexer);

bool is_keyword(lexer_t *lexer , const char *word);
void skip_whitespace(lexer_t *lexer);
double parse_number(lexer_t *lexer);
bool is_ssytem(char c);
token_t *check_keyword(lexer_t *lexer);

lexer_t *tokenize(const char *code);

#endif
