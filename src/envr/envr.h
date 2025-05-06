#ifndef ENVER_H
#define ENVER_H

#include <stdbool.h>
#include "../utils/arr.h"
#include "../ast/ast.h"

typedef struct value_t value_t;
typedef struct environment_t environment_t;
typedef value_t (*native_function_ptr)(value_t** args, size_t arg_count);

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

typedef struct value_t {
    value_type type;
    const char* name;
    bool isconst;
    
    union {
        double number;
        char* string;
        bool boolean;
        
        struct {
            value_t** elements;
            size_t element_count;
        } array;

        struct {
            ast_node* declaration;
            environment_t* env;
        } func;

        struct {
            native_function_ptr function;
            const char* name;
        } native_func;
        
        struct {
            char** field_names;
            value_t* field_values;
            size_t field_count;
            const char* struct_name;
        } structure;
    };
} value_t;

typedef struct environment_t {
    struct environment_t* parent;
    char** keys;
    value_t* values;
    bool* constants;
    size_t count;
    size_t capacity;
} environment_t;

environment_t* new_env(environment_t* parent);
void free_env(environment_t* env);
void env_define(environment_t* env, const char* name, value_t value, bool is_const);
value_t env_get(environment_t* env, const char* name);
bool env_assign(environment_t* env, const char* name, value_t value);

value_t create_empty_value();
value_t create_string_value(const char* string);
value_t create_number_value(double number);
value_t create_boolean_value(bool boolean);
value_t create_null_value();
value_t create_array_value(value_t** elements, size_t element_count);
value_t create_function_value(ast_node* decl, environment_t* env);
value_t create_native_function_value(native_function_ptr func, const char* name);
value_t create_struct_value(const char* struct_name, char** field_names, value_t* field_values, size_t field_count);

value_t get_struct_field(value_t structure, const char* field_name);
void set_struct_field(value_t* structure, const char* field_name, value_t value);

void free_value(value_t value);
char* value_to_string(value_t value);

#endif
