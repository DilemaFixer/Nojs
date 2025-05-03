#include <stdbool.h>
#include <string.h>
#include "ast.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../utils/logger.h"

lexer_t *lexer_g = NULL;
size_t current_token = 0;

void syntax_error(const char* format, ...) {
    char msg[256];
    va_list args;
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);
    
    elog("[%zu:%zu] Syntax error : %s", lexer_g->line, lexer_g->column, msg);
}

bool current_token_is(ttype type){
    if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
     if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");
    token_t* token = (token_t*)lexer_g->tokens->items[current_token];
    return token->type == type;
}

bool next_token_is(ttype type){
    if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
    if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");
    
    if(current_token + 1 >= lexer_g->tokens->count)
        elog("Can't get next token in global lexer, is end");

    token_t* token = (token_t*)lexer_g->tokens->items[current_token+1];
    return token->type == type;
}

bool prev_token_is(ttype type){
    if(!lexer_g) elog("Can't check what type of current token , global lexer is null");    
    if(current_token >= lexer_g->tokens->count)
        elog("Global current token index was out of range in global tokens arr");
    
    if(current_token == 0)
        elog("Global current token index point on first element , can't get prev type");

    token_t* token = (token_t*)lexer_g->tokens->items[current_token-1];
    return token->type == type;
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

ast_node *parse_program(){
    arr_t *statments = new_arr(1);
    while(!current_token_is(END)){
        ast_node* stmt = parse_statement();
        arr_push(statments , stmt);
    }
    return create_program_node((ast_node**)statments->items , statments->count);
}

ast_node *parse_statement(){
    if(current_token_is(LET) || current_token_is(CONST)){
        return parse_var_declaration();
    } else if(current_token_is(IF)){
        return parse_if_statement();
    } else if(current_token_is(LOOP)){
        return parse_loop_statement();
    } else if(current_token_is(FUNCTION)){
        return parse_function_declaration();
    } else if(current_token_is(RETURN)){
        return parse_return_statement();
    } else if(current_token_is(PRINT)){
        return parse_print_statement();
    } else if(current_token_is(TAKE)){
        return parse_take_statement();
    } else if(current_token_is(NEXT)){
        tskip();
        if(current_token_is(SEMICOLON)) tskip();
        return create_next_node();
    } else if(current_token_is(STOP)){
        tskip();
        if(current_token_is(SEMICOLON)) tskip();
        return create_stop_node();
    } else if(current_token_is(LBRACE)){
        return parse_block();
    } else {
        ast_node *expr = parse_expression();
        if(current_token_is(SEMICOLON)) tskip();
        return expr;
    }
}

ast_node *parse_var_declaration(){
    bool is_const = current_token_is(CONST);
    tskip();
    if(!current_token_is(IDENTIFIER)){
        if(is_const){
            syntax_error("Have 'const' but have no var name");
        } else{
            syntax_error("Have 'let' but have no var name");
        }
    }

    token_t *token = peek_current_token();
    char* name = token->value.string;
    tskip();

    if(!current_token_is(ASSIGN))
        syntax_error("Have declaratin var %s , but not assign value to it" , name);

    tskip();
    ast_node *set_expr = parse_expression();

    if(!current_token_is(SEMICOLON))
        syntax_error("Not exist ';'");
    tskip();

    if(is_const)
        return create_const_declaration_node(name , set_expr);
    else
        return create_var_declaration_node(name , set_expr);
}

ast_node *parse_if_statement(){
    tskip();
    if(!current_token_is(LPARENT))
        syntax_error("After 'if' must go '('");
    tskip();

    ast_node *condition = parse_expression();

    if(!current_token_is(RPARENT))
        syntax_error("Have '(' expression , but have no ')'");
    tskip();

    if(!current_token_is(LBRACE))
        syntax_error("Not exist '{' , if (expression) <- here");
    ast_node *if_block = parse_block();
    
    ast_node *else_block = NULL;
    if(current_token_is(ELSE)) {
        tskip();
        if(!current_token_is(LBRACE))
            syntax_error("after 'else' must go '{' ");
        else_block = parse_block();
    }

    if(!else_block)
        return create_if_node(condition , if_block);
    else 
        return create_if_else_node(condition , if_block , else_block);
}

ast_node *parse_loop_statement(){
    tskip();
    if(!current_token_is(LPARENT))
        syntax_error("After 'loop' must go '('");
    tskip();

    ast_node *condition = parse_expression();

    if(!current_token_is(RPARENT))
        syntax_error("Have loop with '(' , expression , but have no ')'");
    tskip();

    if(!current_token_is(LBRACE))
        syntax_error("After loop(expression) . <- here must go '{'");
    ast_node *loop_block = parse_block();

    return create_loop_node(condition , loop_block);
}

ast_node *parse_function_declaration() {
    tskip();
    if(!current_token_is(IDENTIFIER))
        syntax_error("have function declaration without name");
    token_t *token = current_token();
    char *func_name = token->value.string;
    tskip();
    
    if(!current_token_is(LPARENT)) 
        syntax_error("After func name %s must go '(' params ')'" , func_name);
    tskip();
    
    arr_t *params = new_arr(1);
    
    if(current_token_is(RPARENT)) {
        tskip();
    } else {
        while(1) {
            if(!current_token_is(IDENTIFIER))
                syntax_error("In function %s must go only names" , func_name);
            
            ast_node *param = parse_expression();
            arr_push(params, param);
            tskip();
            
            if(current_token_is(RPARENT)) {
                tskip();
                break;
            }
            
            if(!current_token_is(COMMA))
                syntax_error("Have something after param(s) name(s), but expected comma in func %s", func_name);
            tskip();
        }
    }
    
    if(!current_token_is(LBRACE))
        syntax_error("After func func_name(params) . <- here , must go '{'");

    ast_node *func_body = parse_block();
    ast_node *node = create_function_node(func_name, params , func_body);
    return node;
}

ast_node *parse_return_statement(){
    tskip();
    ast_node *value = parse_expression();
    return create_return_node(return_body);
}

ast_node *parse_print_statement(){
    elog("peint keyword is not implemented");
    return NULL;
}

ast_node *parse_take_statement(){
    elog("peint keyword is not implemented");
    return NULL;
}

ast_node *parse_block(){
    tskip();
    arr_t *stmts = new_arr(1);
    
    while(!current_token_is(RBRACE)){
        ast_node *stm = parse_statement();
        arr_push(stmts , stm);
    }

    return create_block_node(stmts);
}

ast_node *parse_function_call(const char *name){

}
ast_node *parse_array_literal();

ast_node *parse_expression();
ast_node *parse_equality();
ast_node *parse_comparison();
ast_node *parse_term();
ast_node *parse_factor();
ast_node *parse_unary();
ast_node *parse_primary();
