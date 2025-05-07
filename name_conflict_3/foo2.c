#define _FOO(x, y) (x == 0 ? y : FOO(x - 1, y * x))
#define FOO(x, y) (x == 0 ? y : _FOO(x - 1, y * x))

main()
{
    return FOO(3, 1);
}
