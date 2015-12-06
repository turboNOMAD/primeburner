#ifndef ENUM_H
#define ENUM_H

int enumerate(char*, int, llong, int);

void enumerate_bitset_precomp();

typedef int (*bitset_enum_func)(char*, unsigned, llong);
int enumerate_bitset(char*, unsigned, llong);
int enumerate_bitset_print(char*, unsigned, llong);

#endif
