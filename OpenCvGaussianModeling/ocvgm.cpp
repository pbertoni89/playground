#include "aishack.hpp"

#include <opencv2/opencv.hpp>

/**
 * https://github.com/opencv/opencv/blob/master/samples/cpp/em.cpp
 *
 * @param N_CENTROIDS - is 1 for XSpectra
 */
void em_opencv_example(const int N_CNTR = 4, const int N_BANDS = 2, const int N_SAMPLES = 10000)
{
	const int N_CNTR_ROOT = (int) std::sqrt((double)N_CNTR);

	const cv::Scalar colors[] =
	{
		cv::Scalar(0, 0, 255), cv::Scalar(0, 255, 0),
		cv::Scalar(0, 255, 255), cv::Scalar(255, 255, 0)
	};

	const int SAMPLES_PER_CENTROID = N_SAMPLES/N_CNTR;

	// sampleDist cted as a N_SAMPLES r x N_BANDS c x 1 ch
	cv::Mat sampleDist(N_SAMPLES, N_BANDS, CV_32FC1);
	cv::Mat labels;
	cv::Mat img = cv::Mat::zeros(cv::Size(500, 500), CV_8UC3);
	cv::Mat sample(1, N_BANDS, CV_32FC1);

	const auto MEAN_QUARTER = img.rows/(N_CNTR_ROOT+1);

	// now sampleDist is a N_SAMPLES r x 1 c x N_BANDS ch   - - - for quicker data entry
	sampleDist = sampleDist.reshape(N_BANDS, 0);

	// Generate the training sampleDist and print its statistic features
	std::cout << "Cntr\tMean\t\tSigma\n";

	for(int c=0; c<N_CNTR; c++)
	{
		// Process each centroid as a different sampleDist partition
		cv::Mat samples_part = sampleDist.rowRange(c * SAMPLES_PER_CENTROID, (c+1) * SAMPLES_PER_CENTROID);

		const cv::Scalar SC_MEAN(((c%N_CNTR_ROOT)+1) * MEAN_QUARTER, ((c/N_CNTR_ROOT)+1) * MEAN_QUARTER);
		const cv::Scalar SC_SIGMA(30 + c*2, 30 - c*2);

		std::cout << c << "\t" << SC_MEAN << "\t" << SC_SIGMA << "\n";
		// Fill the sampleDist partition with random gaussian values
		cv::randn(samples_part, SC_MEAN, SC_SIGMA);
	}
	// now sampleDist is a N_SAMPLES r x N_BANDS c x 1 ch   - - - again
	sampleDist = sampleDist.reshape(1, 0);

	// TRAINING: Cluster the data
	cv::Ptr<cv::ml::EM> em_model = cv::ml::EM::create();

	em_model->setClustersNumber(N_CNTR);
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

	// PROCESSING: Classify every image pixel
	for(int r=0; r<img.rows; r++)
	{
		for(int c=0; c<img.cols; c++)
		{
			sample.at<float>(0) = (float)c;
			sample.at<float>(1) = (float)r;
			int response = cvRound(em_model->predict2(sample, cv::noArray())[1]);
			cv::Scalar col = colors[response];

			circle(img, cv::Point(c, r), 1, col*0.75, cv::FILLED);
		}
	}

	// Draw the clustered samples
	for(int s=0; s<N_SAMPLES; s++)
	{
		cv::Point pt(cvRound(sampleDist.at<float>(s, 0)), cvRound(sampleDist.at<float>(s, 1)));
		circle(img, pt, 1, colors[labels.at<int>(s)], cv::FILLED);
	}

	imshow( "EM-clustering result", img);
	cv::waitKey(0);
}


int main(int argc, char* argv[])
{
	run_gaussian_1d();

	em_opencv_example();

	return 0;
}
