main()
{
    char s[100];
    memset(s, 'a' ,sizeof(s));
    s[5] = 0;
    scanf("%3s", s);
    printf(s);
    scanf("%3s", s);
    printf(s);
    return 0;
}
