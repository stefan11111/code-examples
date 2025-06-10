#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float qsqrtf(const float x)
{
    const int ix = *(int*)&x;
    float lf;
    float hf;

    if (x > 1) {
        lf = 1.f;
        hf = x;
    } else {
        lf = x;
        hf = 1.f;
    }

    float mid;
    int imid;

    int lo = *(int*)&lf;
    int hi = *(int*)&hf;

    while (lo < hi) {
        imid = (unsigned)(lo + hi) >> 1;
        mid = *(float*)&imid;
#if 0
        double y = (double)mid * mid;

        float y = mid * mid;
        int iy = *(int*)&y;
#endif
extern int float_cmp(float, float);
        if (float_cmp(mid, x)) {
            lo = imid + 1;
        }
        else {
            hi = imid - 1;
        }
    }

//    printf("here\n");

    return *(float*)&lo;
}
