#include "sieve_func.h"

#include <string.h> /* strncmp */
#include <stdio.h>  /* printf */

#include "atkin.h"
#include "erath.h"
#include "trydivide.h"

sieve_func func = atkin_mt;


typedef struct
{
    const char* name;
    sieve_func func;
} sieve;

sieve sieves[] =
{
    { "atkin", atkin },
    { "atkin_mt", atkin_mt },
    { "erath", erath },
    { "erath_mt", erath_mt },
    { "trydivide", trydivide },
    { NULL, NULL }
};

sieve_func find_sieve_func(const char* name)
{
    sieve_func ret = func;
    size_t name_len = strlen(name);
    for (sieve* p = sieves; p->name != NULL; ++p)
    {
        if (strncmp(name, p->name, name_len) == 0)
        {
            ret = p->func;
            break;
        }
    }

    return ret;
}

void enum_sieve_funcs()
{
    for (sieve* p = sieves; p->name != NULL; ++p)
    {
        printf("                  %s\n", p->name);
    }
}

