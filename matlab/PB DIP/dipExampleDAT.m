clear all
close all
clc

pkg load image

if computer() == 'x86_64-pc-linux-gnu'
  dirprefix = '/media/xnext/DATA/';
else
  dirprefix = 'D:\';
end

dir_to_add = strcat(dirprefix, 'Software/MATLAB/XSlibrary');
filename = strcat(dirprefix, '/17-06-01/tom_120kV0dot2mA.dat');

addpath(dir_to_add)

N_PIXELS = 128;
N_BINS = 128;
N_BOARDS = 3;
N_PIXELS_TOT = N_PIXELS * N_BOARDS;
N_COUNTS_TOT = N_PIXELS_TOT * N_BINS;

%%%% mat = openDAT(filename, N_PIXELS_TOT, N_BINS);

fileID = fopen(filename, 'rb');

[acquisitionArray, count] = fread(fileID, 'uint16');

fclose(fileID);

tacq = count / N_COUNTS_TOT;

% each row contains data for a <pixel,bin> coordinate
% 128*128*3 rows
% each col contains data for a <acquisition> coordinate
% n acquisitions
dataRaw = reshape(acquisitionArray, [N_COUNTS_TOT, tacq]);

% First dimension is Bin
%	Second dimension is Pixel
% Third dimension is Acquisition
matDetwinninned = reshape(dataRaw, [N_BINS, N_PIXELS_TOT, tacq]);

% First dimension is Pixel
%	Second dimension is Acquisition
% Third dimension is Bin
mat = permute(matDetwinninned, [2, 3, 1]);


matInt = getIntegration(mat);
figure('name', 'IMAGE')
subplot(3, 1, 1)
imagesc(matInt);

%% G INPUT
enableGinput = 0;
if computer() == 'x86_64-pc-linux-gnu'
  while enableGinput
    printf('Click anywhere over the figure')
    [tc, px] = ginput(1);
    printf(' Selected px %d, tc %d\n', px, tc);
  end
end

%% DEAD PIXELS
printf('Suggesting dead pixels')
pxDeads = suggestDeadPixels(matInt, 5);
disp(pxDeads')

%% EDGE DETECTOR
matIntDiff = suggestEdgeDetection(matInt);
minDiff = min(matIntDiff);
matIntDiff = matIntDiff + minDiff;
subplot(3, 1, 2)
imagesc(matIntDiff)

matIntThresh = im2bw(matIntDiff);
subplot(3, 1, 3)
imagesc(matIntThresh)
