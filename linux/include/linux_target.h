#ifndef LINUX_TARGET_H
#define LINUX_TARGET_H

typedef enum {
  TARGET_EXE,
  TARGET_SO, 
  TARGET_A
} TargetType;

void linux_generate_target(const char *filename, const char *code, int code_size, TargetType target_type);

#endif