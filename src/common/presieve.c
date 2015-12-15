#include "common/presieve.h"

#include <string.h> /* memset */
#include <math.h>   /* ceil, sqrt */
#include <stdlib.h> /* malloc, free */

#include "common/enum.h"

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

static llong calc_offset(uint32_t p, llong lower)
{
    llong k = p;
    if ((p * k) < lower)
    {
        k = lower / p;
        if ((p * k) < lower)
        {
            ++k;
        }
    }

    return (p * k - lower);
}

static void presieve_small_primes(bucket_list* list, char* arr, uint32_t count, llong lower)
{
    uint32_t chunk_size = list->chunk_size;
    uint32_t max_small_prime = chunk_size;
    if (max_small_prime > count)
    {
        max_small_prime = count;
    }

    uint32_t p = 0;
    for (; p < max_small_prime; ++p)
    {
        if (arr[p] == 0)
        {
            continue;
        }

        uint32_t offset = calc_offset(p, lower) % chunk_size;
        presieved_prime pp = {p, offset};
        *(list->small.end++) = pp;
    }
    list->presieved_primes = arr;
    list->max_presieved = p - 1;

    presieved_prime pp = {SMALL_PRIMES_END, 0};
    *(list->small.end++) = pp;
}

void presieve_large_primes(bucket_list* list, uint32_t bucket_index, uint32_t maximum, llong lower)
{
    char* arr = list->presieved_primes;
    uint32_t p = list->max_presieved;
    for(; p < maximum; ++p)
    {
        if (arr[p] == 0)
        {
            continue;
        }

        bucket_list_put(list, p, calc_offset(p, lower), bucket_index);
    }
    list->max_presieved = p - 1;
}

llong presieve(llong* lower, llong upper, bucket_list** list, uint32_t chunk_size, int print)
{
    llong ret = 0;
    llong root = ceil(sqrt(upper));
    char* arr = (char*)malloc(root * sizeof(char));
    if (arr == NULL)
    {
        return ret;
    }

    erath_less_than(arr, root);
    int count = enumerate(arr, arr + root, 0, 0);
    *list = bucket_list_create(root, chunk_size, count);
    if (*list == 0)
    {
        return ret;
    }

    llong oldlower = *lower;
    llong newlower = *lower;
    if (oldlower < root)
    {
        newlower = root;
        ret = enumerate(arr + oldlower, arr + root, oldlower, print);
        *lower = newlower;
    }

    presieve_small_primes(*list, arr, root, newlower);

    return ret;
}

