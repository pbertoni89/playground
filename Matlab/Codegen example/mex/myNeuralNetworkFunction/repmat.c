/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * repmat.c
 *
 * Code generation for function 'repmat'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "myNeuralNetworkFunction.h"
#include "repmat.h"
#include "eml_int_forloop_overflow_check.h"
#include "myNeuralNetworkFunction_emxutil.h"
#include "myNeuralNetworkFunction_data.h"
#include "blas.h"

/* Variable Definitions */
static emlrtRSInfo e_emlrtRSI = { 112, "repmat",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\elmat\\repmat.m"
};

static emlrtRTEInfo h_emlrtRTEI = { 1, 14, "repmat",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\elmat\\repmat.m"
};

static emlrtRTEInfo j_emlrtRTEI = { 53, 23, "assertValidSizeArg",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\assertValidSizeArg.m"
};

/* Function Definitions */
void repmat(const emlrtStack *sp, real_T varargin_2, emxArray_real_T *b)
{
  boolean_T b0;
  int32_T jtilecol;
  int32_T ibtile;
  int32_T k;
  static const real_T a[2] = { 2.880745322789922, -0.11360821349890438 };

  emlrtStack st;
  emlrtStack b_st;
  st.prev = sp;
  st.tls = sp->tls;
  st.site = &d_emlrtRSI;
  b_st.prev = &st;
  b_st.tls = st.tls;
  if ((!(varargin_2 != varargin_2)) && (!(2.147483647E+9 < varargin_2))) {
    b0 = true;
  } else {
    b0 = false;
  }

  if (b0) {
  } else {
    emlrtErrorWithMessageIdR2012b(&st, &j_emlrtRTEI,
      "Coder:MATLAB:NonIntegerInput", 4, 12, MIN_int32_T, 12, MAX_int32_T);
  }

  jtilecol = b->size[0] * b->size[1];
  b->size[0] = 2;
  b->size[1] = (int32_T)varargin_2;
  emxEnsureCapacity(sp, (emxArray__common *)b, jtilecol, (int32_T)sizeof(real_T),
                    &h_emlrtRTEI);
  if (!((int32_T)varargin_2 == 0)) {
    st.site = &e_emlrtRSI;
    if ((!(1 > (int32_T)varargin_2)) && ((int32_T)varargin_2 > 2147483646)) {
      b_st.site = &f_emlrtRSI;
      check_forloop_overflow_error(&b_st);
    }

    for (jtilecol = 1; jtilecol <= (int32_T)varargin_2; jtilecol++) {
      ibtile = (jtilecol - 1) << 1;
      for (k = 0; k < 2; k++) {
        b->data[ibtile + k] = a[k];
      }
    }
  }
}

/* End of code generation (repmat.c) */
