function [matAlarmedClose] = processPomiGMhl (data, gmfit, validPixIdx, bands, bgEnd, element, elementSize)
% Classify spectra of a new matrix according to a family of fitted GM models.
%
% INPUT ARGUMENTS 
%
% - mat:
%		nPixels*nColumns*nBins matrix containing one or more bricks to be classified
% - gmfit:
%		family of GM models, one for each pixel in the image, as result of trainPomiGMhl
% - validPixIdx:
%		vector index of pixels validated by trainPomiGMhl
% - bands:
%		vector of integration intervals for each pixel, as result of trainPomiGMhl
% - bgEnd:
%		background ending column (i.e. 1:bgEnd are background columns in mat)
% - element:
%		structure element or morphological closing (common choice is 'disk')
% - elementSize:
%		size of structure element
%
% OUTPUT
%
% - matAlarmedClose:
%		alarmed matrix after the operation of morphological closing


data = data(validPixIdx, :, :);

nPixels = size(data, 1);
nColumns = size(data, 2);


%% compute mean background energies

bg = data(:, 1:bgEnd, :);
bg_E = compute_E(bg, 1, bands);


%% create l,h

% tic;

data_e = compute_e_logE(data, bg_E, bands);


%% Compute distances (NOT supported by Coder)

mahalDistances = zeros(nPixels, nColumns);

for i = 1:nPixels

	for j = 1:nColumns

		ptCoords = squeeze(data_e(i, j, :))';		% 1x2

		% (nTestPts x nDims), (nTrainPts, nDims)		con
		% {   ?     x   2  }, {    1    ,   2  }
		% [   4     x   2  ], [  100    ,   2  ]        buoni
		% NOT supported by Coder
 		mahalDistances(i, j) = mahal(gmfit{i}, ptCoords);

	end
end


%% Classify pixel spectra

THRESH = sqrt(chi2inv(0.9, 2));
matAlarmed = zeros(nPixels, nColumns);

for i = 1:nPixels

	for j = 1:nColumns

		% if true <-> tomato; otherwise false <-> contaminant
		matAlarmed(i, j) = (mahalDistances(i, j) <= THRESH);

	end

end

% toc;


%% morphological closing

el = strel(element, elementSize);
matAlarmedClose = imclose(matAlarmed, el);

end % end of function
