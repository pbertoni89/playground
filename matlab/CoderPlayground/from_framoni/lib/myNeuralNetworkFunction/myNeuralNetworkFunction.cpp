//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: myNeuralNetworkFunction.cpp
//
// MATLAB Coder version            : 3.1
// C/C++ source code generated on  : 03-Feb-2017 12:29:29
//

// Include Files
#include "rt_nonfinite.h"
#include "myNeuralNetworkFunction.h"
#include "myNeuralNetworkFunction_emxutil.h"

// Function Declarations
static void mapminmax_apply(const emxArray_real_T *x, emxArray_real_T *y);

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
static void mapminmax_apply(const emxArray_real_T *x, emxArray_real_T *y)
{
  int csz_idx_1;
  int i0;
  int asub;
  int ak;
  emxArray_real_T *a;
  int ck;
  double av[43];
  static const double b[43] = { 0.656041428808909, 0.648132924656596,
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

  static const double b_b[43] = { 8.52823403899756, 8.27626809615877,
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

  //  ===== MODULE FUNCTIONS ========
  //  Map Minimum and Maximum Input Processing Function
  csz_idx_1 = x->size[1];
  i0 = y->size[0] * y->size[1];
  y->size[0] = 43;
  y->size[1] = csz_idx_1;
  emxEnsureCapacity((emxArray__common *)y, i0, (int)sizeof(double));
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

  emxInit_real_T(&a, 2);
  i0 = a->size[0] * a->size[1];
  a->size[0] = 43;
  a->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)a, i0, (int)sizeof(double));
  csz_idx_1 = y->size[0] * y->size[1];
  for (i0 = 0; i0 < csz_idx_1; i0++) {
    a->data[i0] = y->data[i0];
  }

  csz_idx_1 = y->size[1];
  i0 = y->size[0] * y->size[1];
  y->size[0] = 43;
  y->size[1] = csz_idx_1;
  emxEnsureCapacity((emxArray__common *)y, i0, (int)sizeof(double));
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
  emxEnsureCapacity((emxArray__common *)a, i0, (int)sizeof(double));
  csz_idx_1 = y->size[0] * y->size[1];
  for (i0 = 0; i0 < csz_idx_1; i0++) {
    a->data[i0] = y->data[i0];
  }

  csz_idx_1 = y->size[1];
  i0 = y->size[0] * y->size[1];
  y->size[0] = 43;
  y->size[1] = csz_idx_1;
  emxEnsureCapacity((emxArray__common *)y, i0, (int)sizeof(double));
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
}

