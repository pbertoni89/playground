@echo off
set MATLAB=C:\PROGRA~1\MATLAB\R2016a
set MATLAB_ARCH=win64
set MATLAB_BIN="C:\Program Files\MATLAB\R2016a\bin"
set ENTRYPOINT=mexFunction
set OUTDIR=.\
set LIB_NAME=myNeuralNetworkFunction_mex
set MEX_NAME=myNeuralNetworkFunction_mex
set MEX_EXT=.mexw64
call "C:\PROGRA~1\MATLAB\R2016a\sys\lcc64\lcc64\mex\lcc64opts.bat"
echo # Make settings for myNeuralNetworkFunction > myNeuralNetworkFunction_mex.mki
echo COMPILER=%COMPILER%>> myNeuralNetworkFunction_mex.mki
echo COMPFLAGS=%COMPFLAGS%>> myNeuralNetworkFunction_mex.mki
echo OPTIMFLAGS=%OPTIMFLAGS%>> myNeuralNetworkFunction_mex.mki
echo DEBUGFLAGS=%DEBUGFLAGS%>> myNeuralNetworkFunction_mex.mki
echo LINKER=%LINKER%>> myNeuralNetworkFunction_mex.mki
echo LINKFLAGS=%LINKFLAGS%>> myNeuralNetworkFunction_mex.mki
echo LINKOPTIMFLAGS=%LINKOPTIMFLAGS%>> myNeuralNetworkFunction_mex.mki
echo LINKDEBUGFLAGS=%LINKDEBUGFLAGS%>> myNeuralNetworkFunction_mex.mki
echo MATLAB_ARCH=%MATLAB_ARCH%>> myNeuralNetworkFunction_mex.mki
echo BORLAND=%BORLAND%>> myNeuralNetworkFunction_mex.mki
echo OMPFLAGS= >> myNeuralNetworkFunction_mex.mki
echo OMPLINKFLAGS= >> myNeuralNetworkFunction_mex.mki
echo EMC_COMPILER=lcc64>> myNeuralNetworkFunction_mex.mki
echo EMC_CONFIG=optim>> myNeuralNetworkFunction_mex.mki
"C:\Program Files\MATLAB\R2016a\bin\win64\gmake" -B -f myNeuralNetworkFunction_mex.mk
