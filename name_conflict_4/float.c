int main()
{
    volatile double x;

    volatile int ret = (x < 0.7) + (x < 1.4) + (x < 2.1);
}
