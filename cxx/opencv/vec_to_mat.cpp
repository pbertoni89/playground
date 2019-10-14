/*
 * https://stackoverflow.com/questions/36722044/how-to-initialize-a-cvmat-using-a-vector-of-floats
 */

#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <iostream>


using namespace cv;
using namespace std;


#define USE_FLOAT
// #define PLOT


int main(int argc, char* argv[])
{
#ifdef USE_FLOAT
	#define type
	vector<float> vec{
		0.1, 1.9, 2.2,
		3.8, 4.3, 5.7,
		7.4, 6.6, 8.5};
#else
	vector<unsigned char> vec{
		1, 9, 2,
		8, 3, 7,
		4, 6, 5};
#endif

	Mat m1( vec );
#ifdef PLOT
	imshow("m1",m1);
	waitKey();
#endif
	Mat m2( 1,vec.size(), CV_32FC1,vec.data());
#ifdef PLOT
	imshow("m2",m2);
	waitKey();
#endif
	Mat1f m3( vec.size(), 1, vec.data());
#ifdef PLOT
	imshow("m3",m3);
	waitKey();
#endif
	Mat1f m4( 1, vec.size(), vec.data());
#ifdef PLOT
	imshow("m4",m4);
	waitKey();
#endif

	cout << "m1 " << m1.size << "\n" << m1 << "\n\n";
	// reshape with same (0) number of channels, and 3 rows. hence cols = numel / rows
	auto m = m1.reshape(0, 3);
	cout << "m1rs " << m.size << "\n" << m << "\n\n";

	cout << "as seen below all Mat and vector use same data\n";
	cout << vec[0] << endl;
	m1 *= 2;
	cout << vec[0] << endl;
	m2 *= 2;
	cout << vec[0] << endl;
	m3 *= 2;
	cout << vec[0] << endl;
	m4 *= 2;
	cout << vec[0] << endl;

	// of course yes
	cout << "does m shares changes? yes ofc\n" << m << "\n\n";

	// convert a mat to another. this clones the data !
	cv::Mat mf;
	m.convertTo(mf, CV_64FC1);

	mf *= 2;
	cout << "now mf " << mf << " while vec[0] " << vec[0] << endl;

	// now convert to itself: data is not shared anymore !
	m.convertTo(m, CV_32SC1);

	m *= 2;
	cout << "now m " << m << " while vec[0] " << vec[0] << endl;


	Mat mat16 = (Mat_<uint16_t>(3, 3) << 1, 3, 7, 2, 4, 256, 0, 65000, 70000);

	cout << "mat16 is " << mat16 << endl;

	Mat mat8;
	mat16.convertTo(mat8, CV_8UC1);
	cout << "mat8 is " << mat8 << endl;

	const auto mat8above = mat8 > 3;
	const auto nAbove = cv::countNonZero(mat8above);
	cout << "mat8above: " << nAbove << ", " << mat8above << endl;
	
	return 0;
}
