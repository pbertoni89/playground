/*
 * pbfib.cpp
 *
 * Code generation for function 'pbfib'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "pbfib.h"

/* Function Definitions */
double pbfib(double n)
{
  double f;

  /* PBFIB Summary of this function goes here */
  /*    Detailed explanation goes here */
  if (n == 1.0) {
    f = 1.0;
  } else if (n == 2.0) {
    f = 2.0;
  } else {
    f = pbfib(n - 1.0) + pbfib(n - 2.0);
  }

  return f;
}

/* End of code generation (pbfib.cpp) */
