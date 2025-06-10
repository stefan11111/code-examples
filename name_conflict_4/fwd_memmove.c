#ifdef __GNUC__
#define MAY_ALIAS __attribute((_may_alias_))
#else
#define MAY_ALIAS
#endif

#include <stddef.h>

typedef void* MAY_ALIAS WORD;

static void fwd_memmove(void *_dst, const void *_src, size_t count)
{
  char *dst = _dst;
  const char *src = _src;

  if (count < 8) { /* too few bytes to copy */
    goto unrolled_copy;
  }

  /* align dst to word size */
  switch ((int)dst & 0x7) {
  case 7:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 6:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 5:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 4:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 3:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 2:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 1:
    *dst++ = *src++;
    count--;
  }

  while(count >= 8) { /* copy word by word */
    *(WORD*)dst++ = *(const WORD*)src++;
    count -= 8;
  }

unrolled_copy:
  /* unrolled copy of the remaining bytes */
  switch (count & 0x7) {
  case 7:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 6:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 5:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 4:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 3:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 2:
    *dst++ = *src++;
    count--;
    /* FALLTHROUGH */
  case 1:
    *dst++ = *src++;
    count--;
  }
}

#include <stdio.h>

int main()
{
  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int pos = 6;

  fwd_memmove(arr + pos, arr + pos + 1, sizeof(arr) - (pos + 1) * sizeof(*arr));

  for(int i = 0; i < sizeof(arr)/sizeof(*arr) - 1; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}
