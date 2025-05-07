#include <unistd.h>
#include <fcntl.h>

int main()
{
int LinuxConsoleFd = open("/dev/tty5", O_RDWR, 0);
char testbuf[101];
int nread;
nread = read(LinuxConsoleFd, testbuf, 100);
testbuf[nread] = 0;
int ofd;
ofd = open("/dev/pts/9", O_RDWR);
write(ofd, testbuf, nread + 1);
}
