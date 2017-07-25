%% https://it.mathworks.com/help/coder/examples/edge-detection-on-images.html

im = imread('hello.jpg');

subplot(1, 2, 1)
image(im);

gray = (0.2989 * double(im(:,:,1)) + 0.5870 * double(im(:,:,2)) + 0.1140 * double(im(:,:,3)))/255;

%% Generate Mex file

codegen sobel

edgeIm = sobel_mex(gray, 0.7);

im3 = repmat(edgeIm, [1 1 3]);

subplot(1, 2, 2)
image(im3);

%% Generate Standalone C code

codegen -config coder.config('lib') sobel