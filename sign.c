#include <stdint.h>
main()
{
    ((intptr_t)(0) > (intptr_t)(-1)) ? printf("bad\n") : printf("good\n");
}
