#ifndef COMMON_H
#define COMMON_H

typedef long long llong;

#define ERATH_CHUNK_WIDTH 18
#define ERATH_CHUNK_SIZE (1L << ERATH_CHUNK_WIDTH)

#define ATKIN_CHUNK_WIDTH 23
#define ATKIN_CHUNK_SIZE (1L << ATKIN_CHUNK_WIDTH)
#define ATKIN_CHUNK_BYTES (ATKIN_CHUNK_SIZE >> 3)

int enumerate(char*, int, llong, int);

void enumerate_bitset_precomp();

typedef int (*bitset_enum_func)(char*, unsigned, llong);
int enumerate_bitset(char*, unsigned, llong);
int enumerate_bitset_print(char*, unsigned, llong);

#endif

