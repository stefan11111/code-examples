#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv) {
	if (argc != 2) return -1;
	
	int destFd = open(argv[1], O_CREAT);
	fchmod(destFd, 0644);

	close(destFd);

	return 0;
}
