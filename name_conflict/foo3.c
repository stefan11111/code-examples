#include <stdio.h>
#include <stdlib.h>

void foo()
{
}

int main()
{
    void* ptr1;
    void (*ptr2)();

    ptr1 = &foo; /* bad */
    ptr2 = &foo; /* good */
    sizeof(void (*)()) == sizeof(void*);
}
