#include "../include/parser.h"
#include <stdlib.h>

Parser *parser_create(Lexer *lexer) {
  Parser *parser = (Parser *)malloc(sizeof(Parser));
  parser->lexer = lexer;
  parser->current_token = lexer_next_token(lexer);
  return parser;
}

static void parser_eat(Parser *parser, TokenType expected_type) {
  if (parser->current_token.type == expected_type) {
    parser->current_token = lexer_next_token(parser->lexer);
  }
}

static ASTNode *parser_parse_expr(Parser *parser);
static ASTNode *parser_parse_term(Parser *parser);
static ASTNode *parser_parse_factor(Parser *parser);

ASTNode *parser_parse(Parser *parser) {
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));

  if (parser->current_token.type == TOK_PRINT) {
    node->type = ND_PRINT;
    parser_eat(parser, TOK_PRINT);

    if (parser->current_token.type == TOK_STRING) {
      node->left = (ASTNode *)malloc(sizeof(ASTNode));
      node->left->type = ND_EXPR;
      node->left->token = parser->current_token;
      parser_eat(parser, TOK_STRING);
    }
    else {
      node->left = parser_parse_expr(parser);
    }

    parser_eat(parser, TOK_SEMICOLON);
    return node;
  }

  return parser_parse_expr(parser);
}

static ASTNode *parser_parse_expr(Parser *parser) {
  ASTNode *node = parser_parse_term(parser);

  while (parser->current_token.type == TOK_PLUS ||
    parser->current_token.type == TOK_MINUS) {
    ASTNode *binop = (ASTNode *)malloc(sizeof(ASTNode));
    binop->type = ND_BINOP;
    binop->token = parser->current_token;
    binop->left = node;
    parser_eat(parser, parser->current_token.type);
    binop->right = parser_parse_term(parser);
    node = binop;
  }

  return node;
}

static ASTNode *parser_parse_term(Parser *parser) {
  ASTNode *node = parser_parse_factor(parser);

  while (parser->current_token.type == TOK_MUL ||
    parser->current_token.type == TOK_DIV) {
    ASTNode *binop = (ASTNode *)malloc(sizeof(ASTNode));
    binop->type = ND_BINOP;
    binop->token = parser->current_token;
    binop->left = node;
    parser_eat(parser, parser->current_token.type);
    binop->right = parser_parse_factor(parser);
    node = binop;
  }

  return node;
}

static ASTNode *parser_parse_factor(Parser *parser) {
  ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));

  if (parser->current_token.type == TOK_NUMBER) {
    node->type = ND_EXPR;
    node->token = parser->current_token;
    parser_eat(parser, TOK_NUMBER);
    return node;
  }
  else if (parser->current_token.type == TOK_LPAREN) {
    parser_eat(parser, TOK_LPAREN);
    node = parser_parse_expr(parser);
    parser_eat(parser, TOK_RPAREN);
    return node;
  }

  // Default to number if nothing matches
  node->type = ND_EXPR;
  node->token.type = TOK_NUMBER;
  node->token.start = "0";
  node->token.length = 1;
  return node;
}

void parser_free(Parser *parser) {
  lexer_free(parser->lexer);
  free(parser);
}