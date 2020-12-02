#include "linedetector.h"

LineDetector::LineDetector()
{

}

cv::Mat LineDetector::perform_line_detection(const cv::Mat &input_image)
{
    cv::Mat hsv_image;
    cv::cvtColor(input_image, hsv_image, CV_BGR2HSV);

    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv_image, hsv_channels);

    int red_hue_value = 0;
    int red_hue_range = 5;
    cv::Mat red_hue_mask;
    cv::inRange(hsv_channels.at(0),
                red_hue_value-red_hue_range,
                red_hue_value+red_hue_range,
                red_hue_mask);

    int min_saturation = 90;
    int min_value = 90;
    cv::Mat saturation_mask = hsv_channels.at(1) > min_saturation;
    cv::Mat value_mask = hsv_channels.at(2) > min_value;

    cv::Mat red_mask = (red_hue_mask & saturation_mask) & value_mask;
    cv::Mat bgr_red_mask;
    cv::cvtColor(red_mask, bgr_red_mask, CV_GRAY2BGR);
//    emit line_detected(bgr_red_mask);
    return bgr_red_mask;
}
