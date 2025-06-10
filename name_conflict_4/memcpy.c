void copy (dst, src, count)
char * restrict dst, * restrict src;
int count;
{
    int i;

    for(i = 0; i < count; i++) {
        dst[i] = src[i];
    }
}
