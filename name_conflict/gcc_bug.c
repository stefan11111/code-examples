#include <stdio.h>

extern void init_var(int *x);

int main()
{
    char arr[10] = "abcd";
    int x;
    init_var(&x);
    printf("%c\n", arr[x]);
}
