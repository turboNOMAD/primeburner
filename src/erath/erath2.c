#include "erath/erath2.h"

#include <stdlib.h> /* malloc, free */
#include <string.h> /* memset */
#include <math.h>   /* ceil, sqrt */

#define CHUNK_WIDTH 18
#include "common/defs.h"

#include "common/enum.h"
#include "common/presieve.h"
#include "common/bitset.h"

static inline uint32_t erath2_chunk_sieve_small(char* chunk, bucket_list* list, uint32_t maximum)
{
    llong chunk_size = list->chunk_size;

    uint32_t p = 0;
    for (presieved_prime* pp = list->small.begin; (p = pp->p) < maximum; ++pp)
    {
        llong i = pp->offset;
        for (; i < chunk_size; i += p)
        {
            bitset_clear(chunk, i);
        }
        pp->offset = i - chunk_size;
    }

    return p;
}

static inline void erath2_chunk(char* chunk, bucket_list* list, uint32_t bucket_index)
{
    llong chunk_size = list->chunk_size;
    bucket* b = list->buckets + bucket_index;
    presieved_prime* end_marker = b->end;

    for (presieved_prime* pp = b->begin; pp < end_marker; ++pp)
    {
        llong i = pp->offset;
        uint32_t p = pp->p;
        for (; i < chunk_size; i += p)
        {
            bitset_clear(chunk, i);
        }
        bucket_list_put(list, p, i, bucket_index);
    }
    bucket_list_release_bucket(b);
}

llong erath2(llong lower, llong upper, int print)
{
    llong ret = 0;

    enumerate_bitset_precomp();

    bucket_list* list = NULL;

    ret = presieve(&lower, upper, &list, CHUNK_SIZE, print);
    if (list == NULL)
    {
        return ret;
    }

    char* chunk = (char*)malloc(CHUNK_BYTES);
    if (chunk == NULL)
    {
        bucket_list_destroy(list);
        
        return 0;
    }

    bitset_enum_func enumerator = enumerate_bitset;
    if (print != 0)
    {
        enumerator = enumerate_bitset_print;
    }


    llong chunk_lower = lower;
    llong chunk_upper = chunk_lower + CHUNK_SIZE;
    for (; chunk_lower < upper; chunk_lower = chunk_upper, chunk_upper += CHUNK_SIZE)
    {
        unsigned chunk_bytes = CHUNK_BYTES;
        if (chunk_upper > upper)
        {
            chunk_upper = upper;
            chunk_bytes = ceil((chunk_upper - chunk_lower) / 8.0);
        }
        memset(chunk, 0xFF, chunk_bytes);
        uint32_t maximum = ceil(sqrt(chunk_upper));
        uint32_t last_small_prime = erath2_chunk_sieve_small(chunk, list, maximum);
        if (last_small_prime == SMALL_PRIMES_END)
        {
            uint32_t bucket_index = bucket_list_get_bucket(list);
            presieve_large_primes(list, bucket_index, maximum, chunk_lower);
            erath2_chunk(chunk, list, bucket_index);
        }
        bitset_truncate(chunk, chunk_upper - chunk_lower - 1);
        ret += enumerator(chunk, chunk_bytes, chunk_lower);
    }

    free(chunk);
    bucket_list_destroy(list);

    return ret;
}

