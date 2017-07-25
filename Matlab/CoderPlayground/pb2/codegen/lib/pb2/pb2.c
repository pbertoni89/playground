/*
 * pb2.c
 *
 * Code generation for function 'pb2'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "pb2.h"
#include "pb2_rtwutil.h"
#include <stdio.h>
#include <stdlib.h>

/* Variable Definitions */
static rtBoundsCheckInfo emlrtBCI = { -1,/* iFirst */
  -1,                                  /* iLast */
  7,                                   /* lineNo */
  13,                                  /* colNo */
  "v",                                 /* aName */
  "pb2",                               /* fName */
  "/media/xnext/DATA/Software/xspectra-ui/code-playground/Matlab/CoderPlayground/pb2/pb2.m",/* pName */
  0                                    /* checkKind */
};

/* Function Definitions */

/*
 * function [ i ] = pb2(v)
 */
double pb2(const double v_data[], const int v_size[2])
{
  double i;
  int e;

  /* UNTITLED2 Summary of this function goes here */
  /*    Detailed explanation goes here */
  /* 'pb2:5' i = 0; */
  i = 0.0;
  
  printf("kernel function. size is %d, [%d]\n", v_size[0], v_size[1]);

  /* 'pb2:6' for e = 1:length(v) */
  for (e = 0; e < v_size[1]; e++) {
    /* 'pb2:7' i = i + v(e); */
    if (!((e + 1 >= 1) && (e + 1 <= v_size[1]))) {
      rtDynamicBoundsError(e + 1, 1, v_size[1], &emlrtBCI);
    }
	printf("%d\t%f + %f = ", e, i, v_data[e]);
    i += v_data[e];
	printf("%f\n", i);
  }

  return i;
}

/* End of code generation (pb2.c) */
