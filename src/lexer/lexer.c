#include "utils/arr.h"
#include "utils/logger.h"
#include "utils/queue.h"

#include "lexer.h"
#include "token.h"

#include <ctype.h>
#include <stdbool.h>
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

bool is_keyword(const char *word_one, const char *word_two) {
  return strcmp(word_one, word_two) == 0;
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

bool is_system(char c) {
  return c == '.' || c == '-' || c == '+' || c == '/' || c == '*' || c == ')' ||
         c == '(' || c == '{' || c == '}' || c == '[' || c == ']' || c == ';' ||
         c == ':' || c == ',' || c == '=' || c == '>' || c == '<' || c == '!' ||
         c == '&' || c == '|';
}

token_t *check_keyword(lexer_t *lexer, const char *word) {
  if (is_keyword(word, "*"))
    return new_token(lexer, MUL);
  if (is_keyword(word, "/"))
    return new_token(lexer, DIV);
  if (is_keyword(word, "+"))
    return new_token(lexer, PLUS);
  if (is_keyword(word, "-"))
    return new_token(lexer, MINUS);

  if (is_keyword(word, "("))
    return new_token(lexer, LPARENT);
  if (is_keyword(word, ")"))
    return new_token(lexer, RPARENT);
  if (is_keyword(word, "{"))
    return new_token(lexer, LBRACE);
  if (is_keyword(word, "}"))
    return new_token(lexer, RBRACE);
  if (is_keyword(word, "["))
    return new_token(lexer, LBRACKET);
  if (is_keyword(word, "]"))
    return new_token(lexer, RBRACKET);

  if (is_keyword(word, "=="))
    return new_token(lexer, EQUALS);
  if (is_keyword(word, "!="))
    return new_token(lexer, NOT_EQUALS);
  if (is_keyword(word, ">"))
    return new_token(lexer, GREATER);
  if (is_keyword(word, "<"))
    return new_token(lexer, LESS);
  if (is_keyword(word, ">="))
    return new_token(lexer, GREATER_EQUAL);
  if (is_keyword(word, "<="))
    return new_token(lexer, LESS_EQUAL);

  if (is_keyword(word, "&&"))
    return new_token(lexer, AND);
  if (is_keyword(word, "||"))
    return new_token(lexer, OR);
  if (is_keyword(word, "!"))
    return new_token(lexer, NOT);

  if (is_keyword(word, "="))
    return new_token(lexer, ASSIGN);

  if (is_keyword(word, "if"))
    return new_token(lexer, IF);
  if (is_keyword(word, "else"))
    return new_token(lexer, ELSE);
  if (is_keyword(word, "loop"))
    return new_token(lexer, LOOP);
  if (is_keyword(word, "next"))
    return new_token(lexer, NEXT);
  if (is_keyword(word, "stop"))
    return new_token(lexer, STOP);

  if (is_keyword(word, "function"))
    return new_token(lexer, FUNCTION);
  if (is_keyword(word, "return"))
    return new_token(lexer, RETURN);

  if (is_keyword(word, "print"))
    return new_token(lexer, PRINT);
  if (is_keyword(word, "take"))
    return new_token(lexer, TAKE);

  if (is_keyword(word, "let"))
    return new_token(lexer, LET);
  if (is_keyword(word, "const"))
    return new_token(lexer, CONST);

  if (is_keyword(word, "true"))
    return new_boolean_token(lexer, BOOLEAN, true);
  if (is_keyword(word, "false"))
    return new_boolean_token(lexer, BOOLEAN, false);
  if (is_keyword(word, "null"))
    return new_token(lexer, NULL_VAL);

  if (is_keyword(word, ";"))
    return new_token(lexer, SEMICOLON);
  if (is_keyword(word, ":"))
    return new_token(lexer, COLON);
  if (is_keyword(word, ","))
    return new_token(lexer, COMMA);
  if (is_keyword(word, "."))
    return new_token(lexer, DOT);

  return NULL;
}

void skip_comment(lexer_t *lexer) {
  if (lexer_peak(lexer) != '/' || lexer_peak_next(lexer) != '/')
    return;
  char c;
  while ((c = lexer_peak(lexer)) != '\n' && !lexer_is_end(lexer)) {
    lexer_advance(lexer);
  }

  lexer_advance(lexer);
}

char *take_word(lexer_t *lexer) {
  char c = lexer_peak(lexer);

  if (is_system(c)) {
    size_t len = 1;
    char n;
    if ((n = lexer_peak_next(lexer)) == '=' || n == '&' || n == '|') {
      len++;
    }
    char *result = (char *)malloc(sizeof(char) * (len + 1));
    if (!result)
      elog("Error allocation memory for taking word in lexer");
    strncpy(result, &lexer->source[lexer->position] , len);
    lexer->position += len;
    result[len] = '\0';
    return result;
  }

  size_t len = 0;
  size_t base_pos = lexer->position;
  if(lexer_peak(lexer) == '"'){
    lexer_advance(lexer);
    len++;
    while(1){
        dlog("Char %c", lexer_peak(lexer));
        
        if(lexer_peak(lexer) == '\0')
            elog("Error parsing string , search where =))");
            
        if(lexer_peak(lexer) == '"'){
            lexer_advance(lexer);
            len++; 
            break;
        }
        
        len++;
        lexer_advance(lexer);
    }
    
    char *result = (char*)malloc(sizeof(char) * (len + 1));
    if(!result) elog("Error allocation memory for word");
    
    strncpy(result, &lexer->source[base_pos], len);
    result[len] = '\0';
    
    return result;
}

  while (1) {
    c = lexer_peak(lexer);
    if (c == '\n' || c == ' ' || c == '\0')
      break;

    if (is_system(c)) {
      if (c == '.' && isdigit(lexer_peak_next(lexer))) {
        len++;
        lexer_advance(lexer);
      } else {
        break;
      }
    } else {
      len++;
      lexer_advance(lexer);
    }
  }

  char *result = (char *)malloc(sizeof(char) * (len + 1));
  if (!result)
    elog("Error allocation memory for taking word");
  strncpy(result, &lexer->source[base_pos], len);
  result[len] = '\0';

  return result;
}

bool is_number(char *word) {
  while (*word) {
    if (!isdigit(*word)) {
      if (*word != '.' && !isdigit(*(word + 1)))
        return false;
    }

    word++;
  }
  return true;
}

bool is_string(char *word) {
  if (*word != '"')
    return false;
  word++;
  while (*word) {
    if (*word == '"') {
      word++;
      if (*word)
        elog("Lexer : invalid string formating where =)");
      return true;
    }
    word++;
 }
  return false;
}

token_t *get_next_token(lexer_t *lexer) {
  if(lexer_is_end(lexer))
      elog("Error , try tokenize after ending");

  skip_whitespace(lexer);
  skip_comment(lexer);

  char *word = take_word(lexer);
  dlog("Parse word : %s" , word);
  skip_whitespace(lexer);

  if(is_string(word)){
      dlog("create string token");
     return new_string_token(lexer , STRING , word);
  }

  if (is_number(word)) {
    double value = atof(word);
    free(word);
    return new_number_token(lexer, NUMBER, value);
  }

  token_t *token = check_keyword(lexer, word);

  if (token)
    return token;
  
  token = new_string_token(lexer , IDENTIFIER , word);
  free(word);
  return token;
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

  arr_push(lexer->tokens, new_token(lexer, END));

  return lexer;
}
