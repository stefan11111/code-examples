static int comp(const void *a, const void *b)
{
    extern int strcmp();
    return strcmp(*(char**)a, *(char**)b);
}

void _string_sort(long a1, long a2, long a3, long a4, long a5, long a6,
                  volatile long n)
{
    volatile char **base = (volatile char**)(&n + 1);

    extern int qsort();
    extern int printf();

    qsort(base, n, sizeof(char*), &comp);

    int i;

    for(i = 0; i < n; i = i + 1) {
        printf("%s\n", base[i]);
    }
}

int __attribute__((naked)) red_zone()
{
    volatile register long sp asm("rsp");
    volatile register long rax asm("rax");
    volatile long *ptr = (volatile long*)sp - 2;
    ptr[1] = 10;
    rax = ptr[0];
    asm("ret");
}

#define string_sort(...) ((void (*)())_string_sort)(0, 0, 0, 0, 0, 0, sizeof((char*[]){__VA_ARGS__})/sizeof(char*), __VA_ARGS__)

int main()
{
    string_sort("foo", "bar", "baz");
    return ((int (*)())red_zone)();
}
