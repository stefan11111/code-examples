main()
{
    char p[100];
    char c;
    extern void* fopen();
    void *f = fopen("foofffoo.txt", "r");
    while (fscanf(f, "%50[^\n\n]", p) != -1) {
        fscanf(f, "%c", &c);
        printf("%s\n", p);
    }
}
