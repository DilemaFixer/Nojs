#include "ast.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "utils/logger.h"

ast_node* create_ast_node(ast_type type) {
    ast_node *node = (ast_node*)malloc(sizeof(ast_node));
    if(!node) elog("Error allocation memory for ast node with type %d", type);
    node->type = type;
    return node;
}

ast_node* create_number_node(double value) {
    ast_node* node = create_ast_node(AST_NUMBER);
    node->number.value = value;
    return node;
}

ast_node* create_string_node(const char* value) {
    ast_node* node = create_ast_node(AST_STRING);
    node->string.value = strdup(value);
    if(!node->string.value) elog("Error allocating memory for string value");
    return node;
}

ast_node* create_boolean_node(bool value) {
    ast_node* node = create_ast_node(AST_BOOLEAN);
    node->boolean.value = value;
    return node;
}

ast_node* create_null_node() {
    return create_ast_node(AST_NULL);
}

ast_node* create_identifier_node(const char* name) {
    ast_node* node = create_ast_node(AST_IDENTIFIER);
    node->identifier.name = strdup(name);
    if(!node->identifier.name) elog("Error allocating memory for identifier name");
    return node;
}

ast_node* create_var_declaration_node(const char* name, ast_node* initializer) {
    ast_node* node = create_ast_node(AST_VAR_DECLARATION);
    node->var_declaration.name = strdup(name);
    if(!node->var_declaration.name) elog("Error allocating memory for variable declaration name");
    node->var_declaration.initializer = initializer;
    return node;
}

ast_node* create_const_declaration_node(const char* name, ast_node* initializer) {
    ast_node* node = create_ast_node(AST_CONST_DECLARATION);
    node->var_declaration.name = strdup(name);
    if(!node->var_declaration.name) elog("Error allocating memory for constant declaration name");
    node->var_declaration.initializer = initializer;
    return node;
}

ast_node* create_assignment_node(ast_node* left, ast_node* right) {
    ast_node* node = create_ast_node(AST_ASSIGNMENT);
    node->assignment.left = left;
    node->assignment.right = right;
    return node;
}

ast_node* create_binary_op_node(binary_op_type op, ast_node* left, ast_node* right) {
    ast_node* node = create_ast_node(AST_BINARY_OP);
    node->binary_op.op = op;
    node->binary_op.left = left;
    node->binary_op.right = right;
    return node;
}

ast_node* create_unary_op_node(unary_op_type op, ast_node* operand) {
    ast_node* node = create_ast_node(AST_UNARY_OP);
    node->unary_op.op = op;
    node->unary_op.operand = operand;
    return node;
}

ast_node* create_if_node(ast_node* condition, ast_node* body) {
    ast_node* node = create_ast_node(AST_IF);
    node->if_statement.condition = condition;
    node->if_statement.body = body;
    return node;
}

ast_node* create_if_else_node(ast_node* condition, ast_node* if_body, ast_node* else_body) {
    ast_node* node = create_ast_node(AST_IF_ELSE);
    node->if_else_statement.condition = condition;
    node->if_else_statement.if_body = if_body;
    node->if_else_statement.else_body = else_body;
    return node;
}

ast_node* create_loop_node(ast_node* condition, ast_node* body) {
    ast_node* node = create_ast_node(AST_LOOP);
    node->loop.condition = condition;
    node->loop.body = body;
    return node;
}

ast_node* create_next_node() {
    return create_ast_node(AST_NEXT);
}

ast_node* create_stop_node() {
    return create_ast_node(AST_STOP);
}

ast_node* create_function_declaration_node(const char* name, ast_node** parameters, size_t parameter_count, ast_node* body) {
    ast_node* node = create_ast_node(AST_FUNCTION_DECLARATION);
    node->function_declaration.name = strdup(name);
    if(!node->function_declaration.name) elog("Error allocating memory for function name");
    
    node->function_declaration.parameters = (ast_node**)malloc(parameter_count * sizeof(ast_node*));
    if(!node->function_declaration.parameters && parameter_count > 0) 
        elog("Error allocating memory for function parameters");
    
    for(size_t i = 0; i < parameter_count; i++) {
        node->function_declaration.parameters[i] = parameters[i];
    }
    
    node->function_declaration.parameter_count = parameter_count;
    node->function_declaration.body = body;
    return node;
}

ast_node* create_function_call_node(ast_node* callee, ast_node** arguments, size_t argument_count) {
    ast_node* node = create_ast_node(AST_FUNCTION_CALL);
    node->function_call.callee = callee;
    
    node->function_call.arguments = (ast_node**)malloc(argument_count * sizeof(ast_node*));
    if(!node->function_call.arguments && argument_count > 0) 
        elog("Error allocating memory for function call arguments");
    
    for(size_t i = 0; i < argument_count; i++) {
        node->function_call.arguments[i] = arguments[i];
    }
    
    node->function_call.argument_count = argument_count;
    return node;
}

ast_node* create_return_node(ast_node* value) {
    ast_node* node = create_ast_node(AST_RETURN);
    node->return_statement.value = value;
    return node;
}

ast_node* create_print_node(ast_node** arguments, size_t argument_count) {
    ast_node* identifier = create_identifier_node("print");
    return create_function_call_node(identifier, arguments, argument_count);
}

