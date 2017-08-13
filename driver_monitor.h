#ifndef DRIVER_MONITOR_H
#define DRIVER_MONITOR_H

//#include <mainwindow.h>

#include <QString>
#include <QDateTime>
#include <QTime>

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
    int status;
    bool instance_trigger;

    void shift_register(std::vector<int> &sample_vector, unsigned int size, bool print);
    int count;

    void classify(std::vector<int> &sample_vector, char inst, bool &instance_trigger);
    string string_status;

    void guiDisplay_text(QString &instance_string, QTime instance_timer, QString &separate, int &total_count);
    QTime time = QTime::currentTime();
    QString time_string = time.toString("hh:mm:ss ap");
    int instance_time;

    void instance_rate(std::vector<int> &instance_count, int time_rate, int threshold_time);
    int current_time_seconds;

private:
    Mat image;
    const dlib::full_object_detection shape;

};

#endif // DRIVER_MONITOR_H
