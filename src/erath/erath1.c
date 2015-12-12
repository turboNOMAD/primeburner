#include "erath/erath1.h"

#include <stdlib.h> /* malloc, free */
#include <string.h> /* memset */
#include <math.h>   /* ceil, sqrt */

#define CHUNK_WIDTH 19
#include "common/defs.h"

#include "common/enum.h"
#include "common/presieve.h"
#include "common/bitset.h"

static inline int erath1_chunk(char* chunk, llong lower, llong upper, const char* sieved)
{
    const llong chunk_size = upper - lower;

    memset(chunk, 0xFF, CHUNK_BYTES);
    for (llong p = 2; p*p < upper; ++p)
    {
        if (sieved[p] != 0)
        {
            llong mod = lower % p;
            llong i = p * (mod != 0) - mod;
            for (; i < chunk_size; i += p)
            {
                bitset_clear(chunk, i);
            }
        }
    }

    return 0;
}

llong erath1(llong lower, llong upper, int print)
{
    llong ret = 0;
    llong root = ceil(sqrt(upper));
    char* arr = (char*)malloc(root * sizeof(char));
    if (arr == NULL)
    {
        return ret;
    }

    enumerate_bitset_precomp();

    erath_less_than(arr, root);

    ret = enumerate(arr + lower, root - lower, lower, print);

    char* chunk = (char*)malloc(CHUNK_BYTES);
    if (chunk == NULL)
    {
        free(arr);
        
        return 0;
    }

    bitset_enum_func enumerator = enumerate_bitset;
    if (print != 0)
    {
        enumerator = enumerate_bitset_print;
    }

    llong chunk_lower = (root < lower) ? lower : root;
    llong chunk_upper = chunk_lower + CHUNK_SIZE;
    for (; chunk_upper < upper; chunk_lower += CHUNK_SIZE, chunk_upper += CHUNK_SIZE)
    {
        erath1_chunk(chunk, chunk_lower, chunk_upper, arr);
        ret += enumerator(chunk, CHUNK_BYTES, chunk_lower);
    }

    unsigned last_chunk_bytes = ceil((upper - chunk_lower) / 8.0);
    erath1_chunk(chunk, chunk_lower, upper, arr);
    bitset_truncate(chunk, upper - chunk_lower - 1);
    ret += enumerator(chunk, last_chunk_bytes, chunk_lower);

    free(chunk);
    free(arr);

    return ret;
}

llong erath1_mt(llong lower, llong upper, int print)
{
    llong ret = 0;
    llong root = ceil(sqrt(upper));
    char* arr = (char*)malloc(root * sizeof(char));
    if (arr == NULL)
    {
        return ret;
    }

    enumerate_bitset_precomp();

    erath_less_than(arr, root);

    ret = enumerate(arr + lower, root - lower, lower, print);

    bitset_enum_func enumerator = enumerate_bitset;
    if (print != 0)
    {
        enumerator = enumerate_bitset_print;
    }

    llong i, first_chunk_start = (root < lower) ? lower : root;
    #pragma omp parallel for ordered, schedule(dynamic)
    for (i = first_chunk_start; i < upper; i += CHUNK_SIZE)
    {
        char* chunk = (char*)malloc(CHUNK_BYTES);
        llong chunk_size = CHUNK_SIZE;
        size_t chunk_bytes = CHUNK_BYTES;
        if ((i + chunk_size) > upper)
        {
            chunk_size = upper - i;
            chunk_bytes = ceil(chunk_size / 8.0);
        }
        erath1_chunk(chunk, i, i + chunk_size, arr);
        if ((chunk_size & 7) != 0)
        {
            bitset_truncate(chunk, chunk_size);
        }

        #pragma omp ordered
        ret += enumerator(chunk, chunk_bytes, i);

        free(chunk);
    }

    free(arr);

    return ret;
}

