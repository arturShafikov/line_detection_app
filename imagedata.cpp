#include "imagedata.h"

ImageData::ImageData()
{

}

cv::Mat ImageData::get_input_image() const
{
    return input_image;
}

void ImageData::set_input_image(const cv::Mat &value)
{
    input_image = value;
}

cv::Mat ImageData::get_output_image() const
{
    return output_image;
}

void ImageData::set_output_image(const cv::Mat &value)
{
    output_image = value;
}

std::vector<cv::Point> ImageData::get_laser_points() const
{
    return laser_points;
}

void ImageData::set_laser_points(const std::vector<cv::Point> &value)
{
    laser_points = value;
    draw_laser_lines();
}

void ImageData::draw_laser_lines()
{
    //Darkening the image
    cv::Mat hsv_image;
    std::vector<cv::Mat> split;
    cv::cvtColor(this->input_image, hsv_image, CV_BGR2HSV);
    cv::split(hsv_image, split);
    split[2] = split[2]*0.2;
    cv::merge(split,hsv_image);
    cv::cvtColor(hsv_image, this->output_image, CV_HSV2BGR);

    //Drawing lines
    for (size_t i = 0; i < this->laser_points.size(); i++) {
        cv::circle(this->output_image, laser_points.at(i), 1, cv::Scalar(0, 0, 255), -1);
    }
}
