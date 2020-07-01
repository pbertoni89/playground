#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


void * thread_function(void *ignoredInThisExample)
{
    char * a = malloc(10);
    strcpy(a, "hello world");
    pthread_exit((void*)a);
}


void * thread_function_1(void * unu)
{
	printf("thread int setting 7\n");
	int * rv = calloc(1, sizeof(int));
	* rv = 7;
	pthread_exit((void*) rv);
}


void thread_sleeper(void * unu)
{
	printf("thread sleeper start\n");
	usleep(10 * 1000000);
	printf("thread sleeper end\n");
}


int main()
{
	pthread_t thread_id;
	char * b;
	pthread_create (&thread_id, NULL, &thread_function, NULL);
	pthread_join (thread_id, (void**) &b); //here we are reciving one pointer value so to use that we need double pointer
	printf("b is %s\n", b);
	free(b); // lets free the memory

	pthread_t thread_id_1;
	int * rv;
	pthread_create (&thread_id_1, NULL, &thread_function_1, NULL);
	pthread_join (thread_id_1, (void**) &rv); //here we are reciving one pointer value so to use that we need double pointer
	printf("rv is %d\n", *rv);
	free(rv); // lets free the memory

	//  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

	pthread_t thread_id_2;
	pthread_create (&thread_id_2, NULL, &thread_sleeper, NULL);

	int st = 110;  // strerror(3) == "No such process".  strerror(110) = "Connection timed out"
	while (st == 110)
	{
		struct timespec ts;
		if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
		{
			fprintf(stderr, "cannot get time\n");
			exit(1);
		}
		ts.tv_sec += 3;

		printf("waiting s for some seconds\n");
		st = pthread_timedjoin_np(thread_id_2, NULL, &ts);
		if (st != 0)
		{
			fprintf(stderr, "error %d aka %s\n", st, strerror(st));  // errno is not used by that call
			usleep(1000000);
		}
	}

	printf("exiting main\n");
	return 0;
}
