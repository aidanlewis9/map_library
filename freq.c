/* freq.c */

#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Globals */

char * PROGRAM_NAME = NULL;

/* Functions */

void usage(int status) {
    fprintf(stderr, "Usage: %s\n", PROGRAM_NAME);
    fprintf(stderr, "    -f FORMAT        Output format (KEY, KEY_VALUE, VALUE, VALUE_KEY)\n");
    fprintf(stderr, "    -l LOAD_FACTOR   Load factor for hash table\n");
    exit(status);
}

void freq_stream(FILE *stream, double load_factor, DumpMode mode) {
    char buffer[BUFSIZ];
    Map * m = map_create(0, load_factor);
    while (fscanf(stream, "%s", buffer) != EOF)
    {
        Entry * e = map_search(m, buffer);
        if (e)
            e->value.number++;
        else
            map_insert(m, buffer, (Value) (int64_t) 1, NUMBER);
    }
    map_dump(m, stdout, mode);
    map_delete(m);
}

/* Main Execution */

int main(int argc, char *argv[]) {
    /* Parse command line arguments */
    int argind = 1;
    DumpMode mode = VALUE_KEY;
    double load_factor = DEFAULT_LOAD_FACTOR;
    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-')
    {
        char *arg = argv[argind++];
        switch (arg[1])
        {
            case 'h':
                usage(0);
                break;
            case 'f':
                if (strcmp(argv[argind], "KEY") == 0)
                    mode = KEY;
                else if (strcmp(argv[argind], "VALUE") == 0)
                    mode = VALUE;
                else if (strcmp(argv[argind], "KEY_VALUE") == 0)
                    mode = KEY_VALUE;
                else
                    mode = VALUE_KEY;
                argind++;
                break;
            case 'l':
                load_factor = atof(argv[argind++]);
                load_factor = (double) load_factor;
                break;
            default:
                usage(1);
                break;
        }
    }

    freq_stream(stdin, load_factor, mode);
    /* Compute frequencies of data from stdin */
    return EXIT_SUCCESS;
}

/* vim: set sts=4 sw=4 ts=8 expandtab ft=c: */
