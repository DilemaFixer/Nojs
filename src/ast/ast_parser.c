#include <stdbool.h>
#include "ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/logger.h"

lexer_t *lexer_g = NULL;
size_t current_token = 0;

bool current_token_is(ttype type){
    if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
     if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");

    return lexer_g->tokens->items[current_token]->type == type;
}

bool next_token_is(ttype type){
    if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
    if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");
    
    if(current_token + 1 >= lexer_g->tokens->count)
        elog("Can't get next token in global lexer, is end");

    return lexer_g->tokens->items[current_token+1]->type == type;
}

bool prev_token_is(ttype type){
    if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
    if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");
    
    if(current_token == 0)
        elog("Global current token index point on first element , can't get prev type");

    return lexer_g->tokens->items[current_token-1]->type == type;
}

token_t *peek_current_token(){
     if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
     if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");

    return lexer_g->tokens->items[current_token];
}

token_t *peek_next_token(){
  if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
    if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");
    
    if(current_token + 1 >= lexer_g->tokens->count)
        elog("Can't get next token in global lexer, is end");

    return lexer_g->tokens->items[current_token+1];
}

token_t *peek_prev_token(){
    if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
    if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");
    
    if(current_token == 0)
        elog("Global current token index point on first element , can't get prev type");

    return lexer_g->tokens->items[current_token-1];

}

void tskip(){
    if (current_token + 1 >= lexer_g->tokens->count)
        elog("Cannot skip token - would go out of range");
    current_token++;
}
