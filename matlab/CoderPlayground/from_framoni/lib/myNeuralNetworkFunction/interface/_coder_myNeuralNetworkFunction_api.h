/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_myNeuralNetworkFunction_api.h
 *
 * MATLAB Coder version            : 3.1
 * C/C++ source code generated on  : 03-Feb-2017 12:29:29
 */

#ifndef _CODER_MYNEURALNETWORKFUNCTION_API_H
#define _CODER_MYNEURALNETWORKFUNCTION_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_myNeuralNetworkFunction_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void myNeuralNetworkFunction(emxArray_real_T *x1, emxArray_real_T *b_y1);
extern void myNeuralNetworkFunction_api(const mxArray *prhs[1], const mxArray
  *plhs[1]);
extern void myNeuralNetworkFunction_atexit(void);
extern void myNeuralNetworkFunction_initialize(void);
extern void myNeuralNetworkFunction_terminate(void);
extern void myNeuralNetworkFunction_xil_terminate(void);

#endif

/*
 * File trailer for _coder_myNeuralNetworkFunction_api.h
 *
 * [EOF]
 */
