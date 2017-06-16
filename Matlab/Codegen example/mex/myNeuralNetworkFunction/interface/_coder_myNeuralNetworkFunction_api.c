/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * _coder_myNeuralNetworkFunction_api.c
 *
 * Code generation for function '_coder_myNeuralNetworkFunction_api'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "myNeuralNetworkFunction.h"
#include "_coder_myNeuralNetworkFunction_api.h"
#include "myNeuralNetworkFunction_emxutil.h"
#include "myNeuralNetworkFunction_data.h"
#include "blas.h"

/* Variable Definitions */
static emlrtRTEInfo i_emlrtRTEI = { 1, 1, "_coder_myNeuralNetworkFunction_api",
  "" };

/* Function Declarations */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *x1, const
  char_T *identifier, emxArray_real_T *y);
static const mxArray *emlrt_marshallOut(const emxArray_real_T *u);

/* Function Definitions */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  c_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret)
{
  static const int32_T dims[2] = { -1, 43 };

  boolean_T bv0[2] = { true, false };

  int32_T iv3[2];
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, dims, &bv0[0],
    iv3);
  ret->size[0] = iv3[0];
  ret->size[1] = iv3[1];
  ret->allocatedSize = ret->size[0] * ret->size[1];
  ret->data = (real_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *x1, const
  char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  thisId.bParentIsCell = false;
  b_emlrt_marshallIn(sp, emlrtAlias(x1), &thisId, y);
  emlrtDestroyArray(&x1);
}

static const mxArray *emlrt_marshallOut(const emxArray_real_T *u)
{
  const mxArray *y;
  const mxArray *m1;
  static const int32_T iv2[1] = { 0 };

  y = NULL;
  m1 = emlrtCreateNumericArray(1, iv2, mxDOUBLE_CLASS, mxREAL);
  mxSetData((mxArray *)m1, (void *)u->data);
  emlrtSetDimensions((mxArray *)m1, u->size, 1);
  emlrtAssign(&y, m1);
  return y;
}

void myNeuralNetworkFunction_api(const mxArray * const prhs[1], const mxArray
  *plhs[1])
{
  emxArray_real_T *x1;
  emxArray_real_T *b_y1;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &x1, 2, &i_emlrtRTEI, true);
  emxInit_real_T1(&st, &b_y1, 1, &i_emlrtRTEI, true);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "x1", x1);

  /* Invoke the target function */
  myNeuralNetworkFunction(&st, x1, b_y1);

  /* Marshall function outputs */
  plhs[0] = emlrt_marshallOut(b_y1);
  b_y1->canFreeData = false;
  emxFree_real_T(&b_y1);
  x1->canFreeData = false;
  emxFree_real_T(&x1);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/* End of code generation (_coder_myNeuralNetworkFunction_api.c) */
