main(argc)
{
    printf("%d\n", argc);
    if (argc == 0) {
        return;
    };
    char *argv[] = {(char*)0};
    execl("argc0", argv);
}
