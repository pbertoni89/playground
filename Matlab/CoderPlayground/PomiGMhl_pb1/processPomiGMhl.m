function [matAlarmedClose] = processPomiGMhl (testData, gmfit, validPixIdx, bands, bgEnd, element, elementSize)
% Classify spectra of a new matrix according to a family of fitted GM models.
%
% INPUT ARGUMENTS 
%
% mat: 256*nColumn*128 matrix containing one or more Briks to be classified
% gmfit: family of GM models, one for each pixel in the image, as result of trainPomiGMhl
% validPixIdx: vector index of pixels validated by trainPomiGMhl
% bands: vector of integration intervals for each pixel, as result of trainPomiGMhl
% bgEnd: background ending column (i.e. 1:bgEnd are background columns in mat)
% element: structure element or morphological closing (common choice is 'disk')
% elementSize: size of structure element
%
% OUTPUT
%
% matAlarmedClose: alarmed matrix after the operation of morphological closing


testData = testData(105:245,:,1:80);
testData = testData(validPixIdx,:,:);

nPix = size(testData,1);
nBands = size(bands,2)-1;


%% compute matrix energies

matEnergies = [];

for i = 1:nPix
    
	for j = 1:nBands

		matEnergies(i,:,j) = sum(squeeze(testData(i,:,bands(i,j):bands(i,j+1))),2);

	end
    
end


%% compute mean background energies

bg = testData(:, 1:bgEnd, :);
bgEnergies = [];

for i = 1:nPix

	for j = 1:nBands

		bgEnergies(i,:,j) = sum(squeeze(bg(i,:,bands(i,j):bands(i,j+1))),2);

	end

end

bgEnergies = squeeze(mean(bgEnergies,2));


%% classify pixel spectra

% tic;

threshold = sqrt(chi2inv(0.9,2));

matAlarmed = zeros(nPix,size(testData,2));

for i = 1:nPix
    
	for j = 1:size(testData,2)

		el = sum(squeeze(testData(i,j,bands(i,1):bands(i,2))));
		el = -log(el/bgEnergies(i,1));
		eh = sum(squeeze(testData(i,j,bands(i,2):bands(i,3))));
		eh = -log(eh/bgEnergies(i,2));

		% NOT supported by Coder
		mahalDist = mahal(gmfit{i},[el,eh]);

		if mahalDist > threshold

			matAlarmed(i,j) = 0;		% contaminant

		else

			matAlarmed(i,j) = 1;		% tomato

		end

	end

end

% toc;


%% morphological closing

el = strel(element,elementSize);
matAlarmedClose = imclose(matAlarmed,el);

end
