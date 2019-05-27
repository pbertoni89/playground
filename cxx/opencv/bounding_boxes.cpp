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

using t_vPt = std::vector<Point>;
using t_vvPt = std::vector<t_vPt>;

Mat mat_src;
int gaussianBlur = 5, threshMin = 20, threshMax = 180, areaMin = 4000, areaMax = 10000, distMin = 50, boundingSize = 10;


Scalar rnd_col()
{
	auto unif = [] () ->double
	{
		static RNG rng(12345);
		return rng.uniform(0, 256);
	};
	return {unif(), unif(), unif()};
}


class Object
{
public:
	/*const*/ int m_idx;
	/*const*/ cv::Scalar m_col;
	t_vPt m_polygon;
	cv::Rect m_boundRect;
	cv::Point2f m_circleCenter;
	float m_circleRadius;
	float m_areaRect, m_areaCircle;

	Object(int contour_idx, const t_vPt & contour) :
		m_idx(contour_idx),
		m_col(rnd_col())
	{
		cv::approxPolyDP(contour, m_polygon, 3, true);

		// minimal up-right bounding rectangle for the specified point set or non-zero pixels of gray-scale image
		m_boundRect = cv::boundingRect(m_polygon);
		m_boundRect += cv::Size(boundingSize, boundingSize);
		m_boundRect -= cv::Point(boundingSize, boundingSize) / 2;

		// find a circle of the minimum area enclosing a 2D point set
		cv::minEnclosingCircle(m_polygon, m_circleCenter, m_circleRadius);

		////////////// m_circleRadius += (boundingSize / 2);
		m_areaRect = m_boundRect.area();
		m_areaCircle = M_PI * std::pow(m_circleRadius, 2);
		std::cout << m_idx << " radius " << m_circleRadius << " -> " << m_areaCircle << std::endl;
	}

	float area() const
	{
		return m_areaCircle;
	}

	void draw(cv::Mat & m) const
	{
		// std::cout << "drawing " << m_idx << " with " << m_col << std::endl;
		cv::rectangle(m, m_boundRect.tl(), m_boundRect.br(), m_col, 2);
		cv::circle(m, m_circleCenter, m_circleRadius, m_col, 2);
		cv::putText(m, std::to_string(m_idx), m_boundRect.tl(), cv::FONT_HERSHEY_TRIPLEX, 1, m_col);
	}

	// USELESS
	void draw_contour(cv::Mat & m, const t_vvPt & contours) const
	{
		cv::drawContours(m, contours, m_idx, m_col);
	}

	bool too_near(const Object & OTH) const
	{
		const auto mag = cv::norm(m_circleCenter -  OTH.m_circleCenter);
		const bool TOO_NEAR = (mag > 0 and mag < distMin);
		std::cout << "\t" << *this << "\n\t" << OTH << "\n\t\t" << (TOO_NEAR ? "TOO" : "not") << " near (" << mag << ")" << std::endl;
		return TOO_NEAR;
	}

	bool away(const Object & OTH) const
	{
		return not too_near(OTH);
	}

	bool area_feasible() const
	{
		return area() >= areaMin and area() <= areaMax;
	}

	bool contains(const Object & OTH) const
	{
		return m_boundRect.contains(OTH.m_circleCenter);
	}

	bool operator == (const Object & OTH) const
	{
		return m_idx == OTH.m_idx;
	}

	bool operator != (const Object & OTH) const
	{
		return not (*this == OTH);
	}

	bool operator < (const Object & OTH) const
	{
		return area() < OTH.area();
	}

	friend std::ostream & operator << (std::ostream & o, const Object & O)
	{
		o << "obj " << O.m_idx << "\tcenter " << O.m_circleCenter << "\tarea " << size_t(O.area());
	}
};


void hist(cv::Mat & m)
{
	m.convertTo(m, CV_16UC1);
	/// Separate the image in 3 places ( B, G and R )

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 } ;
	const float * histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat mat_hist;

	/// Compute the histograms:
	calcHist(& m, 1, 0, Mat(), mat_hist, 1, &histSize, &histRange, uniform, accumulate);

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound( (double) hist_w/histSize );

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(mat_hist, mat_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

	/// Draw for each channel
	for( int i = 1; i < histSize; i++ )
	{
		line(histImage,
			Point( bin_w * (i-1),
					hist_h - cvRound(mat_hist.at<float>(i - 1))),
			Point( bin_w * (i),
					hist_h - cvRound(mat_hist.at<float>(i))),
			Scalar( 255, 0, 0), 2, 8, 0  );
	}

	imshow("calcHist Demo", histImage);
}


