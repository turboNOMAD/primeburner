#include "common/presieve.h"

#include <string.h> /* memset */

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

