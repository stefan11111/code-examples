extern float qsqrtf(float x);
extern float sqrtf(float x);
extern double sqrt(double x);

int main (int ac, char **av)
{
//    float f = strtof(av[1], NULL);

    float mine;
    float libc;

    const float tmp = 1.f;
    float x = 0;
    int *i = (int*)&x;

//    int j = *(int*)&tmp;
    int j = *i + 0xfffff;

    volatile float res;

    for(*i = 0; *i < j; (*i)++) {
#if 1
        res = qsqrtf(x);
#else
        mine = qsqrtf(x);
        libc = sqrtf(x);
        extern int printf(const char *, ...);
        if (mine != libc) {
            printf("mine: %f %f\n"
                   "libc: %f %f\n", mine, mine * mine, libc, libc * libc);
        }
#endif
    }
}


