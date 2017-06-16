//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: myNeuralNetworkFunction_emxAPI.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Feb-2017 12:29:29
//
#ifndef MYNEURALNETWORKFUNCTION_EMXAPI_H
#define MYNEURALNETWORKFUNCTION_EMXAPI_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "myNeuralNetworkFunction_types.h"

// Function Declarations
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);
extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for myNeuralNetworkFunction_emxAPI.h
//
// [EOF]
//
