#ifndef WIN_TARGET_H
#define WIN_TARGET_H

typedef enum {
  TARGET_EXE,
  TARGET_DLL,
  TARGET_LIB
} TargetType;

void win_generate_target(const char *filename, const char *code, int code_size, TargetType target_type);

#endif