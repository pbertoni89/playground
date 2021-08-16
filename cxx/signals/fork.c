/*
	https://unix.stackexchange.com/questions/176235/fork-and-how-signals-are-delivered-to-processes
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void parent_trap(int sig)
{
	fprintf(stderr, "They got back together!\n");
}


void child_trap(int sig)
{
	fprintf(stderr, "Caught signal in CHILD.\n");
}


int main(int argc, char **argv)
{
	const int PID = fork();
    if (!PID)
    {
		fprintf(stderr, "forked: I'm the child\n");
        signal(SIGINT, &child_trap);
        sleep(1000);
        exit(0);
    }

    fprintf(stderr, "forked: I'm the parent %d\n", PID);
    signal(SIGINT, &parent_trap);
    sleep(1000);
    return 0;
}
