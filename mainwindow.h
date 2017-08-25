#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <driver_monitor.h>

#include <QMainWindow>
#include <QTimer>
#include <QScrollBar>

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

    void on_pushButton_reset_clicked();

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

    bool FaceLeft_instanceTrigger = false;
    bool FaceRight_instanceTrigger = false;
    bool Blink_instanceTrigger= false;
    bool Yawn_instanceTrigger = false;

    std::vector<int> FaceLeft_register;
    std::vector<int> FaceRight_register;
    std::vector<int> Blink_register;
    std::vector<int> Yawn_register;

    int HeadTurn_count = 0;
    int Blink_count = 0;
    int Yawn_count = 0;

    QString main_string = "Start: \n";
    QString HeadTurn_string = "Start: \n";
    QString Blink_string = "Start: \n";
    QString Yawn_string = "Start: \n";

    QTime HeadTurn_timer;
    QTime Blink_timer;
    QTime Yawn_timer;

    bool FaceLeft_displayTrigger = false;
    bool FaceRight_displayTrigger = false;
    bool Blink_displayTrigger = false;
    bool Yawn_displayTrigger = false;

    std::vector<int> HeadTurn_rate;
    std::vector<int> Blink_rate;
    std::vector<int> SlowBlink_rate;
    std::vector<int> Yawn_rate;

    int HeadTurn_timeSpan = 10;
    int Blink_timeSpan = 10;
    int SlowBlink_timeSpan = 10;
    int Yawn_timeSpan = 10;

    int blue;
    int green;
    int red;

    QVector<double> HeadTurn_plot_data;
    QVector<double> LeftEye_plot_data, RightEye_plot_data;
    QVector<double> Mouth_plot_data;

    QVector<double> xAxis_plot_data;
};

#endif // MAINWINDOW_H
