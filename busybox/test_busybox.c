#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int ret;
	DIR *dir = opendir("/bin/testsuite");

	if (dir) {
		closedir(dir);
	}
	else if (ENOENT == errno) {
		fprintf(stderr, "There is no busybox test suite to run, build project with \"long_test\"\n");
		exit(EXIT_FAILURE);
	}
	else {
		fprintf(stderr, "There is problem with opening existing /bin/testsuite directory: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if ((ret = vfork()) < 0) {
		fprintf(stderr, "vfork function failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	else if (!ret) {
		execv("/bin/posixsrv", argv);
		fprintf(stderr, "exec function failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("Posixsrv ran in background\n");

	/* without this delay sometimes posixsrv is not yet running before the next function call */
	usleep(100000);

	if ((ret = system("cd /bin/testsuite/ && export PATH=/bin:/sbin:/usr/bin:/usr/sbin && ./runtest")) < 0) {
		fprintf(stderr, "system function failed: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("\nRuntest done\n\n");

	return 0;
}
