clear all
close all
clc

addpath('D:\Software\MATLAB\XSlibrary')

filename = 'D:\17-06-01\tomVirgin.mat';
filenameOutput = 'D:\17-06-01\tomVirgin.xsx';

N_PIXELS = 128;
N_BINS = 128;
N_BOARDS = 3;
N_PIXELS_TOT = N_PIXELS * N_BOARDS;
N_COUNTS_COL = N_PIXELS_TOT * N_BINS;

tic
load(filename);

% tomVirgin is a mat:
%
% First dimension is Pixel
%	Second dimension is Acquisition
% Third dimension is Bin

mat = tomVirgin;
xsx = writeXSXfromMAT(mat, filenameOutput);

%% PROOF R/W

mat1 = readMATfromXSX(filenameOutput, N_PIXELS_TOT, N_BINS);

if mat ==  mat1
	figure
	imagesc(getIntegration(mat))
else	
	fprintf('mats different. print diff image\n')
	figure
	subplot(3, 1, 1), imagesc(getIntegration(mat))
	subplot(3, 1, 2), imagesc(getIntegration(mat1))
	subplot(3, 1, 3), imagesc(getIntegration(abs(mat-mat1)))
end
%
%arrMat = reshape(mat, [numel(mat), 1]);
%arrXsx = reshape(xsx, [numel(mat), 1]);
%
%
%%% THIS PROOFS only that data is just rearranged, never modifyied
%figure
%subplot(2, 1, 1)
%hist(arrMat)
%subplot(2, 1, 2)
%hist(arrXsx)

toc

%%

%N_ACQ = size(xsx, 1);
%N_COUNTS_FILE = N_ACQ * N_COUNTS_COL;

%[xsxNNacq, xsxNNpxbin] = find(xsx, 1);
%[xsxNNpx, xsxNNbin] = ind2sub([N_PIXELS_TOT, N_BINS], xsxNNpxbin); 

%iFirstNNacq = floor(iFirstNN / N_ACQ);
%[I, J, K] = ind2sub(size(xsx), iFirstNN);