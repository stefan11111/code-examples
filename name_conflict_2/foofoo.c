#include <stdio.h>

void foo(int x)
{
    printf("%d\n", x);
}

int main(int argc)
{
    void (*f)(...) = (void (*)(...))&foo;
    (*f)(argc, argc);
}
