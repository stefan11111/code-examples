#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int comp(const void *a, const void *b)
{
    return strcmp(*(char**)a, *(char**)b);
}

int main()
{
    char* base[] = {"a", "c", "b" };
    qsort(base, 3, sizeof(*base), &comp);
    int i;
    for(i = 0; i < 3; i++) {
        printf("%s\n", base[i]);
    }
}
