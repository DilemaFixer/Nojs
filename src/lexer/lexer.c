#include "lexer.h"
#include "utils/arr.h"
#include "utils/logger.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

token_t *new_token(ttype type, size_t line , size_t column){
    token_t *token = (token_t*)malloc(sizeof(token_t));
    if(!type) elog("Error allocation memory for token_t struct");

    token->type = type;
    token->value.number = 0.0;
    token->line = line;
    token->column = column;
    return token;
}

token_t *new_number_token(ttype type , size_t line , size_t column, double number){
    token_t *token = new_token(type , line , column);
    token->value.number = number;
    return token;
}

void free_token(token_t *token){
    if(token)
        free(token);
}

bool is_system_symbol(char c){
    return c == '.';
}

char *take_word(const char *code){
    if(!code) elog("Can't take word from NULL ptr on code string");
    if(!*code) elog("Can't take word from empty code string");

    size_t copy_count = 0;

    while(*code != '\n'){
        if(is_system_symbol(*c) && copy_count)
        copy_count
    }
}

arr_t *tokenize(const char *code){
    if(!code) elog("Can't parse code to tokens , ptr on code is NULL");
    if(!*code) elog("Can't parse code to tokens , code is empty string");

    arr_t *tokens = new_arr(1);
    size_t line = 1;
    size_t column = 0;

    while(*code){
        if(*code == '\n'){
            line++;
            column = 0;
            code++;
            continue;
        }
        

    }

    arr_push(tokens , new_token(END , line , column));
    return tokens;
}
