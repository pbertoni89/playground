clc
clear variables
close all

pardata = csvread('./parfor.csv', 1);

figure('name', 'Par vs Ser')
semilogy(pardata(:, 3), 'r')
hold on
semilogy(pardata(:, 4), 'b')
legend('par', 'ser')

