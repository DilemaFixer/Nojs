#ifndef VM_INST_H
#define VM_INST_H

#include <stdint.h>
#include <stdlib.h>
#include "vm.h"

typedef enum inst_type {
    PUSH , 
    POP ,
    DUP ,
    SWAP ,
    ROT , 
    OVER , 
    DROP , 
    CLEAR ,

    ADD , 
    SUB , 
    MUL , 
    DIV , 
    MOD , 

    EQ , 
    NEQ , 
    GT , 
    LT , 
    GTE ,
    LTE ,

    AND , 
    OR , 
    NOT ,

    IF ,
    ELSE ,
    LOOP ,
    IDENT ,
    DEF , 
    PRINT , 
    SEMICOLON,
    DSIGN
} inst_type;

typedef struct inst_t {
    uintptr_t value;
    size_t line;
    inst_type type;
} inst_t;

inst_t *new_inst(inst_type type);
void free_inst(inst_t *inst);
char *inst_type_to_string(inst_type type);
arr_t *parse(char *code);
#endif
