#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    for(int i = 1; i < argc; i++) {
        if(!strncmp(argv[i], "--target=", sizeof("--target=") - 1)) {
            char *p = argv[i] + sizeof("--target=") - 1;
            (void)!fopen(p, "w");
            return 0;
        }
    }
    return 0;
}
