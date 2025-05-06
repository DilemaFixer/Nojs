#include "envr.h"
#include "../utils/arr.h"
#include "../utils/logger.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

environment_t *new_env(){
    environment_t *env = (environment_t*)malloc(sizeof(environment_t));
    if(!env) elog("Error allocation memory for enviroment");
    arr_t *values = new_arr(1);
    env->values = values;
    return env;
}

void free_env(environment_t *env){
    size_t v_count = env->values->count;
    arr_t *v = env->values;
    for(size_t i = 0; i < v_count; i++){
        free_value((value_t*)v->items[i]);
    }

    free_arr(v);
    free(env);
}

environment_t *copy_env(environment_t *env){
    if(!env) elog("Can't copy env from null ptr");

    environment_t *new_env = (environment_t*)malloc(sizeof(environment_t));
    if(!env) elog("Error allocation memory for copy enviroment");
    arr_t *values = new_arr(env->values->count == 0 ? 1 : env->values->count);
    new_env->values = values;

    if(env->values->count == 0)
        return new_env;
    
    memcpy(new_env->values->items , env->values->items , sizeof(value_t*) * env->values->count);
    new_env->values->count = env->values->count;
    return new_env;
}

void push_all_const(environment_t *from , environment_t *to){
    if(!from) elog("Can't copy consts values from enviroment one , ptr is null");
    if(!to) elog("Can't push consts values in two , ptr is null");
    
    size_t v_count = from->values->count;
    arr_t *values = from->values;
    arr_t *end = to->values; 

    for(size_t i = 0 ; i < v_count; i++){
        value_t *v = (value_t*)values->items[i];
        if(v->isconst){
            value_t *new_v = create_empty_value();
            memcpy(new_v , v , sizeof(value_t));
            arr_push(end , new_v);
        }
    }
}

void push_value_to_env(environment_t *env , value_t *value){
    if(!env) elog("Can't push value to null env");
    if(!value) elog("Can't push null value");

    arr_push(env->values , value);
}

value_t *get_value_from_env(environment_t *env , const char* name){
    if(!env) elog("Can't get value from null env");
    if(!name) elog("Can't get value by null name");

    size_t v_count = env->values->count;
    arr_t *values = env->values;

    for(size_t i = 0; i < v_count ; i++){
        if(strcmp(&values->items[i].name  , name) == 0)
            return &values->items[i];
    }

    return NULL;
}

void update_value(environment_t *env , value_t *new_value , const char *name){
    if(!env) elog("Can't update value from null env");
    if(!name) elog("Can't update value by null name");

    size_t v_count = env->values->count;
    arr_t *values = env->values;

    for(size_t i = 0; i < v_count ; i++){
        if(strcmp(&values->items[i].name  , name) == 0){
            free_value(&value->items[i]);
            value->items[i] = new_value;
            break;
        }
    }
}

value_t *create_empty_value();
value_t *create_string_value(char *string);
value_t *create_number_value(double number);
value_t *create_boolean_value(bool boolean);
value_t *create_array_value(arr_t *values);
value_t *create_func_value(ast_node *decl , environment_t *env);
value_t *create_native_func_value(native_function_ptr func , const char *name);
value_t *create_struct_value(arr_t *props , const char* name);
void free_value(value_t *value);

char* value_to_string(value_t *value);

