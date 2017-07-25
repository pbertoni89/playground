/*
 * _coder_pb2_api.c
 *
 * Code generation for function '_coder_pb2_api'
 *
 */

/* Include files */
#include "tmwtypes.h"
#include "_coder_pb2_api.h"
#include "_coder_pb2_mex.h"

/* Variable Definitions */
emlrtCTX emlrtRootTLSGlobal = NULL;
emlrtContext emlrtContextGlobal = { true,/* bFirstTime */
  false,                               /* bInitialized */
  131450U,                             /* fVersionInfo */
  NULL,                                /* fErrorFunction */
  "pb2",                               /* fFunctionName */
  NULL,                                /* fRTCallStack */
  false,                               /* bDebugMode */
  { 2045744189U, 2170104910U, 2743257031U, 4284093946U },/* fSigWrd */
  NULL                                 /* fSigMem */
};

/* Function Declarations */
static void b_emlrt_marshallIn(const mxArray *u, const emlrtMsgIdentifier
  *parentId, real_T **y_data, int32_T y_size[2]);
static void c_emlrt_marshallIn(const mxArray *src, const emlrtMsgIdentifier
  *msgId, real_T **ret_data, int32_T ret_size[2]);
static void emlrt_marshallIn(const mxArray *v, const char_T *identifier, real_T **
  y_data, int32_T y_size[2]);
static const mxArray *emlrt_marshallOut(const real_T u);

/* Function Definitions */
static void b_emlrt_marshallIn(const mxArray *u, const emlrtMsgIdentifier
  *parentId, real_T **y_data, int32_T y_size[2])
{
  c_emlrt_marshallIn(emlrtAlias(u), parentId, y_data, y_size);
  emlrtDestroyArray(&u);
}

static void c_emlrt_marshallIn(const mxArray *src, const emlrtMsgIdentifier
  *msgId, real_T **ret_data, int32_T ret_size[2])
{
  static const int32_T dims[2] = { 1, 5 };

  const boolean_T bv0[2] = { false, true };

  int32_T iv0[2];
  emlrtCheckVsBuiltInR2012b(emlrtRootTLSGlobal, msgId, src, "double", false, 2U,
    dims, &bv0[0], iv0);
  ret_size[0] = iv0[0];
  ret_size[1] = iv0[1];
  *ret_data = (real_T *)mxGetData(src);
  emlrtDestroyArray(&src);
}

static void emlrt_marshallIn(const mxArray *v, const char_T *identifier, real_T **
  y_data, int32_T y_size[2])
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = (const char *)identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(emlrtAlias(v), &thisId, y_data, y_size);
  emlrtDestroyArray(&v);
}

static const mxArray *emlrt_marshallOut(const real_T u)
{
  const mxArray *y;
  const mxArray *m0;
  y = NULL;
  m0 = emlrtCreateDoubleScalar(u);
  emlrtAssign(&y, m0);
  return y;
}

void pb2_api(const mxArray * const prhs[1], const mxArray *plhs[1])
{
  real_T (*v_data)[5];
  int32_T v_size[2];
  real_T i;

  /* Marshall function inputs */
  emlrt_marshallIn(emlrtAlias(prhs[0]), "v", (real_T **)&v_data, v_size);

  /* Invoke the target function */
  i = pb2(*v_data, v_size);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(i);
}

void pb2_atexit(void)
{
  mexFunctionCreateRootTLS();
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  pb2_xil_terminate();
}

void pb2_initialize(void)
{
  mexFunctionCreateRootTLS();
  emlrtClearAllocCountR2012b(emlrtRootTLSGlobal, false, 0U, 0);
  emlrtEnterRtStackR2012b(emlrtRootTLSGlobal);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

void pb2_terminate(void)
{
  emlrtLeaveRtStackR2012b(emlrtRootTLSGlobal);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/* End of code generation (_coder_pb2_api.c) */
