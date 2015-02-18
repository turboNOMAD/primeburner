#include "erath.h"

#include <stdlib.h> /* malloc, free */
#include <string.h> /* memset */
#include <math.h>   /* ceil, sqrt */

static inline int erath_chunk(char* chunk, llong lower, llong upper, const char* sieved)
{
    const llong chunk_size = upper - lower;

    memset(chunk, 1, chunk_size);
    for (llong p = 2; p*p < upper; ++p)
    {
        if (sieved[p] != 0)
        {
            llong mod = lower % p;
            llong i = p * (mod != 0) - mod;
            for (; i < chunk_size; i += p)
            {
                chunk[i] = 0;
            }
        }
    }

    return 0;
}

int erath_less_than(char* arr, llong upper)
{
    memset(arr, 1, upper);
    arr[0] = 0;
    arr[1] = 0;

    for (llong p = 2; p*p < upper; ++p)
    {
        if (arr[p] != 0)
        {
            for (llong i = p * 2; i < upper; i += p)
            {
                arr[i] = 0;
            }
        }
    }

    return 0;
}

llong erath(llong lower, llong upper, int print)
{
    llong ret = 0;
    llong root = ceil(sqrt(upper));
    char* arr = (char*)malloc(root * sizeof(char));
    if (arr == NULL)
    {
        return ret;
    }

    erath_less_than(arr, root);

    ret = enumerate(arr + lower, root - lower, lower, print);

    char chunk[ERATH_CHUNK_SIZE];
    llong chunk_lower = (root < lower) ? lower : root;
    llong chunk_upper = chunk_lower + ERATH_CHUNK_SIZE;
    for (; chunk_upper < upper; chunk_lower += ERATH_CHUNK_SIZE, chunk_upper += ERATH_CHUNK_SIZE)
    {
        erath_chunk(chunk, chunk_lower, chunk_upper, arr);
        ret += enumerate(chunk, ERATH_CHUNK_SIZE, chunk_lower, print);
    }
    erath_chunk(chunk, chunk_lower, upper, arr);
    ret += enumerate(chunk, upper - chunk_lower, chunk_lower, print);

    free(arr);

    return ret;
}

llong erath_mt(llong lower, llong upper, int print)
{
    llong ret = 0;
    llong root = ceil(sqrt(upper));
    char* arr = (char*)malloc(root * sizeof(char));
    if (arr == NULL)
    {
        return ret;
    }

    erath_less_than(arr, root);

    ret = enumerate(arr + lower, root - lower, lower, print);

    llong i, first_chunk_start = (root < lower) ? lower : root;
    #pragma omp parallel for ordered, schedule(dynamic)
    for (i = first_chunk_start; i < upper; i += ERATH_CHUNK_SIZE)
    {
        char chunk[ERATH_CHUNK_SIZE];
        llong chunk_size = ERATH_CHUNK_SIZE;
        if ((i + chunk_size) > upper)
        {
            chunk_size = upper - i;
        }
        erath_chunk(chunk, i, i + chunk_size, arr);

        #pragma omp ordered
        ret += enumerate(chunk, chunk_size, i, print);
    }

    free(arr);

    return ret;
}

