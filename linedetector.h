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
    cv::Mat input_image;
};

#endif // LINEDETECTOR_H
