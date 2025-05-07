static int comp(const void *a, const void *b)
{
    extern int strcmp();
    return strcmp(*(char**)a, *(char**)b);
}

void _string_sort(long a1, long a2, long a3, long a4, long a5, long a6,
                  long n)
{
    char **base = (char**)(&n + 1);

    extern int qsort();
    extern int printf();

    qsort(base, n, sizeof(char*), &comp);

    int i;

    for(i = 0; i < n; i = i + 1) {
        printf("%s\n", base[i]);
    }
}

#define MAX(a, b) a > b ? a : b

int  __attribute__((naked)) red_zone_max(int *a, int sa)
{
    volatile register int *sp asm("rsp");
    #define max sp[-1]

    max = 0;

    while(sa) {
        max = MAX(max, a[sa - 1]);
        sa--;
    }

    volatile register int eax asm("eax") = max;

    asm("ret");
}
