#include <dlfcn.h>

#define NULL ((void*)0)

#if 1
int main(int argc, char **argv)
#else
void _start()
#endif
{
  void *gtk2 = dlopen("libgtk-x11-2.0.so", RTLD_NOW);

  void (*f)() = dlsym(gtk2, "gtk_draw_insertion_cursor");

  f(NULL, NULL, NULL, NULL);

  f(NULL, NULL, NULL, NULL);
}
