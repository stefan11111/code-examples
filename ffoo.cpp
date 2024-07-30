#include <stdio.h>
#include <stdint.h>

template<typename T>
void _swap(T &a, T &b) {
    T tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    char foo[10] = "foo\n";
    char bar[10] = "bar\n";

    _swap(foo,bar);

    printf("%s", foo);
}
