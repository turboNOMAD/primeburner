#include "common/enum.h"

#include <stdio.h> /* printf */

#include "common/bitset.h"

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

int enumerate(char* start, char* end, llong offset, int print)
{
    int ret = 0;

    for (char* p = start; p < end; ++p)
    {
        if (*p != 0)
        {
            ++ret;
            if (print)
            {
                printf("%lu\n", p - start + offset);
            }
        }
    }

    return ret;
}

/* Simple enumeration does not need to know chunk offset */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int enumerate_bitset(char* arr, unsigned len, llong offset)
{
    int ret = 0;
    char* end = arr + len;

    for (char* p = arr; p < end; ++p)
    {
        unsigned char cur_byte = *p;
        ret += bitset_counters[cur_byte];
    }

    return ret;
}
#pragma GCC diagnostic pop

int enumerate_bitset_print(char* arr, unsigned len, llong offset)
{
    int ret = 0;
    unsigned count = len << 3;

    for (unsigned i = 0; i < count; ++i)
    {
        if (bitset_get(arr, i) != 0)
        {
            ++ret;
            printf("%lu\n", i + offset);
        }
    }

    return ret;
}

