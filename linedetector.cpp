#include "linedetector.h"

LineDetector::LineDetector()
{
    this->min_red_hue_1 = 0;
    this->max_red_hue_2 = 180;
    this->red_hue_range = 10;
    this->min_saturation = 120;
    this->max_saturation = 255;
    this->min_value = 90;
    this->max_value = 255;
}

cv::Mat LineDetector::perform_line_detection(const cv::Mat &input_image)
{
    cv::Mat hsv_image;
    cv::cvtColor(input_image, hsv_image, CV_BGR2HSV);

    cv::Mat red_mask = this->generate_red_mask(hsv_image);

    cv::Mat kernel_3 = cv::Mat::ones(3,3, CV_32F);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_OPEN, kernel_3);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_ERODE, kernel_3);

    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(red_mask, lines, 1, CV_PI/180, 50, 50, 10);

    cv::Mat lined = input_image.clone();
    for (size_t i = 0; i < lines.size(); i++) {
        cv::Vec4i l = lines[i];
        cv::line(lined, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
                cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    return lined;
}

cv::Mat LineDetector::generate_red_mask(const cv::Mat &hsv_image)
{
    cv::Mat red_mask_1;
    cv::Mat red_mask_2;
    cv::inRange(hsv_image,
                cv::Scalar(this->min_red_hue_1,
                           this->min_saturation,
                           this->min_value),
                cv::Scalar(this->min_red_hue_1+this->red_hue_range,
                           this->max_saturation,
                           this->max_value),
                red_mask_1);
    cv::inRange(hsv_image,
                cv::Scalar(this->max_red_hue_2-this->red_hue_range,
                           this->min_saturation,
                           this->min_value),
                cv::Scalar(this->max_red_hue_2,
                           this->max_saturation,
                           this->max_value),
                red_mask_2);
    return red_mask_1 + red_mask_2;
}
