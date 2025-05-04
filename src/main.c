#include "lexer/lexer.h"
#include "lexer/token.h"

#include "ast/ast_parser.h"
#include "ast/ast_printer.h"

#include "utils/logger.h"

int main(void) {
 char *test_code = "{\n"
                      "    let x = 5;\n"
                      "    const y = 10;\n"
                      "    let sum = x + y;\n"
                      "    if (sum > 10) {\n"
                      "        let result = sum * 2;\n"
                      "    } else {\n"
                      "        let result = sum;\n"
                      "    }\n"
                      "}";
  dlog("Will parse %s \n" , test_code);
  lexer_t *lexer = tokenize(test_code);
  
  dlog("Tokens list : ");
  for(size_t i = 0 ; i < lexer->tokens->count; i++){
      log_token((token_t*)lexer->tokens->items[i]);
  }
    
  lexer_g = lexer;
  current_token = 0;

  ast_node *prog = parse_program();
  print_ast(prog , 0);
  return 0;
}

/* 
 *
 *
 *
*/
