/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * myNeuralNetworkFunction_terminate.c
 *
 * Code generation for function 'myNeuralNetworkFunction_terminate'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "myNeuralNetworkFunction.h"
#include "myNeuralNetworkFunction_terminate.h"
#include "_coder_myNeuralNetworkFunction_mex.h"
#include "myNeuralNetworkFunction_data.h"
#include "blas.h"

/* Function Definitions */
void myNeuralNetworkFunction_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);

  /* Free instance data */
  covrtFreeInstanceData(&emlrtCoverageInstance);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

void myNeuralNetworkFunction_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (myNeuralNetworkFunction_terminate.c) */