ast_node* create_take_node(ast_node** arguments, size_t argument_count) {
    ast_node* identifier = create_identifier_node("take");
    return create_function_call_node(identifier, arguments, argument_count);
}

ast_node* create_block_node(ast_node** statements, size_t statement_count) {
    ast_node* node = create_ast_node(AST_BLOCK);
    
    node->block.statements = (ast_node**)malloc(statement_count * sizeof(ast_node*));
    if(!node->block.statements && statement_count > 0) 
        elog("Error allocating memory for block statements");
    
    for(size_t i = 0; i < statement_count; i++) {
        node->block.statements[i] = statements[i];
    }
    
    node->block.statement_count = statement_count;
    return node;
}

ast_node* create_array_node(ast_node** elements, size_t element_count) {
    ast_node* node = create_ast_node(AST_ARRAY);
    
    node->array.elements = (ast_node**)malloc(element_count * sizeof(ast_node*));
    if(!node->array.elements && element_count > 0) 
        elog("Error allocating memory for array elements");
    
    for(size_t i = 0; i < element_count; i++) {
        node->array.elements[i] = elements[i];
    }
    
    node->array.element_count = element_count;
    return node;
}

ast_node* create_array_access_node(ast_node* array, ast_node* index) {
    ast_node* node = create_ast_node(AST_ARRAY_ACCESS);
    node->array_access.array = array;
    node->array_access.index = index;
    return node;
}

ast_node* create_property_access_node(ast_node* object, const char* property) {
    ast_node* node = create_ast_node(AST_PROPERTY_ACCESS);
    node->property_access.object = object;
    
    node->property_access.property = strdup(property);
    if(!node->property_access.property) 
        elog("Error allocating memory for property name");
    
    return node;
}

ast_node* create_program_node(ast_node** statements, size_t statement_count) {
    ast_node* node = create_ast_node(AST_PROGRAM);
    
    node->program.statements = (ast_node**)malloc(statement_count * sizeof(ast_node*));
    if(!node->program.statements && statement_count > 0) 
        elog("Error allocating memory for program statements");
    
    for(size_t i = 0; i < statement_count; i++) {
        node->program.statements[i] = statements[i];
    }
    
    node->program.statement_count = statement_count;
    return node;
}

void free_ast_node(ast_node* node) {
    if(!node) return;
    
    switch(node->type) {
        case AST_STRING:
            free(node->string.value);
            break;
        
        case AST_IDENTIFIER:
            free(node->identifier.name);
            break;
        
        case AST_VAR_DECLARATION:
        case AST_CONST_DECLARATION:
            free(node->var_declaration.name);
            if(node->var_declaration.initializer) 
                free_ast_node(node->var_declaration.initializer);
            break;
        
        case AST_ASSIGNMENT:
            free_ast_node(node->assignment.left);
            free_ast_node(node->assignment.right);
            break;
        
        case AST_BINARY_OP:
            free_ast_node(node->binary_op.left);
            free_ast_node(node->binary_op.right);
            break;
        
        case AST_UNARY_OP:
            free_ast_node(node->unary_op.operand);
            break;
        
        case AST_IF:
            free_ast_node(node->if_statement.condition);
            free_ast_node(node->if_statement.body);
            break;
        
        case AST_IF_ELSE:
            free_ast_node(node->if_else_statement.condition);
            free_ast_node(node->if_else_statement.if_body);
            free_ast_node(node->if_else_statement.else_body);
            break;
        
        case AST_LOOP:
            if(node->loop.condition) 
                free_ast_node(node->loop.condition);
            free_ast_node(node->loop.body);
            break;
        
        case AST_FUNCTION_DECLARATION:
            free(node->function_declaration.name);
            for(size_t i = 0; i < node->function_declaration.parameter_count; i++) {
                free_ast_node(node->function_declaration.parameters[i]);
            }
            free(node->function_declaration.parameters);
            free_ast_node(node->function_declaration.body);
            break;
        
        case AST_FUNCTION_CALL:
            free_ast_node(node->function_call.callee);
            for(size_t i = 0; i < node->function_call.argument_count; i++) {
                free_ast_node(node->function_call.arguments[i]);
            }
            free(node->function_call.arguments);
            break;
        
        case AST_RETURN:
            if(node->return_statement.value) 
                free_ast_node(node->return_statement.value);
            break;
        
        case AST_BLOCK:
            for(size_t i = 0; i < node->block.statement_count; i++) {
                free_ast_node(node->block.statements[i]);
            }
            free(node->block.statements);
            break;
        
        case AST_ARRAY:
            for(size_t i = 0; i < node->array.element_count; i++) {
                free_ast_node(node->array.elements[i]);
            }
            free(node->array.elements);
            break;
        
        case AST_ARRAY_ACCESS:
            free_ast_node(node->array_access.array);
            free_ast_node(node->array_access.index);
            break;
        
        case AST_PROPERTY_ACCESS:
            free_ast_node(node->property_access.object);
            free(node->property_access.property);
            break;
        
        case AST_PROGRAM:
            for(size_t i = 0; i < node->program.statement_count; i++) {
                free_ast_node(node->program.statements[i]);
            }
            free(node->program.statements);
            break;
            
        // Cases that don't require additional cleanup
        case AST_NUMBER:
        case AST_BOOLEAN:
        case AST_NULL:
        case AST_NEXT:
        case AST_STOP:
            break;
    }
    
    free(node);
}
