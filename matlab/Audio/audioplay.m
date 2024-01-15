% https://it.mathworks.com/help/matlab/ref/audiowrite.html

load handel.mat

filename = 'handel.wav';

[yh, Fs] = audioread(filename);

%sound(yh, Fs);

nsamples = length(yh);

x = linspace(0, 100, nsamples);
ys = sin(x)';

ymod = yh .* ys;

sound(ys, Fs);
sound(ymod, Fs)

Yh = fft(yh);
Ys = fft(ys);
Ymod = fft(ymod);

close all
figure
subplot(2, 1, 1)
plot([yh ys ymod]);
subplot(2, 1, 2)
plot([abs(Yh) abs(Ymod)])