#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/logger.h"

int main(void) {
  char *code = "3.5 * (5.75 + 4) \" bla bla bla \" >= , blabla";
  dlog("Will parse %s \n" , code);
  lexer_t *lexer = tokenize(code);
  
  dlog("Tokens list : ");
  for(size_t i = 0 ; i < lexer->tokens->count; i++){
      log_token((token_t*)lexer->tokens->items[i]);
  }

  return 0;
}

/* 
 *
 *
 *
*/
