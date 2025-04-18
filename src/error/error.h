#ifndef ERROR_H
#define ERROR_H

#include "try.h"

#define MALLOC_ERROR 0
#define NULL_REF_ERROR 1

void error_handler_init();
void handling_error(error *er);

#endif
