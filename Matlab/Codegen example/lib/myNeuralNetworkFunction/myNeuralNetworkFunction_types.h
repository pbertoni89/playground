//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: myNeuralNetworkFunction_types.h
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Feb-2017 12:29:29
//
#ifndef MYNEURALNETWORKFUNCTION_TYPES_H
#define MYNEURALNETWORKFUNCTION_TYPES_H

// Include Files
#include "rtwtypes.h"

// Type Definitions
#ifndef struct_emxArray__common
#define struct_emxArray__common

struct emxArray__common
{
  void *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray__common

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 //struct_emxArray_real_T
#endif

//
// File trailer for myNeuralNetworkFunction_types.h
//
// [EOF]
//
