/*
 * _coder_pbfib_api.h
 *
 * Code generation for function '_coder_pbfib_api'
 *
 */

#ifndef _CODER_PBFIB_API_H
#define _CODER_PBFIB_API_H

/* Include files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_pbfib_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern real_T pbfib(real_T n);
extern void pbfib_api(const mxArray * const prhs[1], const mxArray *plhs[1]);
extern void pbfib_atexit(void);
extern void pbfib_initialize(void);
extern void pbfib_terminate(void);
extern void pbfib_xil_terminate(void);

#endif

/* End of code generation (_coder_pbfib_api.h) */
