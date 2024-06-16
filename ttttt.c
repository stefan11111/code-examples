//#include <stdio.h>
//extern void* fopen();
// main() { void *f = (void*)fopen("ttt.c", "r"); int n; fscanf(f, "%d", &n); }


main()
{
    char arr[100];
    memset_s(arr, 'a', 20);
    puts(arr);
    return 0;
}
