@echo off
setlocal enabledelayedexpansion

set CC=gcc
set CFLAGS=-Iinclude -Wall -O2
set BINDIR=bin
set SRCDIR=src
set OUT=%BINDIR%\minic.exe

mkdir %BINDIR% 2>nul

echo Compiling frontend...
%CC% %CFLAGS% -c %SRCDIR%/frontend/lexer.c -o %BINDIR%/lexer.o
%CC% %CFLAGS% -c %SRCDIR%/frontend/parser.c -o %BINDIR%/parser.o

echo Compiling backend...
%CC% %CFLAGS% -c %SRCDIR%/backend/codegen.c -o %BINDIR%/codegen.o
%CC% %CFLAGS% -c %SRCDIR%/backend/win_target.c -o %BINDIR%/win_target.o

echo Compiling runtime...
%CC% %CFLAGS% -c %SRCDIR%/runtime/minicrt.c -o %BINDIR%/minicrt.o

echo Compiling CLI...
%CC% %CFLAGS% -c %SRCDIR%/cli/minic.c -o %BINDIR%/minic.o

echo Linking compiler...
%CC% -o %OUT% %BINDIR%/lexer.o %BINDIR%/parser.o %BINDIR%/codegen.o %BINDIR%/win_target.o %BINDIR%/minicrt.o %BINDIR%/minic.o

echo Building runtime DLL...
%CC% %CFLAGS% -shared -o %BINDIR%/minicrt.dll %SRCDIR%/runtime/minicrt.c

echo Building static library...
%CC% %CFLAGS% -c %SRCDIR%/lib/minic_lib.c -o %BINDIR%/minic_lib.o
ar rcs %BINDIR%/libminic.a %BINDIR%/minic_lib.o

echo Building sample DLL...
%CC% %CFLAGS% -shared -o %BINDIR%/sample.dll %SRCDIR%/lib/minic_lib.c

echo Done. Output is in %BINDIR%

pause