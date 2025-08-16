@echo off
setlocal enabledelayedexpansion

set CC=gcc
set SRCDIR=src
set INCDIR=include
set BINDIR=bin
set OBJDIR=obj
set RUNTIMEDIR=%SRCDIR%\runtime

set CFLAGS=-I%INCDIR% -Wall -O2 -D_WIN32
set LDFLAGS=-L%BINDIR% -s -mconsole

if not exist %BINDIR% mkdir %BINDIR%
if not exist %OBJDIR% mkdir %OBJDIR%

set OBJS=

echo Compiling frontend...
for /r %SRCDIR%\frontend %%f in (*.c) do (
  set "objfile=!OBJDIR!\%%~nf.o"
  %CC% %CFLAGS% -c "%%f" -o "!objfile!"
  set OBJS=!OBJS! "!objfile!"
)

echo Compiling backend...
for /r %SRCDIR%\backend %%f in (*.c) do (
  set "objfile=!OBJDIR!\%%~nf.o"
  %CC% %CFLAGS% -c "%%f" -o "!objfile!"
  set OBJS=!OBJS! "!objfile!"
)

echo Compiling runtime...
for /r %RUNTIMEDIR% %%f in (*.c) do (
  set "objfile=!OBJDIR!\%%~nf.o"
  %CC% %CFLAGS% -c "%%f" -o "!objfile!"
  set OBJS=!OBJS! "!objfile!"
)

echo Compiling CLI...
for /r %SRCDIR%\cli %%f in (*.c) do (
  set "objfile=!OBJDIR!\%%~nf.o"
  %CC% %CFLAGS% -c "%%f" -o "!objfile!"
  set OBJS=!OBJS! "!objfile!"
)

echo Compiling library...
for /r %SRCDIR%\lib %%f in (*.c) do (
  set "objfile=!OBJDIR!\%%~nf.o"
  %CC% %CFLAGS% -c "%%f" -o "!objfile!"
  set OBJS=!OBJS! "!objfile!"
)

echo Building runtime DLL...
%CC% %CFLAGS% -shared -o %BINDIR%\minicrt.dll %RUNTIMEDIR%\minicrt.c -Wl,--out-implib=%BINDIR%\libminicrt.a

echo Linking compiler...
%CC% %CFLAGS% %OBJS% -o %BINDIR%\minic.exe %LDFLAGS% -lminicrt

echo Building static library...
ar rcs %BINDIR%\libminic.a %OBJDIR%\minic_lib.o

echo Building sample DLL...
%CC% %CFLAGS% -shared -o %BINDIR%\sample.dll %SRCDIR%\lib\minic_lib.c

echo Done. Output is in %BINDIR%

pause
