#include "../include/codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

CodeGen *codegen_create() {
  CodeGen *gen = (CodeGen *)malloc(sizeof(CodeGen));
  gen->size = 1024 * 1024; // 1MB buffer
  gen->buffer = (char *)malloc(gen->size);
  gen->pos = 0;
  return gen;
}

static void emit(CodeGen *gen, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int n = vsnprintf(gen->buffer + gen->pos, gen->size - gen->pos, fmt, args);
  va_end(args);
  gen->pos += n;
}

static void generate_expr(CodeGen *gen, ASTNode *node) {
  if (node->type == ND_EXPR) {
    if (node->token.type == TOK_NUMBER) {
      char num[32];
      strncpy(num, node->token.start, node->token.length);
      num[node->token.length] = '\0';
      emit(gen, "push qword %s\n", num);
    }
    else if (node->token.type == TOK_STRING) {
      char str[256];
      strncpy(str, node->token.start, node->token.length);
      str[node->token.length] = '\0';
      emit(gen, "push offset str_%zx\n", (uintptr_t)node->token.start);
      // In real compiler, we'd store string in data section
    }
  }
  else if (node->type == ND_BINOP) {
    generate_expr(gen, node->left);
    generate_expr(gen, node->right);

    switch (node->token.type) {
    case TOK_PLUS:
      emit(gen, "add rax, rbx\n");
      break;
    case TOK_MINUS:
      emit(gen, "sub\n");
      break;
    case TOK_MUL:
      emit(gen, "imul\n");
      break;
    case TOK_DIV:
      emit(gen, "idiv\n");
      break;
    default:
      break;
    }
  }
}

void codegen_generate(CodeGen *gen, ASTNode *node) {
  if (node->type == ND_PRINT) {
    generate_expr(gen, node->left);
    emit(gen, "call minicrt_print\n");
  }
  else {
    generate_expr(gen, node);
  }
}

void codegen_free(CodeGen *gen) {
  free(gen->buffer);
  free(gen);
}