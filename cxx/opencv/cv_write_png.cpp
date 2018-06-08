#include <vector>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void createAlphaMat(Mat &mat)
{
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            Vec4b& rgba = mat.at<Vec4b>(i, j);
            rgba[0] = UCHAR_MAX;
            rgba[1] = saturate_cast<uchar>((float (mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX);
            rgba[2] = saturate_cast<uchar>((float (mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX);
            rgba[3] = saturate_cast<uchar>(0.5 * (rgba[1] + rgba[2]));
        }
    }
}


void createPaletteMat(Mat & mat)
{
	for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {

            Vec3b& rgb = mat.at<Vec3b>(i, j);

            rgb[0] = saturate_cast<uchar>((float (mat.cols - j)) / ((float)mat.cols) * UCHAR_MAX);
            rgb[1] = saturate_cast<uchar>((float (mat.rows - i)) / ((float)mat.rows) * UCHAR_MAX);
            rgb[2] = saturate_cast<uchar>(0.5 * (rgb[1] + rgb[2]));

			const uchar r = cv::theRNG().uniform(0, UCHAR_MAX), g = cv::theRNG().uniform(0, UCHAR_MAX), b = cv::theRNG().uniform(0, UCHAR_MAX);
            mat.at<Vec3b>(i, j) = Vec3b(r, g, b);
        }
    }
}


int main(int argv, char **argc)
{
    // Create mat with alpha channel
    Mat mat4(480, 640, CV_8UC4);
    createAlphaMat(mat4);
 
	Mat3b mat3(480, 640);
	createPaletteMat(mat3);

    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(9);

    try {
        imwrite("alpha.png", mat4, compression_params);
        imwrite("palet.png", mat3, compression_params);
    }
    catch (runtime_error& ex) {
        fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
        return 1;
    }

    fprintf(stdout, "Saved PNG files.\n");
    return 0;
}