void callback_ed(int, void *)
{
	system("clear");
	std::cout << "~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  ~  \n";
	Mat mat_wip;
	cvtColor(mat_src, mat_wip, COLOR_BGR2GRAY);

	threshMax = std::max(threshMin + 1, threshMax);

#ifdef OTSU
	cv::threshold(mat_wip, mat_wip, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	// try to cut out damned dead pixels (dilation inverses erosion)
	cv::dilate(mat_wip, mat_wip, cv::Mat(), {-1, -1}, 1);
#endif
	cv::blur(mat_wip, mat_wip, {gaussianBlur, gaussianBlur});

	Mat canny_output;
	cv::Canny(mat_wip, canny_output, threshMin, threshMax);

	cv::imshow("Canny", canny_output);

	// each element is a countour i.e. a vector of Points
	t_vvPt contours, contours_poly;

	// retrieval mode was RETR_TREE: https://docs.opencv.org/4.0.0/d3/dc0/group__imgproc__shape.html#ga819779b9857cc2f8601e6526a3a5bc71
	cv::findContours(canny_output, contours, RETR_EXTERNAL, CHAIN_APPROX_TC89_L1);

	const size_t N_OBJS_TOT = contours.size();
	std::vector<Object> v;

	auto print_objs = [&v] (const std::string & LBL)
	{
		std::cout << "Objects " << LBL << " ~   ~   ~   ~   ~   ~   ~  \n";
		for (const auto & OB : v)
		{
			std::cout << "\t" << OB << "\n";
		}
		std::cout << "   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~\n";
	};

	for (size_t i=0; i<N_OBJS_TOT; i++)
	{
		v.emplace_back(i, contours[i]);
		// contours_poly.emplace_back(v.back().m_polygon);  // quite useless
	}


	// custom sort: area DESCENDING
	std::sort(v.rbegin(), v.rend());
	print_objs("sorted");



#ifdef HIST_AREAS
	cv::Mat_<float> mat_areas(N_OBJS_TOT, 1);
	for (size_t i=0; i<N_OBJS_TOT; i++)
	{
		mat_areas(i, 0) = v[i].area();
	}
	hist(mat_areas);
#endif



	// Filter by area
	auto it_area = std::remove_if(v.begin(), v.end(), [] (const Object & o)
								{
									return not o.area_feasible();
								});
	v.erase(it_area, v.end());
	print_objs("filter by area");



	// Filter by distance: first, sort on circleCenter polar coords
#if 0
	std::sort(v.begin(), v.end(), [] (const Object & o1, const Object & o2)
								{

								});
	auto it_unique = std::unique(v.begin(), v.end(), [] (const Object & o1, const Object & o2)
								{
									return o1.too_near(o2);
								});
	v.erase(it_unique, v.end());
#else
	std::vector<Object> v_uniq;
	auto is_new = [&v_uniq] (const Object & OB)
		{
			auto it = std::find(std::begin(v_uniq), std::end(v_uniq), OB);
			return it == std::end(v_uniq);
		};
	for (const auto & OB_NEW : v)
	{
		bool acceptable = true;
		for (const auto & OB_OK : v_uniq)
		{
			if (OB_NEW == OB_OK or OB_NEW.too_near(OB_OK))
			{
				acceptable = false;
			}
		}
		if (acceptable)
		{
			v_uniq.emplace_back(OB_NEW);
		}
		std::cout << (acceptable ? "accepting " : "rejecting ") << OB_NEW << std::endl;
	}
	v = v_uniq;
#endif
	print_objs("filter by distance");



	std::cout << "Canny thresholds: Min " << threshMin << ", Max " << threshMax << " | Area: Min " << areaMin << ", Max " << areaMax
		<< " -> Objects " << N_OBJS_TOT << ", retained " << v.size() << "\n\n\n" << std::endl;

	// Finally, draw boxes and circles
	auto draw_all = [&] (cv::Mat & m, const std::string & lbl)
	{
		for (const auto & OB : v)
		{
			OB.draw(m);
			// OBJ.draw_contour(m, contours_poly);
		}
		cv::imshow(lbl, m);
	};

	cv::Mat mat_draw_black = Mat::zeros(canny_output.size(), CV_8UC3);
	cv::Mat mat_draw_gray;
	cv::cvtColor(mat_wip, mat_draw_gray, cv::COLOR_GRAY2RGB);

	// draw_all(mat_draw_black, "Contours/Black");
	draw_all(mat_draw_gray, "Object Detection");
}


int main(int argc, char** argv)
{
	CommandLineParser parser(argc, argv, "{@input | ../data/stuff.jpg | input image}");
	mat_src = imread(parser.get<String>("@input"));
	if (mat_src.empty())
	{
 		std::cerr << "could not open or find the image!\n\nusage: " << argv[0] << " <Input image>\n";
		return -1;
	}

	const char * src_win = "Source";
	cv::namedWindow(src_win);
	imshow(src_win, mat_src);

	const int THRESH_MAX_MAX = 255, AREA_MAX_MAX = 50000, DIST_MIN_MAX = 600, GAUSS_BLUR_MAX = 20, BOUNDING_SIZE_MAX = 500;
	createTrackbar("gaussianBlur:", src_win, &gaussianBlur, GAUSS_BLUR_MAX, callback_ed);
	createTrackbar("threshMin:", src_win, &threshMin, THRESH_MAX_MAX, callback_ed);
	createTrackbar("threshMax:", src_win, &threshMax, THRESH_MAX_MAX, callback_ed);
	createTrackbar("areaMin:", src_win, &areaMin, AREA_MAX_MAX, callback_ed);
	createTrackbar("areaMax:", src_win, &areaMax, AREA_MAX_MAX, callback_ed);
	createTrackbar("distMin:", src_win, &distMin, DIST_MIN_MAX, callback_ed);
	createTrackbar("boundingSize:", src_win, &boundingSize, BOUNDING_SIZE_MAX, callback_ed);

	// Explicit one-time call of callback_ed is necessary to display the "Contours" window simultaneously with the "Source" window
	callback_ed(0, nullptr);
	waitKey();
	return 0;

	// baa baa
}
