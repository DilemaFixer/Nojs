#include <stdlib.h>
#include <stdint.h>
#include "vm_inst.h"
#include "try.h"
#include "vm.h"
#include "error.h"

inst_t *new_inst(inst_type type){
    inst_t *inst = (inst_t*)malloc(sizeof(inst_t));
    if(!inst)
        throw(format("inst %s" , inst_type_to_string(type)) , MALLOC_ERROR);

    inst->type = type;
    return inst;
}

void free_inst(inst_t *inst){
    if(inst){
        if(inst->value)
            free(inst->value);
        
        free(inst);
    }
}

char *inst_type_to_string(inst_type type) {
   switch (type) {
       case PUSH: return "PUSH";
       case POP: return "POP";
       case DUP: return "DUP";
       case SWAP: return "SWAP";
       case ROT: return "ROT";
       case OVER: return "OVER";
       case DROP: return "DROP";
       case CLEAR: return "CLEAR";
       case ADD: return "ADD";
       case SUB: return "SUB";
       case MUL: return "MUL";
       case DIV: return "DIV";
       case MOD: return "MOD";
       case EQ: return "EQ";
       case NEQ: return "NEQ";
       case GT: return "GT";
       case LT: return "LT";
       case GTE: return "GTE";
       case LTE: return "LTE";
       case AND: return "AND";
       case OR: return "OR";
       case NOT: return "NOT";
       default: return "UNKNOWN";
   }
}

arr_t *parse(char *code){
    if(!code) 
        throw("vm_inst");

}
