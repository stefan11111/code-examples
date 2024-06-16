#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define SIZE 256

int main()
{
    struct in6_addr addr;
    char *value="2a02:2f0d:b206:af00:a2fb:109c:ea4:3302";
    inet_pton(AF_INET6, value, &addr);
    printf("%s\n", addr);
    return 0;
}
