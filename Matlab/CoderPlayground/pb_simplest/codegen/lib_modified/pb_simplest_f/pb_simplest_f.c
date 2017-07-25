#include "rt_nonfinite.h"
#include "pb_simplest_f.h"
#include <stdio.h>

static int cfprintf(double varargin_1);
static void fileManager(FILE * *f, boolean_T *a);
static int cfprintf(double varargin_1)
{
  int nbytesint;
  FILE * b_NULL;
  FILE * filestar;
  boolean_T autoflush;
  static const char cfmt[14] = { 'O', 'u', 't', 'p', 'u', 't', ' ', 'i', 's',
    ' ', '%', 'f', '\x0a', '\x00' };

  b_NULL = NULL;
  nbytesint = 0;
  fileManager(&filestar, &autoflush);
  if (!(filestar == b_NULL)) {
    nbytesint = fprintf(filestar, cfmt, varargin_1);
    fflush(filestar);
  }

  return nbytesint;
}

static void fileManager(FILE * *f, boolean_T *a)
{
  *f = stdout;
  *a = true;
}

double pb_simplest_f(const emxArray_real_T *a)
{
  double o1;
  o1 = a->size[1];
  cfprintf(a->size[1]);
			  		printf("Actually is %d\n", a->size[0]);
					printf("Or eventually is %d\n", a->size[2]);
					printf("Ma cazzo is %d\n", a->size[3]);
  return o1;
}

void pb_simplest_f_initialize(void)
{
  rt_InitInfAndNaN(8U);
}

void pb_simplest_f_terminate(void)
{
}
