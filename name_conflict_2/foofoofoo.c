#include <string.h>
#include <stdio.h>

int main()
{
    char foo[sizeof("   foo   :   barw") - 1] = "   foo   :   barw";
    char *tok = strtok(foo, "w");
    printf("\x1b[35mlogging back on\r\n");
}
