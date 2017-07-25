/*
 * _coder_sobel_api.h
 *
 * Code generation for function '_coder_sobel_api'
 *
 */

#ifndef _CODER_SOBEL_API_H
#define _CODER_SOBEL_API_H

/* Include files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_sobel_api.h"

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

#ifndef struct_emxArray_uint8_T
#define struct_emxArray_uint8_T

struct emxArray_uint8_T
{
  uint8_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_uint8_T*/

#ifndef typedef_emxArray_uint8_T
#define typedef_emxArray_uint8_T

typedef struct emxArray_uint8_T emxArray_uint8_T;

#endif                                 /*typedef_emxArray_uint8_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void sobel(emxArray_real_T *originalImage, real_T threshold,
                  emxArray_uint8_T *edgeImage);
extern void sobel_api(const mxArray * const prhs[2], const mxArray *plhs[1]);
extern void sobel_atexit(void);
extern void sobel_initialize(void);
extern void sobel_terminate(void);
extern void sobel_xil_terminate(void);

#endif

/* End of code generation (_coder_sobel_api.h) */
