float rsqrt(float x)
{
#if 1
  asm (".intel_syntax;"
       "vsqrtss %0,%0,%1"
       : "=x"(x)
       : "x"(x)
      );
  return 1/x;
#else
  extern float sqrtf(float);
  return 1/sqrtf(x);
#endif
}

float q_rsqrt(float x)
{
  asm (".intel_syntax;"
       "vrsqrtss %0,%0,%1"
       : "=x"(x)
       : "x"(x)
      );
  return x;
}
