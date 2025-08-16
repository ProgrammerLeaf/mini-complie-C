#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
  ND_PRINT,
  ND_EXPR,
  ND_BINOP
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
  NodeType type;
  ASTNode *left;
  ASTNode *right;
  Token token;
};

typedef struct {
  Lexer *lexer;
  Token current_token;
} Parser;

Parser *parser_create(Lexer *lexer);
ASTNode *parser_parse(Parser *parser);
void parser_free(Parser *parser);

#endif