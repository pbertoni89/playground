/**
 * https://www.cs.kent.edu/~ruttan/sysprog/lectures/multi-thread/pthread_cond_init.html
 */

// https://stackoverflow.com/a/22015496/1315480  # TODO consider moving to cmake
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdarg.h>    // variadic
#include <unistd.h>
#include <time.h>      // gettimeofday
#include <sys/time.h>  // gettimeofday
#include <errno.h>     // ETIMEDOUT
// #include <math.h>

pthread_mutex_t log_mutex;


void micrologger(const char actor, const char * const fmt, ...)
{
	char buff[128];

	pthread_mutex_lock(&log_mutex);
	const time_t now = time(0);
	strftime(buff, 100, "%H:%M:%S", localtime(&now));
	printf("%s\t%c\t", buff, actor);

	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	pthread_mutex_unlock(&log_mutex);
}


typedef struct t_pt
{
	struct  // values
	{
		int x, y;
	} v;

	struct  // times
	{
		int tts_init_s, tts_cycle_a, timeout_s, niter_a;
	} t;
} t_pt;

pthread_mutex_t cv_mutex;
pthread_cond_t cv_cond;

const int LOGMTX = 0;


void * fun_cv_sensor(void * p)
{
	struct timeval now;
	struct timespec timeout;
	int retcode = 0;
	const t_pt * const ppt = p;

	micrologger('S', "sleeping %i [s], timeout %i [s] from lock\n", ppt->t.tts_init_s, ppt->t.timeout_s);
	usleep(1000 * 1000 * ppt->t.tts_init_s);

	if (LOGMTX) micrologger('S', "locking\n");
	pthread_mutex_lock(&cv_mutex);

	gettimeofday(&now, NULL);
	timeout.tv_sec = now.tv_sec + ppt->t.timeout_s;
	timeout.tv_nsec = now.tv_usec * 1000;

	while (ppt->v.x <= ppt->v.y && retcode != ETIMEDOUT)
	{
		micrologger('S', "waiting condition, x<=y %i<=%i, retcode %i\n", ppt->v.x, ppt->v.y, retcode);
		retcode = pthread_cond_timedwait(&cv_cond, &cv_mutex, &timeout);
	}

	micrologger('S', "waited, x %i, y %i, retcode %i, ETIMEDOUT %i\n", ppt->v.x, ppt->v.y, retcode, ETIMEDOUT);

	if (LOGMTX) micrologger('S', "unlocking\n");
	pthread_mutex_unlock(&cv_mutex);
	return NULL;
}


void * fun_cv_actuator(void * p)
{
	t_pt * const ppt = p;

	for (int i=0; i<ppt->t.niter_a; i++)
	{
		micrologger('A', "sleeping %i [s], cycling %i/%i [#]\n", ppt->t.tts_cycle_a, i, ppt->t.niter_a);
		usleep(1000 * 1000 * ppt->t.tts_cycle_a);

		if (LOGMTX) micrologger('A', "locking\n");
		pthread_mutex_lock(&cv_mutex);

		// do something that may fulfill the condition
		micrologger('A', "bumping x -> %i\n", ppt->v.x + 1);
		ppt->v.x ++;

		if (LOGMTX) micrologger('A', "unlocking\n");
		pthread_mutex_unlock(&cv_mutex);
	}

	micrologger('A', "signal condition\n");  // to wake up `sensor` thread
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
		pthread_t tcv_s, tcv_a;

		t_pt pt;
		pt.v.x = 0;
		pt.v.y = 6;
		pt.t.tts_init_s = 2;
		pt.t.tts_cycle_a = 1;
		pt.t.timeout_s = 20;
		pt.t.niter_a = 10;

		pthread_create(&tcv_s, NULL, &fun_cv_sensor, &pt);
		pthread_create(&tcv_a, NULL, &fun_cv_actuator, &pt);
		pthread_join(tcv_s, NULL);
		pthread_join(tcv_a, NULL);
	}

	printf("exiting main\n");
	return 0;
}
