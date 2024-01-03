/**
 * https://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/pthread_cond_init.html
 */

// https://stackoverflow.com/a/22015496/1315480  # TODO consider moving to cmake
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>  // variadic
#include <unistd.h>
#include <time.h>  // gettimeofday
#include <sys/time.h>  // gettimeofday
#include <errno.h>  // ETIMEDOUT
#include <math.h>

pthread_mutex_t log_mutex;


void micrologger(const char * const fmt, ...)
{
	char buff[128];

	pthread_mutex_lock(&log_mutex);
	const time_t now = time(0);
	strftime(buff, 100, "%H:%M:%S", localtime(&now));
	printf("%s: ", buff);

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	pthread_mutex_unlock(&log_mutex);
}


typedef struct t_pt
{
	int x, y;
} t_pt;

pthread_mutex_t cv_mutex;
pthread_cond_t cv_cond;


void * fun_cv_1(void * p)
{
	struct timeval now;
	struct timespec timeout;
	int retcode = 0;

	micrologger("fun_cv_1 sleeping\n");
	usleep(1000 * 1000 * 2);

	micrologger("fun_cv_1 locking\n");
	pthread_mutex_lock(&cv_mutex);

	gettimeofday(&now, NULL);
	timeout.tv_sec = now.tv_sec + 5;
	timeout.tv_nsec = now.tv_usec * 1000;

	const t_pt * const ppt = p;
	while (ppt->x <= ppt->y && retcode != ETIMEDOUT)
	{
		micrologger("fun_cv_1 waiting, x<=y %d<=%d, retcode %d\n", ppt->x, ppt->y, retcode);
		retcode = pthread_cond_timedwait(&cv_cond, &cv_mutex, &timeout);
	}

	micrologger("fun_cv_1 waited, x %d, y %d, retcode %d, ETIMEDOUT %d\n", ppt->x, ppt->y, retcode, ETIMEDOUT);

	micrologger("fun_cv_1 unlocking\n");
	pthread_mutex_unlock(&cv_mutex);
	return NULL;
}


void * fun_cv_2(void * p)
{
	micrologger("fun_cv_2 sleeping\n");
	usleep(1000 * 1000 * 3);

	micrologger("fun_cv_2 locking\n");
	pthread_mutex_lock(&cv_mutex);

	t_pt * const ppt = p;
	micrologger("fun_cv_2 do something that may fulfill the condition: bumping x\n");
	ppt->x ++;

	micrologger("fun_cv_2 unlocking\n");
	pthread_mutex_unlock(&cv_mutex);

	micrologger("fun_cv_2 wake up fun_cv_1\n");
	pthread_cond_signal(&cv_cond);
	return NULL;
}


void * thread_function(void * ignoredInThisExample)
{
	const size_t SZN = 12;
	char * a = malloc(SZN);
	strncpy(a, "hello world", SZN);
	pthread_exit(a);
}


void * thread_function_1(void * unu)
{
	printf("thread int setting 7\n");
	int * rv = calloc(1, sizeof(int));
	* rv = 7;
	pthread_exit(rv);
}


void * thread_sleeper(void * unu)
{
	printf("thread sleeper start\n");
	usleep(10 * 1000000);
	printf("thread sleeper end\n");
	return NULL;
}


int main()
{
	const int DEMO_1 = 0, DEMO_2 = 0, DEMO_CV = 1;

	if (DEMO_1)
	{
		pthread_t thread_id;
		char * b;
		pthread_create(&thread_id, NULL, &thread_function, NULL);
		pthread_join(thread_id, (void**) &b); //here we are reciving one pointer value so to use that we need double pointer
		printf("b is %s\n", b);
		free(b); // lets free the memory

		pthread_t thread_id_1;
		int * rv;
		pthread_create(&thread_id_1, NULL, &thread_function_1, NULL);
		pthread_join(thread_id_1, (void**) &rv); //here we are reciving one pointer value so to use that we need double pointer
		printf("rv is %d\n", *rv);
		free(rv); // lets free the memory
	}

	if (DEMO_2)
	{
		pthread_t thread_id_2;
		pthread_create(&thread_id_2, NULL, &thread_sleeper, NULL);

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
	}

	if (DEMO_CV)
	{
		pthread_mutex_init(&log_mutex, NULL);
		pthread_mutex_init(&cv_mutex, NULL);
		pthread_cond_init(&cv_cond, NULL);
		pthread_t tcv1, tcv2;

		t_pt pt;
		pt.x = 10;
		pt.y = 10;
		pthread_create(&tcv1, NULL, &fun_cv_1, &pt);
		pthread_create(&tcv2, NULL, &fun_cv_2, &pt);
		pthread_join(tcv1, NULL);
		pthread_join(tcv2, NULL);
	}

	printf("exiting main\n");
	return 0;
}
