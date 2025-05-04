#ifndef AST_H
#define AST_H

#include <stddef.h>
#include <stdbool.h>
#include "../utils/arr.h"

typedef enum ast_type {
    // Literal values
    AST_NUMBER,          // Numeric literal
    AST_STRING,          // String literal
    AST_BOOLEAN,         // Boolean literal
    AST_NULL,            // Null value
    
    // Variables
    AST_IDENTIFIER,      // Variable reference
    AST_VAR_DECLARATION, // Variable declaration
    AST_CONST_DECLARATION, // Constant declaration
    AST_ASSIGNMENT,      // Assignment operation
    
    // Binary operations
    AST_BINARY_OP,       // Binary operation (arithmetic, comparison, logical)
    
    // Unary operations
    AST_UNARY_OP,        // Unary operation (negation, logical not)
    
    // Control flow
    AST_IF,              // If statement
    AST_IF_ELSE,         // If-else statement
    AST_LOOP,            // Loop statement
    AST_NEXT,            // Next statement (continue)
    AST_STOP,            // Stop statement (break)
    
    // Functions
    AST_FUNCTION_DECLARATION, // Function declaration
    AST_FUNCTION_CALL,   // Function call
    AST_RETURN,          // Return statement
    
    // Built-in functions
    AST_PRINT,           // Print statement
    AST_TAKE,            // Take statement (input)
    
    // Compound structures
    AST_BLOCK,           // Block of statements
    AST_ARRAY,           // Array literal
    AST_ARRAY_ACCESS,    // Array element access
    AST_PROPERTY_ACCESS, // Object property access
    
    // Program
    AST_PROGRAM          // Top-level program node
} ast_type;

typedef enum binary_op_type {
    // Arithmetic
    OP_ADD,          // +
    OP_SUBTRACT,     // -
    OP_MULTIPLY,     // *
    OP_DIVIDE,       // /
    
    // Comparison
    OP_EQUALS,       // ==
    OP_NOT_EQUALS,   // !=
    OP_GREATER,      // >
    OP_LESS,         // 
    OP_GREATER_EQUAL, // >=
    OP_LESS_EQUAL,   // <=
    
    // Logical
    OP_AND,          // &&
    OP_OR            // ||
} binary_op_type;

typedef enum unary_op_type {
    OP_NEGATE,       // -
    OP_NOT           // !
} unary_op_type;

typedef struct ast_node {
    ast_type type;
    
    union {
        // For AST_NUMBER
        struct {
            double value;
        } number;
        
        // For AST_STRING
        struct {
            char* value;
        } string;
        
        // For AST_BOOLEAN
        struct {
            bool value;
        } boolean;
        
        // For AST_IDENTIFIER
        struct {
            char* name;
        } identifier;
        
        // For AST_VAR_DECLARATION, AST_CONST_DECLARATION
        struct {
            char* name;
            struct ast_node* initializer; // Can be NULL
        } var_declaration;
        
        // For AST_ASSIGNMENT
        struct {
            struct ast_node* left;  // Target (identifier or property access)
            struct ast_node* right; // Value
        } assignment;
        
        // For AST_BINARY_OP
        struct {
            binary_op_type op;
            struct ast_node* left;
            struct ast_node* right;
        } binary_op;
        
        // For AST_UNARY_OP
        struct {
            unary_op_type op;
            struct ast_node* operand;
        } unary_op;
        
        // For AST_IF
        struct {
            struct ast_node* condition;
            struct ast_node* body;
        } if_statement;
        
        // For AST_IF_ELSE
        struct {
            struct ast_node* condition;
            struct ast_node* if_body;
            struct ast_node* else_body;
        } if_else_statement;
        
        // For AST_LOOP
        struct {
            struct ast_node* condition; // Can be NULL for infinite loop
            struct ast_node* body;
        } loop;
        
        // For AST_FUNCTION_DECLARATION
        struct {
            char* name;
            arr_t *parameters; // Array of AST_IDENTIFIER nodes
            struct ast_node* body;
        } function_declaration;
        
        // For AST_FUNCTION_CALL, AST_PRINT, AST_TAKE
        struct {
            struct ast_node* callee; // Function name (AST_IDENTIFIER) or expression
            struct ast_node** arguments;
            size_t argument_count;
        } function_call;
        
        // For AST_RETURN
        struct {
            struct ast_node* value; // Can be NULL
        } return_statement;
        
        // For AST_BLOCK
        struct {
            arr_t *stmts;
        } block;
        
        // For AST_ARRAY
        struct {
            struct ast_node** elements;
            size_t element_count;
        } array;
        
        // For AST_ARRAY_ACCESS
        struct {
            struct ast_node* array;
            struct ast_node* index;
        } array_access;
        
        // For AST_PROPERTY_ACCESS
        struct {
            struct ast_node* object;
            char* property;
        } property_access;
        
        // For AST_PROGRAM
        struct {
            struct ast_node** statements;
            size_t statement_count;
        } program;
    };
} ast_node;

ast_node* create_ast_node(ast_type type);
ast_node* create_number_node(double value);
ast_node* create_string_node(const char* value);
ast_node* create_boolean_node(bool value);
ast_node* create_null_node();
ast_node* create_identifier_node(const char* name);
ast_node* create_var_declaration_node(const char* name, ast_node* initializer);
ast_node* create_const_declaration_node(const char* name, ast_node* initializer);
ast_node* create_assignment_node(ast_node* left, ast_node* right);
ast_node* create_binary_op_node(binary_op_type op, ast_node* left, ast_node* right);
ast_node* create_unary_op_node(unary_op_type op, ast_node* operand);
ast_node* create_if_node(ast_node* condition, ast_node* body);
ast_node* create_if_else_node(ast_node* condition, ast_node* if_body, ast_node* else_body);
ast_node* create_loop_node(ast_node* condition, ast_node* body);
ast_node* create_next_node();
ast_node* create_stop_node();
ast_node* create_function_declaration_node(const char* name, arr_t *params, ast_node* body);
ast_node* create_function_call_node(ast_node* callee, ast_node** arguments, size_t argument_count);
ast_node* create_return_node(ast_node* value);
ast_node* create_print_node(ast_node** arguments, size_t argument_count);
ast_node* create_take_node(ast_node** arguments, size_t argument_count);
ast_node* create_block_node(arr_t *stmts);
ast_node* create_array_node(ast_node** elements, size_t element_count);
ast_node* create_array_access_node(ast_node* array, ast_node* index);
ast_node* create_property_access_node(ast_node* object, const char* property);
ast_node* create_program_node(ast_node** statements, size_t statement_count);

void free_ast_node(ast_node* node);

#endif
