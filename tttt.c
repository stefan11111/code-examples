char p[10][10];

main()
{

    *((char*)p + 10 * 3 + 3) = 'a';

//    memset(p, 'z', sizeof(p));

    printf("%c\n", p[3][3]);
}
