#include "../../include/compiler.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/codegen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compile_program(const char *source, const CompilerConfig *config) {
  // Create lexer
  Lexer *lexer = lexer_create(source);

  // Create parser
  Parser *parser = parser_create(lexer);

  // Parse AST
  ASTNode *ast = parser_parse(parser);

  // Create code generator
  CodeGen *codegen = codegen_create();

  // Generate code
  codegen_generate(codegen, ast);

  // Determine output type
  if (config->outputType == COMPILE_RUN) {
    // In a real compiler, we'd interpret or JIT the code
    if (ast->type == ND_PRINT) {
      if (ast->left->token.type == TOK_STRING) {
        char str[256];
        strncpy(str, ast->left->token.start, ast->left->token.length);
        str[ast->left->token.length] = '\0';
        printf("%s\n", str);
      }
      else {
        printf("42\n"); // Simplified output for expressions
      }
    }
  }
  else {    // Generate target file
    TargetType target;
    switch (config->outputType) {
    case COMPILE_EXE:
      target = TARGET_EXE;
      break;
    case COMPILE_DLL:
      target = TARGET_DLL;
      break;
    case COMPILE_LIB:
      target = TARGET_LIB;
      break;
    default:
      target = TARGET_EXE;
    }

    win_generate_target(config->outputName, codegen->buffer, codegen->pos, target);
  }

  // Cleanup
  codegen_free(codegen);
  parser_free(parser);
}

int main(int argc, char *argv[]) {

  CompilerConfig config = {COMPILE_EXE, "a.exe", 0};
  const char *sourceFile = NULL;

  // Parse command line arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 < argc) {
        config.outputName = argv[++i];
      }
    }
    else if (strcmp(argv[i], "--dll") == 0) {
      config.outputType = COMPILE_DLL;
    }
    else if (strcmp(argv[i], "--lib") == 0) {
      config.outputType = COMPILE_LIB;
    }
    else if (strcmp(argv[i], "--run") == 0) {
      config.outputType = COMPILE_RUN;
    }
    else {
      sourceFile = argv[i];
    }
  }

  if (!sourceFile) {
    fprintf(stderr, "Error: No source file provided.\n");
    return 1;
  }

  // Read source file
  FILE *file = fopen(sourceFile, "rb");
  if (!file) {
    perror("Error opening source file");
    return 1;
  }
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *source = (char *)malloc(size + 1);
  fread(source, 1, size, file);
  source[size] = '\0';
  fclose(file);

  // Compile program
  compile_program(source, &config);

  free(source);
  return 0;
}