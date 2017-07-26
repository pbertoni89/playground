function [ energies ] = computeEnergies(data, doMean, bands, nBands)
%computeEnergies compute (mean) energies

% Energie  e' sinonimo di L
energies = [];

nPix = size(data, 1);

for i = 1:nPix

    for j = 1:nBands

        energies(i, :, j) = sum(squeeze(data(i, :, bands(i, j) : bands(i, j+1))), 2);
    end

end

% media sulle colonne di L e di H
if doMean == 1
	energies = squeeze(mean(energies, 2));
end

end
