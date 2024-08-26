#include <stdint.h>
#include <stddef.h>

extern void* malloc();
extern void free();
extern void* memcpy();

#define swap(x, y) _swap(&(x), &(y), sizeof((x)))

void _swap(void *p, void *q, size_t size)
{
    /* alloca and vla's are not portable */
    void *tmp = malloc(size);
    /* assume no overlap for more efficiency */
    memcpy(tmp, p, size);
    memcpy(p, q, size);
    memcpy(q, tmp, size);

    free(tmp);
}

#define min(x, y) _min((x), (y), (((x) < ((x) * 0)) ? -1 : 1), (((y) < ((y) * 0)) ? -1 : 1))

uintmax_t _min(uintmax_t foo, uintmax_t bar, signed char sign1, signed char sign2)
{
    if(sign1 < sign2) {
        return foo;
    }

    if(sign1 > sign2) {
        return bar;
    }

    if (sign1 < 0) {
        intmax_t foo1 = (intmax_t)foo;
        intmax_t bar1 = (intmax_t)bar;
        return foo1 < bar1 ? foo : bar;
    }

    return foo < bar ? foo : bar;
}

extern int printf();

int main()
{
    printf("%lld\n", min(-1, -2));
}
