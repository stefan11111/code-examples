extern void _string_sort();
extern int red_zone_max();
extern int printf();

#define string_sort(...) _string_sort(0, 0, 0, 0, 0, 0, sizeof((char*[]){__VA_ARGS__})/sizeof(char*), __VA_ARGS__)
#define red_zone(a) red_zone_max(a, sizeof(a)/sizeof(*a))

int main()
{
    int a[] = {0, 1, 2};

    string_sort("foo", "bar", "baz");
    printf("%d\n", red_zone(a));
    return 0;
}
