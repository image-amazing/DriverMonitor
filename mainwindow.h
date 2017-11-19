#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <driver_monitor.h>
#include <iostream>
#include <fstream>

#include <QMainWindow>
#include <QTimer>
#include <QScrollBar>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
#include <QThread>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QDebug>

#include <chat.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void show_frame(Mat &);
    void face_layout(const dlib::full_object_detection&, Mat image);

//    void ResizeImage();
    void RotateImage();
    void ResetData();
    void ResetSettings();
    void ResetTriggers();

signals:
    void passToBluetooth(const QString &message);
    void sendEntry(const QString &message);

private slots:
    void fromBluetooth(const QString &message);

    void displayBluetoothText(const QString &sender, const QString &message);

    void uiFunctions();

    void DisplayCurrentTime();

    void ProcessCameraFrame();

    void ProcessVideoFrame();

    void on_OpenCamera_pushButton_clicked();

    void on_CloseCamera_pushButton_clicked();

//    void on_pushButton_reset_clicked();

    void on_SaveOutputData_pushButton_clicked();

    void on_LoadVideo_pushButton_clicked();

    void on_PlayVideo_pushButton_clicked();

    void on_PauseVideo_pushButton_clicked();

    void on_StopVideo_pushButton_clicked();

    void on_Video_horizontalSlider_sliderMoved(int position);

    void on_LoadSettings_pushButton_clicked();

    void on_SaveSettings_pushButton_clicked();

    void on_Rotate_pushButton_clicked();

    void on_HeadTurnLeft_spinBox_valueChanged(int arg1);

    void on_HeadTurnRight_spinBox_valueChanged(int arg1);

    void on_Blink_spinBox_valueChanged(int arg1);

    void on_YawnMouth_spinBox_valueChanged(int arg1);

    void on_YawnEyes_spinBox_valueChanged(int arg1);

    void on_LoadImage_pushButton_clicked();

    void on_LiptoSideMouth_spinBox_valueChanged(int arg1);

    void on_HeadTurnBlink_spinBox_valueChanged(int arg1);

    void on_SmileBlink_spinBox_valueChanged(int arg1);

    void on_ResetData_pushButton_clicked();

    void on_ResetSettings_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    Chat *chat;

    QTimer *timer;
    VideoCapture cap;

    QImage qt_image;
    Mat frame;

    frontal_face_detector detector;
    shape_predictor shape_model;

    int LeftEye_max, LeftEye_min = 100;
    int RightEye_max, RightEye_min = 100;

    bool HeadTurnLeft_instanceTrigger = false;
    bool HeadTurnRight_instanceTrigger = false;
    bool Blink_instanceTrigger= false;
    bool Yawn_instanceTrigger = false;

    int HeadTurn_count = 0;
    int HeadTurn_prevCount;
    int Blink_count = 0;
    int Yawn_count = 0;

    int HeadTurn_thresholdTime = 0;
    int Blink_thresholdTime = 0;
    int SlowBlink_thresholdTime = 200;
    int Yawn_thresholdTime = 200;

    int HeadTurnLeft_ThresholdLevel = 65;
    int HeadTurnRight_ThresholdLevel = 35;
    int Blink_ThresholdLevel = 25;
    int YawnEyes_ThresholdLevel = 35;
    int YawnMouth_ThresholdLevel = 30;

    QString main_string = "Start: \n";
    QString HeadTurn_string = "Start: \n";
    QString Blink_string = "Start: \n";
    QString Yawn_string = "Start: \n";

    QTime HeadTurn_timer;
    QTime Blink_timer;
    QTime Yawn_timer;

    bool HeadTurnLeft_displayTrigger = false;
    bool HeadTurnRight_displayTrigger = false;
    bool Blink_displayTrigger = false;
    bool Yawn_displayTrigger = false;

    std::vector<int> HeadTurn_rate;
    std::vector<int> Blink_rate;
    std::vector<int> SlowBlink_rate;
    std::vector<int> Yawn_rate;

    int HeadTurn_refreshRate = 10;
    int Blink_refreshRate = 10;
    int SlowBlink_refreshRate = 10;
    int Yawn_refreshRate = 10;

    int blue;
    int green;
    int red;

    QVector<double> HeadTurn_PlotData;
    QVector<double> LeftEye_PlotData, RightEye_PlotData;
    QVector<double> Mouth_PlotData;

    QVector<double> xAxis_PlotData;


    bool CloseCamera = false;

    unsigned int frameIndex;
    double frameDelay;
    double OptimalFrameDelay;
    double EffectiveFrameDelay;
    QTime ProcessDelayTimer;
    bool PauseVideo = false;
    bool StopVideo = false;
    double PauseTime;

    double HeadTurnStartTime;
    double BlinkStartTime;
    double YawnStartTime;

    int Rotate = 0;

    QMediaPlayer *alert = new QMediaPlayer();

    int SmileMax = 0;
    int SmileMin;

    QString ImagePath;
    QString VideoPath;

    QVector<double> SideMouth1_PlotData;
    QVector<double> SideMouth2_PlotData;

    int SmileThresholdLevel;

    bool SmileStatusIndicator;

    int HeadTurnBlink_ThresholdLevel = 40;
    int SmileBlink_ThresholdLevel = 5;

    bool enableDistracted;

    QString tableEntry;

    int mainRowCount;
    int headTurnRowCount;
    int blinkRowCount;
    int yawnRowCount;
};

#endif // MAINWINDOW_H
