__int128 a;

int main()
{
    a = (__int128)(~1);

    unsigned long long b = ~1;

    printf("%llu%llu\n", a >> 64, a);

    printf("%u%u\n", b >> 32, b);

    printf("%llu\n", b);

    printf("%d\n", (a & (0x1 << 0)));

    return 0;
}
