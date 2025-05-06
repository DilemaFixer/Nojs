#include "envr.h"
#include "../utils/arr.h"
#include "../utils/logger.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

environment_t* new_env(environment_t* parent) {
    environment_t* env = (environment_t*)malloc(sizeof(environment_t));
    if (!env) 
        elog("Error allocating memory for environment");
    
    env->parent = parent;
    env->capacity = 8;   
    env->count = 0;
    
    env->keys = (char**)malloc(env->capacity * sizeof(char*));
    env->values = (value_t*)malloc(env->capacity * sizeof(value_t));
    env->constants = (bool*)malloc(env->capacity * sizeof(bool));
    
    if (!env->keys || !env->values || !env->constants)
        elog("Error allocating memory for environment storage");
    
    return env;
}

void free_env(environment_t* env) {
    if (!env)
        return;
    
    for (size_t i = 0; i < env->count; i++) {
        free(env->keys[i]);
        free_value(env->values[i]);
    }
    
    free(env->keys);
    free(env->values);
    free(env->constants);
    free(env);
}

static void env_resize(environment_t* env) {
    size_t new_capacity = env->capacity * 2;
    
    char** new_keys = (char**)realloc(env->keys, new_capacity * sizeof(char*));
    value_t* new_values = (value_t*)realloc(env->values, new_capacity * sizeof(value_t));
    bool* new_constants = (bool*)realloc(env->constants, new_capacity * sizeof(bool));
    
    if (!new_keys || !new_values || !new_constants)
        elog("Error reallocating memory for environment");
    
    env->keys = new_keys;
    env->values = new_values;
    env->constants = new_constants;
    env->capacity = new_capacity;
}

void env_define(environment_t* env, const char* name, value_t value, bool is_const) {
    if (!env) 
        elog("Can't define variable in null environment");
    
    if (env->count >= env->capacity) {
        env_resize(env);
    }
    
    for (size_t i = 0; i < env->count; i++) {
        if (strcmp(env->keys[i], name) == 0) {
            if (env->constants[i]) {
                elog("Cannot reassign to constant '%s'", name);
            }
            free_value(env->values[i]);
            env->values[i] = value;
            return;
        }
    }
    
    env->keys[env->count] = strdup(name);
    env->values[env->count] = value;
    env->constants[env->count] = is_const;
    env->count++;
}

value_t env_get(environment_t* env, const char* name) {
    if (!env) 
        elog("Can't get value from null environment");
    
    for (size_t i = 0; i < env->count; i++) {
        if (strcmp(env->keys[i], name) == 0) {
            return env->values[i];
        }
    }
    
    if (env->parent) {
        return env_get(env->parent, name);
    }
    
    // Не найдено
    elog("Undefined variable '%s'", name);
    return create_null_value(); 
}

bool env_assign(environment_t* env, const char* name, value_t value) {
    if (!env) 
        elog("Can't assign value to null environment");
    
    for (size_t i = 0; i < env->count; i++) {
        if (strcmp(env->keys[i], name) == 0) {
            if (env->constants[i]) {
                elog("Cannot reassign to constant '%s'", name);
            }
            free_value(env->values[i]);
            env->values[i] = value;
            return true;
        }
    }
    
    if (env->parent) {
        return env_assign(env->parent, name, value);
    }
    
    return false;
}

value_t create_empty_value() {
    value_t v;
    v.type = VAL_NULL;
    v.isconst = false;
    v.name = NULL;
    return v;
}

value_t create_string_value(const char* string) {
    value_t v;
    v.type = VAL_STRING;
    v.string = strdup(string);
    v.isconst = false;
    v.name = NULL;
    return v;
}

value_t create_number_value(double number) {
    value_t v;
    v.type = VAL_NUMBER;
    v.number = number;
    v.isconst = false;
    v.name = NULL;
    return v;
}

value_t create_boolean_value(bool boolean) {
    value_t v;
    v.type = VAL_BOOLEAN;
    v.boolean = boolean;
    v.isconst = false;
    v.name = NULL;
    return v;
}

value_t create_null_value() {
    value_t v;
    v.type = VAL_NULL;
    v.isconst = false;
    v.name = NULL;
    return v;
}

value_t create_array_value(value_t** elements, size_t element_count) {
    value_t v;
    v.type = VAL_ARRAY;
    v.isconst = false;
    v.name = NULL;
    
    v.array.elements = (value_t**)malloc(element_count * sizeof(value_t*));
    if (!v.array.elements && element_count > 0) 
        elog("Error allocating memory for array elements");
    
    for (size_t i = 0; i < element_count; i++) {
        v.array.elements[i] = (value_t*)malloc(sizeof(value_t));
        *(v.array.elements[i]) = *elements[i];
    }
    
    v.array.element_count = element_count;
    return v;
}

value_t create_function_value(ast_node* decl, environment_t* env) {
    value_t v;
    v.type = VAL_FUNCTION;
    v.isconst = false;
    v.name = NULL;
    v.func.declaration = decl;
    v.func.env = env;
    return v;
}

value_t create_native_function_value(native_function_ptr func, const char* name) {
    value_t v;
    v.type = VAL_NATIVE_FUNCTION;
    v.isconst = false;
    v.name = NULL;
    v.native_func.function = func;
    v.native_func.name = name;
    return v;
}

