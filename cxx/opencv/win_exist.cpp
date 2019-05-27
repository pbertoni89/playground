/*
 * https://docs.opencv.org/4.0.0/da/d0c/tutorial_bounding_rects_circles.html
 *
 * Bounding Rect: https://docs.opencv.org/4.0.0/d3/dc0/group__imgproc__shape.html#ga103fcbda2f540f3ef1c042d6a9b35ac7
 * Enclosing Circles: https://docs.opencv.org/4.0.0/d3/dc0/group__imgproc__shape.html#ga8ce13c24081bbc7151e9326f412190f1
 * Find Contours: https://docs.opencv.org/4.0.0/d3/dc0/group__imgproc__shape.html#gadf1ad6a0b82947fa1fe3c3d497f260e0
 *
 * g++ bounding_boxes.cpp -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <ostream>
#include <algorithm>
#include <cmath>


using namespace cv;

const std::string NN = "araberara";

int main(int argc, char** argv)
{
	auto exist_named_move = [] (const std::string & N, int x = 0, int y = 20)
	{
		const auto PROP = cv::getWindowProperty(N, cv::WindowPropertyFlags::WND_PROP_ASPECT_RATIO);
		std::cerr << N << " prop: " << PROP << "\n";
		if (PROP == -1.)
		{
			std::cerr << N << " doesn't exist\n";
			cv::namedWindow(N); cv::moveWindow(N, x, y);
		}
	};

	exist_named_move(NN);
	exist_named_move(NN);
	cv::Mat m = cv::imread("/media/xnext/DATA/data/castagne/excerpt.png");
	cv::imshow(NN, m);
	exist_named_move(NN);
	cv::waitKey();
	exist_named_move(NN);
	return 0;
}
