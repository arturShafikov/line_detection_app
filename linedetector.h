#ifndef LINEDETECTOR_H
#define LINEDETECTOR_H

#include <QObject>
#include <opencv2/opencv.hpp>

class LineDetector : public QObject
{
    Q_OBJECT
public:
    LineDetector();
    cv::Mat perform_line_detection(const cv::Mat &input_image);

private:
    int min_red_hue_1;
    int max_red_hue_2;
    int red_hue_range;
    int min_saturation;
    int max_saturation;
    int min_value;
    int max_value;

    cv::Mat detect_red_color(const cv::Mat &input_image);
    cv::Mat draw_lines(const cv::Mat &input_image, const std::vector<cv::Vec4i> &lines);
};

#endif // LINEDETECTOR_H
