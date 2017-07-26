clear variables
close all
clc

load('tomVirgin.mat');
load('corn_1.mat');

%% fit gm models

[gmfit, validPixIdx, bands] = trainPomiGMhl(tomVirgin, 8, 500, 1);

%% process GM hl algorithm

[matAlarmedClose] = processPomiGMhl(mat, gmfit, validPixIdx, bands, 500, 'disk', 2);

figure('name', 'Output')
imagesc(matAlarmedClose);


%% Matlab correctness test (comparison with FRamoni original output)

load correctOutput.mat
if isequal(matAlarmedClose, correctOutput)
	fprintf('Correctness test passed.\n')
else
	fprintf('* * * * * Correctness test NOT passed !\n')
	matDiff = matAlarmedClose-correctOutput;
	figure('name', 'Error Mat')
	imagesc(matDiff)
end