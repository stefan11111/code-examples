#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
    char foo[10] = "1234";
    char *ptr = stpncpy(foo, "", 10);
    printf("%ld\n", ptr - foo);
}
