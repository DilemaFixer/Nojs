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
        case MUL : ilog("MUL");
        case DIV : ilog("DIV");
        case PLUS : ilog("PLUS");
        case MINUS : ilog("MINUS");
        case LPARENT : ilog("LPARENT");
        case RPARENT : ilog("RPARENT");
        case NUMBER : ilog("NUMBER : %f" , token->value.number);
        case END : ilog("END");
        default : ilog("Indefine token type");
    }
}
