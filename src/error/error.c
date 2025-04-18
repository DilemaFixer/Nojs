#include "error.h"
#include "try.h"
#include "logger.h"
#include <stdbool.h>

void handling_error(error *er){
    if(!er) elog("Can't handling error , error is null ptr");

    switch(er->code){
        case NULL_REF_ERROR : elog("[%zu:%zu] Null regerence error : " , er->line , er->offset, er->message); 
        case MALLOC_ERROR : elog("[%zu:%zu] Error allocation memory for %s" , er->line , er->offset , er->message);
        default : elog("[%zu:%zu] Unknow error with message : %s" , er->line , er->offset , er->message);
    }
}
