/* map.c: separate chaining hash table */

#include "map.h"

/**
 * Create map data structure.
 * @param   capacity        Number of buckets in the hash table.
 * @param   load_factor     Maximum load factor before resizing.
 * @return  Allocated Map structure.
 */
Map *	        map_create(size_t capacity, double load_factor) {
    Map *new = calloc(1, sizeof(Map));
    if (new == NULL)
        exit(1);
    if (capacity)
        new->capacity = capacity;
    else
        new->capacity = DEFAULT_CAPACITY;
    new->buckets = calloc(new->capacity, sizeof(Entry));
    if (new->buckets == NULL)
        exit(1);
    if (load_factor > 0)
        new->load_factor = load_factor;
    else
        new->load_factor = DEFAULT_LOAD_FACTOR;
    new->size = 0;
    
    return new;
}

/**
 * Delete map data structure.
 * @param   m               Map data structure.
 * @return  NULL.
 */
Map *	        map_delete(Map *m) {
    for (int i = 0; i < m->capacity; i++)
    {
        if (m->buckets[i].next)
            entry_delete(m->buckets[i].next, true);
    }
    free(m->buckets);
    free(m);
    return NULL;
}

/**
 * Insert or update entry into map data structure.
 * @param   m               Map data structure.
 * @param   key             Entry's key.
 * @param   value           Entry's value.
 * @param   type            Entry's value's type.
 */

void            map_insert(Map *m, const char *key, const Value value, Type type) {
    Entry *e = map_search(m, key);
    if (e)
    {
        entry_update(e, value, type);
    }
    else
    {
        if (((double)m->size + 1)/(double)m->capacity > m->load_factor)
        {
            map_resize(m, m->capacity*2);
        }
        int bucket = fnv_hash(key, strlen(key)) % m->capacity;
        m->buckets[bucket].next = entry_create(key, value, m->buckets[bucket].next, type);
        m->size++;
    }
}

/**
 * Search map data structure by key.
 * @param   m               Map data structure.
 * @param   key             Key of the entry to search for.
 * @param   Pointer to the entry with the specified key (or NULL if not found).
 */

Entry *         map_search(Map *m, const char *key) {
    int bucket = fnv_hash(key, strlen(key)) % m->capacity;
    Entry *e = m->buckets[bucket].next;
    while (e)
    {
        if (fnv_hash(e->key, strlen(e->key)) == fnv_hash(key, strlen(key)))
        {
            return e;
        }
        e = e->next;
    }
    return NULL;
}

/**
 * Remove entry from map data structure with specified key.
 * @param   m               Map data structure.
 * @param   key             Key of the entry to remove.
 * return   Whether or not the removal was successful.
 */
bool            map_remove(Map *m, const char *key) {
    int bucket = fnv_hash(key, strlen(key)) % m->capacity;
    Entry *prev = NULL;
    Entry *curr = m->buckets[bucket].next;
    if (curr == NULL)
        return false;
    Entry *next = m->buckets[bucket].next->next;
    while (curr)
    {
        if (fnv_hash(curr->key, strlen(curr->key)) == fnv_hash(key, strlen(key)))
        {
            if (prev)
                prev->next = next;
            else
                m->buckets[bucket].next = next;
            entry_delete(curr, false);
            m->size--;
            return true;
        }
        prev = curr;
        curr = next;
        if (next == NULL)
            return false;
        next = next->next;
    }
    return false;
}

/**
 * Dump all the entries in the map data structure.
 * @param   m               Map data structure.
 * @param   stream          File stream to write to.
 * @param   mode            Dump mode to use.
 */
void		map_dump(Map *m, FILE *stream, const DumpMode mode) {
    for (int i = 0; i < m->capacity; i++)
    {
        Entry *e = m->buckets[i].next;
        while (e)
        {
            entry_dump(e, stream, mode);
            e = e->next;
        }
    }
}

/**
 * Resize the map data structure.
 * @param   m               Map data structure.
 * @param   new_capacity    New capacity for the map data structure.
 */
void            map_resize(Map *m, size_t new_capacity) {
    Entry * new = calloc(new_capacity, sizeof(Entry));
    if (new == NULL)
        exit(1);
    for (int i = 0; i < m->capacity; i++)
    {
        Entry * e = m->buckets[i].next;
        while (e != NULL)
        {
            int bucket = fnv_hash(e->key, strlen(e->key)) % new_capacity;
            Entry *temp = e->next;
            e->next = new[bucket].next;
            new[bucket].next = e;
            e = temp;
        }
    }
    m->capacity = new_capacity;
    free(m->buckets);
    m->buckets = new;  
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */

