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
    cv::HoughLinesP(red_mask, lines, 1, CV_PI/180, 100, 5, 5);

    this->time_of_line_detection = timer.elapsed();

    return this->draw_lines(input_image, lines);
}

int LineDetector::getTime_of_line_detection() const
{
    return time_of_line_detection;
}

cv::Mat LineDetector::detect_red_color(const cv::Mat &input_image)
{
    cv::Mat tmp_image = input_image.clone();

    cv::GaussianBlur(input_image, tmp_image, cv::Size(3,3), 0);

    std::vector<cv::Mat> split;
    cv::split(tmp_image, split);
    tmp_image = split[2] - split[1];

    cv::Mat sorted_idx;
    cv::sortIdx(tmp_image, sorted_idx, cv::SORT_EVERY_COLUMN+cv::SORT_DESCENDING);

    cv::Mat output = cv::Mat::zeros(tmp_image.rows, tmp_image.cols, CV_8UC1);

    for (int i = 0; i < sorted_idx.cols; i++) {
        cv::circle(output, cv::Point(i, sorted_idx.at<int>(0,i)), 2, cv::Scalar(255), -1);
    }

    return output;
}

cv::Mat LineDetector::draw_lines(const cv::Mat &input_image,
                                 const std::vector<cv::Vec4i> &lines)
{
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
                cv::Scalar(0, 0, 255), 2, cv::LINE_AA);
    }

    return lined_image;
}
