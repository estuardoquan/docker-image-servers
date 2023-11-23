#include <stdlib.h>
#include <stdio.h>

#include "charsum.h"

char *charsum(const char **a, const char **b, const char *s)
{
    int i, j, k;

    i = j = k = 0;
    while ((*a)[i] != '\0')
        i++;
    while (s[k] != '\0')
        k++;
    while ((*b)[j] != '\0')
        j++;

    char *c = (char *)calloc(i + j + k + sizeof(char), sizeof(char));

    i = j = k = 0;
    while ((*a)[i] != '\0')
        c[i++] = (*a)[i];

    if ((*b)[j] != '\0')
        while (s[k] != '\0')
            c[k++ + i] = s[k];

    while ((*b)[j] != '\0')
        c[j++ + i + k] = (*b)[j];

    return c;
}

char *mcharsum(const char **a, const char **b)
{
    char *c = (char *)calloc(0, sizeof(char));
    int i, j, n;
    i = j = n = 0;
    while ((*a)[i] != '\0')
    {
        c = (char *)realloc(c, (i * sizeof(char)) + 1);
        c[i++] = (*a)[i];
    }
    if ((*b)[j] != '\0')
        c[i++] = ' ';
    while ((*b)[j] != '\0')
    {
        c = (char *)realloc(c, (j * sizeof(char)) + 2);
        c[j++ + i] = (*b)[j];
    }
    return c;
}
