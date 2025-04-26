#include "token.h"
#include "lexer.h"
#include "utils/arr.h"
#include "utils/logger.h"
#include <stdlib.h>

token_t *new_token(lexer_t *lexer, ttype type) {
  if (!lexer)
    elog("Can't create token with NULL ptr on lexer");

  token_t *token = (token_t *)malloc(sizeof(token_t));
  if (!token)
    elog("Error allocation memory for token_t struct");

  token->type = type;
  token->line = lexer->line;
  token->column = lexer->column;
  return token;
}

token_t *new_number_token(lexer_t *lexer, ttype type, double number) {
  token_t *token = new_token(lexer, type);
  token->value.number = number;
  return token;
}

void free_token(token_t *token) {
  if (!token)
    return;

  free(token->lexem);
  free(token);
}

void log_token(token_t *token){
    if(!token) elog("Can't print token by NULL ptr on it");
    switch(token->type){
        case MUL : dlog("MUL"); break;
        case DIV : dlog("DIV"); break;
        case PLUS : dlog("PLUS"); break;
        case MINUS : dlog("MINUS"); break;
        case LPARENT : dlog("LPARENT"); break;
        case RPARENT : dlog("RPARENT"); break;
        case NUMBER : dlog("NUMBER : %f" , token->value.number); break;
        case END : dlog("END"); break;
        default : dlog("Indefine token type");
    }
}
