#include "ast_parser.h"
#include "ast.h"
#include "lexer/lexer.h"
#include "utils/logger.h"
#include "ast_printer.h"
#include <stdio.h>

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

const char* ast_type_to_string(ast_type type) {
    switch(type) {
        case AST_NUMBER: return "NUMBER";
        case AST_STRING: return "STRING";
        case AST_BOOLEAN: return "BOOLEAN";
        case AST_NULL: return "NULL";
        case AST_IDENTIFIER: return "IDENTIFIER";
        case AST_VAR_DECLARATION: return "VAR_DECLARATION";
        case AST_CONST_DECLARATION: return "CONST_DECLARATION";
        case AST_ASSIGNMENT: return "ASSIGNMENT";
        case AST_BINARY_OP: return "BINARY_OP";
        case AST_UNARY_OP: return "UNARY_OP";
        case AST_IF: return "IF";
        case AST_IF_ELSE: return "IF_ELSE";
        case AST_LOOP: return "LOOP";
        case AST_NEXT: return "NEXT";
        case AST_STOP: return "STOP";
        case AST_FUNCTION_DECLARATION: return "FUNCTION_DECLARATION";
        case AST_FUNCTION_CALL: return "FUNCTION_CALL";
        case AST_RETURN: return "RETURN";
        case AST_PRINT: return "PRINT";
        case AST_TAKE: return "TAKE";
        case AST_BLOCK: return "BLOCK";
        case AST_ARRAY: return "ARRAY";
        case AST_ARRAY_ACCESS: return "ARRAY_ACCESS";
        case AST_PROPERTY_ACCESS: return "PROPERTY_ACCESS";
        case AST_PROGRAM: return "PROGRAM";
        default: return "UNKNOWN";
    }
}

const char* binary_op_to_string(binary_op_type op) {
    switch(op) {
        case OP_ADD: return "+";
        case OP_SUBTRACT: return "-";
        case OP_MULTIPLY: return "*";
        case OP_DIVIDE: return "/";
        case OP_EQUALS: return "==";
        case OP_NOT_EQUALS: return "!=";
        case OP_GREATER: return ">";
        case OP_LESS: return "<";
        case OP_GREATER_EQUAL: return ">=";
        case OP_LESS_EQUAL: return "<=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        default: return "UNKNOWN";
    }
}

const char* unary_op_to_string(unary_op_type op) {
    switch(op) {
        case OP_NEGATE: return "-";
        case OP_NOT: return "!";
        default: return "UNKNOWN";
    }
}

void print_ast(ast_node* node, int indent) {
    if (!node) {
        print_indent(indent);
        printf("(null)\n");
        return;
    }
    
    print_indent(indent);
    printf("(%s", ast_type_to_string(node->type));
    
    switch(node->type) {
        case AST_NUMBER:
            printf(" %.2f)\n", node->number.value);
            break;
            
        case AST_STRING:
            printf(" \"%s\")\n", node->string.value);
            break;
            
        case AST_BOOLEAN:
            printf(" %s)\n", node->boolean.value ? "true" : "false");
            break;
            
        case AST_NULL:
            printf(")\n");
            break;
            
        case AST_IDENTIFIER:
            printf(" %s)\n", node->identifier.name);
            break;
            
        case AST_VAR_DECLARATION:
        case AST_CONST_DECLARATION:
            printf(" %s\n", node->var_declaration.name);
            print_indent(indent + 1);
            printf("initializer:\n");
            print_ast(node->var_declaration.initializer, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_ASSIGNMENT:
            printf("\n");
            print_indent(indent + 1);
            printf("left:\n");
            print_ast(node->assignment.left, indent + 2);
            print_indent(indent + 1);
            printf("right:\n");
            print_ast(node->assignment.right, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_BINARY_OP:
            printf(" %s\n", binary_op_to_string(node->binary_op.op));
            print_indent(indent + 1);
            printf("left:\n");
            print_ast(node->binary_op.left, indent + 2);
            print_indent(indent + 1);
            printf("right:\n");
            print_ast(node->binary_op.right, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_UNARY_OP:
            printf(" %s\n", unary_op_to_string(node->unary_op.op));
            print_indent(indent + 1);
            printf("operand:\n");
            print_ast(node->unary_op.operand, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_IF:
            printf("\n");
            print_indent(indent + 1);
            printf("condition:\n");
            print_ast(node->if_statement.condition, indent + 2);
            print_indent(indent + 1);
            printf("body:\n");
            print_ast(node->if_statement.body, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_IF_ELSE:
            printf("\n");
            print_indent(indent + 1);
            printf("condition:\n");
            print_ast(node->if_else_statement.condition, indent + 2);
            print_indent(indent + 1);
            printf("if_body:\n");
            print_ast(node->if_else_statement.if_body, indent + 2);
            print_indent(indent + 1);
            printf("else_body:\n");
            print_ast(node->if_else_statement.else_body, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_LOOP:
            printf("\n");
            if(node->loop.condition) {
                print_indent(indent + 1);
                printf("condition:\n");
                print_ast(node->loop.condition, indent + 2);
            }
            print_indent(indent + 1);
            printf("body:\n");
            print_ast(node->loop.body, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_FUNCTION_DECLARATION:
            printf(" %s\n", node->function_declaration.name);
            print_indent(indent + 1);
            printf("parameters:\n");
            for(size_t i = 0; i < node->function_declaration.parameters->count; i++) {
                print_ast(node->function_declaration.parameters->items[i], indent + 2);
            }
            print_indent(indent + 1);
            printf("body:\n");
            print_ast(node->function_declaration.body, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_FUNCTION_CALL:
            printf("\n");
            print_indent(indent + 1);
            printf("callee:\n");
            print_ast(node->function_call.callee, indent + 2);
            print_indent(indent + 1);
            printf("arguments:\n");
            for(size_t i = 0; i < node->function_call.argument_count; i++) {
                print_ast(node->function_call.arguments[i], indent + 2);
            }
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_RETURN:
            printf("\n");
            if(node->return_statement.value) {
                print_indent(indent + 1);
                printf("value:\n");
                print_ast(node->return_statement.value, indent + 2);
            }
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_BLOCK:
            printf("\n");
            for(size_t i = 0; i < node->block.stmts->count; i++) {
                print_ast(node->block.stmts->items[i], indent + 1);
            }
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_ARRAY:
            printf("\n");
            for(size_t i = 0; i < node->array.element_count; i++) {
                print_ast(node->array.elements[i], indent + 1);
            }
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_ARRAY_ACCESS:
            printf("\n");
            print_indent(indent + 1);
            printf("array:\n");
            print_ast(node->array_access.array, indent + 2);
            print_indent(indent + 1);
            printf("index:\n");
            print_ast(node->array_access.index, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_PROPERTY_ACCESS:
            printf(" %s\n", node->property_access.property);
            print_indent(indent + 1);
            printf("object:\n");
            print_ast(node->property_access.object, indent + 2);
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_PROGRAM:
            printf("\n");
            for(size_t i = 0; i < node->program.statement_count; i++) {
                print_ast(node->program.statements[i], indent + 1);
            }
            print_indent(indent);
            printf(")\n");
            break;
            
        case AST_NEXT:
        case AST_STOP:
            printf(")\n");
            break;
            
        default:
            printf(" UNKNOWN)\n");
            break;
    }
}
