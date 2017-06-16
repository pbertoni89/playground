function [flagsIncomings, flagsOutgoings] = edge_detector_classify_registration(ed, params)

	N_PIXELS = 32;
	N_LINES = size(ed.AllDeltas, 1);
	firstPxAnalysis = (params.BOARD_ANALYSIS * N_PIXELS) + 1;
	lastPxAnalysis = (params.BOARD_ANALYSIS + 1) * N_PIXELS;

	sTitleSuffix = sprintf('t(%.1f, %.1f), f(%d, %d), b %d = [%d, %d]', ...
		params.THRESH_INC, params.THRESH_OUT, params.FLAGS_INC, params.FLAGS_OUT, params.BOARD_ANALYSIS, firstPxAnalysis, lastPxAnalysis);

	%% Matlab Algorithm
	flagsIncomings = sum(ed.AllDeltas < params.THRESH_INC, 2);
	flagsOutgoings = sum(ed.AllDeltas > params.THRESH_OUT, 2);

	signalIncoming = ed.Tcs(flagsIncomings >= params.FLAGS_INC);
	signalOutgoing = ed.Tcs(flagsOutgoings >= params.FLAGS_OUT);
	
	signals(signalIncoming) = +1;
	signals(signalOutgoing) = -1;
	signals = signals(signals~=0);

	errors = cumsum(signals);
	errors = errors(abs(errors)>1);

	signalsFiltered = inhibitSubsequentSignals(signals);

	nBricksIncoming = length(signalsFiltered==1);
	nBricksOutgoing = length(signalsFiltered==-1);

	%% Count Bricks
	if nBricksIncoming == nBricksOutgoing
		fprintf('Matlab Bricks detected: %d\n', nBricksIncoming);
	else
		fprintf('Matlab Bricks detected: %d ? %d', nBricksIncoming, nBricksOutgoing);
	end

	%% Matlab Flags
	sTitle = sprintf('Matlab Signals %s', sTitleSuffix);
	figure('name', sTitle);
	subplot(2, 1, 1)
	stem(signals, 'b')
	hold on
	stem(signalsFiltered, 'g')
	title(sTitle)
	subplot(2, 1, 2)
	plot(errors, 'r')
	title('Errors if not filtered')

	%% Matlab Flags
	sTitle = sprintf('Matlab Flags %s', sTitleSuffix);
	figure('name', sTitle);
	plot(flagsIncomings, 'r')
	hold on
	plot(flagsOutgoings, 'b')
	plot([1 N_LINES], [params.FLAGS_INC params.FLAGS_INC], '-.k');
	plot([1 N_LINES], [params.FLAGS_OUT params.FLAGS_OUT], ':k');
	legend('Incoming', 'Outgoing', 'minFlagsInc', 'minFlagsOut')
	title(sTitle)

	%% Overview Board MAs
	MasToPlot = ed.AllMas(:, firstPxAnalysis : lastPxAnalysis);

	sTitle = sprintf('Board Mas %s', sTitleSuffix);
	figure('name', sTitle)
	plot(MasToPlot)
	title(sTitle)

	%% Overview Board Deltas
	DeltasToPlot = ed.AllDeltas(:, (params.BOARD_ANALYSIS*N_PIXELS)+1 : ((params.BOARD_ANALYSIS+1)*N_PIXELS));

	sTitle = sprintf('Board Deltas %s', sTitleSuffix);
	figure('name', sTitle)
	plot(DeltasToPlot)
	hold on
	plot([1 N_LINES], [params.THRESH_INC params.THRESH_INC], 'k');
	plot([1 N_LINES], [params.THRESH_OUT params.THRESH_OUT], 'k');
	title(sTitle)

	%% Inspecting Board Delta Pixels
	sTitle = sprintf('Pixels board %s', sTitleSuffix);
	figure('name', sTitle)
	for p = 1:N_PIXELS
		subplot(4, 8, p)
		relPx = p-1;
		absPx = (params.BOARD_ANALYSIS-1)*N_PIXELS+(relPx);
		sSubTitle = sprintf('Px%d=%d', relPx, absPx);
		plot(DeltasToPlot(:, p))
		title(sSubTitle)
	end

	%% Cpp Flags and Signals
	sTitle = sprintf('CppSignals board %s', sTitleSuffix);
	figure('name', sTitle)
	plot(ed.CppFlags(:, 1), 'r')
	hold on
	plot(ed.CppFlags(:, 2), 'b')
	stem(ed.CppSignals(:, 1) .* ed.CppFlags(:, 1), 'r')
	stem(ed.CppSignals(:, 2) .* ed.CppFlags(:, 2), 'b')
	legend('fInc', 'fOut', 'bInc', 'bOut')
	title(sTitle)
	
	%% Experimental
	function [filtered] = inhibitSubsequentSignals(sig)
		filtered = sig;
		for i = 1:length(sig)-1
			j = i+1;
			while (j <= length(sig)) && (sig(j) ~= 0) && (sig(j) == sig(i))
				filtered(j) = 0;
				j = j+1;
			end
		end
	end

	%% Hist
	THRESH_OUTLIER = 1;
	ed.AllDeltas(abs(ed.AllDeltas) > THRESH_OUTLIER) = THRESH_OUTLIER;

	sTitle = sprintf('Histogram from %s', sTitleSuffix);
	figure('name', sTitle);
	%AllDeltasInARow = reshape(AllDeltas, 1, numel(AllDeltas));
	hist(ed.AllDeltas);
	title(sTitle)

end
