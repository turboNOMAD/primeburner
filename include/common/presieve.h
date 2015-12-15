#ifndef PRESIEVE_H
#define PRESIEVE_H

#include "common/defs.h"
#include "common/bucket_list.h"

int erath_less_than(char*, llong);

void presieve_large_primes(bucket_list*, uint32_t, uint32_t, llong);
llong presieve(llong*, llong, bucket_list**, uint32_t, int);

#endif

