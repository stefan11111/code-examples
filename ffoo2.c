#include <stddef.h>
#include <stdint.h>

extern void* malloc();
extern void free();
extern void* memcpy();

#define small_memcpy(dst, src, x) *(x*)dst = *(x*)src

void swap(void *p, void *q, size_t size)
{
#ifdef CHAR_BIT /* should be for any C implementation */
#if CHAR_BIT != 8 /* posix requires CHAR_BIT == 8, but not everything is posix */
    /* alloca and vla's are not portable */
    void *tmp = malloc(size);
    /* assume no overlap for more efficiency */
    memcpy(tmp, p, size);
    memcpy(p, q, size);
    memcpy(q, tmp, size);

    free(tmp);
    return;
#endif
#endif

    switch (size) {
    case 1:
    int8_t tmp1;
    small_memcpy(&tmp1, p, int8_t);
    small_memcpy(p, q, int8_t);
    small_memcpy(q, &tmp1, int8_t);
    return;
    case 2:
    int16_t tmp2;
    small_memcpy(&tmp2, p, int16_t);
    small_memcpy(p, q, int16_t);
    small_memcpy(q, &tmp2, int16_t);
    return;
    case 4:
    int32_t tmp4;
    small_memcpy(&tmp4, p, int32_t);
    small_memcpy(p, q, int32_t);
    small_memcpy(q, &tmp4, int32_t);
    return;
    case 8:
    int64_t tmp8;
    small_memcpy(&tmp8, p, int64_t);
    small_memcpy(p, q, int64_t);
    small_memcpy(q, &tmp8, int64_t);
    return;
    default:
    /* alloca and vla's are not portable */
    void *tmp = malloc(size);
    /* assume no overlap for more efficiency */
    memcpy(tmp, p, size);
    memcpy(p, q, size);
    memcpy(q, tmp, size);

    free(tmp);
    return;
    }
}
