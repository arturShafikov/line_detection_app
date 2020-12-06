#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <QElapsedTimer>
#include <opencv2/opencv.hpp>

class LineDetector
{
public:
    LineDetector();
    cv::Mat perform_line_detection(const cv::Mat &input_image);

    int getTime_of_line_detection() const;

private:
    int time_of_line_detection = 0;

    cv::Mat detect_red_color(const cv::Mat &input_image);
    cv::Mat draw_lines(const cv::Mat &input_image, const std::vector<cv::Vec4i> &lines);
};

#endif // LINEDETECTOR_H
