#ifndef SVM_H
#define SVM_H

// stack base vm (svm)

#include <stdint.h>
#include <stdlib.h>
#include <arr.h>

#define REGS_COUNT 16

#ifndef BASE_STACK_SIZE
 #define BASE_STACK_SIZE 4 * 1020
#endif

typedef uintptr_t reg;

typedef struct svm_t {
    reg regs[REGS_COUNT - 1];
    reg rsp;
    void *stack_top;
} svm_t;

svm_t *new_svm();
void free_svm(svm_t *svm);
void vm_do(svm_t *svm , arr_t *insts);

#endif // SVM_H
