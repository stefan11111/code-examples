foo()
{
    return 256;
}

bar()
{
    return 0;
}

main(void) {
    void *ptr = &foo;
    char foo1 = ((char (*)())ptr)();
    int foo2 = ((int (*)())ptr)();
    printf("%d\n", foo1 == foo2); /* 0 */

    ptr = &bar;

    foo1 = ((char (*)())ptr)();
    foo2 = ((int (*)())ptr)();
    printf("%d\n", foo1 == foo2); /* 1 */
    return 0;
}
