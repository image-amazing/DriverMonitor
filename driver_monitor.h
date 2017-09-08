#ifndef DRIVER_MONITOR_H
#define DRIVER_MONITOR_H

//#include <mainwindow.h>

#include <QString>
#include <QDateTime>
#include <QTime>
#include <QTableWidget>

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dlib/opencv.h>
#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

using namespace std;
using namespace cv;
using namespace dlib;

class driver_monitor
{
public:
    driver_monitor(const dlib::full_object_detection &d);

    void measure(int main_pt1, int main_pt2, char main_axis, int scale_pt1, int scale_pt2, char scale_axis, int scale_factor);
    int facial_feature;

    void eye_parameters(int &max, int &min);
    void head_parameters(int main_side, int minor_side);
    int percent;

    void instance(char inst, bool &instance_trigger, QTime &instance_timer, int parameter1, int threshold1, int parameter2 = 0, int threshold2 = 0, int parameter3 = 0, int threshold3 = 0);
    void instance(char inst, bool &instance_trigger, double CurrentVideoTime, double &StartTime, int parameter1, int threshold1, int parameter2 = 0, int threshold2 = 0, int parameter3 = 0, int threshold3 = 0);
    int status;

    void classify(char inst, bool &instance_trigger, QTime instance_timer, int threshold_time, bool &display_trigger);
    void classify(char inst, bool &instance_trigger, double VideoTime, double StartTime, int threshold_time, bool &display_trigger);
    QString string_status;

    void instance_rate(std::vector<int> &instance_count, int time_span, int threshold_time, bool &display_trigger);
    void instance_rate(std::vector<int> &instance_count, int time_span, int threshold_time, bool &display_trigger, double VideoTime);
    int current_time_seconds;

    QTime time = QTime::currentTime();
    QString time_string = time.toString("hh:mm:ss ap");
    int instance_time;    

    void DriverStatus_Drowsy(unsigned int threshold, std::vector<int> &instance_count);
    void DriverStatus_Distracted(int threshold, QTime &instance_timer, int instance_count);
    void DriverStatus_Asleep(int threshold, QTime &instance_timer, int instance_count);
    QString DriverStatus_string;

    void DriverStatus_Distracted(int threshold, double VideoTime, double StartTime, int instance_count);
    void DriverStatus_Asleep(int threshold,  double VideoTime, double StartTime, int instance_count);

    void DisplayTo_QTableWidget(QTableWidget *Main_tableWidget, QTableWidget *Instance_tableWidget, bool &displayTrigger, int &count);
    void DisplayTo_QTableWidget(QTableWidget *Main_tableWidget, QTableWidget *Instance_tableWidget, bool &displayTrigger, int &count, QString VideoTime_string);
    void DisplayTo_QTableWidget(QTableWidget *Main_tableWidget);
    void DisplayTo_QTableWidget(QTableWidget *Main_tableWidget, QString VideoTime_string);


private:
    Mat image;
    const dlib::full_object_detection shape;

};

#endif // DRIVER_MONITOR_H
