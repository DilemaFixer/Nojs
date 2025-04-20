#include <stdio.h>
#include "utils/logger.h"
#include "utils/arr.h"
#include "lexer/lexer.h"

int main(void){
    char *code = "2 *(5 + 19)\n";
    ilog("Will procces code : %s\n" , code);
    
    arr_t *tokens = tokenize(code);
    (void)tokens;
    return 0;
}

