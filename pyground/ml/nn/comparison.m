close all
clear variables
clc

m_src = readMATfromDAT('./raw_rep7.bin', 128, 128);
% m_den = readMATfromDAT('./out_rescale.bin', 128, 128);
m_den = readMATfromDAT('./out_marcore.bin', 128, 128);
% m_den = readMATfromDAT('./out_no_rescale.bin', 128, 128);

img_src = getIntegration(m_src);
img_den = getIntegration(m_den);

figure('name', 'Cube')
subplot(2, 1, 1)
imagesc(img_src)
colorbar()
subplot(2, 1, 2)
imagesc(img_den)
colorbar()

n_col = size(img_src, 2);
n_col_rnd = 25;
v_col_rnd = floor(rand(n_col_rnd, 1) * n_col);

i_col = 100;
figure('name', 'Column')
subplot(2, 1, 1)
col_src = squeeze(m_src(:, i_col, :));
col_den = squeeze(m_den(:, i_col, :));
imagesc(col_src)
subplot(2, 1, 2)
imagesc(col_den)

figure('name', 'Spe')
subplot(2, 1, 1)
plot(col_src')
subplot(2, 1, 2)
plot(col_den')

figure('name', 'Columns')
col_rnd_den = squeeze(m_den(:, v_col_rnd, :));
for i = 1 : n_col_rnd
    subplot(5, 5, i)
    tmp_col_den = squeeze(col_rnd_den(:, i, :));
    imagesc(tmp_col_den)
    title(v_col_rnd(i))
end
