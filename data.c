#include <stdio.h>
#include <stdlib.h>

typedef struct Data
{
    int x;
    int y;
} Data;

Data *data_create(int x, int y)
{
    Data *node = malloc(sizeof(Data));
    node->x = x;
    node->y = y;
    return node;
}

// claim ownership of char *
char *data_to_string(Data *_data)
{
#define LENGTH 32
    char *p = malloc(sizeof(char) * (LENGTH + 1));
    snprintf(p, LENGTH, "(%d,%d)", _data->x, _data->y);
    p[LENGTH] = 0;
#undef LENGTH
    return p;
}