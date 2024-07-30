#include <arpa/inet.h>

union sockaddr_in46 {
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
    unsigned short sin_family;
};

struct foo { int x; int arr[]; };

int main()
{
    union sockaddr_in46 sin46;
    sin46.sin.sin_family = AF_INET;
    struct foo bar;
    int x[10];
    bar.arr = x;
    bar.arr[5] = 1;
    printf("%d", bar.arr[5]);
}
