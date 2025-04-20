#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>
#include "utils/arr.h"

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
    size_t line;
    size_t column;
    union {
        double number;
    } value;
} token_t;

token_t *new_token(ttype type , size_t line , size_t column);
token_t *new_number_token(ttype type , size_t line , size_t column , double number);
void free_token(token_t *token);

arr_t *tokenize(const char *code);

#endif
