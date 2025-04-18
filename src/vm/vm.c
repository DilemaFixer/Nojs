#include <stdlib.h>
#include "try.h"
#include "arr.h"
#include "vm.h"
#include "error.h"

svm_t new_svm(){
    svm_t *svm = (svm_t*)malloc(sizeof(svm_t));
    if(!svm) 
        throw("vm_t struct" , MALLOC_ERROR);

    void *stack = malloc(sizeof(char) * BASE_STACK_SIZE);
    if!stack) 
        throw("svm_t -> stack_top" , MALLOC_ERROR);

    svm->stack_top = stack;
    svm->rsp = (reg)stack;
    return svm;
}

void free_svm(svm_t *svm){
    if(svm){
        if(svm->stack_top)
            free(svm->stack_top);
        
        free(svm);
    }
}

void vm_do(vsm_t *vsm , arr_t *insts){

}
