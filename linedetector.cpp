#include "linedetector.h"

LineDetector::LineDetector()
{
}

void LineDetector::perform_line_detection(ImageData &image_data, int sensitivity)
{
    QElapsedTimer timer;
    timer.start();

    //Detecting red color
    cv::Mat red_mask = this->detect_red_color(image_data.get_input_image());

    //Detecting horizontal lines
    image_data.set_laser_points(this->detect_lines(red_mask, sensitivity));

    this->time_of_line_detection = timer.elapsed();
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

std::vector<cv::Point> LineDetector::detect_lines(cv::Mat &red_mask, int sensitivity)
{
    int horizontal_size = red_mask.cols / sensitivity;
    cv::Mat horizontal_elem = cv::getStructuringElement(cv::MORPH_RECT,
                                                        cv::Size(horizontal_size, 1));
    cv::erode(red_mask, red_mask, horizontal_elem, cv::Point(-1, -1));
    cv::dilate(red_mask, red_mask, horizontal_elem, cv::Point(-1, -1));

    std::vector<cv::Point> laser_points;
    cv::findNonZero(red_mask, laser_points);

    return laser_points;
}
