#ifndef BUCKET_LIST_H
#define BUCKET_LIST_H

#include "common/defs.h"

typedef struct
{
    presieved_prime* begin;
    presieved_prime* end;
} bucket;

typedef struct
{
    bucket small;
    bucket* buckets;
    uint32_t current;
    uint32_t last;
    uint32_t chunk_size;

    char* presieved_primes;
    uint32_t presieved_threshold;
} bucket_list;

bucket_list* bucket_list_create(llong, uint32_t, uint32_t);
void bucket_list_destroy(bucket_list*);

inline void bucket_list_put(bucket_list* list, uint32_t p, llong offset, uint32_t index)
{
    uint32_t chunk_size = list->chunk_size;
    uint32_t buckets_to_skip = offset / chunk_size;
    uint32_t final_index = (index + buckets_to_skip) & list->last;
    uint32_t final_offset = offset - buckets_to_skip * chunk_size;
    bucket* b = list->buckets + final_index;
    presieved_prime pp = {p, final_offset};
    *b->end++ = pp;
}

inline uint32_t bucket_list_get_bucket(bucket_list* list)
{
    uint32_t current = list->current;
    uint32_t ret = current;
    list->current = (current + 1) & list->last;

    return ret;
}

inline void bucket_list_release_bucket(bucket* bucket)
{
    bucket->end = bucket->begin;
}

#endif

