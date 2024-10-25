#ifndef EX13_H
#define EX13_H

#define MAX_STRING_LENGTH 100

typedef struct {
    char string[MAX_STRING_LENGTH];
    int numReaders;
    int numWriters;
} shared_data_type;

#endif
