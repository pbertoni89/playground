function [ f ] = pbfib(n)
%PBFIB Summary of this function goes here
%   Detailed explanation goes here

	if (n==1)
		f= 1;
	elseif (n == 2)
		f = 2;
	else
		f = pbfib(n-1) + pbfib(n-2);   
	end
end
