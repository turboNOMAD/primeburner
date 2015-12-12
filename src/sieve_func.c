#include "sieve_func.h"

#include <string.h> /* strncmp */
#include <stdio.h>  /* printf */

#include "atkin/atkin.h"
#include "erath/erath1.h"
#include "erath/erath2.h"
#include "trydivide/trydivide.h"


typedef struct
{
    const char* name;
    sieve_func func;
} sieve;

sieve sieves[] =
{
    { "atkin", atkin },
    { "atkin_mt", atkin_mt },
    { "erath1", erath1 },
    { "erath1_mt", erath1_mt },
    { "erath2", erath2 },
    { "trydivide", trydivide },
    { NULL, NULL }
};

sieve_func find_sieve_func(const char* name)
{
    sieve_func ret = atkin_mt;
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

