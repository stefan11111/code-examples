int main()
{
    float x;
    int *p = (int*)&x;
    int j = 0xffff;
    float y = *(float*)&j;
#define i (*p)
extern int float_cmp(float, float);

    for (i = 0; i < 0xffffff; i++) {
        float_cmp(i, j);
    }
}
