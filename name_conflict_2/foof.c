#include <stdio.h>

int main(int argc, char **argv)
{
printf("%d\n", argc);
while(*argv) {
printf("%s ", *argv);
argv++;
}
printf("\n");
}
