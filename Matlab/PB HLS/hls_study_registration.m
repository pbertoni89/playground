function [ output_args ] = hls_study_registration(dirpath)
%hls_study_registration Summary of this function goes here
%   Detailed explanation goes here

	N_PIXELS = 128;
	N_VALUES_PER_PIXEL = 4;
	index_pixels = 1:N_VALUES_PER_PIXEL:N_PIXELS*N_VALUES_PER_PIXEL;
	fileName = sprintf('%s\\hl_0_classification.csv', dirpath);

	% TC, 0_l,0_h,0_mod,0_lbl, 1_l,1_h,1_mod,1_lbl, ..., 127_l,127_h,127_mod,127_lbl
	index_tc = 1;
	index_sync = 2;
	index_l = index_sync + index_pixels;
	index_h = index_l + 1;
	index_mod = index_h + 1;
	index_lbl = index_mod + 1;
	CsvContent = csvread(fileName, 1);
	v_tcs = CsvContent(:, index_tc);
	v_syncs = CsvContent(:, index_sync);
	v_coords_l = CsvContent(:, index_l);
	v_coords_h = CsvContent(:, index_h);
	v_mods = CsvContent(:, index_mod);
	v_lbls = CsvContent(:, index_lbl);
	
	N_COLUMNS = size(v_coords_l, 1);
	N_POINTS_PER_VAL = numel(v_coords_l);
	
	figure('name', 'hl coordinates')
	scatter(reshape(v_coords_l, 1, N_POINTS_PER_VAL), reshape(v_coords_h, 1, N_POINTS_PER_VAL))
	

	%% outliers
	OUTLIER_COORDINATE_VALUE = 2;
	outliersPos = find(CsvContent > OUTLIER_COORDINATE_VALUE);
	outliers = CsvContent(CsvContent > OUTLIER_COORDINATE_VALUE);
	
	%% Modulus

	figure('name', 'hl modulus')
	subplot(2, 1, 1)
	hist(v_mods)

	
end
