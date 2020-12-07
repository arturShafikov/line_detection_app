#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <opencv2/opencv.hpp>

class ImageData
{
public:
    ImageData();

    cv::Mat get_input_image() const;
    void set_input_image(const cv::Mat &value);

    cv::Mat get_output_image() const;
    void set_output_image(const cv::Mat &value);

    std::vector<cv::Point> get_laser_points() const;
    void set_laser_points(const std::vector<cv::Point> &value);

private:
    cv::Mat input_image;
    cv::Mat output_image;
    std::vector<cv::Point> laser_points;

    void draw_laser_lines();
};

#endif // IMAGEDATA_H
