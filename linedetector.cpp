#include "linedetector.h"

LineDetector::LineDetector()
{
}

cv::Mat LineDetector::perform_line_detection(const cv::Mat &input_image)
{
    QElapsedTimer timer;
    timer.start();

    //Detecting red color
    cv::Mat red_mask = this->detect_red_color(input_image);

    //Detecting lines
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(red_mask, lines, 1, CV_PI/180, 50, 50, 10);

    this->time_of_line_detection = timer.elapsed();

    return this->draw_lines(input_image, lines);
}

int LineDetector::getTime_of_line_detection() const
{
    return time_of_line_detection;
}

int LineDetector::getTime_of_line_visualization() const
{
    return time_of_line_visualization;
}

cv::Mat LineDetector::detect_red_color(const cv::Mat &input_image)
{
    std::vector<cv::Mat> split;
    cv::split(input_image, split);

    cv::Mat red_mask;
    red_mask = split[2] - split[1];
    cv::adaptiveThreshold(red_mask, red_mask, 255,
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 15, 12);

    cv::Mat kernel_3 = cv::Mat::ones(3,3, CV_32F);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_CLOSE, kernel_3);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_DILATE, kernel_3);
    cv::morphologyEx(red_mask, red_mask, cv::MORPH_OPEN, kernel_3);

    return red_mask;
}

cv::Mat LineDetector::draw_lines(const cv::Mat &input_image,
                                 const std::vector<cv::Vec4i> &lines)
{
    QElapsedTimer timer;
    timer.start();

    //Darkening the image
    cv::Mat lined_image, hsv_image;
    std::vector<cv::Mat> split;
    cv::cvtColor(input_image, hsv_image, CV_BGR2HSV);
    cv::split(hsv_image, split);
    split[2] = split[2]*0.2;
    cv::merge(split,hsv_image);
    cv::cvtColor(hsv_image, lined_image, CV_HSV2BGR);

    //Drawing lines
    for (size_t i = 0; i < lines.size(); i++) {
        cv::Vec4i l = lines[i];
        cv::line(lined_image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]),
                cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
    }

    this->time_of_line_visualization = timer.elapsed();

    return lined_image;
}
