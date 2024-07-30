/*#define __is_constexpr(x) \
	(sizeof(int) == sizeof(*(8 ? ((void *)((long)(x) * 0l)) : (int *)8)))
*/
#define __is_constexpr(x) \
(sizeof(int) == sizeof(*((int*)8)))

#define FNM_MAX INET6_ADDRSLEN

#define STR_(a) #a
#define STR(a) STR_(a)

#include <netinet/in.h>

main() { printf("%d\n", sizeof("\x1b[6n")); }
