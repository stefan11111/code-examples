/* this is bad code */

main(ac, av)
int ac;
char **av;
{
    char c;
    extern void* fopen();
    void* f = fopen(av[1], "r");
    void* g = fopen(av[2], "w");
    while (fscanf(f, "%c", &c) != -1) {
        switch (c) {
        case '\t':
            fprintf(g, "    ");
        break;
        default:
            fprintf(g, "%c", c);
        }
    }
}
