#ifndef AST_PARSER_H
#define AST_PARSER_H

#include <stdbool.h>
#include "ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"

extern lexer_t *lexer_g;
extern size_t current_token;

bool current_token_is(ttype type);
bool next_token_is(ttype type);
bool prev_token_is(ttype type);
token_t *peek_current_token();
token_t *peek_next_token();
token_t *peek_prev_token();
void tskip();

ast_node *parse_program();
ast_node *parse_statement();
ast_node *parse_expression();
ast_node *parse_equality();
ast_node *parse_comparison();
ast_node *parse_term();
ast_node *parse_factor();
ast_node *parse_unary();
ast_node *parse_primary();

#endif
