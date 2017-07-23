/* fnv.c: fnv hash function */

#include "map.h"

// http://isthe.com/chongo/tech/comp/fnv/

#define FNV_OFFSET_BASIS    (0xcbf29ce484222325ULL)
#define FNV_PRIME           (0x100000001b3ULL)

/**
 * Compute FNV-1 hash.
 * @param   d               Data to hash.
 * @param   n               Number of bytes in data.
 * @return  Computed hash.
 */
uint64_t	fnv_hash(const void *d, size_t n) {
    uint64_t hash = FNV_OFFSET_BASIS;
    uint8_t * bytes = (uint8_t *) d;
    for (int i = 0; i < n; i++)
        hash = (hash ^ bytes[i]) * FNV_PRIME;
    return hash;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
