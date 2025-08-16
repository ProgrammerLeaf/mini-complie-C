#include "../include/lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Lexer *lexer_create(const char *source) {
  Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
  lexer->source = source;
  lexer->position = 0;
  lexer->line = 1;
  lexer->col = 1;
  return lexer;
}

Token lexer_next_token(Lexer *lexer) {
  Token token = {TOK_EOF, 0, 0, 0, 0};
  const char *s = lexer->source + lexer->position;

  // Skip whitespace
  while (isspace(*s)) {
    if (*s == '\n') {
      lexer->line++;
      lexer->col = 1;
    }
    else {
      lexer->col++;
    }
    lexer->position++;
    s = lexer->source + lexer->position;
  }

  if (*s == '\0') {
    token.type = TOK_EOF;
    return token;
  }

  token.start = s;
  token.line = lexer->line;
  token.col = lexer->col;

  if (isdigit(*s)) {
    token.type = TOK_NUMBER;
    while (isdigit(*s)) {
      s++;
      lexer->col++;
    }
    token.length = s - token.start;
    lexer->position += token.length;
    return token;
  }

  if (*s == '"') {
    s++;
    lexer->col++;
    lexer->position++;
    token.start = s;
    token.type = TOK_STRING;
    while (*s && *s != '"') {
      s++;
      lexer->col++;
    }
    token.length = s - token.start;
    if (*s == '"') {
      lexer->position++;
      lexer->col++;
    }
    lexer->position += token.length;
    return token;
  }

  if (isalpha(*s)) {
    while (isalnum(*s)) {
      s++;
      lexer->col++;
    }
    token.length = s - token.start;
    lexer->position += token.length;

    // Check for keywords
    if (strncmp(token.start, "print", 5) == 0 && token.length == 5) {
      token.type = TOK_PRINT;
    }
    else {
      token.type = TOK_IDENT;
    }
    return token;
  }

  // Single character tokens
  token.length = 1;
  lexer->position++;
  lexer->col++;

  switch (*s) {
  case '+':
    token.type = TOK_PLUS;
    break;
  case '-':
    token.type = TOK_MINUS;
    break;
  case '*':
    token.type = TOK_MUL;
    break;
  case '/':
    token.type = TOK_DIV;
    break;
  case '(':
    token.type = TOK_LPAREN;
    break;
  case ')':
    token.type = TOK_RPAREN;
    break;
  case ';':
    token.type = TOK_SEMICOLON;
    break;
  default:
    token.type = TOK_EOF;
    break;
  }

  return token;
}

void lexer_free(Lexer *lexer) {
  free(lexer);
}