#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->detect_line_button->setDisabled(true);
    ui->extract_points_button->setDisabled(true);
    ui->file_writing_progress_bar->hide();
    connect(ui->upload_image_button, SIGNAL(clicked()),
            this, SLOT(choose_image()));
    connect(ui->detect_line_button, SIGNAL(clicked()),
            this, SLOT(detect_line()));
    connect(ui->extract_points_button, SIGNAL(clicked()),
            this, SLOT(extract_points()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::display_image(QLabel *image_label, const cv::Mat &displayed_image)
{

    cv::Mat resized_image = resize_displayed_image(displayed_image);

    QImage image_display((uchar*)resized_image.data,
                         resized_image.cols,
                         resized_image.rows,
                         resized_image.step,
                         QImage::Format_BGR888);
    image_label->setPixmap(QPixmap::fromImage(image_display));
}

void MainWindow::choose_image()
{
    QString file_name =
            QFileDialog::getOpenFileName(this,
                                         tr("Upload Image"),
                                         "/home",
                                         tr("Image files (*.JPEG)"),
                                         nullptr,
                                         QFileDialog::DontUseNativeDialog);
    if (file_name.isEmpty())
        return;

    this->input_image = cv::imread(file_name.toUtf8().constData());

    if (this->input_image.empty())
        return;

    display_image(ui->input_image_label, this->input_image);
    this->ui->detect_line_button->setDisabled(false);
    this->ui->extract_points_button->setDisabled(true);
    this->ui->file_writing_progress_bar->hide();
    this->ui->output_image_label->clear();
}

void MainWindow::detect_line()
{
    this->output_image = this->line_detector.perform_line_detection(this->input_image);
    this->display_image(ui->output_image_label, this->output_image);
    this->ui->time_of_line_detection_line_edit->setText(
                QString::number(this->line_detector.getTime_of_line_detection()));
    this->ui->extract_points_button->setDisabled(false);
    this->ui->file_writing_progress_bar->hide();
}

void MainWindow::extract_points()
{
    std::vector<cv::Point> laser_points =
            this->point_extractor.extract_points(this->output_image);

    if (laser_points.empty())
        return;

    QString file_name = QFileDialog::
            QFileDialog::getSaveFileName(this,
                                         tr("Save point list file"),
                                         "",
                                         tr("Text file (*.txt)"),
                                         nullptr,
                                         QFileDialog::DontUseNativeDialog);

    if (file_name.isEmpty())
        return;

    QFile points_file(file_name);
    if(!points_file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    this->ui->file_writing_progress_bar->setRange(0, laser_points.size());
    int progress_bar_value = 0;
    this->ui->file_writing_progress_bar->setValue(0);
    this->ui->file_writing_progress_bar->show();


    QTextStream out(&points_file);
    for (const cv::Point &p : laser_points) {
        out << p.x << " " << p.y << "\n";
        progress_bar_value++;
        this->ui->file_writing_progress_bar->setValue(progress_bar_value);
    }
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

