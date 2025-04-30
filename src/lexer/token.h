#ifndef TOKENS_H
#define TOKENS_H

#include "lexer.h"
#include "utils/arr.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct lexer_t lexer_t;

typedef enum ttype {
    NUMBER,
    LPARENT,
    RPARENT,
    MUL,
    DIV,
    PLUS,
    MINUS,
    
    /* Дополнительные токены */
    STRING,        /* Строковые литералы */
    BOOLEAN,       /* true, false */
    NULL_VAL,      /* null */
    
    /* Переменные и константы */
    LET,           /* let */
    CONST,         /* const */
    
    /* Операторы сравнения */
    EQUALS,        /* == */
    NOT_EQUALS,    /* ! = */
    GREATER,       /* > */
    LESS,          /* < */
    GREATER_EQUAL, /* > = */
    LESS_EQUAL,    /* < = */
    
    /* Логические операторы */
    AND,           /* && */
    OR,            /* || */
    NOT,           /* ! */
    
    /* Присваивание */
    ASSIGN,        /* = */
    
    /* Управляющие конструкции */
    IF,            /* if */
    ELSE,          /* else */
    LOOP,          /* loop */
    NEXT,          /* next */
    STOP,          /* stop */
    
    /* Функции */
    FUNCTION,      /* function */
    RETURN,        /* return */
    
    /* Встроенные функции */
    PRINT,         /* print */
    TAKE,          /* take */
    
    /* Дополнительные символы */
    LBRACE,        /* { */
    RBRACE,        /* } */
    LBRACKET,      /* [ */
    RBRACKET,      /* ] */
    SEMICOLON,     /* ; */
    COLON,         /* : */
    COMMA,         /* , */
    DOT,           /* . */
    
    /* Идентификатор */
    IDENTIFIER,    /* Имена переменных, функций и т.д. */
    
    END            /* Конец файла */
} ttype; // token type
         
typedef struct token_t {
    ttype type;
    size_t line;
    size_t column;
    union {
        double number;
        char *string;
        bool boolean;
    } value;
} token_t;

token_t *new_token(lexer_t *lexer , ttype type);
token_t *new_number_token(lexer_t *lexer , ttype type, double number);
token_t *new_boolean_token(lexer_t *lexer , ttype type , bool boolean);
token_t *new_string_token(lexer_t *lexer , ttype type , char* string);
void free_token(token_t *token);
void log_token(token_t *token);

#endif
