# Nojs 🚀

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Language](https://img.shields.io/badge/language-C-orange.svg)
![Status](https://img.shields.io/badge/status-in_progress-yellow.svg)
![Version](https://img.shields.io/badge/version-0.1.0-green.svg)

A lightweight JavaScript-like language implementation in C, featuring structures and dynamic method addition. This project is designed for Unix-like operating systems.

## 💻 Installation

### Unix-based Systems (Linux, macOS)

```bash
# Create directory for the project
mkdir -p nojs
cd nojs

# Clone the repository
git clone https://github.com/DilemaFixer/Nojs.git .

# Build the project
./b && ./nojs
```

## 🔧 API and Usage Examples

### AST (Abstract Syntax Tree)

The AST module handles the creation and manipulation of the syntax tree for the Nojs language.

#### Core Structures

```c
// AST Node Types
typedef enum ast_type {
    AST_NUMBER,          // Numeric literal
    AST_STRING,          // String literal
    AST_BOOLEAN,         // Boolean literal
    AST_NULL,            // Null value
    AST_IDENTIFIER,      // Variable reference
    AST_VAR_DECLARATION, // Variable declaration
    AST_CONST_DECLARATION, // Constant declaration
    AST_ASSIGNMENT,      // Assignment operation
    AST_BINARY_OP,       // Binary operation
    AST_UNARY_OP,        // Unary operation
    AST_IF,              // If statement
    AST_IF_ELSE,         // If-else statement
    AST_LOOP,            // Loop statement
    AST_NEXT,            // Next statement (continue)
    AST_STOP,            // Stop statement (break)
    AST_FUNCTION_DECLARATION, // Function declaration
    AST_FUNCTION_CALL,   // Function call
    AST_RETURN,          // Return statement
    AST_PRINT,           // Print statement
    AST_TAKE,            // Take statement (input)
    AST_BLOCK,           // Block of statements
    AST_ARRAY,           // Array literal
    AST_ARRAY_ACCESS,    // Array element access
    AST_PROPERTY_ACCESS, // Object property access
    AST_PROGRAM          // Top-level program node
} ast_type;

// Binary operations
typedef enum binary_op_type {
    OP_ADD, OP_SUBTRACT, OP_MULTIPLY, OP_DIVIDE,  // Arithmetic
    OP_EQUALS, OP_NOT_EQUALS, OP_GREATER, OP_LESS, OP_GREATER_EQUAL, OP_LESS_EQUAL,  // Comparison
    OP_AND, OP_OR  // Logical
} binary_op_type;

// AST Node
typedef struct ast_node {
    ast_type type;
    
    union {
        struct { double value; } number;
        struct { char* value; } string;
        struct { bool value; } boolean;
        struct { char* name; } identifier;
        // ... other node-specific structures
    };
} ast_node;
```

#### Functions

```c
// Create various types of AST nodes
ast_node* create_number_node(double value);
ast_node* create_string_node(const char* value);
ast_node* create_boolean_node(bool value);
ast_node* create_null_node();
ast_node* create_identifier_node(const char* name);
ast_node* create_var_declaration_node(const char* name, ast_node* initializer);
ast_node* create_const_declaration_node(const char* name, ast_node* initializer);
ast_node* create_binary_op_node(binary_op_type op, ast_node* left, ast_node* right);
// ... other node creation functions

// Free an AST node and all its children
void free_ast_node(ast_node* node);
```

#### Usage Example

```c
#include "ast/ast.h"

ast_node* create_simple_program() {
    // Create variable declaration: let x = 5;
    ast_node* x_decl = create_var_declaration_node("x", create_number_node(5.0));
    
    // Create binary operation: x + 10
    ast_node* x_plus_10 = create_binary_op_node(
        OP_ADD,
        create_identifier_node("x"),
        create_number_node(10.0)
    );
    
    // Create program with these statements
    ast_node* statements[2] = {x_decl, x_plus_10};
    return create_program_node(statements, 2);
}
```

Required headers: `ast/ast.h`

### Lexer

The lexer module is responsible for tokenizing the source code into a sequence of tokens.

#### Core Structures

```c
// Token Types
typedef enum ttype {
    NUMBER, STRING, BOOLEAN, NULL_VAL,
    IDENTIFIER, 
    LPARENT, RPARENT, LBRACE, RBRACE, LBRACKET, RBRACKET,
    MUL, DIV, PLUS, MINUS,
    EQUALS, NOT_EQUALS, GREATER, LESS, GREATER_EQUAL, LESS_EQUAL,
    AND, OR, NOT,
    ASSIGN,
    IF, ELSE, LOOP, NEXT, STOP,
    FUNCTION, RETURN,
    PRINT, TAKE,
    SEMICOLON, COLON, COMMA, DOT,
    LET, CONST,
    END
} ttype;

// Token structure
typedef struct token_t {
    ttype type;
    size_t line;
    size_t column;
    union {
        double number;
        char* string;
        bool boolean;
    } value;
} token_t;

// Lexer structure
typedef struct lexer_t {
    const char* source;
    size_t length;
    size_t position;
    size_t line;
    size_t column;
    arr_t* tokens;
} lexer_t;
```

#### Functions

```c
// Create a new lexer for the given source code
lexer_t* new_lexer(const char* source);

// Free resources used by the lexer
void free_lexer(lexer_t* lexer);

// Tokenize the entire source code
lexer_t* tokenize(const char* code);

// Create tokens of different types
token_t* new_token(lexer_t* lexer, ttype type);
token_t* new_number_token(lexer_t* lexer, ttype type, double number);
token_t* new_string_token(lexer_t* lexer, ttype type, char* string);
token_t* new_boolean_token(lexer_t* lexer, ttype type, bool boolean);
```

#### Usage Example

```c
#include "lexer/lexer.h"

void process_code(const char* code) {
    lexer_t* lexer = tokenize(code);
    
    // Print all tokens
    for (size_t i = 0; i < lexer->tokens->count; i++) {
        token_t* token = (token_t*)lexer->tokens->items[i];
        log_token(token);
    }
    
    free_lexer(lexer);
}
```

Required headers: `lexer/lexer.h`, `lexer/token.h`

### Environment

The environment module provides variable storage and scope management.

#### Core Structures

```c
// Value types
typedef enum value_type {
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_NULL,
    VAL_ARRAY,
    VAL_STRUCT,
    VAL_FUNCTION,
    VAL_NATIVE_FUNCTION
} value_type;

// Value structure
typedef struct value_t {
    value_type type;
    const char* name;
    bool isconst;
    
    union {
        double number;
        char* string;
        bool boolean;
        // ... other value-specific structures
    };
} value_t;

// Environment structure
typedef struct environment_t {
    struct environment_t* parent;
    char** keys;
    value_t* values;
    bool* constants;
    size_t count;
    size_t capacity;
} environment_t;
```

#### Functions

```c
// Create a new environment with an optional parent
environment_t* new_env(environment_t* parent);

// Free an environment and all its values
void free_env(environment_t* env);

// Define a variable or constant in the environment
void env_define(environment_t* env, const char* name, value_t value, bool is_const);

// Get a value from the environment
value_t env_get(environment_t* env, const char* name);

// Assign a value to an existing variable
bool env_assign(environment_t* env, const char* name, value_t value);

// Create values of different types
value_t create_number_value(double number);
value_t create_string_value(const char* string);
value_t create_boolean_value(bool boolean);
value_t create_null_value();
value_t create_array_value(value_t** elements, size_t element_count);
value_t create_function_value(ast_node* decl, environment_t* env);
value_t create_struct_value(const char* struct_name, char** field_names, value_t* field_values, size_t field_count);

// Structure operations
value_t get_struct_field(value_t structure, const char* field_name);
void set_struct_field(value_t* structure, const char* field_name, value_t value);

// Free a value and its resources
void free_value(value_t value);

// Convert a value to a string representation
char* value_to_string(value_t value);
```

#### Usage Example

```c
#include "envr/envr.h"

void environment_demo() {
    // Create a global environment
    environment_t* global = new_env(NULL);
    
    // Define variables
    env_define(global, "x", create_number_value(42), false);
    env_define(global, "PI", create_number_value(3.14159), true);
    
    // Create a local environment
    environment_t* local = new_env(global);
    env_define(local, "y", create_number_value(10), false);
    
    // Access variables
    value_t x = env_get(local, "x");  // Found in parent
    value_t pi = env_get(local, "PI"); // Found in parent
    value_t y = env_get(local, "y");  // Found in local
    
    // Free resources
    free_env(local);
    free_env(global);
}
```

Required headers: `envr/envr.h`

### Utility Modules

#### Array Utility

Dynamic array implementation for storing arbitrary pointers.

```c
// Array structure
typedef struct {
    void** items;
    size_t count;
    size_t capacity;
} arr_t;

// Create a new array with initial capacity
arr_t* new_arr(size_t initial_capacity);

// Free the array and its resources
void free_arr(arr_t* arr);

// Add an element to the array
bool arr_push(arr_t* arr, void* element);

// Get an element from the array
void* arr_get(arr_t* arr, size_t index);

// Set an element in the array
bool arr_set(arr_t* arr, size_t index, void* element);

// Remove an element from the array
bool arr_remove(arr_t* arr, size_t index);

// Get the number of elements in the array
size_t arr_size(arr_t* arr);

// Resize the array to a new capacity
bool arr_resize(arr_t* arr, size_t new_capacity);
```

#### Logger Utility

Flexible logging system with support for different levels and handlers.

```c
// Log levels
typedef enum log_level {
   DEBUG,
   INFO,
   WARN,
   ERR,
   NONE
} log_level;

// Log message structure
typedef struct log_message {
    const char* file;
    int line;
    log_level level;
    const char* level_str;
    const char* color;
    char* time_str;
    char* formatted_message;
} log_message;

// Log to the specified level
void logger(const char* file, int line, log_level level, const char* format, ...);

// Convenience macros
#define dlog(format, ...) logger(__FILE__, __LINE__, DEBUG, format, ##__VA_ARGS__)
#define ilog(format, ...) logger(__FILE__, __LINE__, INFO, format, ##__VA_ARGS__)
#define wlog(format, ...) logger(__FILE__, __LINE__, WARN, format, ##__VA_ARGS__)
#define elog(format, ...) // Error logging with exit
```

#### Usage Example

```c
#include "utils/arr.h"
#include "utils/logger.h"

void utils_demo() {
    // Array usage
    arr_t* numbers = new_arr(10);
    
    // Store integers as pointers (for demonstration)
    int a = 1, b = 2, c = 3;
    arr_push(numbers, &a);
    arr_push(numbers, &b);
    arr_push(numbers, &c);
    
    // Logging usage
    dlog("Created array with %zu elements", numbers->count);
    ilog("Array capacity: %zu", numbers->capacity);
    
    // Cleanup
    free_arr(numbers);
}
```

Required headers: `utils/arr.h`, `utils/logger.h`
