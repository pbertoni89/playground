/*
 * conv2.c
 *
 * Code generation for function 'conv2'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "sobel.h"
#include "conv2.h"
#include "sobel_emxutil.h"

/* Function Definitions */
void b_conv2(const emxArray_real_T *a, emxArray_real_T *c)
{
  int aidx;
  int firstRowA;
  boolean_T b1;
  int a_idx_1;
  int a_idx_0;
  int ma;
  int na;
  int firstColB;
  int lastColB;
  int firstRowB;
  int lastRowB;
  int lastColA;
  int k;
  int b_firstColB;
  int iC;
  int iA;
  int iB;
  int i;
  int b_i;
  int a_length;
  int r;
  static const signed char iv1[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };

  aidx = c->size[0] * c->size[1];
  c->size[0] = a->size[0];
  c->size[1] = a->size[1];
  emxEnsureCapacity((emxArray__common *)c, aidx, sizeof(double));
  firstRowA = a->size[0] * a->size[1];
  for (aidx = 0; aidx < firstRowA; aidx++) {
    c->data[aidx] = 0.0;
  }

  b1 = ((a->size[0] == 0) || (a->size[1] == 0));
  if (!b1) {
    a_idx_1 = a->size[1];
    a_idx_0 = a->size[0];
    ma = a->size[0];
    na = a->size[1] - 1;
    if (a->size[1] < 1) {
      firstColB = 2;
    } else {
      firstColB = 0;
    }

    aidx = a->size[1];
    if (3 <= aidx) {
      lastColB = 2;
    } else {
      lastColB = a->size[1];
    }

    if (a->size[0] < 1) {
      firstRowB = 2;
    } else {
      firstRowB = 0;
    }

    firstRowA = a->size[0];
    if (3 <= firstRowA) {
      lastRowB = 2;
    } else {
      lastRowB = a->size[0];
    }

    while (firstColB <= lastColB) {
      if (firstColB + na < a_idx_1) {
        lastColA = na;
      } else {
        lastColA = a_idx_1 - firstColB;
      }

      for (k = (firstColB < 1); k <= lastColA; k++) {
        if (firstColB + k > 1) {
          b_firstColB = (firstColB + k) - 1;
        } else {
          b_firstColB = 0;
        }

        iC = b_firstColB * a_idx_0;
        iA = k * ma;
        iB = firstRowB + firstColB * 3;
        for (i = firstRowB; i <= lastRowB; i++) {
          firstRowA = (i < 1);
          if (i + ma <= a_idx_0) {
            b_i = ma;
          } else {
            b_i = (a_idx_0 - i) + 1;
          }

          a_length = b_i - firstRowA;
          aidx = iA + firstRowA;
          firstRowA = iC;
          for (r = 1; r <= a_length; r++) {
            c->data[firstRowA] += (double)iv1[iB] * a->data[aidx];
            aidx++;
            firstRowA++;
          }

          iB++;
          if (i >= 1) {
            iC++;
          }
        }
      }

      firstColB++;
    }
  }
}

void conv2(const emxArray_real_T *a, emxArray_real_T *c)
{
  int aidx;
  int firstRowA;
  boolean_T b0;
  int a_idx_1;
  int a_idx_0;
  int ma;
  int na;
  int firstColB;
  int lastColB;
  int firstRowB;
  int lastRowB;
  int lastColA;
  int k;
  int b_firstColB;
  int iC;
  int iA;
  int iB;
  int i;
  int b_i;
  int a_length;
  int r;
  static const signed char iv0[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };

  aidx = c->size[0] * c->size[1];
  c->size[0] = a->size[0];
  c->size[1] = a->size[1];
  emxEnsureCapacity((emxArray__common *)c, aidx, sizeof(double));
  firstRowA = a->size[0] * a->size[1];
  for (aidx = 0; aidx < firstRowA; aidx++) {
    c->data[aidx] = 0.0;
  }

  b0 = ((a->size[0] == 0) || (a->size[1] == 0));
  if (!b0) {
    a_idx_1 = a->size[1];
    a_idx_0 = a->size[0];
    ma = a->size[0];
    na = a->size[1] - 1;
    if (a->size[1] < 1) {
      firstColB = 2;
    } else {
      firstColB = 0;
    }

    aidx = a->size[1];
    if (3 <= aidx) {
      lastColB = 2;
    } else {
      lastColB = a->size[1];
    }

    if (a->size[0] < 1) {
      firstRowB = 2;
    } else {
      firstRowB = 0;
    }

    firstRowA = a->size[0];
    if (3 <= firstRowA) {
      lastRowB = 2;
    } else {
      lastRowB = a->size[0];
    }

    while (firstColB <= lastColB) {
      if (firstColB + na < a_idx_1) {
        lastColA = na;
      } else {
        lastColA = a_idx_1 - firstColB;
      }

      for (k = (firstColB < 1); k <= lastColA; k++) {
        if (firstColB + k > 1) {
          b_firstColB = (firstColB + k) - 1;
        } else {
          b_firstColB = 0;
        }

        iC = b_firstColB * a_idx_0;
        iA = k * ma;
        iB = firstRowB + firstColB * 3;
        for (i = firstRowB; i <= lastRowB; i++) {
          firstRowA = (i < 1);
          if (i + ma <= a_idx_0) {
            b_i = ma;
          } else {
            b_i = (a_idx_0 - i) + 1;
          }

          a_length = b_i - firstRowA;
          aidx = iA + firstRowA;
          firstRowA = iC;
          for (r = 1; r <= a_length; r++) {
            c->data[firstRowA] += (double)iv0[iB] * a->data[aidx];
            aidx++;
            firstRowA++;
          }

          iB++;
          if (i >= 1) {
            iC++;
          }
        }
      }

      firstColB++;
    }
  }
}

/* End of code generation (conv2.c) */
