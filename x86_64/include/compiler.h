#ifndef COMPILER_H
#define COMPILER_H

typedef enum {
  COMPILE_EXE,
  COMPILE_DLL,
  COMPILE_LIB,
  COMPILE_RUN
} OutputType;

typedef struct {
  OutputType outputType;
  const char* outputName;
  int optimizeLevel;
} CompilerConfig;

void compile_program(const char* source, const CompilerConfig* config);

#endif