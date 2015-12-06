#ifndef SIEVE_FUNC_H
#define SIEVE_FUNC_H

#include "common/defs.h"

typedef llong (*sieve_func)(llong, llong, int);

sieve_func func;
sieve_func find_sieve_func(const char* name);
void enum_sieve_funcs();

#endif

