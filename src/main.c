#include "lexer/lexer.h"

int main(void) {
  char *code = "3.5 * (5.75 + 4)";
  lexer_t *lexer = tokenize(code);
  arr_t *tokens = lexer->tokens;

  for(size_t i = 0 ; i < tokens->count;i++){
      log_token(tokens->items[i]);
  }

  return 0;
}
