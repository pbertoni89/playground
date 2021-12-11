// #define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	fprintf(stdout, "starting\n");

	for (int i=0; i<100; i++)
	{
		if (i % 10 == 0)
		{
			// fprintf(stdout, "flusha");
			fflush(stdout);
		}
		if (i % 25 == 0)
			fprintf(stderr, "e%03d ", i);

		fprintf(stdout, "o%03d ", i);
		usleep(500000);
	}

	fprintf(stdout, "\nclosing\n");
}
