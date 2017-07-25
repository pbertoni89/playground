/*
 * _coder_pb2_api.h
 *
 * Code generation for function '_coder_pb2_api'
 *
 */

#ifndef _CODER_PB2_API_H
#define _CODER_PB2_API_H

/* Include files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_pb2_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern real_T pb2(real_T v_data[], int32_T v_size[2]);
extern void pb2_api(const mxArray * const prhs[1], const mxArray *plhs[1]);
extern void pb2_atexit(void);
extern void pb2_initialize(void);
extern void pb2_terminate(void);
extern void pb2_xil_terminate(void);

#endif

/* End of code generation (_coder_pb2_api.h) */
