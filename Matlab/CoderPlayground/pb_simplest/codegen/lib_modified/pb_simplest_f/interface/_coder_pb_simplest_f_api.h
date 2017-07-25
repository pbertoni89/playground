#ifndef _CODER_PB_SIMPLEST_F_API_H
#define _CODER_PB_SIMPLEST_F_API_H
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_pb_simplest_f_api.h"
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

#endif

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif

extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;
extern real_T pb_simplest_f(emxArray_real_T *a);
extern void pb_simplest_f_api(const mxArray * const prhs[1], const mxArray *
  plhs[1]);
extern void pb_simplest_f_atexit(void);
extern void pb_simplest_f_initialize(void);
extern void pb_simplest_f_terminate(void);
extern void pb_simplest_f_xil_terminate(void);

#endif
