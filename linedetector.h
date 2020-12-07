#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <QElapsedTimer>
#include <opencv2/opencv.hpp>
#include <imagedata.h>

class LineDetector
{
public:
    LineDetector();
    void perform_line_detection(ImageData &image_data);

    int getTime_of_line_detection() const;

private:
    int time_of_line_detection = 0;

    cv::Mat detect_red_color(const cv::Mat &input_image);
    std::vector<cv::Point> detect_lines(cv::Mat &red_mask);
};

#endif // LINEDETECTOR_H
