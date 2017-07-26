function [gmfit, validPixIdx, bands] = trainPomiGMhl (data, nBricks, bgEnd, kSize)
% Fits a GM model for each pixel covering a Pomi Brik. 
%
% INPUT ARGUMENTS
%
% tomVirgin: 256*nColumn*128 matrix containing the Pomi Briks used to fit the models
% nBricks: number of Bricks in tomVirgin
% bgEnd: background ending column (i.e. 1:bgEnd are background columns in tomVirgin)
% kSize: number of gaussian distributions used to fit the model (default is 1)
% 
% OUTPUT 
%
% gmfit: family of GM models, one for each pixel in the image
% validPixIdx: vector index of valid pixels
% bands: vector of integration intervals for each pixel


nPixels = size(data, 1);
nBands = 2;
nBandPts = nBands + 1;

dataIntOverBins = squeeze(sum(data, 3));


%% Find tomato columns

dataIntED = edge(dataIntOverBins, 'Prewitt');
NHOOD = 15;

for i = 1:nPixels

	cc = bwconncomp(1 - dataIntED(i, :));

	% exactly nBricks connected components are found
	if cc.NumObjects == nBricks

		se = strel('square', NHOOD);
		tomatoColumnIndexes = find(imerode(1-dataIntED(i, :), se));
		break

	end

end


%% Check Edge Detection

if exist('tomatoColumnIndexes', 'var') ~= 1

	error('Cannot train the classifier (cannot find %d bricks in training dataset)\n', nBricks)

end


%% find valid pixels (NOT supported by Coder)

dataIntOverBins = dataIntOverBins(:, tomatoColumnIndexes);
faultPixels = zeros(nPixels, 1);

for i = 1:nPixels

	E = mean(dataIntOverBins(i, :));
	dev = std(dataIntOverBins(i, :));

	if dev > 0
		trainingDataIntOverBinsStd = (dataIntOverBins(i, :)-E)/dev;
		% NOT supported by Coder. gaussianity test
		faultPixels(i) = kstest(trainingDataIntOverBinsStd);
	else
		faultPixels(i) = 1;
	end
end


%% update training data over valid pixels only

validPixIdx = find(1-faultPixels);

data = data(validPixIdx, :, :);
tomato = data(:, tomatoColumnIndexes, :);

nPixels = size(data, 1);


%% find integration bands for each pixel

bg = data(:, 1:bgEnd, :);
% spettro di bg cumulato sui bin
bgIntOverBins = squeeze(sum(bg, 2));

bands = zeros(nPixels, nBandPts);

MIN_SPECTRUM_RATIO = 0.01;

for i = 1:nPixels

	% prendi quei valori dei bin dove hai almeno 1/100 del massimo dello
	% spettro (in pratica tagliane le code, dove e' troppo piccolo)
	spectrumNiceRange = find(bgIntOverBins(i, :) > MIN_SPECTRUM_RATIO * max(bgIntOverBins(i, :)));
	m = min(spectrumNiceRange);
	M = max(spectrumNiceRange);
	% generi le bande come vettori che hanno min med e max di bin
	bands(i, :) = round(linspace(m, M, nBandPts));

end


%% compute mean background energies

bg_E_Mean = compute_E(bg, 1, bands);


%% Prepare l,h (separated from fit GM models to cut off Coder problems

tomato_e = compute_e_logE(tomato, bg_E_Mean, bands);


%% fit GM models (NOT supported by Coder)

gmfit = cell(1, nPixels);

for i = 1:nPixels

	pxTomato_e = squeeze(tomato_e(i, :, :));
	% NOT supported by Coder
	gmfit{i} = fitgmdist(pxTomato_e, kSize, 'CovarianceType', 'full', 'SharedCovariance', true);

end


end % end of function