//
// MYNEURALNETWORKFUNCTION neural network simulation function.
//
//  Generated by Neural Network Toolbox function genFunction, 03-Feb-2017 12:16:19.
//
//  [y1] = myNeuralNetworkFunction(x1) takes these arguments:
//    x = Qx43 matrix, input #1
//  and returns:
//    y = Qx1 matrix, output #1
//  where Q is the number of samples.
// Arguments    : const emxArray_real_T *x1
//                emxArray_real_T *b_y1
// Return Type  : void
//
void myNeuralNetworkFunction(const emxArray_real_T *x1, emxArray_real_T *b_y1)
{
  emxArray_real_T *b_x1;
  int nx;
  int br;
  emxArray_real_T *xp1;
  int ibtile;
  emxArray_real_T *a1;
  int unnamed_idx_1;
  emxArray_real_T *C;
  unsigned int b_unnamed_idx_1;
  int exitg1;
  static const double a[2] = { 2.880745322789922, -0.11360821349890438 };

  int ic;
  int ar;
  int ib;
  int ia;
  static const double dv0[86] = { -2.5523381575684638, 1.1327141935167773,
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

  emxArray_real_T *b_C;
  int n;
  emxArray_real_T *x;
  emxInit_real_T(&b_x1, 2);

  //  ===== NEURAL NETWORK CONSTANTS =====
  //  Input 1
  //  Layer 1
  //  Layer 2
  //  ===== SIMULATION ========
  //  Dimensions
  //  samples
  //  Input 1
  nx = b_x1->size[0] * b_x1->size[1];
  b_x1->size[0] = 43;
  b_x1->size[1] = x1->size[0];
  emxEnsureCapacity((emxArray__common *)b_x1, nx, (int)sizeof(double));
  br = x1->size[0];
  for (nx = 0; nx < br; nx++) {
    for (ibtile = 0; ibtile < 43; ibtile++) {
      b_x1->data[ibtile + b_x1->size[0] * nx] = x1->data[nx + x1->size[0] *
        ibtile];
    }
  }

  emxInit_real_T(&xp1, 2);
  emxInit_real_T(&a1, 2);
  mapminmax_apply(b_x1, xp1);

  //  Layer 1
  unnamed_idx_1 = x1->size[0];
  nx = a1->size[0] * a1->size[1];
  a1->size[0] = 2;
  a1->size[1] = unnamed_idx_1;
  emxEnsureCapacity((emxArray__common *)a1, nx, (int)sizeof(double));
  emxFree_real_T(&b_x1);
  if (!(unnamed_idx_1 == 0)) {
    nx = 1;
    do {
      exitg1 = 0;
      unnamed_idx_1 = x1->size[0];
      if (nx <= unnamed_idx_1) {
        ibtile = (nx - 1) << 1;
        for (br = 0; br < 2; br++) {
          a1->data[ibtile + br] = a[br];
        }

        nx++;
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  emxInit_real_T(&C, 2);
  b_unnamed_idx_1 = (unsigned int)xp1->size[1];
  nx = C->size[0] * C->size[1];
  C->size[0] = 2;
  C->size[1] = (int)b_unnamed_idx_1;
  C->size[0] = 2;
  emxEnsureCapacity((emxArray__common *)C, nx, (int)sizeof(double));
  br = C->size[1];
  for (nx = 0; nx < br; nx++) {
    for (ibtile = 0; ibtile < 2; ibtile++) {
      C->data[ibtile + C->size[0] * nx] = 0.0;
    }
  }

  if (xp1->size[1] == 0) {
  } else {
    nx = (xp1->size[1] - 1) << 1;
    for (ibtile = 0; ibtile <= nx; ibtile += 2) {
      for (ic = ibtile + 1; ic <= ibtile + 2; ic++) {
        C->data[ic - 1] = 0.0;
      }
    }

    br = 0;
    for (ibtile = 0; ibtile <= nx; ibtile += 2) {
      ar = -1;
      for (ib = br; ib + 1 <= br + 43; ib++) {
        if (xp1->data[ib] != 0.0) {
          ia = ar;
          for (ic = ibtile; ic + 1 <= ibtile + 2; ic++) {
            ia++;
            C->data[ic] += xp1->data[ib] * dv0[ia];
          }
        }

        ar += 2;
      }

      br += 43;
    }
  }

  emxFree_real_T(&xp1);

  //  Sigmoid Symmetric Transfer Function
  nx = a1->size[0] * a1->size[1];
  a1->size[0] = 2;
  emxEnsureCapacity((emxArray__common *)a1, nx, (int)sizeof(double));
  nx = a1->size[0];
  ibtile = a1->size[1];
  br = nx * ibtile;
  for (nx = 0; nx < br; nx++) {
    a1->data[nx] = -2.0 * (a1->data[nx] + C->data[nx]);
  }

  emxFree_real_T(&C);
  nx = a1->size[1] << 1;
  for (br = 0; br + 1 <= nx; br++) {
    a1->data[br] = exp(a1->data[br]);
  }

  nx = a1->size[0] * a1->size[1];
  a1->size[0] = 2;
  emxEnsureCapacity((emxArray__common *)a1, nx, (int)sizeof(double));
  nx = a1->size[0];
  ibtile = a1->size[1];
  br = nx * ibtile;
  for (nx = 0; nx < br; nx++) {
    a1->data[nx] = 2.0 / (1.0 + a1->data[nx]) - 1.0;
  }

  emxInit_real_T(&b_C, 2);

  //  Layer 2
  unnamed_idx_1 = x1->size[0];
  b_unnamed_idx_1 = (unsigned int)a1->size[1];
  nx = b_C->size[0] * b_C->size[1];
  b_C->size[0] = 1;
  b_C->size[1] = (int)b_unnamed_idx_1;
  emxEnsureCapacity((emxArray__common *)b_C, nx, (int)sizeof(double));
  n = a1->size[1] - 1;
  nx = b_C->size[0] * b_C->size[1];
  b_C->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)b_C, nx, (int)sizeof(double));
  br = b_C->size[1];
  for (nx = 0; nx < br; nx++) {
    b_C->data[b_C->size[0] * nx] = 0.0;
  }

  if (a1->size[1] == 0) {
  } else {
    for (ibtile = 1; ibtile - 1 <= n; ibtile++) {
      for (ic = ibtile; ic <= ibtile; ic++) {
        b_C->data[ic - 1] = 0.0;
      }
    }

    br = 0;
    for (ibtile = 0; ibtile <= n; ibtile++) {
      ar = -1;
      for (ib = br; ib + 1 <= br + 2; ib++) {
        if (a1->data[ib] != 0.0) {
          ia = ar;
          for (ic = ibtile; ic + 1 <= ibtile + 1; ic++) {
            ia++;
            b_C->data[ic] += a1->data[ib] * (-4.558551039706531 +
              8.3289066101558 * (double)ia);
          }
        }

        ar++;
      }

      br += 2;
    }
  }

  emxFree_real_T(&a1);

  //  Sigmoid Positive Transfer Function
  nx = b_C->size[0] * b_C->size[1];
  b_C->size[0] = 1;
  b_C->size[1] = unnamed_idx_1;
  emxEnsureCapacity((emxArray__common *)b_C, nx, (int)sizeof(double));
  for (nx = 0; nx < unnamed_idx_1; nx++) {
    b_C->data[nx] = -(4.6099497644430638 + b_C->data[nx]);
  }

  emxInit_real_T(&x, 2);
  nx = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = b_C->size[1];
  emxEnsureCapacity((emxArray__common *)x, nx, (int)sizeof(double));
  br = b_C->size[0] * b_C->size[1];
  for (nx = 0; nx < br; nx++) {
    x->data[nx] = b_C->data[nx];
  }

  for (br = 0; br + 1 <= b_C->size[1]; br++) {
    x->data[br] = exp(x->data[br]);
  }

  emxFree_real_T(&b_C);

  //  Output 1
  nx = b_y1->size[0];
  b_y1->size[0] = x->size[1];
  emxEnsureCapacity((emxArray__common *)b_y1, nx, (int)sizeof(double));
  br = x->size[1];
  for (nx = 0; nx < br; nx++) {
    b_y1->data[nx] = 1.0 / (1.0 + x->data[x->size[0] * nx]);
  }

  emxFree_real_T(&x);
}

//
// File trailer for myNeuralNetworkFunction.cpp
//
// [EOF]
//
