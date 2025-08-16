#ifndef LEXER_H
#define LEXER_H

typedef enum {
  TOK_EOF,
  TOK_IDENT,
  TOK_NUMBER,
  TOK_STRING,
  TOK_PRINT,
  TOK_PLUS,
  TOK_MINUS,
  TOK_MUL,
  TOK_DIV,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_SEMICOLON
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  int length;
  int line;
  int col;
} Token;

typedef struct {
  const char *source;
  int position;
  int line;
  int col;
} Lexer;

Lexer *lexer_create(const char *source);
Token lexer_next_token(Lexer *lexer);
void lexer_free(Lexer *lexer);

#endif