#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;

/// Global variables

Mat src, src_gray;
Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
char* window_name = "Edge Map";

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  // Reduce noise with a kernel 3x3
  blur(src_gray, detected_edges, Size(3,3) );

  // Canny detector
  Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);

  // Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow(window_name, dst);
 }

void LaplacianThreshold(int, void*)
{
  Mat src, src_gray, dst;
  int kernel_size = 3;
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;
  char* window_name = "Laplace Demo";

  int c;

  // Load an image
  src = imread( argv[1] );

  if( !src.data )
    { return -1; }

  // Remove noise by blurring with a Gaussian filter
  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Create window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Apply Laplace function
  Mat abs_dst;

  Laplacian( src_gray, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( dst, abs_dst );

  /// Show what you got
  imshow( window_name, abs_dst );

  waitKey(0);
}


/** @function main */
int main( int argc, char** argv )
{
  // Load an image
  src = imread( argv[1] );

  if( !src.data )
  {
    std::cerr << "No image from " << argv[1] << "\n";
    return -1;
  }

  // Create a matrix of the same type and size as src (for dst)
  dst.create(src.size(), src.type());

  // Convert the image to grayscale
  cvtColor(src, src_gray, CV_BGR2GRAY);

  // Create a window
  namedWindow(window_name, CV_WINDOW_AUTOSIZE);

  // Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

  // Show the image
  CannyThreshold(0, 0);

  // Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
  }
