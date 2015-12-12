#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>

typedef int64_t llong;

typedef struct
{
    uint32_t p;
    uint32_t offset;
} __attribute__((packed)) presieved_prime;

#define CHUNK_SIZE (1L << (CHUNK_WIDTH))
#define CHUNK_BYTES (1L << ((CHUNK_WIDTH) - 3))

#endif

