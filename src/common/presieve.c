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
    int count = enumerate(arr, root, 0, 0);
    *list = bucket_list_create(root, chunk_size, count);
    if (*list == 0)
    {
        return ret;
    }
    ret = enumerate(arr + *lower, root - *lower, *lower, print);
    llong newlower = (root < *lower) ? *lower : root;
    *lower = newlower;
    bucket_list_initial_fill(*list, arr, root, newlower);

    free(arr);

    return ret;
}

