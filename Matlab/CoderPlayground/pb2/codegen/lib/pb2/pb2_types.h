/*
 * pb2_types.h
 *
 * Code generation for function 'pb2'
 *
 */

#ifndef PB2_TYPES_H
#define PB2_TYPES_H

/* Include files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_rtBoundsCheckInfo
#define typedef_rtBoundsCheckInfo

typedef struct {
  int iFirst;
  int iLast;
  int lineNo;
  int colNo;
  const char * aName;
  const char * fName;
  const char * pName;
  int checkKind;
} rtBoundsCheckInfo;

#endif                                 /*typedef_rtBoundsCheckInfo*/
#endif

/* End of code generation (pb2_types.h) */
