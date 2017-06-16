/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * myNeuralNetworkFunction_initialize.c
 *
 * Code generation for function 'myNeuralNetworkFunction_initialize'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "myNeuralNetworkFunction.h"
#include "myNeuralNetworkFunction_initialize.h"
#include "_coder_myNeuralNetworkFunction_mex.h"
#include "myNeuralNetworkFunction_data.h"
#include "blas.h"

/* Function Declarations */
static void myNeuralNetworkFunction_once(void);

/* Function Definitions */
static void myNeuralNetworkFunction_once(void)
{
  /* Allocate instance data */
  covrtAllocateInstanceData(&emlrtCoverageInstance);

  /* Initialize Coverage Information */
  covrtScriptInit(&emlrtCoverageInstance,
                  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m", 0,
                  4, 4, 0, 0, 0, 0, 0, 0, 0, 0);

  /* Initialize Function Information */
  covrtFcnInit(&emlrtCoverageInstance, 0, 0, "myNeuralNetworkFunction", 0, -1,
               4179);
  covrtFcnInit(&emlrtCoverageInstance, 0, 1, "mapminmax_apply", 4268, -1, 4422);
  covrtFcnInit(&emlrtCoverageInstance, 0, 2, "logsig_apply", 4461, -1, 4519);
  covrtFcnInit(&emlrtCoverageInstance, 0, 3, "tansig_apply", 4559, -1, 4623);

  /* Initialize Basic Block Information */
  covrtBasicBlockInit(&emlrtCoverageInstance, 0, 0, 427, -1, 4174);
  covrtBasicBlockInit(&emlrtCoverageInstance, 0, 1, 4309, -1, 4417);
  covrtBasicBlockInit(&emlrtCoverageInstance, 0, 2, 4492, -1, 4514);
  covrtBasicBlockInit(&emlrtCoverageInstance, 0, 3, 4590, -1, 4618);

  /* Initialize If Information */
  /* Initialize MCDC Information */
  /* Initialize For Information */
  /* Initialize While Information */
  /* Initialize Switch Information */
  /* Start callback for coverage engine */
  covrtScriptStart(&emlrtCoverageInstance, 0U);
}

void myNeuralNetworkFunction_initialize(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  mexFunctionCreateRootTLS();
  emlrtBreakCheckR2012bFlagVar = emlrtGetBreakCheckFlagAddressR2012b();
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  if (emlrtFirstTimeR2012b(emlrtRootTLSGlobal)) {
    myNeuralNetworkFunction_once();
  }
}

/* End of code generation (myNeuralNetworkFunction_initialize.c) */
