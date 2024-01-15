%% Troubleshoot Variables in parfor-Loops
A = zeros(4, 10);
tic
parfor i = 1:4
    v = zeros(1, 10);
    for j = 1:10
        v(j) = i + j;
    end
    %disp(v(1))
    A(i, :) = v;
end

disp(A)
toc


%% https://it.mathworks.com/help/matlab/matlab_prog/vectorization.html

x = (-2:0.2:2)'; % 21-by-1
y = -1.5:0.2:1.5; % 1-by-16
F = x.*exp(-x.^2-y.^2); % 21-by-16
surf(F)

%% Help me

Nmin = 2; Nmax = 5; Nspan = Nmax - Nmin + 1;
Ns = logspace(Nmin, Nmax, Nspan);
Perfs = zeros(2, length(Ns));

parfor iN = 1:length(Ns)
	x = 1:Ns(iN);
	fprintf('Spawned N %d: \n', Ns(iN))
	tic
	ylength = (length(x) - mod(length(x),5))/5;
	y(1:ylength) = 0;
	for n= 5:5:length(x)
		y(n/5) = sum(x(1:n));
	end
	t1 = toc;

	tic
	xsums = cumsum(x);
	y = xsums(5:5:length(x)); 
	t2 = toc;

	ts = [t1; t2];
	%Perfs(:, iN) = ts;
	fprintf('N %d: %f vs %f seconds\n', Ns(iN), t1, t2)
end
