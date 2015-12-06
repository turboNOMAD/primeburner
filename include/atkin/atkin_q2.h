#ifndef ATKIN_Q2_H
#define ATKIN_Q2_h

#include "common/defs.h"

/* Flip numbers that are solutions to Atkin 2nd quadratic form: */
/* 3x^2 + y^2 = n */
void atkin_q2_precomp();
void atkin_chunk_q2(char*, llong, llong);

#endif

