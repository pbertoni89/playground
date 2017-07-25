/*
 * pb2_rtwutil.c
 *
 * Code generation for function 'pb2_rtwutil'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "pb2.h"
#include "pb2_rtwutil.h"
#include <stdio.h>
#include <stdlib.h>

/* Function Declarations */
static boolean_T rtIsNullOrEmptyString(const char *aString);
static void rtReportErrorLocation(const char * aFcnName, const int aLineNo);

/* Function Definitions */
static boolean_T rtIsNullOrEmptyString(const char *aString)
{
  return (aString == NULL) || (*aString == '\x00');
}

static void rtReportErrorLocation(const char * aFcnName, const int aLineNo)
{
  fprintf(stderr, "Error in %s (line %d)", aFcnName, aLineNo);
  fprintf(stderr, "\n");
}

void rtDynamicBoundsError(int aIndexValue, int aLoBound, int aHiBound, const
  rtBoundsCheckInfo *aInfo)
{
  if (aLoBound == 0) {
    aIndexValue++;
    aLoBound = 1;
    aHiBound++;
  }

  if (rtIsNullOrEmptyString(aInfo->aName)) {
    fprintf(stderr,
            "Index exceeds array dimensions.  Index value %d exceeds valid range [%d-%d].",
            aIndexValue, aLoBound, aHiBound);
    fprintf(stderr, "\n");
  } else {
    fprintf(stderr,
            "Index exceeds array dimensions.  Index value %d exceeds valid range [%d-%d] of array %s.",
            aIndexValue, aLoBound, aHiBound, aInfo->aName);
    fprintf(stderr, "\n");
  }

  if (aInfo != NULL) {
    rtReportErrorLocation(aInfo->fName, aInfo->lineNo);
  }

  fflush(stderr);
  abort();
}

/* End of code generation (pb2_rtwutil.c) */
