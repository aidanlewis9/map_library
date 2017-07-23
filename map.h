/* map.h */

#ifndef MAP_H
#define MAP_H

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Type Definitions */

typedef union {
    int64_t  number;
    char    *string;
} Value;

typedef enum {
    NUMBER,
    STRING,
} Type;

typedef struct Entry Entry;
struct Entry {
    char    *key;
    Value    value;
    Type     type;
    Entry   *next;
};

typedef enum {
    KEY,
    VALUE,
    KEY_VALUE,
    VALUE_KEY,
} DumpMode;

typedef struct {
    Entry   *buckets;
    size_t   capacity;
    size_t   size;
    double   load_factor;
} Map;

/* Constants */

#define DEFAULT_CAPACITY    (1<<10)
#define DEFAULT_LOAD_FACTOR (0.900)

/* Macros */

#define debug(M, ...) \
    fprintf(stderr, "DEBUG %s:%d:%s: " M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)

/* Entry Function Prototypes */

Entry *         entry_create(const char *key, const Value value, Entry *next, Type type);
Entry *         entry_delete(Entry *e, bool recursive);
void            entry_update(Entry *e, const Value value, Type type);
void            entry_dump(Entry *e, FILE *stream, const DumpMode mode);

/* Hash Function Prototypes */

uint64_t	fnv_hash(const void *d, size_t n);

/* Map Function Prototypes */

Map *	        map_create(size_t capacity, double load_factor);
Map *	        map_delete(Map *m);

void            map_insert(Map *m, const char *key, const Value value, Type type);
Entry *         map_search(Map *m, const char *key);
bool            map_remove(Map *m, const char *key);

void		map_dump(Map *m, FILE *stream, const DumpMode mode);
void            map_resize(Map *m, size_t new_capacity);

#endif

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
