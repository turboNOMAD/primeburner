#include <stdio.h>  /* printf */
#include <getopt.h> /* getopt_long */
#include <stdlib.h> /* exit, atoll */

#include "sieve_func.h"

int print = 1;
llong lower = 2;
llong upper = 1000000000;
extern sieve_func func;

void print_help()
{
    printf("Usage: primesieve [-q] [-l N] [-u N] [-m METHOD]\n" \
           "Generate all prime numbers in the range [lower, upper)\n" \
           "using a selected method, and print them to standard output.\n" \
           "\n" \
           "  -q              do not print the numbers, only count them\n" \
           "                  this is the fastest mode, usable for benchmarking\n" \
           "\n" \
           "  -l N            lower bound for sieving range (default: 2)\n" \
           "\n" \
           "  -u N            upper bound for sieving range (default: 1 billion)\n" \
           "\n" \
           "  -m METHOD       method of sieving (default: atkin_mt)\n" \
           "\n" \
           "                  Currently supported methods:\n");

    enum_sieve_funcs();
}

void parse_cmdline(int argc, char* argv[])
{
    int opt;
    extern char *optarg;
    struct option longopts[] =
    {
        {"help", 0, NULL, 'h'},
        {0}
    };

    while ((opt = getopt_long(argc, argv, "hql:u:m:", longopts, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);
        case 'q':
            print = 0;
            break;
        case 'l':
            lower = atoll(optarg);
            break;
        case 'u':
            upper = atoll(optarg);
            break;
        case 'm':
            func = find_sieve_func(optarg);
            break;
        }
    }
}

int check_args()
{
    if (lower < 2)
    {
        printf("Invalid lower bound = %lld specified!\n", lower);
        return 0;
    }

    if (upper < lower)
    {
        printf("Upper bound below lower! (%lld > %lld)\n", lower, upper);
        return 0;
    }

    return 1;
}

int main(int argc, char* argv[])
{
    parse_cmdline(argc, argv);

    if (!check_args())
    {
        return 2;
    }

    llong res = func(lower, upper + 1, print);

    if (res > 0)
    {
        printf("%llu primes found.\n", res);
    }
    else
    {
        printf("Failed to find any primes!\n");
        return 1;
    }

    return 0;
}
