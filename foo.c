#define DENOM 8 * 11 * 23

main()
{
  for(int i = 1; ;i++) {
    long long big = (8 * 11 * 11 * 23 * 23)*i*i + (11 * 11 * 23 * 23 - 8 * 11 * 11)*i;

    if(big / DENOM == (double)big/DENOM) {
      printf("%d\n", i);
      return;
    }
  }
}
