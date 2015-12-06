#ifndef ATKIN_Q3_H
#define ATKIN_Q3_h

#include "common.h"

/* Flip numbers that are solutions to Atkin 3rd quadratic form: */
/* 3x^2 - y^2 = n, x > y */
void atkin_q3_precomp();
void atkin_chunk_q3(char*, llong, llong);

#endif

