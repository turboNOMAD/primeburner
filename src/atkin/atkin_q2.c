#include "atkin/atkin_q2.h"

#include <math.h> /* ceil, sqrt */

#include "common/bitset.h"

/* Apply to 7,19,31,43 mod 60 */
static char atkin_q2_filter[60] =
{
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Seeds for the lattice point visitor */
static char atkin_q2_x0[10 * 30];
static char atkin_q2_y0[10 * 30];

static inline void atkin_q2_visit(char* chunk, llong lower, llong range, llong x, llong y0)
{
    for (; x > 0; x-= 10)
    {
        llong q = 3 * x * x + y0 * y0 - lower;

        while (q < 0)
        {
            q += y0 * 60 + 900;
            y0 += 30;
        }

        for (llong y = y0; q < range; q += y * 60 + 900, y += 30)
        {
            bitset_flip(chunk, q);
        }
    }
}

void atkin_q2_precomp()
{
    int i = 0;

    for (int x = 1; x <= 10; ++x)
    {
        for (int y = 1; y <= 30; ++y)
        {
            if (atkin_q2_filter[(3 * x * x + y * y) % 60] != 0)
            {
                atkin_q2_x0[i] = x;
                atkin_q2_y0[i] = y;

                ++i;
            }
        }
    }

    atkin_q2_x0[i] = 0;
    atkin_q2_y0[i] = 0;
}

void atkin_chunk_q2(char* chunk, llong lower, llong upper)
{
    llong range = upper - lower;
    /* Skip all the points guaranteed to be out of our chunk */
    llong delta = ceil(sqrt(upper / 3));
    delta = delta / 10 * 10;

    for (unsigned i = 0; atkin_q2_x0[i] + atkin_q2_y0[i] > 0; ++i)
    {
        atkin_q2_visit(chunk, lower, range, atkin_q2_x0[i] + delta, atkin_q2_y0[i]);
    }
}

