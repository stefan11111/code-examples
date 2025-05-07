__uint128_t large_int ()
{
    __uint128_t large = 0;
    large--;
    return large;
}

int main()
{
    volatile __uint128_t res = large_int();
    volatile __uint128_t *p = &res;
    *p = large_int();
    return res;
}
