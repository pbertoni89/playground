function [ e ] = compute_e_logE ( data, bgE, bands )
%compute_e_logE compute log energies (i.e. the l, h values)

	nPixels = size(data, 1);
	nColumns = size(data, 2);
	nBands = size(bands, 2) - 1;

	e = zeros(nPixels, nColumns, nBands);

	for i = 1:nPixels

		for j = 1:nColumns

			for k = 1:nBands

				E = sum(squeeze(data(i, j, bands(i, k) : bands(i, k+1))));
				LRatio = E / bgE(i, k);
				e(i, j, k) = - log(LRatio);

			end
		end

	end

	%% Another way would be

	% senza media ovviamente
	% E = compute_E(data, 0, bands);
	% e = cell(1, nPixels);
	%
	% for i = 1:nPixels
	%
	% 	l = - log(tomatoEnergies(i, :, 1) / bgEnergiesMean(i, 1));
	% 	h = - log(tomatoEnergies(i, :, 2) / bgEnergiesMean(i, 2));
	% 	e{i} = [l ; h]';
	%
	% end

end

