/* entry.c: map entry */

#include "map.h"

/**
 * Create entry structure.
 * @param   key             Entry's key.
 * @param   value           Entry's value.
 * @param   next            Reference to next entry.
 * @param   type            Entry's value's type.
 * @return  Allocate Entry structure.
 */
Entry *		entry_create(const char *key, const Value value, Entry *next, Type type) {
    Entry * new = calloc(1, sizeof(Entry));
    if (new == NULL)
        exit(1);
    new->key = strdup(key);
    if (new->key == NULL)
        exit(2);
    new->type = type;
    if (type == STRING)
    {
        new->value.string = strdup(value.string);
        if (new->value.string == NULL)
            exit(3);
    }
    else
        new->value.number = value.number;
    new->next = next;
    return new;
}

/**
 * Delete entry structure.
 * @param   e               Entry structure.
 * @param   recursive       Whether or not to delete remainder of entries.
 * return   NULL.
 */
Entry *		entry_delete(Entry *e, bool recursive) {
    if (recursive && e->next != NULL)
        entry_delete(e->next, true);
    if (e->type == STRING)
        free(e->value.string);
    free(e->key);
    free(e);
    
    return NULL;
}

/**
 * Update entry's value.
 * @param   e               Entry structure.
 * @param   value           New value for entry.
 * @param   type            New value's type.
 */
void            entry_update(Entry *e, const Value value, Type type) {
    if (e != NULL && e->type == STRING)
        free(e->value.string);
    if (type == STRING)
    {
        e->value.string = strdup(value.string);
        if (e->value.string == NULL)
            exit(4);
    }
    else
        e->value.number = value.number;
    e->type = type;
}

/**
 * Dump entry.
 * @param   e               Entry structure.
 * @param   stream          File stream to write to.
 * @param   mode            Dump mode to use.
 */
void            entry_dump(Entry *e, FILE *stream, const DumpMode mode) {
    if (e == NULL)
        return;
    if (mode == KEY)
        printf("%s\n", e->key);
    else if (mode == VALUE && e->type == STRING)
        printf("%s\n", e->value.string);
    else if (mode == VALUE && e->type == NUMBER)
        printf("%zu\n", e->value.number);
    else if (mode == KEY_VALUE && e->type == STRING)
        printf("%s\t%s\n", e->key, e->value.string);
    else if (mode == KEY_VALUE && e->type == NUMBER)
        printf("%s\t%zu\n", e->key, e->value.number);
    else if (mode == VALUE_KEY && e->type == STRING)
        printf("%s\t%s\n", e->value.string, e->key);
    else if (mode == VALUE_KEY && e->type == NUMBER)
        printf("%zu\t%s\n", e->value.number, e->key);
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
