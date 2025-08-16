#!/bin/bash

CC=gcc
SRCDIR=src
INCDIR=include
BINDIR=bin
OBJDIR=obj
RUNTIMEDIR=${SRCDIR}/runtime

CFLAGS="-I${INCDIR} -Wall -O2 -fPIC"
LDFLAGS="-L${BINDIR} -s"

mkdir -p ${BINDIR} ${OBJDIR}

OBJS=""

echo "Compiling frontend..."
for f in $(find ${SRCDIR}/frontend -name "*.c"); do
  objfile="${OBJDIR}/$(basename ${f%.c}.o)"
  ${CC} ${CFLAGS} -c "${f}" -o "${objfile}"
  OBJS+=" ${objfile}"
done

echo "Compiling backend..."
for f in $(find ${SRCDIR}/backend -name "*.c"); do
  objfile="${OBJDIR}/$(basename ${f%.c}.o)"
  ${CC} ${CFLAGS} -c "${f}" -o "${objfile}"
  OBJS+=" ${objfile}"
done

echo "Compiling runtime..."
for f in $(find ${RUNTIMEDIR} -name "*.c"); do
  objfile="${OBJDIR}/$(basename ${f%.c}.o)"
  ${CC} ${CFLAGS} -c "${f}" -o "${objfile}"
  OBJS+=" ${objfile}"
done

echo "Compiling CLI..."
for f in $(find ${SRCDIR}/cli -name "*.c"); do
  objfile="${OBJDIR}/$(basename ${f%.c}.o)"
  ${CC} ${CFLAGS} -c "${f}" -o "${objfile}"
  OBJS+=" ${objfile}"
done

echo "Compiling library..."
for f in $(find ${SRCDIR}/lib -name "*.c"); do
  objfile="${OBJDIR}/$(basename ${f%.c}.o)"
  ${CC} ${CFLAGS} -c "${f}" -o "${objfile}"
  OBJS+=" ${objfile}"
done

echo "Building runtime shared library..."
${CC} ${CFLAGS} -shared -o ${BINDIR}/libminicrt.so ${RUNTIMEDIR}/minicrt.c

echo "Linking compiler..."
${CC} ${CFLAGS} ${OBJS} -o ${BINDIR}/minic ${LDFLAGS} -lminicrt -L${BINDIR}

echo "Building static library..."
ar rcs ${BINDIR}/libminic.a ${OBJDIR}/minic_lib.o

echo "Building sample shared library..."
${CC} ${CFLAGS} -shared -o ${BINDIR}/libsample.so ${SRCDIR}/lib/minic_lib.c

echo "Done. Output is in ${BINDIR}"

read -p "Press any key to continue..."