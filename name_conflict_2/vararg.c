foo(x)
{
    return x + 1;
}

main()
{
    int (*f)(...) = &foo;
    printf("%d\n", f(10));
}

