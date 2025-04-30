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

  free(token);
}

token_t *new_boolean_token(lexer_t *lexer , ttype type , bool boolean){
    token_t *token = new_token(lexer , type);
    token->value.boolean = boolean;
    return token;
}

token_t *new_string_token(lexer_t *lexer , ttype type , char* string){
    token_t *token = new_token(lexer , type);
    token->value.string = strdup(string);
    return token;
}

void log_token(token_t *token){
    if(!token) elog("Can't print token by NULL ptr on it");
    switch(token->type){
        case MUL    : dlog("MUL"); break;
        case DIV    : dlog("DIV"); break;
        case PLUS   : dlog("PLUS"); break;
        case MINUS  : dlog("MINUS"); break;
        case LPARENT : dlog("LPARENT"); break;
        case RPARENT : dlog("RPARENT"); break;
        case NUMBER  : dlog("NUMBER : %f" , token->value.number); break;
        case END     : dlog("END"); break;
        
        case STRING   : dlog("STRING : %s", token->value.string); break;
        case BOOLEAN  : dlog("BOOLEAN : %s", token->value.boolean ? "true" : "false"); break;
        case NULL_VAL : dlog("NULL_VAL"); break;
        
        case LET      : dlog("LET"); break;
        case CONST    : dlog("CONST"); break;
        
        case EQUALS        : dlog("EQUALS"); break;
        case NOT_EQUALS    : dlog("NOT_EQUALS"); break;
        case GREATER       : dlog("GREATER"); break;
        case LESS          : dlog("LESS"); break;
        case GREATER_EQUAL : dlog("GREATER_EQUAL"); break;
        case LESS_EQUAL    : dlog("LESS_EQUAL"); break;
        
        case AND    : dlog("AND"); break;
        case OR     : dlog("OR"); break;
        case NOT    : dlog("NOT"); break;
        
        case ASSIGN : dlog("ASSIGN"); break;
        
        case IF     : dlog("IF"); break;
        case ELSE   : dlog("ELSE"); break;
        case LOOP   : dlog("LOOP"); break;
        case NEXT   : dlog("NEXT"); break;
        case STOP   : dlog("STOP"); break;
        
        case FUNCTION : dlog("FUNCTION"); break;
        case RETURN   : dlog("RETURN"); break;
        
        case PRINT   : dlog("PRINT"); break;
        case TAKE    : dlog("TAKE"); break;
        
        case LBRACE    : dlog("LBRACE"); break;
        case RBRACE    : dlog("RBRACE"); break;
        case LBRACKET  : dlog("LBRACKET"); break;
        case RBRACKET  : dlog("RBRACKET"); break;
        case SEMICOLON : dlog("SEMICOLON"); break;
        case COLON     : dlog("COLON"); break;
        case COMMA     : dlog("COMMA"); break;
        case DOT       : dlog("DOT"); break;
        
        case IDENTIFIER : dlog("IDENTIFIER : %s", token->value.string); break;
        
        default : dlog("Indefine token type");
    }
}
