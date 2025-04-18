#ifndef LEXER_H
#define LEXER_H

#include <stdlib.h>

typed enum token_type {

} token_type;

typedef struct token_t {
    token_type type;
    size_t line;
    size_t offset;
    union { 
        double number;
        char *string;
    } value;
} token_t;



#endif 
