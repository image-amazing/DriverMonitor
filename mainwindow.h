#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QScrollBar>

#include <driver_monitor.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_open_camera_clicked();

    void on_pushButton_close_camera_clicked();

    void update_window();

private:
    Ui::MainWindow *ui;

    QTimer *timer;
    VideoCapture cap;

    QImage qt_image;
    Mat frame;

    frontal_face_detector detector;
    shape_predictor shape_model;

    void show_frame(Mat &);
    void face_layout(const dlib::full_object_detection&);

    int LeftEye_max, LeftEye_min = 100;
    int RightEye_max, RightEye_min = 100;

    bool FaceLeft_trigger = false;
    bool FaceRight_trigger = false;
    bool Blink_trigger = false;
    bool Yawn_trigger = false;

    std::vector<int> FaceLeft_register;
    std::vector<int> FaceRight_register;
    std::vector<int> Blink_register;
    std::vector<int> Yawn_register;

    int HeadTurn_count = 0;
    int Blink_count = 0;
    int Yawn_count = 0;

    QString instance_string = "Start: \n";
    QString HeadTurn_string = "Start: \n";
    QString Blink_string = "Start: \n";
    QString Yawn_string = "Start: \n";

    QTime HeadTurn_timer;
    QTime Blink_timer;
    QTime Yawn_timer;

    std::vector<int> HeadTurn_rate;
    std::vector<int> Blink_rate;
    std::vector<int> SlowBlink_rate;
    std::vector<int> Yawn_rate;

    int HeadTurn_time_rate = 10;
    int Blink_time_rate = 10;
    int SlowBlink_time_rate = 10;
    int Yawn_time_rate = 10;

    int blue;
    int green;
    int red;

    QVector<double> HeadTurn_plot_data;
    QVector<double> LeftEye_plot_data, RightEye_plot_data;
    QVector<double> Mouth_plot_data;

    QVector<double> xAxis_plot_data;
};

#endif // MAINWINDOW_H
