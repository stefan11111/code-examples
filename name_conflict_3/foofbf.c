/* x is unsigned long long and constant*/
#define T_(x) (x * (x + 1) / 2)
#define P_(x) (x * (3 * x - 1) / 2)
#define H_(x) (x * (2 * x - 1))

/* these are defined later */
#define T T_(T_n)
#define P P_(P_n)
#define H H_(H_n)

/* x, y, z are constant */
#define MIN_(x, y) (x > y ? y : x)
#define MIN(x, y, z) (MIN_(MIN_(x, y), z))

int main()
{
    unsigned long long T_n = 286;
    unsigned long long P_n = 166;
    unsigned long long H_n = 164;

    while (T != P || T != H) { /* this works because the sequences are increasing */
        const unsigned long long min = MIN(T, P, H);
        if (min == T) {
            T_n++;
            continue;
        }
        if (min == P) {
            P_n++;
            continue;
        }
        if (min == H) {
            H_n++;
            continue;
        }
    }
    extern int printf(const char * restrict format, ...);
    printf("T_%llu = P_%llu = H_%llu = %llu\n", T_n, P_n, H_n, T);
    return 0;
}
