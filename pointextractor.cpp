#include "pointextractor.h"

PointExtractor::PointExtractor()
{

}

std::vector<cv::Point> PointExtractor::extract_points(const cv::Mat &lined_image)
{
    cv::Mat laser_lines;
    cv::inRange(lined_image,
                cv::Scalar(0, 0, 255), cv::Scalar(0, 0, 255),
                laser_lines);
    std::vector<cv::Point> laser_points;
    cv::findNonZero(laser_lines, laser_points);
    return laser_points;
}
