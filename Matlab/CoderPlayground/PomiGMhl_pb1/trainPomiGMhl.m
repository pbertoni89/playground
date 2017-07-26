function [gmfit, validPixIdx, bands] = trainPomiGMhl (trainingData, nBricks, bgEnd, k)
% Fits a GM model for each pixel covering a Pomi Brik. 
%
% INPUT ARGUMENTS
%
% tomVirgin: 256*nColumn*128 matrix containing the Pomi Briks used to fit the models
% nBricks: number of Bricks in tomVirgin
% bgEnd: background ending column (i.e. 1:bgEnd are background columns in tomVirgin)
% k: number of gaussian distributions used to fit the model (default is 1)
% 
% OUTPUT 
%
% gmfit: family of GM models, one for each pixel in the image
% validPixIdx: vector index of valid pixels
% bands: vector of integration intervals for each pixel


% da parametrizzare. sotto nastro, sopra vuoto
trainingData = trainingData(105:245, :, 1:80);
tomVirginInt = squeeze(sum(trainingData, 3));

nPix = size(trainingData, 1);
nBands = 2;


%% find tomato columns

tomVirginIntD = edge(tomVirginInt, 'Prewitt');

for i = 1:nPix

	cc = bwconncomp(1 - tomVirginIntD(i, :));

	if cc.NumObjects == nBricks

		se = strel('square',15);
		colIdx = find(imerode(1-tomVirginIntD(i,:),se));
		break

	end

end


%% find valid pixels


tomInt = tomVirginInt(:,colIdx);
faultPix = zeros(nPix,1);

for i = 1:nPix

	E = mean(tomInt(i, :));
	dev = std(tomInt(i, :));

	if dev > 0
		% test gaussianita'
		% NOT supported by Coder
		faultPix(i) = kstest((tomInt(i,:)-E)/dev);
	else
		faultPix(i) = 1;
	end
end


%% update tomato matrix

validPixIdx = find(1-faultPix);
trainingData = trainingData(validPixIdx, :, :);
nPix = size(trainingData, 1);


%% find integration bands for each pixel

bg = trainingData(:, 1:bgEnd, :);
% sdpettro di bg cumulato sui bin
bgCum = squeeze(sum(bg, 2));

bands = [];

for i = 1:nPix
    
	% prendi quei valori dei bin dove hai almeno 1/100 del massimo dello
	% spettro (in pratica tagliane le code, dove e' troppo piccolo)
    range = find(bgCum(i, :) > 0.01*max(bgCum(i,:)));
    m = min(range);
    M = max(range);
	% generi le bande come vettori che hanno min med e max di bin
    bands = [bands; round(linspace(m,M,nBands+1))];
    
end


%% compute mean background energies

%%%bgEnergies = computeEnergies(bg, 1, bands, nBands);

bgEnergies = [];

for i = 1:nPix

	for j = 1:nBands

		bgEnergies(i,:,j) = sum(squeeze(bg(i,:,bands(i,j):bands(i,j+1))),2);

	end

end

bgEnergies = squeeze(mean(bgEnergies,2));

%% compute tomato energies

% processo analogo senza media ovviamente
tom = trainingData(:, colIdx, :);
tomEnergies = [];

for i = 1:nPix
    
    for j = 1:nBands
        
        tomEnergies(i, :, j) = sum(squeeze(tom(i, :, bands(i, j):bands(i,j+1))),2);
        
    end
    
end


%% fit GM models

gmfit = {};

for i = 1:nPix

	% vettori degli l e h
	l = -log(tomEnergies(i, :, 1)/bgEnergies(i,1));
	h = -log(tomEnergies(i, :, 2) / bgEnergies(i, 2));
	data = [l ; h]';

	% NOT supported by Coder
	gmfit{i} = fitgmdist(data, k, 'CovarianceType', 'full', 'SharedCovariance', true);
end


end % end of function
