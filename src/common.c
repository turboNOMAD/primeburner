#include "common.h"

#include <stdio.h> /* printf */

#include "bitset.h"

static char bitset_counters[1 << 8];

void enumerate_bitset_precomp()
{
    for (unsigned i = 0; i < (sizeof(bitset_counters) / sizeof(char)); ++i)
    {
        char v = 0;
        int n = i;

        while (n != 0)
        {
            v += n & 1;
            n >>= 1;
        }

        bitset_counters[i] = v;
    }
}

int enumerate(char* arr, int len, llong offset, int print)
{
    int ret = 0;

    for (int i = 0; i < len; ++i)
    {
        if (arr[i] != 0)
        {
            ++ret;
            if (print)
            {
                printf("%llu\n", i + offset);
            }
        }
    }

    return ret;
}

int enumerate_bitset(char* arr, unsigned len, llong offset)
{
    int ret = 0;
    char* end = arr + len;

    for (char* p = arr; p < end; ++p)
    {
        unsigned char cur_byte = *p;
        ret += bitset_counters[cur_byte];
    }
    offset = offset; /* Just to get rid of "unused parameter" warning */

    return ret;
}

int enumerate_bitset_print(char* arr, unsigned len, llong offset)
{
    int ret = 0;
    unsigned count = len << 3;

    for (unsigned i = 0; i < count; ++i)
    {
        if (bitset_get(arr, i) != 0)
        {
            ++ret;
            printf("%llu\n", i + offset);
        }
    }

    return ret;
}

