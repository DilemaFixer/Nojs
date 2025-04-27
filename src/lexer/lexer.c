#include "utils/arr.h"
#include "utils/logger.h"
#include "utils/queue.h"

#include "lexer.h"
#include "token.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

lexer_t *new_lexer(const char *source) {
  if (!source)
    elog("Can't create lexer_t struct with NULL ptr on source code");
  if (!*source)
    elog("Can't create lexer_t struct with empty string");

  lexer_t *lexer = (lexer_t *)malloc(sizeof(lexer_t));
  if (!lexer)
    elog("Error allocation memory for lexer_t struct");

  lexer->tokens = new_arr(1);
  lexer->line = 1;
  lexer->column = 0;
  lexer->position = 0;
  lexer->length = strlen(source);
  lexer->source = source;

  return lexer;
}

void free_lexer(lexer_t *lexer) {
  if (!lexer)
    return;

  if (lexer->tokens)
    free_arr(lexer->tokens);

  // free(lexer->source);
  free(lexer);
}

bool lexer_is_end(lexer_t *lexer) {
  return lexer->source[lexer->position] == '\0';
}

char lexer_peak(lexer_t *lexer) { return lexer->source[lexer->position]; }

char lexer_peak_next(lexer_t *lexer) {
  if (lexer_is_end(lexer))
    return '\0';
  return lexer->source[lexer->position + 1];
}

char lexer_advance(lexer_t *lexer) {
  if (lexer_is_end(lexer))
    return '\0';

  lexer->position++;
  return lexer->source[lexer->position - 1];
}

token_t *get_previouse_token(lexer_t *lexer) {
  if (lexer->tokens->count == 0)
    return NULL;
  return (token_t *)arr_get(lexer->tokens, lexer->tokens->count - 1);
}

bool is_keyword(lexer_t *lexer, const char *word) {
  size_t word_len = strlen(word);

  if (lexer->position + word_len > lexer->length) {
    return false;
  }

  return strncmp(word, &lexer->source[lexer->position], word_len) == 0;
}

void skip_whitespace(lexer_t *lexer) {
  char c;
  while (isspace(c = lexer_peak(lexer))) {
    if (c == ' ') {
      lexer->column++;
    } else {
      lexer->line++;
      lexer->column = 0;
    }

    lexer_advance(lexer);
  }
}

bool is_ssytem(char c) {
  return c == '.' || c == '-' || c == '+' || c == '/' || c == '*' || c == ')' ||
         c == '(';
}

double parse_number(lexer_t *lexer) {
  if (!isdigit(lexer_peak(lexer)))
    elog("Can't parse number, have no digit char");

  size_t capacity = 16;
  size_t length = 0;
  char *buffer = (char*)malloc(sizeof(char) * capacity);
  if(!buffer) elog("Error allocation memory for number buffer");
  
  while (isdigit(lexer_peak(lexer))) {
    buffer[length++] = lexer_advance(lexer);

    if(length >= capacity){
        capacity *= 2;
        char *new_buffer = realloc(buffer , capacity);
        if(!new_buffer) elog("error allocation memory for parse number buffer");
        buffer = new_buffer;
    }
  }

  if(lexer_peak(lexer) == '.'){
    while (isdigit(lexer_peak(lexer))) {
      buffer[length++] = lexer_advance(lexer);

      if(length >= capacity){
          capacity *= 2;
          char *new_buffer = realloc(buffer , capacity);
          if(!new_buffer) elog("Error allocation memory for parsing number buffer");
          buffer = new_buffer;
      }
    }
  }else{
      elog("Lexer error : can't parse number int %zu:%zu, it have digit : %c" , lexer->line , lexer->column , lexer_peak(lexer));
  }

  buffer[length] = '\0';

  return strtod(buffer, NULL);
}

token_t *check_keyword(lexer_t *lexer) {
  if (is_keyword(lexer, "*"))
    return new_token(lexer, MUL);
  if (is_keyword(lexer, "/"))
    return new_token(lexer, DIV);
  if (is_keyword(lexer, "+"))
    return new_token(lexer, PLUS);
  if (is_keyword(lexer, "-"))
    return new_token(lexer, MINUS);
  if (is_keyword(lexer, "("))
    return new_token(lexer, LPARENT);
  if (is_keyword(lexer, ")"))
    return new_token(lexer, RPARENT);

  return NULL;
}

void skip_comment(lexer_t *lexer){
    if(lexer_peak(lexer) != '/' || lexer_peak_next(lexer) != '/')
            return;
    char c;
    while((c = lexer_peak(lexer)) != '\n' && !lexer_is_end(lexer)) {
        lexer_advance(lexer);
    }

    lexer_advance(lexer);
}

bool is_number(char *word) {
    while(*word){
        if(!isdigit(*word))
            return false;
    }
    return true;
}

token_t *get_next_token(lexer_t *lexer) {
  skip_whitespace(lexer);
  skip_comment(lexer);
    
  if (isdigit(lexer_peak(lexer))) {
    double value = parse_number(lexer);
    return new_number_token(lexer, NUMBER, value);
  }

  token_t *token = check_keyword(lexer);
  if (token) return lexer_advance(lexer);

    

  wlog("Unexpected char : %c", lexer_peak(lexer));
  lexer_advance(lexer); // Skip unexpected character
  return get_next_token(lexer);
}

lexer_t *tokenize(const char *code) {
  if (!code)
    elog("Can't tokenize code, NULL ptr on it");
  if (!*code)
    elog("Can't tokenize code, have empty string");

  lexer_t *lexer = new_lexer(code);

  while (!lexer_is_end(lexer)) {
    token_t *token = get_next_token(lexer);
    arr_push(lexer->tokens, token);
  }
    
  arr_push(lexer->tokens , new_token(lexer , END));

  return lexer;
}
