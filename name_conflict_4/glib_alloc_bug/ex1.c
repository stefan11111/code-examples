#include <glib.h>
#include <stdio.h>

typedef struct {
#ifdef FEATURE_FOO
int foo;
#endif
#ifdef FEATURE_BAR
float bar;
#endif
} foo_t;

void init_foo(foo_t *ptr)
{
#ifdef FEATURE_FOO
ptr->foo = -1;
#endif
#ifdef FEATURE_BAR
ptr->bar = -1.0f;
#endif
}

void do_work_with_foo (foo_t *ptr)
{
#ifdef FEATURE_FOO
  printf("%d\n", ptr->foo);
#endif
#ifdef FEATURE_BAR
  printf("%f\n", ptr->bar);
#endif
}

int main()
{
    foo_t *pfoo = g_slice_new(foo_t);
    init_foo(pfoo);
    do_work_with_foo(pfoo);
    g_slice_free(foo_t, pfoo);
}
