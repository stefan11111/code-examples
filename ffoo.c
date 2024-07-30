#include <stdint.h>
#include <stddef.h>

extern void* malloc();
extern void free();
extern void* memcpy();

void swap(void *p, void *q, size_t size)
{
    /* alloca and vla's are not portable */
    void *tmp = malloc(size);
    /* assume no overlap for more efficiency */
    memcpy(tmp, p, size);
    memcpy(p, q, size);
    memcpy(q, tmp, size);

    free(tmp);
}

uintmax_t min(uintmax_t foo, uintmax_t bar, signed char sign1, signed char sign2)
{
    if(sign1 < sign2) {
        return foo;
    }

    if(sign1 > sign2) {
        return bar;
    }

    if (sign1 < 0) {
        intmax_t foo1 = *(intmax_t*)&foo;
        intmax_t bar1 = *(intmax_t*)&bar;
        return foo1 < bar1 ? foo : bar;
    }

    return foo < bar ? foo : bar;
}

extern int printf();

int main()
{
    printf("%llu\n%lld\n%lld\n%lld\n", min(-1,-2,1,1), min(-1,-2,-1,1), min(-1,-2,1,-1), min(-1,-2,-1,-1));
}
