/*
 * main.c
 *
 * Code generation for function 'main'
 *
 */

/*************************************************************************/
/* This automatically generated example C main file shows how to call    */
/* entry-point functions that MATLAB Coder generated. You must customize */
/* this file for your application. Do not modify this file directly.     */
/* Instead, make a copy of this file, modify it, and integrate it into   */
/* your development environment.                                         */
/*                                                                       */
/* This file initializes entry-point function arguments to a default     */
/* size and value before calling the entry-point functions. It does      */
/* not store or use any values returned from the entry-point functions.  */
/* If necessary, it does pre-allocate memory for returned values.        */
/* You can use this file as a starting point for a main function that    */
/* you can deploy in your application.                                   */
/*                                                                       */
/* After you copy the file, and before you deploy it, you must make the  */
/* following changes:                                                    */
/* * For variable-size function arguments, change the example sizes to   */
/* the sizes that your application requires.                             */
/* * Change the example values of function arguments to the values that  */
/* your application requires.                                            */
/* * If the entry-point functions return values, store these values or   */
/* otherwise use them as required by your application.                   */
/*                                                                       */
/*************************************************************************/
/* Include files */
#include "rt_nonfinite.h"
#include "sobel.h"
#include "main.h"
#include "sobel_terminate.h"
#include "sobel_emxAPI.h"
#include "sobel_initialize.h"

/* Function Declarations */
static emxArray_real_T *argInit_d1024xd1024_real_T(void);
static double argInit_real_T(void);
static void main_sobel(void);

/* Function Definitions */
static emxArray_real_T *argInit_d1024xd1024_real_T(void)
{
  emxArray_real_T *result;
  static int iv2[2] = { 2, 2 };

  int idx0;
  int idx1;

  /* Set the size of the array.
     Change this size to the value that the application requires. */
  result = emxCreateND_real_T(2, iv2);

  /* Loop over the array to initialize each element. */
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
      /* Set the value of the array element.
         Change this value to the value that the application requires. */
      result->data[idx0 + result->size[0] * idx1] = argInit_real_T();
    }
  }

  return result;
}

static double argInit_real_T(void)
{
  return 0.0;
}

static void main_sobel(void)
{
  emxArray_uint8_T *edgeImage;
  emxArray_real_T *originalImage;
  emxInitArray_uint8_T(&edgeImage, 2);

  /* Initialize function 'sobel' input arguments. */
  /* Initialize function input argument 'originalImage'. */
  originalImage = argInit_d1024xd1024_real_T();

  /* Call the entry-point 'sobel'. */
  sobel(originalImage, argInit_real_T(), edgeImage);
  emxDestroyArray_uint8_T(edgeImage);
  emxDestroyArray_real_T(originalImage);
}

int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;

  /* Initialize the application.
     You do not need to do this more than one time. */
  sobel_initialize();

  /* Invoke the entry-point functions.
     You can call entry-point functions multiple times. */
  main_sobel();

  /* Terminate the application.
     You do not need to do this more than one time. */
  sobel_terminate();
  return 0;
}

/* End of code generation (main.c) */
