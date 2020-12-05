#ifndef POINTEXTRACTOR_H
#define POINTEXTRACTOR_H

#include <opencv2/opencv.hpp>

class PointExtractor
{
public:
    PointExtractor();
    std::vector<cv::Point> extract_points(const cv::Mat &lined_image);
};

#endif // POINTEXTRACTOR_H
