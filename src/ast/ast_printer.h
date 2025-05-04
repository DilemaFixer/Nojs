#ifndef AST_PRNT_H
#define AST_PRNT_H

void print_ast(ast_node* node, int indent);
const char* unary_op_to_string(unary_op_type op);
const char* binary_op_to_string(binary_op_type op);
const char* ast_type_to_string(ast_type type);
void print_indent(int indent);

#endif
