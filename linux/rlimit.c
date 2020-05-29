			#define _GNU_SOURCE
//			#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>

#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); \
						} while (0)


int
main(int argc, char *argv[])
{
	struct rlimit old, new;
	struct rlimit *newp;
	pid_t pid;

	if (!(argc == 2 || argc == 4)) {
		fprintf(stderr, "Usage: %s <pid> [<new-soft-limit> "
				"<new-hard-limit>]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	const int pidArg = atoi(argv[1]);
	
	pid = (pidArg > 0 ? pidArg : getpid());        /* PID of target process */


	newp = NULL;
	if (argc == 4) {
		new.rlim_cur = atoi(argv[2]);
		new.rlim_max = atoi(argv[3]);
		newp = &new;
	}

	/* Set CPU time limit of target process; retrieve and display
		previous limit */

	if (prlimit(pid, RLIMIT_STACK, newp, &old) == -1)
		errExit("prlimit-1");
	printf("Previous limits: soft=%lld; hard=%lld\n",
			(long long) old.rlim_cur, (long long) old.rlim_max);

	/* Retrieve and display new CPU time limit */

	if (prlimit(pid, RLIMIT_STACK, NULL, &old) == -1)
		errExit("prlimit-2");
	printf("New limits: soft=%lld; hard=%lld\n",
			(long long) old.rlim_cur, (long long) old.rlim_max);

	exit(EXIT_SUCCESS);
}
