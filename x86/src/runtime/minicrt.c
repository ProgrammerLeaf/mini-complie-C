#include <stdio.h>
#include <windows.h>

__declspec(dllexport) void minicrt_print(const char *str) {
  printf("%s", str);
}

__declspec(dllexport) void minicrt_print_int(int n) {
  printf("%d", n);
}

// DLL entry point
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  return TRUE;
}