#include <stdio.h>

main(ac, av)
int ac;
char **av;
{
    int fd = open(av[1], 66, 0664);
    if (fd < 0) {
         perror(open);
         return;
    }
    read(fd, &ac, 4);
    write(fd, &ac, 4);
    printf("%d", ac);
}