value_t create_struct_value(const char* struct_name, char** field_names, value_t* field_values, size_t field_count) {
    value_t v;
    v.type = VAL_STRUCT;
    v.isconst = false;
    v.name = NULL;
    
    v.structure.struct_name = strdup(struct_name);
    v.structure.field_count = field_count;
    
    v.structure.field_names = (char**)malloc(field_count * sizeof(char*));
    v.structure.field_values = (value_t*)malloc(field_count * sizeof(value_t));
    
    if ((!v.structure.field_names || !v.structure.field_values) && field_count > 0)
        elog("Error allocating memory for struct fields");
    
    for (size_t i = 0; i < field_count; i++) {
        v.structure.field_names[i] = strdup(field_names[i]);
        v.structure.field_values[i] = field_values[i];
    }
    
    return v;
}

value_t get_struct_field(value_t structure, const char* field_name) {
    if (structure.type != VAL_STRUCT) {
        elog("Cannot access field of non-structure value");
    }
    
    for (size_t i = 0; i < structure.structure.field_count; i++) {
        if (strcmp(structure.structure.field_names[i], field_name) == 0) {
            return structure.structure.field_values[i];
        }
    }
    
    elog("Structure '%s' has no field named '%s'", 
         structure.structure.struct_name, field_name);
    return create_null_value();
}

void set_struct_field(value_t* structure, const char* field_name, value_t value) {
    if (structure->type != VAL_STRUCT) {
        elog("Cannot set field of non-structure value");
    }
    
    for (size_t i = 0; i < structure->structure.field_count; i++) {
        if (strcmp(structure->structure.field_names[i], field_name) == 0) {
            free_value(structure->structure.field_values[i]);
            structure->structure.field_values[i] = value;
            return;
        }
    }
    
    size_t new_count = structure->structure.field_count + 1;
    structure->structure.field_names = (char**)realloc(
        structure->structure.field_names, new_count * sizeof(char*));
    structure->structure.field_values = (value_t*)realloc(
        structure->structure.field_values, new_count * sizeof(value_t));
    
    if (!structure->structure.field_names || !structure->structure.field_values)
        elog("Error reallocating memory for new struct field");
    
    structure->structure.field_names[structure->structure.field_count] = strdup(field_name);
    structure->structure.field_values[structure->structure.field_count] = value;
    structure->structure.field_count = new_count;
}

void free_value(value_t value) {
    switch (value.type) {
        case VAL_STRING:
            free(value.string);
            break;
        
        case VAL_ARRAY:
            for (size_t i = 0; i < value.array.element_count; i++) {
                free_value(*value.array.elements[i]);
                free(value.array.elements[i]);
            }
            free(value.array.elements);
            break;
        
        case VAL_STRUCT:
            for (size_t i = 0; i < value.structure.field_count; i++) {
                free(value.structure.field_names[i]);
                free_value(value.structure.field_values[i]);
            }
            free(value.structure.field_names);
            free(value.structure.field_values);
            free((void*)value.structure.struct_name);
            break;
            
        default:
            break;
    }
}

char* value_to_string(value_t value) {
    char buffer[1024];
    
    switch (value.type) {
        case VAL_NUMBER:
            if (floor(value.number) == value.number) {
                snprintf(buffer, sizeof(buffer), "%.0f", value.number);
            } else {
                snprintf(buffer, sizeof(buffer), "%g", value.number);
            }
            break;
            
        case VAL_STRING:
            snprintf(buffer, sizeof(buffer), "\"%s\"", value.string);
            break;
            
        case VAL_BOOLEAN:
            snprintf(buffer, sizeof(buffer), "%s", value.boolean ? "true" : "false");
            break;
            
        case VAL_NULL:
            snprintf(buffer, sizeof(buffer), "null");
            break;
            
        case VAL_ARRAY: {
            char temp[1024] = "[";
            size_t len = 1;
            
            for (size_t i = 0; i < value.array.element_count; i++) {
                char* element_str = value_to_string(*value.array.elements[i]);
                len += snprintf(temp + len, sizeof(temp) - len, "%s%s", 
                              i > 0 ? ", " : "", element_str);
                free(element_str);
            }
            
            snprintf(buffer, sizeof(buffer), "%s]", temp);
            break;
        }
            
        case VAL_FUNCTION:
            snprintf(buffer, sizeof(buffer), "<function %s>", 
                    value.func.declaration->function_declaration.name);
            break;
            
        case VAL_NATIVE_FUNCTION:
            snprintf(buffer, sizeof(buffer), "<native function %s>", value.native_func.name);
            break;
            
        case VAL_STRUCT: {
            char temp[1024];
            snprintf(temp, sizeof(temp), "%s {", value.structure.struct_name);
            size_t len = strlen(temp);
            
            for (size_t i = 0; i < value.structure.field_count; i++) {
                char* field_value = value_to_string(value.structure.field_values[i]);
                len += snprintf(temp + len, sizeof(temp) - len, "%s%s: %s", 
                              i > 0 ? ", " : " ", value.structure.field_names[i], field_value);
                free(field_value);
            }
            
            snprintf(buffer, sizeof(buffer), "%s }", temp);
            break;
        }
            
        default:
            snprintf(buffer, sizeof(buffer), "<unknown>");
            break;
    }
    
    return strdup(buffer);
}
