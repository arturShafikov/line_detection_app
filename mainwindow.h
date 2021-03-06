#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QTextStream>
#include <opencv2/opencv.hpp>
#include <linedetector.h>
#include <imagedata.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void choose_image();
    void detect_line();
    void extract_points();

private:
    Ui::MainWindow *ui;
    LineDetector line_detector;
    ImageData image_data;

    cv::Mat resize_displayed_image(const cv::Mat &original_image);
    void display_image(QLabel *image_label, const cv::Mat &displayed_image);
};
#endif // MAINWINDOW_H
