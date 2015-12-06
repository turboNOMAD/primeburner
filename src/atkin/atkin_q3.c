#include "atkin/atkin_q3.h"

#include <math.h> /* sqrt */

#include "common/bitset.h"

/* Apply to 11,23,47,59 mod 60 */
static char atkin_q3_filter[60] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1
};

/* Seeds for the lattice point visitor */
static char atkin_q3_x0[10 * 30];
static char atkin_q3_y0[10 * 30];

static inline void atkin_q3_visit(char* chunk, llong lower, llong range, llong x, llong y0)
{
    while (1)
    {
        llong q = (3 * x * x - y0 * y0) - lower;

        while (q >= range)
        {
            if (x <= y0)
            {
                return;
            }

            q -= y0 * 60 + 900;
            y0 += 30;
        }

        for (llong y = y0; (q >= 0) && (y < x); q -= y * 60 + 900, y += 30)
        {
            bitset_flip(chunk, q);
        }

        x += 10;
    }
}

void atkin_q3_precomp()
{
    int i = 0;

    for (int x = 1; x <= 10; ++x)
    {
        for (int y = 1; y <= 30; ++y)
        {
            int q = 3 * x * x - y * y;

            while (q < 0)
            {
                q += 60;
            }

            if (atkin_q3_filter[q % 60] != 0)
            {
                atkin_q3_x0[i] = x;
                atkin_q3_y0[i] = y;

                ++i;
            }
        }
    }

    atkin_q3_x0[i] = 0;
    atkin_q3_y0[i] = 0;
}

void atkin_chunk_q3(char* chunk, llong lower, llong upper)
{
    llong range = upper - lower;
    /* Skip all the points guaranteed to be out of our chunk */
    llong delta = sqrt((lower) / 3);
    delta = delta / 10 * 10;

    for (unsigned i = 0; atkin_q3_x0[i] + atkin_q3_y0[i] > 0; ++i)
    {
        atkin_q3_visit(chunk, lower, range, atkin_q3_x0[i] + delta, atkin_q3_y0[i]);
    }
}

