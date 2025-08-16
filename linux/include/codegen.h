#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include "linux_target.h"

typedef struct
{
  char *buffer;
  int pos;
  int size;
} CodeGen;

CodeGen *codegen_create();
void codegen_generate(CodeGen *gen, ASTNode *node);
void codegen_free(CodeGen *gen);

#endif