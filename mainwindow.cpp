#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->detect_line_button->setDisabled(true);
    connect(ui->upload_image_button, SIGNAL(clicked()),
            this, SLOT(choose_image()));
    connect(ui->detect_line_button, SIGNAL(clicked()),
            this, SLOT(detect_line()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display_image(QLabel *image_label, const cv::Mat &displayed_image)
{
    if (displayed_image.empty()) {
        image_label->setText("Unable to read the image!");
        return;
    }

    cv::Mat resized_image = resize_displayed_image(displayed_image);

    QImage image_display((uchar*)resized_image.data,
                         resized_image.cols,
                         resized_image.rows,
                         resized_image.step,
                         QImage::Format_BGR888);
    image_label->setPixmap(QPixmap::fromImage(image_display));
    ui->detect_line_button->setEnabled(true);
}

void MainWindow::choose_image()
{
    QString file_name =
            QFileDialog::getOpenFileName(this,
                                         tr("Upload Image"),
                                         "/home",
                                         tr("Image files (*.bmp)"));
    this->image_name = file_name.toUtf8().constData();
    this->input_image = cv::imread(this->image_name);
    display_image(ui->input_image_label, this->input_image);
}

void MainWindow::detect_line()
{
    this->display_image(ui->output_image_label,
                        this->line_detector.perform_line_detection(this->input_image));
}

cv::Mat MainWindow::resize_displayed_image(const cv::Mat &original_image)
{
  cv::Mat resized_image;
  double resize_coeff = double(ui->input_image_label->size().height())/original_image.rows;
  cv::resize(original_image,
             resized_image,
             cv::Size(),
             resize_coeff,
             resize_coeff);
  return resized_image;
}

