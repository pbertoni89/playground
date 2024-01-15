#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "pb_simplest_f_types.h"

#include "rt_nonfinite.h"
#include "pb_simplest_f.h"
#include "pb_simplest_f_emxAPI.h"

#define PB_N 10

static emxArray_real_T *argInit_1xUnbounded_real_T(void);
static double argInit_real_T(void);
static void main_pb_simplest_f(void);

static emxArray_real_T *argInit_1xUnbounded_real_T(void)
{
  emxArray_real_T *result;
  static int iv0[PB_N];
  for (int i=0; i<PB_N; i++)
  {
    iv0[i] = i*i;
  }

  int idx1;
  result = emxCreateND_real_T(PB_N, iv0);
  for (idx1 = 0; idx1 < result->size[1U]; idx1++)
  {
    result->data[result->size[0] * idx1] = argInit_real_T();
  }

  return result;
}


static double argInit_real_T(void)
{
  return 0.0;
}


static void main_pb_simplest_f(void)
{
  emxArray_real_T *a;
  double o1;
  a = argInit_1xUnbounded_real_T();
  o1 = pb_simplest_f(a);
  emxDestroyArray_real_T(a);
}


int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;
  pb_simplest_f_initialize();
  main_pb_simplest_f();
  pb_simplest_f_terminate();
  return 0;
}

