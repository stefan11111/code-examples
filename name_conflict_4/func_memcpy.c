#include <stdio.h>
#include <string.h>
#include <signal.h>

void foo (char *str)
{
  printf("%c\n", *str);
}

int main()
{
  signal(SIGSEGV, SIG_IGN);
  foo("foo\n");
  memcpy(foo, printf, 1);
  printf("foo\n");
//  foo("foo\n");
}
