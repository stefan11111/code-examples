extern float rsqrt(float);
extern float q_rsqrt(float);
extern float fabsf(float);
#include <stdlib.h>
#include <stdio.h>

#define TRESH (1.5f / 4096)

int main(int ac, char **av)
{
    float x;
    float slow;
    float fast;

    x = (float)atoi(av[1]);
    slow = rsqrt(x);
    fast = q_rsqrt(x);

    printf("(1/)vsqrtss: %f %f\n"
           "vrsqrtss: %f %f\n",
           slow, (1/slow)*(1/slow),
           fast, (1/fast)*(1/fast));

    printf("diff: %f\n"
           "tresh: %f\n",
           fabsf(slow - fast), TRESH);
}
