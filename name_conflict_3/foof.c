#define foo bar

#include <errno.h>

bar(){}

main()
{
    foo();
    int *p, *q;
    p = __errno_location ();
    printf("%p\n", p);
    q = &errno;
    printf("%p\n", q);
}
