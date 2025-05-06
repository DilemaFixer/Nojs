#ifndef ENVER_H
#define ENVER_H

#include <stdbool.h>

typedef enum value_type {
    VAL_NUMBER,
    VAL_STRING,
    VAL_BOOLEAN,
    VAL_NULL,
    VAL_ARRAY,
    VAL_OBJECT,
    VAL_FUNCTION
} value_type;

typedef struct value_t value_t;
typedef struct environment_t environment_t;

typedef struct value_t {
    const char* name;
    char* string;
    bool boolean;
    double number;
    bool isconst;
    union {
        struct {
            arr_t *elements
        } array;

        struct {
            ast_node *declaration;
            environment_t *env;
        } func;

        struct {
            arr_t *props;
        } struct;
    };
} value_t

typedef struct environment_t {
    arr_t *values;
} environment_t;

environment_t *new_env();
void free_env(environment_t *env);
environment_t *copy_env(environment_t *env);
void push_all_const(environment_t *from , environment_t *to);

void push_value_to_env(environment_t *env , value_t *value);
value_t *get_value_from_env(environment_t *env , const char* name);
void update_value(environment_t *env , value_t *new_value , const char *name);

value_t *create_empty_value();
value_t *create_string_value(char *string);
value_t *create_number_value(double number);
value_t *create_boolean_value(bool boolean);
value_t *create_array_value(arr_t *values);
value_t *create_func_value(ast_node *decl , environment_t *env);
value_t *create_struct_value(arr_t *props , const char* name);
void free_value(value_t *value);

char* value_to_string(value_t *value);

#endif
