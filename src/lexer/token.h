#ifndef TOKENS_H
#define TOKENS_H

#include "lexer.h"
#include "utils/arr.h"
#include <stdlib.h>

typedef struct lexer_t lexer_t;

typedef enum ttype {
    NUMBER,
    LPARENT,
    RPARENT,
    MUL,
    DIV,
    PLUS,
    MINUS,
    END
} ttype; // token type

typedef struct token_t {
    ttype type;
    char *lexem;
    size_t line;
    size_t column;
    union {
        double number;
    } value;
} token_t;

token_t *new_token(lexer_t *lexer , ttype type);
token_t *new_number_token(lexer_t *lexer , ttype type, double number);
void free_token(token_t *token);

void log_token(token_t *token);

#endif
