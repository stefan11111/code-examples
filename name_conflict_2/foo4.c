foo()
{
    static int x = 0;
    printf("%d\n", x);
    x++;
}

main()
{
    foo();
    foo();
}
