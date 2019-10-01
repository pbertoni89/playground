
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


int main(int argc, char**argv)
{

    cv::Mat image = cv::imread("/home/xnext/Pictures/alan.jpg"); 

    cv::imshow("Image",image);
    std::cout << "3" << std::endl;
    cv::waitKey(0);

    std::ifstream ifs("/home/xnext/dead-pixels.csv");
    int px;
    std::set<int> PXS;
    while (ifs >> px)
        PXS.insert(px);
    std::cout << "dead pixels:";
    for (const auto & PX : PXS) std::cout << "\t" << PX;
    std::cout << "\n\n";

    // create dead pixels
    for (auto px : PXS)
    {
        const cv::Range RNG_ALL(cv::Range::all()), RNG_ABOVE(px - 1, px),
                        RNG_DEAD(px, px + 1), RNG_BELOW(px + 1, px + 2);
        cv::Mat speAbove = image(RNG_ABOVE, RNG_ALL), speDead = image(RNG_DEAD, RNG_ALL),
                speBelow = image(RNG_BELOW, RNG_ALL);
        speDead = 0;
    }

    cv::imshow("Image Dead",image);
    std::cout << "4" << std::endl;
    cv::waitKey(0);

    // interpolate dead pixels
    for (auto px : PXS)
    {
        int px_above = px - 1, px_below = px + 1;
        while (PXS.find(px_above) != PXS.end() and px_above > 0) px_above --; 
        while (PXS.find(px_below) != PXS.end() and px_below < 255) px_below ++;
        std::cout << "interpolate " << px << " with " << px_above << ", " << px_below << "\n";

        const cv::Range RNG_ALL(cv::Range::all()), RNG_ABOVE(px_above, px_above + 1),
                        RNG_DEAD(px, px + 1), RNG_BELOW(px_below, px_below + 1);
        cv::Mat speAbove = image(RNG_ABOVE, RNG_ALL), speDead = image(RNG_DEAD, RNG_ALL),
                speBelow = image(RNG_BELOW, RNG_ALL);
        speDead = (speAbove + speBelow) / 2;
    }
    
    cv::imshow("Image Interp",image);
    std::cout << "5" << std::endl;
    cv::waitKey(0);

    return 0;
}
