#include "lexer/lexer.h"
#include "lexer/token.h"

int main(void) {
  char *code = "//3.5 * (5.75 + 4) \n 2 + 5";
  lexer_t *lexer = tokenize(code);
  
  for(size_t i = 0 ; i < lexer->tokens->count; i++){
      log_token((token_t*)lexer->tokens->items[i]);
  }
    
  printf("Tokens count : %zu \n" , lexer->tokens->count);
  return 0;
}
