#include "atkin_q1.h"

#include <math.h> /* ceil, sqrt */

#include "bitset.h"

/* Apply to 1, 13, 17, 29, 37, 41, 49, 53 mod 60 */
static char atkin_q1_filter[60] =
{
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
};

/* Seeds for the lattice point visitor */
static char atkin_q1_x0[15 * 30];
static char atkin_q1_y0[15 * 30];

static inline void atkin_q1_visit(char* chunk, llong lower, llong range, llong x, llong y0)
{
    for (; x > 0; x-= 15)
    {
        llong q = 4 * x * x + y0 * y0 - lower;

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

void atkin_q1_precomp()
{
    int i = 0;

    for (int x = 1; x <= 15; ++x)
    {
        for (int y = 1; y <= 30; ++y)
        {
            if (atkin_q1_filter[(4 * x * x + y * y) % 60] != 0)
            {
                atkin_q1_x0[i] = x;
                atkin_q1_y0[i] = y;

                ++i;
            }
        }
    }

    atkin_q1_x0[i] = 0;
    atkin_q1_y0[i] = 0;
}

void atkin_chunk_q1(char* chunk, llong lower, llong upper)
{
    llong range = upper - lower;
    /* Skip all the points guaranteed to be out of our chunk */
    llong delta = ceil(sqrt(upper) / 2);
    delta = delta / 15 * 15;

    for (unsigned i = 0; atkin_q1_x0[i] + atkin_q1_y0[i] > 0; ++i)
    {
        atkin_q1_visit(chunk, lower, range, atkin_q1_x0[i] + delta, atkin_q1_y0[i]);
    }
}

