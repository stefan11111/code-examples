main()
{
/* 1 for -fno-fast-math */
/* 8388608 for -ffast-math */
    int foo = 0;
    int bar = 8388608;
    while (0.0f == *(float*)&foo) {
        foo++;
    }
    printf("%d %.38f %.38f\n", foo, *(float*)&foo, *(float*)&bar);
}
