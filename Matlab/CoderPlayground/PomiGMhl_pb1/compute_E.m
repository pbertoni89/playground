function [ E ] = compute_E(data, doMean, bands)
%compute_E compute (mean) Energies (i.e. the L, H values)

	nPixels = size(data, 1);
	nColumns = size(data, 2);
	nBands = size(bands, 2) - 1;

	E = zeros(nPixels, nColumns, nBands);

	for i = 1:nPixels

		for j = 1:nBands

			E(i, :, j) = sum(squeeze(data(i, :, bands(i, j) : bands(i, j+1))), 2);
		end

	end

	% media sulle colonne di L e di H
	if doMean == 1
		E = squeeze(mean(E, 2));
	end

end
