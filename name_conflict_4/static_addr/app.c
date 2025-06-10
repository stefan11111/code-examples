#include <stdio.h>

extern int* foo_addr(void);

#define foo (*foo_addr())

int main()
{
  printf("%d\n", foo);
  foo = 10;
  printf("%d\n", foo);
}
