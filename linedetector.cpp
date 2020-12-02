#include "linedetector.h"

LineDetector::LineDetector()
{
    this->min_red_hue_1 = 0;
    this->min_red_hue_2 = 170;
    this->red_hue_range = 10;
    this->min_saturation = 120;
    this->max_saturation = 255;
    this->min_value = 70;
    this->max_value = 255;
}

cv::Mat LineDetector::perform_line_detection(const cv::Mat &input_image)
{
    cv::Mat hsv_image;
    cv::cvtColor(input_image, hsv_image, CV_BGR2HSV);

    cv::Mat red_mask = this->generate_red_mask(hsv_image);

    cv::Mat kernel_3 = cv::Mat::ones(3,3, CV_32F);
    cv::Mat kernel_5 = cv::Mat::ones(5,5, CV_32F);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_OPEN, kernel_3);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_DILATE, kernel_3);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_CLOSE, kernel_5);

    cv::Mat blurred_image;
    cv::medianBlur(red_mask, blurred_image, 17);
    cv::morphologyEx(blurred_image, blurred_image, cv::MORPH_DILATE, kernel_3);
    cv::morphologyEx(blurred_image, blurred_image, cv::MORPH_CLOSE, kernel_5);

    cv::Mat bgr_red_mask;
    cv::cvtColor(blurred_image, bgr_red_mask, CV_GRAY2BGR);

    return bgr_red_mask;
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
                cv::Scalar(this->min_red_hue_2,
                           this->min_saturation,
                           this->min_value),
                cv::Scalar(this->min_red_hue_2+this->red_hue_range,
                           this->max_saturation,
                           this->max_value),
                red_mask_2);
    return red_mask_1 + red_mask_2;
}
