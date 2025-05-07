#include <stdio.h>
#include <string.h>

struct large {
    long long a;
    long long b;
};

void* foo()
{
    return 0;
}

struct large bar()
{
    return (struct large){0};
}

struct large (*f)();

int main()
{
    struct large n = ((struct large (*)())foo)();
    printf("%lld %lld\n", n.a, n.b);
    n = bar();
    printf("%lld %lld\n", n.a, n.b);
    return 0;
}
