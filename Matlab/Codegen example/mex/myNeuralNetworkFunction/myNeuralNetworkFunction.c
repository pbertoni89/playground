/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * myNeuralNetworkFunction.c
 *
 * Code generation for function 'myNeuralNetworkFunction'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "myNeuralNetworkFunction.h"
#include "myNeuralNetworkFunction_emxutil.h"
#include "eml_int_forloop_overflow_check.h"
#include "repmat.h"
#include "myNeuralNetworkFunction_data.h"
#include "blas.h"

/* Variable Definitions */
static emlrtRSInfo emlrtRSI = { 36, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRSInfo b_emlrtRSI = { 39, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRSInfo c_emlrtRSI = { 42, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRSInfo g_emlrtRSI = { 68, "eml_mtimes_helper",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\ops\\eml_mtimes_helper.m"
};

static emlrtRSInfo h_emlrtRSI = { 85, "xgemm",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\+blas\\xgemm.m"
};

static emlrtRSInfo i_emlrtRSI = { 65, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRSInfo j_emlrtRSI = { 9, "exp",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\elfun\\exp.m" };

static emlrtRSInfo k_emlrtRSI = { 24, "applyScalarFunctionInPlace",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\eml\\+coder\\+internal\\applyScalarFunctionInPlace.m"
};

static emlrtRSInfo l_emlrtRSI = { 60, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtMCInfo emlrtMCI = { 37, 5, "repmat",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\elmat\\repmat.m"
};

static emlrtRTEInfo emlrtRTEI = { 1, 17, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRTEInfo b_emlrtRTEI = { 36, 1, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRTEInfo c_emlrtRTEI = { 39, 1, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRTEInfo d_emlrtRTEI = { 53, 5, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRTEInfo e_emlrtRTEI = { 52, 14, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRTEInfo f_emlrtRTEI = { 54, 5, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRTEInfo g_emlrtRTEI = { 55, 5, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtECInfo emlrtECI = { 2, 42, 19, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtECInfo b_emlrtECI = { 2, 39, 19, "myNeuralNetworkFunction",
  "C:\\Users\\Francesco\\Desktop\\myNeuralNetworkFunction.m" };

static emlrtRSInfo m_emlrtRSI = { 37, "repmat",
  "C:\\Program Files\\MATLAB\\R2016a\\toolbox\\eml\\lib\\matlab\\elmat\\repmat.m"
};

/* Function Declarations */
static void error(const emlrtStack *sp, const mxArray *b, emlrtMCInfo *location);
static void mapminmax_apply(const emlrtStack *sp, const emxArray_real_T *x,
  emxArray_real_T *y);

/* Function Definitions */
static void error(const emlrtStack *sp, const mxArray *b, emlrtMCInfo *location)
{
  const mxArray *pArray;
  pArray = b;
  emlrtCallMATLABR2012b(sp, 0, NULL, 1, &pArray, "error", true, location);
}

static void mapminmax_apply(const emlrtStack *sp, const emxArray_real_T *x,
  emxArray_real_T *y)
{
  int32_T csz_idx_1;
  int32_T i0;
  int32_T asub;
  int32_T ak;
  emxArray_real_T *a;
  int32_T ck;
  real_T av[43];
  static const real_T b[43] = { 0.656041428808909, 0.648132924656596,
    0.635545769324946, 0.621541160672475, 0.606763860887982, 0.593448298741977,
    0.585675044536291, 0.576356409441272, 0.562195360688892, 0.553726246989116,
    0.543891120500841, 0.533875236941309, 0.526991285439642, 0.516332529001845,
    0.509986959311012, 0.501699200437268, 0.49306218115799, 0.488704877127417,
    0.483133130665391, 0.48604904854347, 0.478451088170606, 0.476180882773912,
    0.476806164072579, 0.469874394960643, 0.470238388964973, 0.461368742537126,
    0.457678016180519, 0.456460877123827, 0.455860843038641, 0.450734761480453,
    0.449303750372066, 0.438931336082757, 0.438060866829063, 0.434951843124672,
    0.426296872623262, 0.417278565667962, 0.423128420604337, 0.397077980474097,
    0.397657648761439, 0.396093545636214, 0.391627819209266, 0.385046856764187,
    0.368003105581474 };

  static const real_T b_b[43] = { 8.52823403899756, 8.27626809615877,
    7.93982723901366, 7.65577991724497, 7.34115379426106, 7.09866972599553,
    7.00176461405321, 6.86310150483152, 6.58172474967607, 6.50261931351099,
    6.38012670170497, 6.33614255812693, 6.2901382730947, 6.13895585476728,
    6.04428213142858, 5.898651498554, 5.82241287574862, 5.85289388197285,
    5.77691223760886, 5.87858472270448, 5.85152540346026, 5.85172088907537,
    5.90838555301795, 5.93706384626704, 5.99550861137606, 5.91230697009378,
    5.86500110871181, 5.7960381100435, 5.8711590731459, 5.89107868887205,
    6.03231169818776, 5.95162842063831, 5.9878461072238, 6.06342423897852,
    5.93739886213791, 5.93834757793424, 6.21499302747404, 5.795102304748,
    5.81915882408124, 5.755103314956, 5.74955820310424, 5.68158508377529,
    5.47151730942343 };

  emlrtHeapReferenceStackEnterFcnR2012b(sp);
  covrtLogFcn(&emlrtCoverageInstance, 0U, 1);
  covrtLogBasicBlock(&emlrtCoverageInstance, 0U, 1);

  /*  ===== MODULE FUNCTIONS ======== */
  /*  Map Minimum and Maximum Input Processing Function */
  csz_idx_1 = x->size[1];
  i0 = y->size[0] * y->size[1];
  y->size[0] = 43;
  y->size[1] = csz_idx_1;
  emxEnsureCapacity(sp, (emxArray__common *)y, i0, (int32_T)sizeof(real_T),
                    &d_emlrtRTEI);
  if (y->size[1] == 0) {
  } else {
    asub = 1;
    ak = 0;
    i0 = 43 * y->size[1] - 43;
    for (ck = 0; ck <= i0; ck += 43) {
      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        av[csz_idx_1] = x->data[ak + csz_idx_1];
      }

      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        av[csz_idx_1] -= b[csz_idx_1];
      }

      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        y->data[ck + csz_idx_1] = av[csz_idx_1];
      }

      if (asub < x->size[1]) {
        ak += 43;
        asub++;
      } else {
        asub = 1;
      }
    }
  }

  emxInit_real_T(sp, &a, 2, &e_emlrtRTEI, true);
  i0 = a->size[0] * a->size[1];
  a->size[0] = 43;
  a->size[1] = y->size[1];
  emxEnsureCapacity(sp, (emxArray__common *)a, i0, (int32_T)sizeof(real_T),
                    &e_emlrtRTEI);
  csz_idx_1 = y->size[0] * y->size[1];
  for (i0 = 0; i0 < csz_idx_1; i0++) {
    a->data[i0] = y->data[i0];
  }

  csz_idx_1 = y->size[1];
  i0 = y->size[0] * y->size[1];
  y->size[0] = 43;
  y->size[1] = csz_idx_1;
  emxEnsureCapacity(sp, (emxArray__common *)y, i0, (int32_T)sizeof(real_T),
                    &f_emlrtRTEI);
  if (y->size[1] == 0) {
  } else {
    asub = 1;
    ak = 0;
    i0 = 43 * y->size[1] - 43;
    for (ck = 0; ck <= i0; ck += 43) {
      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        av[csz_idx_1] = a->data[ak + csz_idx_1];
      }

      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        av[csz_idx_1] *= b_b[csz_idx_1];
      }

      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        y->data[ck + csz_idx_1] = av[csz_idx_1];
      }

      if (asub < a->size[1]) {
        ak += 43;
        asub++;
      } else {
        asub = 1;
      }
    }
  }

  i0 = a->size[0] * a->size[1];
  a->size[0] = 43;
  a->size[1] = y->size[1];
  emxEnsureCapacity(sp, (emxArray__common *)a, i0, (int32_T)sizeof(real_T),
                    &e_emlrtRTEI);
  csz_idx_1 = y->size[0] * y->size[1];
  for (i0 = 0; i0 < csz_idx_1; i0++) {
    a->data[i0] = y->data[i0];
  }

  csz_idx_1 = y->size[1];
  i0 = y->size[0] * y->size[1];
  y->size[0] = 43;
  y->size[1] = csz_idx_1;
  emxEnsureCapacity(sp, (emxArray__common *)y, i0, (int32_T)sizeof(real_T),
                    &g_emlrtRTEI);
  if (y->size[1] == 0) {
  } else {
    asub = 1;
    ak = 0;
    i0 = 43 * y->size[1] - 43;
    for (ck = 0; ck <= i0; ck += 43) {
      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        av[csz_idx_1] = a->data[ak + csz_idx_1];
      }

      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        av[csz_idx_1] += -1.0;
      }

      for (csz_idx_1 = 0; csz_idx_1 < 43; csz_idx_1++) {
        y->data[ck + csz_idx_1] = av[csz_idx_1];
      }

      if (asub < a->size[1]) {
        ak += 43;
        asub++;
      } else {
        asub = 1;
      }
    }
  }

  emxFree_real_T(&a);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
}

void myNeuralNetworkFunction(const emlrtStack *sp, const emxArray_real_T *x1,
  emxArray_real_T *b_y1)
{
  emxArray_real_T *b_x1;
  int32_T k;
  int32_T loop_ub;
  emxArray_real_T *xp1;
  int32_T nx;
  emxArray_real_T *a1;
  emxArray_real_T *C;
  uint32_T unnamed_idx_1;
  real_T alpha1;
  real_T beta1;
  char_T TRANSB;
  char_T TRANSA;
  int32_T b_a1[2];
  ptrdiff_t m_t;
  ptrdiff_t n_t;
  ptrdiff_t k_t;
  int32_T b_C[2];
  ptrdiff_t lda_t;
  ptrdiff_t ldb_t;
  ptrdiff_t ldc_t;
  static const real_T A[86] = { -2.5523381575684638, 1.1327141935167773,
    -2.0331069799021266, -0.75587403542563025, -1.2040605209966788,
    -1.238709331925506, -0.84238524900893819, -1.0538739198258129,
    -1.4582522373803171, 0.77314463817630008, -0.92597601924010819,
    0.9319356644566803, -0.79547450811756526, 0.189179527519751,
    0.29203385568218831, -0.89498277352563838, -0.61044770822895367,
    -0.2025887055730913, 0.18145866696941748, 0.090927888116616631,
    -0.14307417930222993, 0.71692689653308284, -0.012017496394328447,
    -0.0090418352034615339, 0.035639850030070516, -0.29046991883851864,
    -0.44631463366888907, -0.71076449883516168, 0.25045771718470405,
    -0.26445790013930731, -0.68411683894457909, 0.32549408656769085,
    -0.064273005157939669, 0.34948555586327296, -0.022980814240449163,
    -0.28507341208444459, 0.066117582452851667, 0.7055242936211541,
    -0.27696901707718885, 0.15556711636583498, -0.10143514163403786,
    -0.60274715798592038, -0.42651596524656121, 0.090251964614273386,
    -1.0683123003436268, 0.28258011308229769, 0.0431304867911785,
    -0.445005891100002, -0.69816889315356812, -0.204004750895952,
    0.63144710896822343, -0.13745585928653545, 1.0207883694074016,
    -0.16327211763389543, 0.38894316838289839, -0.66557546073970386,
    -0.14799381435054204, -0.087983139448370426, -0.33452525214395823,
    -0.13188403293882478, 0.6686850610256968, -0.57972815763429675,
    -0.085897725067513614, 0.41556339261103226, -0.96814343341267062,
    0.34876711271408189, -0.13046562452878732, -0.30302735918750662,
    -0.15542487224307766, -0.95835794366168514, -0.29208990078753655,
    0.046173225967382325, -0.062128806884480796, -0.24373077812965391,
    0.84880966851389883, 0.443921961982897, 0.16863032146363668,
    0.013899805418541319, -0.24783263116026866, 0.7708302903063915,
    -1.1704788059167939, -0.8225336476036651, 0.26734266771490461,
    0.046571156848892147, 0.78129997412539831, -1.4282252373367854 };

  int32_T b_unnamed_idx_1;
  emxArray_real_T *c_C;
  const mxArray *y;
  char_T u[15];
  static const char_T cv0[15] = { 'M', 'A', 'T', 'L', 'A', 'B', ':', 'p', 'm',
    'a', 'x', 's', 'i', 'z', 'e' };

  const mxArray *m0;
  static const int32_T iv0[2] = { 1, 15 };

  real_T b_A[2];
  int32_T iv1[2];
  int32_T d_C[2];
  emxArray_real_T *x;
  boolean_T overflow;
  emlrtStack st;
  emlrtStack b_st;
  emlrtStack c_st;
  emlrtStack d_st;
  emlrtStack e_st;
  st.prev = sp;
  st.tls = sp->tls;
  b_st.prev = &st;
  b_st.tls = st.tls;
  c_st.prev = &b_st;
  c_st.tls = b_st.tls;
  d_st.prev = &c_st;
  d_st.tls = c_st.tls;
  e_st.prev = &d_st;
  e_st.tls = d_st.tls;
  emlrtHeapReferenceStackEnterFcnR2012b(sp);
  emxInit_real_T(sp, &b_x1, 2, &emlrtRTEI, true);
  covrtLogFcn(&emlrtCoverageInstance, 0U, 0);
  covrtLogBasicBlock(&emlrtCoverageInstance, 0U, 0);

  /* MYNEURALNETWORKFUNCTION neural network simulation function. */
  /*  */
  /*  Generated by Neural Network Toolbox function genFunction, 03-Feb-2017 12:16:19. */
  /*  */
  /*  [y1] = myNeuralNetworkFunction(x1) takes these arguments: */
  /*    x = Qx43 matrix, input #1 */
  /*  and returns: */
  /*    y = Qx1 matrix, output #1 */
  /*  where Q is the number of samples. */
  /*  ===== NEURAL NETWORK CONSTANTS ===== */
  /*  Input 1 */
  /*  Layer 1 */
  /*  Layer 2 */
  /*  ===== SIMULATION ======== */
  /*  Dimensions */
  /*  samples */
  /*  Input 1 */
  k = b_x1->size[0] * b_x1->size[1];
  b_x1->size[0] = 43;
  b_x1->size[1] = x1->size[0];
  emxEnsureCapacity(sp, (emxArray__common *)b_x1, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  loop_ub = x1->size[0];
  for (k = 0; k < loop_ub; k++) {
    for (nx = 0; nx < 43; nx++) {
      b_x1->data[nx + b_x1->size[0] * k] = x1->data[k + x1->size[0] * nx];
    }
  }

  emxInit_real_T(sp, &xp1, 2, &b_emlrtRTEI, true);
  emxInit_real_T(sp, &a1, 2, &c_emlrtRTEI, true);
  emxInit_real_T(sp, &C, 2, &emlrtRTEI, true);
  st.site = &emlrtRSI;
  mapminmax_apply(&st, b_x1, xp1);

  /*  Layer 1 */
  st.site = &b_emlrtRSI;
  repmat(&st, x1->size[0], a1);
  st.site = &b_emlrtRSI;
  unnamed_idx_1 = (uint32_T)xp1->size[1];
  k = C->size[0] * C->size[1];
  C->size[0] = 2;
  C->size[1] = (int32_T)unnamed_idx_1;
  C->size[0] = 2;
  emxEnsureCapacity(&st, (emxArray__common *)C, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  loop_ub = C->size[1];
  emxFree_real_T(&b_x1);
  for (k = 0; k < loop_ub; k++) {
    for (nx = 0; nx < 2; nx++) {
      C->data[nx + C->size[0] * k] = 0.0;
    }
  }

  b_st.site = &g_emlrtRSI;
  if (xp1->size[1] < 1) {
  } else {
    c_st.site = &h_emlrtRSI;
    alpha1 = 1.0;
    beta1 = 0.0;
    TRANSB = 'N';
    TRANSA = 'N';
    m_t = (ptrdiff_t)2;
    n_t = (ptrdiff_t)xp1->size[1];
    k_t = (ptrdiff_t)43;
    lda_t = (ptrdiff_t)2;
    ldb_t = (ptrdiff_t)43;
    ldc_t = (ptrdiff_t)2;
    dgemm(&TRANSA, &TRANSB, &m_t, &n_t, &k_t, &alpha1, &A[0], &lda_t, &xp1->
          data[0], &ldb_t, &beta1, &C->data[0], &ldc_t);
  }

  emxFree_real_T(&xp1);
  for (k = 0; k < 2; k++) {
    b_a1[k] = a1->size[k];
  }

  for (k = 0; k < 2; k++) {
    b_C[k] = C->size[k];
  }

  if ((b_a1[0] != b_C[0]) || (b_a1[1] != b_C[1])) {
    emlrtSizeEqCheckNDR2012b(&b_a1[0], &b_C[0], &b_emlrtECI, sp);
  }

  st.site = &b_emlrtRSI;
  covrtLogFcn(&emlrtCoverageInstance, 0U, 3);
  covrtLogBasicBlock(&emlrtCoverageInstance, 0U, 3);

  /*  Sigmoid Symmetric Transfer Function */
  k = a1->size[0] * a1->size[1];
  a1->size[0] = 2;
  emxEnsureCapacity(&st, (emxArray__common *)a1, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  nx = a1->size[0];
  k = a1->size[1];
  loop_ub = nx * k;
  for (k = 0; k < loop_ub; k++) {
    a1->data[k] = -2.0 * (a1->data[k] + C->data[k]);
  }

  emxFree_real_T(&C);
  b_st.site = &i_emlrtRSI;
  c_st.site = &j_emlrtRSI;
  nx = a1->size[1] << 1;
  d_st.site = &k_emlrtRSI;
  if ((!(1 > nx)) && (nx > 2147483646)) {
    e_st.site = &f_emlrtRSI;
    check_forloop_overflow_error(&e_st);
  }

  for (k = 0; k + 1 <= nx; k++) {
    a1->data[k] = muDoubleScalarExp(a1->data[k]);
  }

  k = a1->size[0] * a1->size[1];
  a1->size[0] = 2;
  emxEnsureCapacity(&st, (emxArray__common *)a1, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  nx = a1->size[0];
  k = a1->size[1];
  loop_ub = nx * k;
  for (k = 0; k < loop_ub; k++) {
    a1->data[k] = 2.0 / (1.0 + a1->data[k]) - 1.0;
  }

  /*  Layer 2 */
  st.site = &c_emlrtRSI;
  b_st.site = &d_emlrtRSI;
  b_unnamed_idx_1 = x1->size[0];
  nx = x1->size[0];
  if (b_unnamed_idx_1 == nx) {
  } else {
    for (k = 0; k < 15; k++) {
      u[k] = cv0[k];
    }

    y = NULL;
    m0 = emlrtCreateCharArray(2, iv0);
    emlrtInitCharArrayR2013a(&st, 15, m0, &u[0]);
    emlrtAssign(&y, m0);
    b_st.site = &m_emlrtRSI;
    error(&b_st, y, &emlrtMCI);
  }

  emxInit_real_T(&st, &c_C, 2, &emlrtRTEI, true);
  st.site = &c_emlrtRSI;
  unnamed_idx_1 = (uint32_T)a1->size[1];
  k = c_C->size[0] * c_C->size[1];
  c_C->size[0] = 1;
  c_C->size[1] = (int32_T)unnamed_idx_1;
  c_C->size[0] = 1;
  emxEnsureCapacity(&st, (emxArray__common *)c_C, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  loop_ub = c_C->size[1];
  for (k = 0; k < loop_ub; k++) {
    c_C->data[c_C->size[0] * k] = 0.0;
  }

  b_st.site = &g_emlrtRSI;
  if (a1->size[1] < 1) {
  } else {
    c_st.site = &h_emlrtRSI;
    alpha1 = 1.0;
    for (k = 0; k < 2; k++) {
      b_A[k] = -4.558551039706531 + 8.3289066101558 * (real_T)k;
    }

    beta1 = 0.0;
    TRANSB = 'N';
    TRANSA = 'N';
    m_t = (ptrdiff_t)1;
    n_t = (ptrdiff_t)a1->size[1];
    k_t = (ptrdiff_t)2;
    lda_t = (ptrdiff_t)1;
    ldb_t = (ptrdiff_t)2;
    ldc_t = (ptrdiff_t)1;
    dgemm(&TRANSA, &TRANSB, &m_t, &n_t, &k_t, &alpha1, &b_A[0], &lda_t,
          &a1->data[0], &ldb_t, &beta1, &c_C->data[0], &ldc_t);
  }

  emxFree_real_T(&a1);
  iv1[0] = 1;
  iv1[1] = b_unnamed_idx_1;
  for (k = 0; k < 2; k++) {
    d_C[k] = c_C->size[k];
  }

  if ((iv1[0] != d_C[0]) || (iv1[1] != d_C[1])) {
    emlrtSizeEqCheckNDR2012b(&iv1[0], &d_C[0], &emlrtECI, sp);
  }

  st.site = &c_emlrtRSI;
  covrtLogFcn(&emlrtCoverageInstance, 0U, 2);
  covrtLogBasicBlock(&emlrtCoverageInstance, 0U, 2);

  /*  Sigmoid Positive Transfer Function */
  b_st.site = &l_emlrtRSI;
  k = c_C->size[0] * c_C->size[1];
  c_C->size[0] = 1;
  c_C->size[1] = b_unnamed_idx_1;
  emxEnsureCapacity(&b_st, (emxArray__common *)c_C, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  for (k = 0; k < b_unnamed_idx_1; k++) {
    c_C->data[k] = -(4.6099497644430638 + c_C->data[k]);
  }

  emxInit_real_T(&b_st, &x, 2, &emlrtRTEI, true);
  c_st.site = &j_emlrtRSI;
  k = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = c_C->size[1];
  emxEnsureCapacity(&c_st, (emxArray__common *)x, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  loop_ub = c_C->size[0] * c_C->size[1];
  for (k = 0; k < loop_ub; k++) {
    x->data[k] = c_C->data[k];
  }

  d_st.site = &k_emlrtRSI;
  overflow = ((!(1 > c_C->size[1])) && (c_C->size[1] > 2147483646));
  if (overflow) {
    e_st.site = &f_emlrtRSI;
    check_forloop_overflow_error(&e_st);
  }

  for (k = 0; k + 1 <= c_C->size[1]; k++) {
    x->data[k] = muDoubleScalarExp(x->data[k]);
  }

  emxFree_real_T(&c_C);

  /*  Output 1 */
  k = b_y1->size[0];
  b_y1->size[0] = x->size[1];
  emxEnsureCapacity(sp, (emxArray__common *)b_y1, k, (int32_T)sizeof(real_T),
                    &emlrtRTEI);
  loop_ub = x->size[1];
  for (k = 0; k < loop_ub; k++) {
    b_y1->data[k] = 1.0 / (1.0 + x->data[x->size[0] * k]);
  }

  emxFree_real_T(&x);
  emlrtHeapReferenceStackLeaveFcnR2012b(sp);
}

/* End of code generation (myNeuralNetworkFunction.c) */
