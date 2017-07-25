/*
 * _coder_pb2_info.c
 *
 * Code generation for function 'pb2'
 *
 */

/* Include files */
#include "_coder_pb2_info.h"

/* Function Definitions */
const mxArray *emlrtMexFcnResolvedFunctionsInfo(void)
{
  const mxArray *nameCaptureInfo;
  const char * data[5] = {
    "789cc593dd4a024114c7c7b0af0bc39bc077109d12a42edbb44850597489488266dd5197e66319c75cbda907097a8d2ea39ea19bae7a9476d75d5dc56505c90e"
    "0c3bbff9c39c73fe7b06242ab50400e0c0599f4f0064768117299fd313045b603e16f5c492afab07b10d921ea742fa87c32f3eb73993d896132026c3f501d5b1",
    "7080218aa7d7189c9a0c31a98d2c0c04ee73f2880d4fe998046b26c5551e822bd3017a1992a6e04aeebed4c3ed87e68002d1ebcfca256100217fde23fa4faee8"
    "cf6d843f699f03bd75710729364c046de6f802cb8aa6c026efc8211218da7d0bb7a540b98109dbdcc0398ba05157f00133600d49827458728e853a3bb6f482bb",
    "f274be9ffb887a7756ec27ea7fef83bd1067ce827cf69afe1dc6f817e8ae2b226f3a4325182279825957f656e83bae8ec588aa238820dfdb9a7d3763f2057aab"
    "52bd59363adc92b0a66855e51c360a47c727084ace89ce6d8829f156d6b30c6603cfe0c4337f6036372fcfaf3fdf5fca66e773d3efe1fff26deafdd52e87eab5",
    "50b54ab17cdad0ebc5b1658c65a80e35264f5c1d20c4cbfafcabfb7f019b17d6f7", "" };

  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(data, 1840U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties(void)
{
  mxArray *xResult;
  mxArray *xEntryPoints;
  const char * fldNames[4] = { "Name", "NumberOfInputs", "NumberOfOutputs",
    "ConstantInputs" };

  mxArray *xInputs;
  const char * b_fldNames[4] = { "Version", "ResolvedFunctions", "EntryPoints",
    "CoverageInfo" };

  xEntryPoints = emlrtCreateStructMatrix(1, 1, 4, fldNames);
  xInputs = emlrtCreateLogicalMatrix(1, 1);
  emlrtSetField(xEntryPoints, 0, "Name", mxCreateString("pb2"));
  emlrtSetField(xEntryPoints, 0, "NumberOfInputs", mxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "NumberOfOutputs", mxCreateDoubleScalar(1.0));
  emlrtSetField(xEntryPoints, 0, "ConstantInputs", xInputs);
  xResult = emlrtCreateStructMatrix(1, 1, 4, b_fldNames);
  emlrtSetField(xResult, 0, "Version", mxCreateString("9.2.0.556344 (R2017a)"));
  emlrtSetField(xResult, 0, "ResolvedFunctions", (mxArray *)
                emlrtMexFcnResolvedFunctionsInfo());
  emlrtSetField(xResult, 0, "EntryPoints", xEntryPoints);
  return xResult;
}

/* End of code generation (_coder_pb2_info.c) */
