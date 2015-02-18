#ifndef ATKIN_Q1_H
#define ATKIN_Q1_h

#include "common.h"

/* Flip numbers that are solutions to Atkin 1st quadratic form: */
/* 4x^2 + y^2 = n */
void atkin_q1_precomp();
void atkin_chunk_q1(char*, llong, llong);

#endif

