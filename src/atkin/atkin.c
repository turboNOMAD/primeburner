#include "atkin.h"
#include "atkin_q1.h"
#include "atkin_q2.h"
#include "atkin_q3.h"

#include <stdlib.h> /* malloc, free */
#include <string.h> /* memset */
#include <math.h>   /* ceil, sqrt */

#include "erath.h"
#include "bitset.h"

static inline int atkin_chunk_sieve_psquares(char* chunk, llong lower, llong upper, const char* sieved)
{
    const llong chunk_size = upper - lower;

    for (llong p = 7; p * p < upper; ++p)
    {
        if (sieved[p] == 1)
        {
            llong p_sqr = p*p;
            llong mod = lower % p_sqr;
            llong i = p_sqr * (mod != 0) - mod;
            for (; i < chunk_size; i += p_sqr)
            {
                bitset_clear(chunk, i);
            }
        }
    }

    return 0;
}

static inline int atkin_chunk(char* chunk, llong lower, llong upper, const char* sieved)
{
    memset(chunk, 0, ATKIN_CHUNK_BYTES);
    atkin_chunk_q1(chunk, lower, upper);
    atkin_chunk_q2(chunk, lower, upper);
    atkin_chunk_q3(chunk, lower, upper);
    atkin_chunk_sieve_psquares(chunk, lower, upper, sieved);

    return 0;
}

static inline void atkin_precomp()
{
    atkin_q1_precomp();
    atkin_q2_precomp();
    atkin_q3_precomp();
}

llong atkin(llong lower, llong upper, int print)
{
    llong ret = 0;
    llong root = sqrt(upper) + 1;
    char* arr = (char*)malloc(root * sizeof(char));
    if (arr == NULL)
    {
        return ret;
    }

    enumerate_bitset_precomp();
    atkin_precomp();

    erath_less_than(arr, root);
    ret = enumerate(arr + lower, root - lower, lower, print);

    char* chunk = (char*)malloc(ATKIN_CHUNK_BYTES);
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
    llong chunk_upper = chunk_lower + ATKIN_CHUNK_SIZE ;
    for (; chunk_upper < upper; chunk_lower += ATKIN_CHUNK_SIZE, chunk_upper += ATKIN_CHUNK_SIZE)
    {
        atkin_chunk(chunk, chunk_lower, chunk_upper, arr);
        ret += enumerator(chunk, ATKIN_CHUNK_BYTES, chunk_lower);
    }

    unsigned last_chunk_bytes = ceil((upper - chunk_lower) / 8.0);
    atkin_chunk(chunk, chunk_lower, upper, arr);
    ret += enumerator(chunk, last_chunk_bytes, chunk_lower);

    free(chunk);
    free(arr);

    return ret;
}

llong atkin_mt(llong lower, llong upper, int print)
{
    llong ret = 0;
    llong root = sqrt(upper) + 1;
    char* arr = (char*)malloc(root * sizeof(char));
    if (arr == NULL)
    {   
        return ret;
    }

    enumerate_bitset_precomp();
    atkin_precomp();

    erath_less_than(arr, root);
    ret = enumerate(arr + lower, root - lower, lower, print);

    bitset_enum_func enumerator = enumerate_bitset;
    if (print != 0)
    {
        enumerator = enumerate_bitset_print;
    }

    llong i, first_chunk_start = (root < lower) ? lower : root; 
    #pragma omp parallel for ordered, schedule(dynamic)
    for (i = first_chunk_start; i < upper; i += ATKIN_CHUNK_SIZE)
    {
        char* chunk = malloc(ATKIN_CHUNK_BYTES);
        llong chunk_size = ATKIN_CHUNK_SIZE;
        if ((i + chunk_size) > upper)
        {
            chunk_size = upper - i;
        }
        atkin_chunk(chunk, i, i + chunk_size, arr);

        #pragma omp ordered
        ret += enumerator(chunk, ATKIN_CHUNK_BYTES, i);

        free(chunk);
    }

    free(arr);

    return ret;
}

