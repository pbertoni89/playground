/*
 * _coder_pbfib_info.c
 *
 * Code generation for function 'pbfib'
 *
 */

/* Include files */
#include "_coder_pbfib_info.h"

/* Function Definitions */
const mxArray *emlrtMexFcnResolvedFunctionsInfo()
{
  const mxArray *nameCaptureInfo;
  const char * data[4] = {
    "789ced92c14ec2401086070388070c279fc02b6e42a2e1284109264048e06294c46d3bc8c6ee6eddb65a4ef8165e7c18a36fe0c1e7b1140a8ba1a131112f4eb2"
    "e97efb27f3cf4c073217ed0c00ec87e77d0250d885288a732ecd10766035beeb9935dfa91e470eb2111735fd2de497399b5278187833b099c08ecf0d542108ca",
    "7191c6929c092abcfed84150e84afb01ad4819321bfb8c634b6ad06421f086262d602a4deff5119a773d9f831ab9cb726d1d409bcf6b42ffd994f3b94e984f69"
    "ceb17e753e201c2d464920c2b990b35abf467a72e83d528524701d343d45cb3e23a6b4b0ecd8747caba42f2cd2a69e4d0d520f9f5577f9ec1895f00c9971c4f5",
    "7e6e12eacda7ec27e97fef4141e3c3d34d7e69e7974b9c5f2ee2a8c514fda5f5cb6bbcea978fd892be61e3ff7efc7c3f9e9e3f069fb5edeee3b6f7ffeffc8284"
    "7c69f7f120c12fdec758a7a27559edb07bebb8dd54d58e7483934aa5b1aca3bbc167531da0f1ba3e7f2bff17f041d932",
    "" };

  nameCaptureInfo = NULL;
  emlrtNameCaptureMxArrayR2016a(data, 1824U, &nameCaptureInfo);
  return nameCaptureInfo;
}

mxArray *emlrtMexFcnProperties()
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
  emlrtSetField(xEntryPoints, 0, "Name", mxCreateString("pbfib"));
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

/* End of code generation (_coder_pbfib_info.c) */
