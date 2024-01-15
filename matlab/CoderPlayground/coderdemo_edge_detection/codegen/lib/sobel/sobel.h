/*
 * sobel.h
 *
 * Code generation for function 'sobel'
 *
 */

#ifndef SOBEL_H
#define SOBEL_H

/* Include files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "sobel_types.h"

/* Function Declarations */
extern void sobel(const emxArray_real_T *originalImage, double threshold,
                  emxArray_uint8_T *edgeImage);

#endif

/* End of code generation (sobel.h) */
