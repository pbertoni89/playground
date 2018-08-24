/*
 * https://docs.opencv.org/master/d8/d01/tutorial_discrete_fourier_transform.html
 */
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;


static void help(void)
{
    std::cout
        <<  "\nThis program demonstrated the use of the discrete Fourier transform (DFT). "
        <<  "\nThe dft of an image is taken and it's power spectrum is displayed."
        <<  "\nUsage:"
        <<  "\n./discrete_fourier_transform [image_name -- default ../data/lena.jpg]" << std::endl;
}


int main(int argc, char ** argv)
{
	help();
	const char * filename = argc >=2 ? argv[1] : "../skyrot.png";
	Mat I = imread(filename, IMREAD_GRAYSCALE);
	if ( I.empty())
	{
		std::cout << "Error opening image" << std::endl;
		return -1;
	}

	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(I.rows);
	int n = getOptimalDFTSize(I.cols);

	// on the border add zero values
	copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	dft(complexI, complexI);            // this way the result may fit in the source matrix

	// compute the magnitude and switch to logarithmic scale
	// => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))

	Mat ph;
	phase(planes[0], planes[1], ph, false);

	Mat magI;
	magnitude(planes[0], planes[1], magI);
	magI += Scalar::all(1);                    // switch to logarithmic scale
	log(magI, magI);

	// crop the spectrum, if it has an odd number of rows or columns
	const int r_crop = magI.rows & -2, c_crop = magI.cols & -2;
	std::cout << "Image is " << I.rows << " x " << I.cols << "\noptimal dft size is " << m << " x " << n << "\ncrop size is " << r_crop << " x " << c_crop << std::endl;
	magI = magI(Rect(0, 0, c_crop, r_crop));

	// Transform the matrix with float values into a viewable image form (float between values 0 and 1)
	normalize(magI, magI, 0, 1, NORM_MINMAX);

	Mat magnNoRearranged = magI.clone();

	// rearrange the quadrants of Fourier image  so that the origin is at the image center
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;
	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);

	// Show the result
	imshow("Input Image"       , I   );
	imshow("spectrum magnitude", magI);
	imshow("spectrum phase", ph);
	imshow("spectrum magnitude (no rearrange)", magnNoRearranged);

	waitKey();
	return 0;
}