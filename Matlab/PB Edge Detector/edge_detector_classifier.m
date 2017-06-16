close all
clc
clear variables

% new csvs with C++ signals
cd 'D:\logging\2017_05_16__14_35_42'

N_PIXELS = 32;
N_BOARDS = 6;
params.THRESH_INC = -0.1;
params.THRESH_OUT = 0.1;
params.FLAGS_INC = 20;
params.FLAGS_OUT = 20;
% la quarta
params.BOARD_ANALYSIS = 3;

for board = 0:N_BOARDS-1

	boardPxFirst = (board * N_PIXELS) + 1;
	boardPxLast = (board + 1) * N_PIXELS;

	fileName = sprintf('log_edge_detector_%d.csv', board);
	% First row is a header; second row is a bad outlier
	CsvContent = csvread(fileName, 2);

	if board == params.BOARD_ANALYSIS
		% writing once... no problem, is synced
		ed.Tcs = CsvContent(:, 1);
		%ed.CppFlags(:, (board*N_INC_OUT)+1 : (board+1)*N_INC_OUT) = CsvContent(:, end-3:end-2);
		ed.CppFlags = CsvContent(:, end-3 : end-2);
		%ed.CppSignals(:, (board*N_INC_OUT)+1 : (board+1)*N_INC_OUT) = CsvContent(:, end-1:end);
		ed.CppSignals = CsvContent(:, end-1 : end);
	end

	currMas = CsvContent(:, 2 : N_PIXELS+1);
	currDeltas = CsvContent(:, N_PIXELS+2 : end-4);

	ed.AllMas(:, boardPxFirst : boardPxLast) = currMas;
	ed.AllDeltas(:, boardPxFirst : boardPxLast) = currDeltas; 
end

edge_detector_classify_registration(ed, params);

% Free OS handle to directory
cd '../'