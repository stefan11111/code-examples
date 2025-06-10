#include <glib.h>
#include <stdio.h>

struct _foo {
};

typedef struct _foo foo;

int main()
{
  void *ptr1 = g_slice_new(foo);
  void *ptr2 = g_slice_new0(foo);

  printf("%lu %p %p\n", sizeof(foo), ptr1, ptr2);

  free(ptr1);
  free(ptr2);

  printf("%lu %p %p\n", sizeof(foo), ptr1, ptr2);
}

