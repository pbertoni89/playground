clear variables
close all
clc

load('tomVirgin.mat');
load('corn_1.mat');

%% prefiltering data && a-priori knowledge

brickValidPixels = 105:245;
brickValidBins = 1:80;
bgEndTraining = 500;
bgEndTest = 500;
nTrainingBricks = 8;
kSize = 1;

tomVirgin = preFilteringPixelsBricks(tomVirgin, brickValidPixels, brickValidBins);
mat = preFilteringPixelsBricks(mat, brickValidPixels, brickValidBins);


%% fit gm models

[gmfit, validPixIdx, bands] = trainPomiGMhl(tomVirgin, nTrainingBricks, bgEndTraining, kSize);


%% process GM hl algorithm

[matAlarmedClose] = processPomiGMhl(mat, gmfit, validPixIdx, bands, bgEndTest, 'disk', 2);

figure('name', 'Output')
imagesc(matAlarmedClose);


%% Matlab correctness test (comparison with FRamoni original output)

load correctOutput.mat
if isequal(matAlarmedClose, correctOutput)
	fprintf('Correctness test passed.\n')
else
	fprintf('* * * * * Correctness test NOT passed !\n')
	matAbsDiff = abs(matAlarmedClose-correctOutput);
	figure('name', 'Abs Error Mat')
	imagesc(matAbsDiff)
end