main(int av)
{
    volatile int volatile * volatile p = (volatile int volatile * volatile)&av;
    return *p;
}
