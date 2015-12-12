#include "common/bucket_list.h"

#include <stdlib.h> /* malloc, free */

static int bucket_create(bucket* b, uint32_t capacity)
{
    void* arr = malloc(capacity * sizeof(presieved_prime));
    if (arr == NULL)
    {
        return 0;
    }

    b->end = b->begin = (presieved_prime*)arr;

    return 1;
}

static void bucket_destroy(bucket* b)
{
    free(b->begin);
}

static void buckets_destroy(bucket*, int32_t);

static int buckets_create(bucket** buckets, int32_t count, uint32_t capacity)
{
    int ret = 0;

    bucket* arr = (bucket*)malloc(count * sizeof(bucket));
    if (arr == NULL)
    {
        return ret;
    }
    *buckets = arr;

    for (int32_t i = 0; i < count; ++i)
    {
        ret = bucket_create(arr + i, capacity);
        if (!ret)
        {
            buckets_destroy(arr, i - 1);

            return ret;
        }
    }
    ret = 1;

    return ret;
}

static void buckets_destroy(bucket* buckets, int32_t count)
{
    for (int32_t i = 0; i < count; ++i)
    {
        bucket_destroy(buckets + i);
    }
    free(buckets);
}

bucket_list* bucket_list_create(llong prime_cutoff, uint32_t chunk_size, uint32_t bucket_capacity)
{
    bucket_list* ret = (bucket_list*)malloc(sizeof(bucket_list));
    if (ret == NULL)
    {
        return ret;
    }

    if (!bucket_create(&ret->small, bucket_capacity))
    {
        free(ret);
        ret = NULL;

        return ret;
    }

    int32_t count = 1;
    while (count <= (prime_cutoff / chunk_size) + 1)
    {
        count <<= 1;
    }
    ret->current = 0;
    ret->last = count - 1;
    ret->chunk_size = chunk_size;

    if (!buckets_create(&(ret->buckets), count, bucket_capacity))
    {
        free(ret);
        ret = NULL;
    }

    return ret;
}

void bucket_list_destroy(bucket_list* list)
{
    buckets_destroy(list->buckets, list->last + 1);
    free(list);
}

void bucket_list_initial_fill(bucket_list* list, const char* arr, uint32_t count, llong lower)
{
    uint32_t max_small_prime = list->chunk_size;

    for (uint32_t p = 0; p < count; ++p)
    {
        if (arr[p] == 0)
        {
            continue;
        }

        llong k = 2;
        if ((p * k) < lower)
        {
            k = lower / p;
            if ((p * k) < lower)
            {
                ++k;
            }
        }
        llong offset = p * k - lower;

        if (p <= max_small_prime)
        {
            presieved_prime pp = {p, offset};
            *(list->small.end++) = pp;
        }
        else
        {
            bucket_list_put(list, p, offset, 0);
        }
    }
}
