#include "aishack.hpp"

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>


/**
 * https://github.com/opencv/opencv/blob/master/samples/cpp/em.cpp
 *
 * @param N_K = #clusters = #centroids -> is 1 for XSpectra
 */
void em_opencv_example(const int N_K = 4, const int N_BANDS = 2, const int N_SAMPLES = 10000)
{
	// sampleDist cted as a N_SAMPLES r x N_BANDS c x 1 ch
	cv::Mat sampleDist(N_SAMPLES, N_BANDS, CV_32FC1);
	cv::Mat labels;

	const cv::Size IMG_SIZE(500, 500);
	cv::Mat img = cv::Mat::zeros(IMG_SIZE, CV_8UC3);

	const int N_K_ROOT = (int) std::sqrt((float)N_K);
	const int N_SAMPLES_PER_K = N_SAMPLES/N_K;
	const int MEAN_QUARTER = img.rows/(N_K_ROOT+1);

	// now sampleDist is a N_SAMPLES r x 1 c x N_BANDS ch   - - - for quicker data entry
	sampleDist = sampleDist.reshape(N_BANDS, 0);

					std::cout << "Cntr\tMean\t\t\tSigma\t\tRows\tCols\n";

	// Generate the training sample gaussian distribution
	for (int c=0; c<N_K; c++)
	{
		// Process each centroid as a different sampleDist partition
		cv::Mat samples_part = sampleDist.rowRange(c * N_SAMPLES_PER_K, (c+1) * N_SAMPLES_PER_K);

		const cv::Scalar SC_MEAN(((c%N_K_ROOT)+1) * MEAN_QUARTER, ((c/N_K_ROOT)+1) * MEAN_QUARTER);
		const cv::Scalar SC_SIGMA(30 + c*2, 30 - c*2);

					std::cout << c << "\t" << SC_MEAN << "\t" << SC_SIGMA << "\t" << samples_part.rows << "\t" << samples_part.cols << "\n";
		// Fill the sampleDist partition with random gaussian values
		cv::randn(samples_part, SC_MEAN, SC_SIGMA);
	}
					// each Mat point is a tuple of N_BANDS elements (the channels)
					std::cout << "sampleDist 0 " << sampleDist.at<cv::Vec2d>(0, 0) << "\n";

	// now sampleDist is a N_SAMPLES r x N_BANDS c x 1 ch   - - - again
	sampleDist = sampleDist.reshape(1, 0);

					// Float because of CV_32FC1 type
					std::cout << "sampleDist 1 " << sampleDist.at<float>(0, 1) << "\n";

	// TRAINING: Cluster the data -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -
	cv::Ptr<cv::ml::EM> em_model = cv::ml::EM::create();

	em_model->setClustersNumber(N_K);
	em_model->setCovarianceMatrixType(cv::ml::EM::COV_MAT_GENERIC);
	em_model->setTermCriteria(cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 300, 0.1));

	em_model->trainEM(sampleDist, cv::noArray(), labels, cv::noArray());

					// XSpectra Trainer output
					std::cout << "Mean:\n" << em_model->getMeans() << "\n";
					std::vector<cv::Mat> vCovariances;
					em_model->getCovs(vCovariances);
					for (const auto & cov : vCovariances)
					{
						std::cout << "Cov\n" << cov << "\n";
					}

	// PROCESSING: Classify every image pixel -  -  -  -  -  -  -  -  -  -  -  -

	// Color labels
	const cv::Scalar colors[] =
	{
		cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0),
		cv::Scalar(0, 255, 255), cv::Scalar(255, 255, 0)
	};

	// sample each `img` point (from 0, 0 to 500, 500) and predict
	cv::Mat sample(1, N_BANDS, CV_32FC1);

	for (int r=0; r<img.rows; r++)
	{
		for (int c=0; c<img.cols; c++)
		{
			sample.at<float>(0) = (float)c;
			sample.at<float>(1) = (float)r;
			const int response = cvRound(em_model->predict2(sample, cv::noArray())[1]);
			cv::Scalar col = colors[response];

			cv::circle(img, cv::Point(c, r), 1, col*0.75, cv::FILLED);
		}
	}

	// Draw the clustered samples -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -

	for (int s=0; s<N_SAMPLES; s++)
	{
		const cv::Point pt(cvRound(sampleDist.at<float>(s, 0)), cvRound(sampleDist.at<float>(s, 1)));
		cv::circle(img, pt, 1, colors[labels.at<int>(s)], cv::FILLED);
	}

	cv::namedWindow("EM", cv::WINDOW_NORMAL);
	cv::imshow("EM", img);
	cv::waitKey(0);
}


int main(int argc, char* argv[])
{
	run_gaussian_1d();

	em_opencv_example();

	cv::destroyAllWindows();
	return 0;
}
