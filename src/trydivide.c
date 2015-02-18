#include "trydivide.h"

#include <stdio.h> /* printf */

static inline int trydivide_one(llong p)
{
    for (llong x = 2; x*x <= p; ++x)
    {
        if ((p % x) == 0)
        {
            return 0;
        }
    }

    return 1;
}

llong trydivide(llong lower, llong upper, int print)
{
    llong ret = 0;

    for (llong p = lower; p < upper; ++p)
    {
        int is_prime = trydivide_one(p);

        if (is_prime)
        {
            ++ret;

            if (print)
            {
                printf("%llu\n", p);
            }
        }
    }

    return ret;
}

