//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: myNeuralNetworkFunction_emxutil.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Feb-2017 12:29:29
//
#ifndef MYNEURALNETWORKFUNCTION_EMXUTIL_H
#define MYNEURALNETWORKFUNCTION_EMXUTIL_H

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "myNeuralNetworkFunction_types.h"

// Function Declarations
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxInit_real_T1(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for myNeuralNetworkFunction_emxutil.h
//
// [EOF]
//
