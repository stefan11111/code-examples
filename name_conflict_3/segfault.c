main()
{
    volatile char s[10];
    volatile char *p = s;
    for (;;) {
        *(p++) = 0;
    }
}
