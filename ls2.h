#ifndef LS2_H_INCLUDED
#define LS2_H_INCLUDED

#include "stack.h"

#define TRUE 0
#define FALSE 1
#define INDENT "    "

// Function declarations
void mode1(const char *path, int indent);
void mode2(const char *path, const char *emp, int indent, stack_t *stk);

#endif
