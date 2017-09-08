#include "mainwindow.h"
#include "ui_mainwindow.h"

QCPItemLine *HeadTurnLeft_threshold_line;
QCPItemLine *HeadTurnRight_threshold_line;
QCPItemLine *Blink_threshold_line;
QCPItemLine *YawnMouth_threshold_line;

int HeadTurnLeft_threshold = 65;
int HeadTurnRight_threshold = 35;
int Blink_threshold = 25;
int YawnMouth_threshold = 30;
int YawnEyes_threshold = 35;

QString OutputData_path;

QTime VideoTimer;
std::vector<Mat> stored_frame;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    ui->HeadTurn_plot->addGraph();

    ui->HeadTurn_plot->yAxis->setRange(0, 100);
    ui->HeadTurn_plot->setInteractions(QCP::iSelectItems | QCP::iRangeDrag);
    ui->HeadTurn_plot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->HeadTurn_plot->graph(0)->setLineStyle(QCPGraph::lsLine);

    HeadTurnLeft_threshold_line = new QCPItemLine(ui->HeadTurn_plot);
    HeadTurnLeft_threshold_line->setSelectable(true);
    HeadTurnLeft_threshold_line->setPen(QPen(Qt::DashDotLine));
    HeadTurnLeft_threshold_line->start->setCoords(QCPRange::minRange, 65);
    HeadTurnLeft_threshold_line->end->setCoords(QCPRange::maxRange, 65);

    HeadTurnRight_threshold_line = new QCPItemLine(ui->HeadTurn_plot);
    HeadTurnRight_threshold_line->setSelectable(true);
    HeadTurnRight_threshold_line->setPen(QPen(Qt::DashDotLine));
    HeadTurnRight_threshold_line->start->setCoords(QCPRange::minRange, 35);
    HeadTurnRight_threshold_line->end->setCoords(QCPRange::maxRange, 35);

    ui->Blink_plot->addGraph();
    ui->Blink_plot->addGraph();

    ui->Blink_plot->yAxis->setRange(0, 100);
    ui->Blink_plot->setInteractions(QCP::iSelectItems | QCP::iRangeDrag);
    ui->Blink_plot->axisRect()->setRangeDrag(Qt::Horizontal);

    ui->Blink_plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->Blink_plot->graph(0)->setPen(QPen(Qt::red));

    //ui->Blink_plot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->Blink_plot->graph(1)->setLineStyle(QCPGraph::lsLine);
    ui->Blink_plot->graph(1)->setPen(QPen(Qt::green));

    Blink_threshold_line = new QCPItemLine(ui->Blink_plot);
    Blink_threshold_line->setSelectable(true);
    Blink_threshold_line->setPen(QPen(Qt::DashDotLine));
    Blink_threshold_line->start->setCoords(QCPRange::minRange, 25);
    Blink_threshold_line->end->setCoords(QCPRange::maxRange, 25);

    ui->Yawn_plot->addGraph();

    ui->Yawn_plot->yAxis->setRange(0, 100);
    ui->Yawn_plot->setInteractions(QCP::iSelectItems | QCP::iRangeDrag);
    ui->Yawn_plot->axisRect()->setRangeDrag(Qt::Horizontal);

    ui->Yawn_plot->graph(0)->setLineStyle((QCPGraph::lsLine));
    ui->Yawn_plot->graph(0)->setPen(QPen(Qt::green));

    YawnMouth_threshold_line = new QCPItemLine(ui->Yawn_plot);
    YawnMouth_threshold_line->setSelectable(true);
    YawnMouth_threshold_line->setPen(QPen(Qt::DashDotLine));
    YawnMouth_threshold_line->start->setCoords(QCPRange::minRange, 30);
    YawnMouth_threshold_line->end->setCoords(QCPRange::maxRange, 30);

    QStringList HeaderName;
    HeaderName << "Time" << "Status" << "Elapsed Time(ms)";

    ui->Main_tableWidget->setColumnCount(3);
    ui->Main_tableWidget->setHorizontalHeaderLabels(HeaderName);

    ui->HeadTurn_tableWidget->setColumnCount(3);
    ui->HeadTurn_tableWidget->setHorizontalHeaderLabels(HeaderName);

    ui->Blink_tableWidget->setColumnCount(3);
    ui->Blink_tableWidget->setHorizontalHeaderLabels(HeaderName);

    ui->Yawn_tableWidget->setColumnCount(3);
    ui->Yawn_tableWidget->setHorizontalHeaderLabels(HeaderName);


    //ui->Main_tableWidget->resizeColumnsToContents();

    ui->Main_tableWidget->setColumnWidth(0, 78);
    ui->Main_tableWidget->setColumnWidth(1, 96);
    ui->Main_tableWidget->setColumnWidth(2, 135);

    ui->HeadTurn_tableWidget->setColumnWidth(0, 78);
    ui->HeadTurn_tableWidget->setColumnWidth(1, 96);
    ui->HeadTurn_tableWidget->setColumnWidth(2, 135);

    ui->Blink_tableWidget->setColumnWidth(0, 78);
    ui->Blink_tableWidget->setColumnWidth(1, 96);
    ui->Blink_tableWidget->setColumnWidth(2, 135);

    ui->Yawn_tableWidget->setColumnWidth(0, 78);
    ui->Yawn_tableWidget->setColumnWidth(1, 96);
    ui->Yawn_tableWidget->setColumnWidth(2, 135);

    detector = get_frontal_face_detector();
    deserialize("C://shape_predictor_68_face_landmarks.dat") >> shape_model;
//    deserialize("shape_predictor_68_face_landmarks.dat") >> shape_model;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OpenCamera_pushButton_clicked()
{
    cap.open(0);
    if(!cap.isOpened())
    {
        ui->statusBar->showMessage("Failed to open camera", 1000);
    }
    else
    {
        ui->statusBar->showMessage("Camera is open", 1000);

        connect(timer, SIGNAL(timeout()), this, SLOT(ProcessCameraFrame()));
        timer->start(20);
        ui->CloseCamera_pushButton->setEnabled(true);
    }
    HeadTurn_timer.start();
    Blink_timer.start();
    Yawn_timer.start();
}

void MainWindow::on_CloseCamera_pushButton_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(ProcessCameraFrame()));
    cap.release();
    Mat image = Mat::zeros(frame.size(), CV_8UC3);
    show_frame(image);

    ui->statusBar->showMessage("Camera is closed", 1000);

    FaceLeft_instanceTrigger = false;
    FaceRight_instanceTrigger = false;
    Blink_instanceTrigger = false;
    Yawn_instanceTrigger = false;
}

void MainWindow::on_LoadVideo_pushButton_clicked()
{
    cap.open(ui->VideoInputPath_lineEdit->text().toStdString());

    if(!cap.isOpened())
        ui->statusBar->showMessage("Video file is not loaded", 1000);
    else
    {
        Mat temp_frame;        
        stored_frame.clear();
        while(!(cap.get(CV_CAP_PROP_FRAME_COUNT) > 0 && cap.get(CV_CAP_PROP_POS_FRAMES) == cap.get(CV_CAP_PROP_FRAME_COUNT) - 1))
        {
            cap >> temp_frame;

            if(cap.get(CV_CAP_PROP_FRAME_WIDTH) > cap.get(CV_CAP_PROP_FRAME_HEIGHT))
            {
                double resize_height = 640 * (cap.get(CV_CAP_PROP_FRAME_HEIGHT) / cap.get(CV_CAP_PROP_FRAME_WIDTH));
                cv::resize(temp_frame, temp_frame, Size(640, resize_height), 0, 0, INTER_CUBIC);
            }
            else
            {
                double resize_width = 480 * (cap.get(CV_CAP_PROP_FRAME_WIDTH) / cap.get(CV_CAP_PROP_FRAME_HEIGHT));
                cv::resize(temp_frame, temp_frame, Size(resize_width, 480), 0, 0, INTER_CUBIC);
            }

            //Mat temp1_frame = temp_frame.clone();
            stored_frame.push_back(temp_frame);
            double LoadingPercent = (cap.get(CV_CAP_PROP_POS_FRAMES) / cap.get(CV_CAP_PROP_FRAME_COUNT)) * 100;
            QString LoadingPercent_string = QString::number(LoadingPercent);
            LoadingPercent_string += "%";
            ui->statusBar->showMessage(LoadingPercent_string, 1000);

        }
        ui->statusBar->showMessage("Finished loading video", 1000);
        frameDelay = 1000 / cap.get(CV_CAP_PROP_FPS);
        ui->OriginalFrameDelay_label->setText(QString::number(frameDelay));
        OptimalFrameDelay = frameDelay;
        frameIndex = 0;
    }
}

void MainWindow::on_PlayVideo_pushButton_clicked()
{
    if(frameIndex == 0 && ui->ProcessVideo_checkBox->isChecked())
        ResetData();
    VideoTimer.start();
    namedWindow("Delay Timer");
    moveWindow("Delay Timer", 0, 0);

    double VideoTime;

    double VideoDuration;
    VideoDuration = trunc(cap.get(CV_CAP_PROP_FRAME_COUNT) / cap.get(CV_CAP_PROP_FPS));

    QString VideoDuration_minute;
    if((VideoDuration / 60) < 10)
    {
        VideoDuration_minute += QString::number(0);
        VideoDuration_minute += QString::number(trunc(VideoDuration / 60));
    }
    else
    {
        VideoDuration_minute += QString::number(trunc(VideoDuration / 60));
    }
    QString VideoDuration_second;
    if((VideoDuration - (VideoDuration / 60)) < 10)
    {
        VideoDuration_second += QString::number(0);
        VideoDuration_second += QString::number(VideoDuration - trunc(VideoDuration / 60) * 60);
    }
    else
    {
        VideoDuration_second += QString::number(VideoDuration - trunc(VideoDuration / 60) * 60);
    }
    QString VideoDuration_string;
    VideoDuration_string += VideoDuration_minute;
    VideoDuration_string += ':';
    VideoDuration_string += VideoDuration_second;

    ui->VideoDuration_label->setText(VideoDuration_string);
    ui->Video_horizontalSlider->setMaximum(stored_frame.size());
    while(frameIndex < stored_frame.size())
    {
        if(PauseVideo == true || StopVideo == true)
            break;

        ProcessDelayTimer.start();
        VideoTime = (frameIndex * cap.get(CV_CAP_PROP_FRAME_COUNT)) / (stored_frame.size() * cap.get(CV_CAP_PROP_FPS));

        QString VideoTime_minute;
        if((VideoTime / 60) < 10)
        {
            VideoTime_minute += QString::number(0);
            VideoTime_minute += QString::number(trunc(VideoTime / 60));
        }
        else
        {
            VideoTime_minute += QString::number(trunc(VideoTime / 60));
        }
        QString VideoTime_second;
        if((VideoTime - trunc(VideoTime / 60)) < 10)
        {
            VideoTime_second += QString::number(0);
            VideoTime_second += QString::number(trunc(VideoTime) - trunc(VideoTime / 60));
        }
        else
        {
            VideoTime_second += QString::number(trunc(VideoTime) - trunc(VideoTime / 60));
        }
        QString VideoTime_string;
        VideoTime_string += VideoTime_minute;
        VideoTime_string += ':';
        VideoTime_string += VideoTime_second;

        ui->VideoTime_label->setText(VideoTime_string);

        if(ui->ProcessVideo_checkBox->isChecked())
        {
            frame = stored_frame.at(frameIndex);
            ProcessVideoFrame(frame, VideoTime, VideoTime_string);
        }
        else
        {
            show_frame(stored_frame.at(frameIndex));
        }
        ui->Video_horizontalSlider->setValue(frameIndex);

        frameIndex++;

        //cout << "Process Delay: " << ProcessDelayTimer.elapsed() << endl;
        OptimalFrameDelay = frameDelay - ProcessDelayTimer.elapsed();
        if(OptimalFrameDelay < 1)
            OptimalFrameDelay = 1;
        ui->EffectiveFrameDelay_label->setText(QString::number(OptimalFrameDelay));
        cvWaitKey(OptimalFrameDelay);
    }

    destroyWindow("Delay Timer");
    if(StopVideo == true)
    {
        StopVideo = false;
        frameIndex = 0;
        show_frame(stored_frame.at(frameIndex));
        ui->Video_horizontalSlider->setValue(frameIndex);
        QString VideoTime_string = "00:00";
        ui->VideoTime_label->setText(VideoTime_string);
        goto exit;
    }
    if(PauseVideo == true)
    {
        PauseVideo = false;
        goto exit;
    }

    frameIndex++;
    ui->Video_horizontalSlider->setValue(frameIndex);
    ui->VideoTime_label->setText(VideoDuration_string);
    frameIndex = 0;

    exit:

    cout << "Video Duration: " << cap.get(CV_CAP_PROP_FRAME_COUNT) / cap.get(CV_CAP_PROP_FPS) << endl;
    cout << "Video Timer: " << VideoTimer.elapsed() << endl;
}

void MainWindow::ProcessCameraFrame()
{
    cap >> frame;

    array2d<bgr_pixel> dlib_image;
    assign_image(dlib_image, cv_image<bgr_pixel>(frame));

    std::vector<dlib::rectangle> faces = detector(dlib_image);

    //int number_of_detected_faces = faces.size();
    //cout << "Number of detected faces : " << number_of_detected_faces << endl;

    std::vector<full_object_detection> shapes;
    for(unsigned long i = 0; i < faces.size(); i++)
    {
        full_object_detection shape = shape_model(dlib_image, faces[i]);
        shapes.push_back(shape);

        face_layout(shape, frame);

        driver_monitor HeadTurnLeft(shape);
        HeadTurnLeft.measure(30, 36, 'x', 35, 31, 'x', 10);

        driver_monitor HeadTurnRight(shape);
        HeadTurnRight.measure(45, 30, 'x', 35, 31, 'x', 10);

        HeadTurnLeft.head_parameters(HeadTurnLeft.facial_feature, HeadTurnRight.facial_feature);
        HeadTurnLeft.instance('h', FaceLeft_instanceTrigger, HeadTurn_timer, HeadTurnLeft.percent, HeadTurnLeft_threshold);
        HeadTurnLeft.classify('l', FaceLeft_instanceTrigger, HeadTurn_timer, HeadTurn_thresholdTime, FaceLeft_displayTrigger);
        HeadTurnLeft.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, FaceLeft_displayTrigger);
        HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, FaceLeft_displayTrigger, HeadTurn_count);
        HeadTurnLeft.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), HeadTurn_timer, HeadTurn_count);
        HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget);

        HeadTurnRight.head_parameters(HeadTurnRight.facial_feature, HeadTurnLeft.facial_feature);
        HeadTurnRight.instance('h', FaceRight_instanceTrigger, HeadTurn_timer, HeadTurnRight.percent, 65);
        HeadTurnRight.classify('r', FaceRight_instanceTrigger, HeadTurn_timer, HeadTurn_thresholdTime, FaceRight_displayTrigger);
        HeadTurnRight.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, FaceRight_displayTrigger);
        HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, FaceRight_displayTrigger, HeadTurn_count);
        HeadTurnRight.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), HeadTurn_timer, HeadTurn_count);
        HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget);


        driver_monitor LeftEye(shape);
        LeftEye.measure(41, 37, 'y', 39, 36,'x', 100);
        LeftEye.eye_parameters(LeftEye_max, LeftEye_min);

        driver_monitor RightEye(shape);
        RightEye.measure(46, 44, 'y', 45, 42, 'x', 100);
        RightEye.eye_parameters(RightEye_max, RightEye_min);

        driver_monitor Blink(shape);
        Blink.instance('b', Blink_instanceTrigger, Blink_timer, LeftEye.percent, Blink_threshold, RightEye.percent, Blink_threshold);
        Blink.classify('b', Blink_instanceTrigger, Blink_timer, Blink_thresholdTime, Blink_displayTrigger);
        Blink.instance_rate(Blink_rate, Blink_refreshRate, Blink_thresholdTime, Blink_displayTrigger);
        Blink.instance_rate(SlowBlink_rate, SlowBlink_refreshRate, SlowBlink_thresholdTime, Blink_displayTrigger);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->Blink_tableWidget, Blink_displayTrigger, Blink_count);
        Blink.DriverStatus_Drowsy(ui->SlowBlink_ThresholdRate_spinBox->value(), SlowBlink_rate);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget);
        Blink.DriverStatus_Asleep(ui->ClosedEyes_TimeLimit_spinBox->value(), Blink_timer, Blink_count);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget);

        driver_monitor Yawn(shape);
        Yawn.measure(66, 62, 'y', 64, 60, 'x', 100);
        Yawn.instance('y', Yawn_instanceTrigger, Yawn_timer, Yawn.facial_feature, YawnMouth_threshold, LeftEye.percent, YawnEyes_threshold, RightEye.percent, YawnEyes_threshold);
        Yawn.classify('y', Yawn_instanceTrigger, Yawn_timer, Yawn_thresholdTime, Yawn_displayTrigger);
        Yawn.instance_rate(Yawn_rate, Yawn_refreshRate, Yawn_thresholdTime, Yawn_displayTrigger);
        Yawn.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->Yawn_tableWidget, Yawn_displayTrigger, Yawn_count);
        Yawn.DriverStatus_Drowsy(ui->Yawn_ThresholdRate_spinBox->value(), Yawn_rate);
        Yawn.DisplayTo_QTableWidget(ui->Main_tableWidget);

        HeadTurn_PlotData.append(double(HeadTurnLeft.percent));
        LeftEye_PlotData.append(LeftEye.percent);
        RightEye_PlotData.append(RightEye.percent);
        Mouth_PlotData.append(Yawn.facial_feature);
        xAxis_PlotData.append(double(HeadTurn_PlotData.size()));

        ui_functions();
    }
    show_frame(frame);
}

void MainWindow::ProcessVideoFrame(Mat temp_frame, double VideoTime, QString VideoTime_string)
{

    frame = temp_frame.clone();
    VideoTime = trunc(VideoTime * 1000);

//    if(cap.get(CV_CAP_PROP_FRAME_COUNT) > 0 && cap.get(CV_CAP_PROP_POS_FRAMES) == cap.get(CV_CAP_PROP_FRAME_COUNT))
//    {
//        cout << VideoTimer.elapsed() << endl;
//        cap.release();
//        disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
//        timer->start(20);
//    }

    array2d<bgr_pixel> dlib_image;
    assign_image(dlib_image, cv_image<bgr_pixel>(frame));

    std::vector<dlib::rectangle> faces = detector(dlib_image);

    //int number_of_detected_faces = faces.size();
    //cout << "Number of detected faces : " << number_of_detected_faces << endl;

    std::vector<full_object_detection> shapes;
    for(unsigned long i = 0; i < faces.size(); i++)
    {
        full_object_detection shape = shape_model(dlib_image, faces[i]);
        shapes.push_back(shape);

        face_layout(shape, frame);

        driver_monitor HeadTurnLeft(shape);
        HeadTurnLeft.measure(30, 36, 'x', 35, 31, 'x', 10);

        driver_monitor HeadTurnRight(shape);
        HeadTurnRight.measure(45, 30, 'x', 35, 31, 'x', 10);

        HeadTurnLeft.head_parameters(HeadTurnLeft.facial_feature, HeadTurnRight.facial_feature);
        HeadTurnLeft.instance('h', FaceLeft_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurnLeft.percent, HeadTurnLeft_threshold);
        HeadTurnLeft.classify('l', FaceLeft_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurn_thresholdTime, FaceLeft_displayTrigger);
        HeadTurnLeft.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, FaceLeft_displayTrigger, VideoTime);
        HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, FaceLeft_displayTrigger, HeadTurn_count, VideoTime_string);
        HeadTurnLeft.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), VideoTime, HeadTurnStartTime, HeadTurn_count);
        HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);

        HeadTurnRight.head_parameters(HeadTurnRight.facial_feature, HeadTurnLeft.facial_feature);
        HeadTurnRight.instance('h', FaceRight_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurnRight.percent, 65);
        HeadTurnRight.classify('r', FaceRight_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurn_thresholdTime, FaceRight_displayTrigger);
        HeadTurnRight.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, FaceRight_displayTrigger, VideoTime);
        HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, FaceRight_displayTrigger, HeadTurn_count, VideoTime_string);
        HeadTurnRight.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), VideoTime, HeadTurnStartTime, HeadTurn_count);
        HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);


        driver_monitor LeftEye(shape);
        LeftEye.measure(41, 37, 'y', 39, 36,'x', 100);
        LeftEye.eye_parameters(LeftEye_max, LeftEye_min);

        driver_monitor RightEye(shape);
        RightEye.measure(46, 44, 'y', 45, 42, 'x', 100);
        RightEye.eye_parameters(RightEye_max, RightEye_min);

        driver_monitor Blink(shape);
        Blink.instance('b', Blink_instanceTrigger, VideoTime, BlinkStartTime, LeftEye.percent, Blink_threshold, RightEye.percent, Blink_threshold);
        Blink.classify('b', Blink_instanceTrigger, VideoTime, BlinkStartTime, Blink_thresholdTime, Blink_displayTrigger);
        Blink.instance_rate(Blink_rate, Blink_refreshRate, Blink_thresholdTime, Blink_displayTrigger, VideoTime);
        Blink.instance_rate(SlowBlink_rate, SlowBlink_refreshRate, SlowBlink_thresholdTime, Blink_displayTrigger, VideoTime);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->Blink_tableWidget, Blink_displayTrigger, Blink_count, VideoTime_string);
        Blink.DriverStatus_Drowsy(ui->SlowBlink_ThresholdRate_spinBox->value(), SlowBlink_rate);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);
        Blink.DriverStatus_Asleep(ui->ClosedEyes_TimeLimit_spinBox->value(), VideoTime, BlinkStartTime, Blink_count);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);

        driver_monitor Yawn(shape);
        Yawn.measure(66, 62, 'y', 64, 60, 'x', 100);
        Yawn.instance('y', Yawn_instanceTrigger, VideoTime, YawnStartTime, Yawn.facial_feature, YawnMouth_threshold, LeftEye.percent, YawnEyes_threshold, RightEye.percent, YawnEyes_threshold);
        Yawn.classify('y', Yawn_instanceTrigger, VideoTime, YawnStartTime, Yawn_thresholdTime, Yawn_displayTrigger);
        Yawn.instance_rate(Yawn_rate, Yawn_refreshRate, Yawn_thresholdTime, Yawn_displayTrigger, VideoTime);
        Yawn.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->Yawn_tableWidget, Yawn_displayTrigger, Yawn_count, VideoTime_string);
        Yawn.DriverStatus_Drowsy(ui->Yawn_ThresholdRate_spinBox->value(), Yawn_rate);
        Yawn.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);

        HeadTurn_PlotData.append(double(HeadTurnLeft.percent));
        LeftEye_PlotData.append(LeftEye.percent);
        RightEye_PlotData.append(RightEye.percent);
        Mouth_PlotData.append(Yawn.facial_feature);
        xAxis_PlotData.append(double(HeadTurn_PlotData.size()));

        ui_functions();
    }
    show_frame(frame);
}

void MainWindow::ui_functions()
{
    //Instance threshold level
    if(ui->HeadTurnLeft_spinBox->value() != HeadTurnLeft_threshold)
    {
        HeadTurnLeft_threshold = ui->HeadTurnLeft_spinBox->value();
        ui->HeadTurn_plot->removeItem(HeadTurnLeft_threshold_line);
        HeadTurnLeft_threshold_line = new QCPItemLine(ui->HeadTurn_plot);
        HeadTurnLeft_threshold_line->setPen(QPen(Qt::DashDotLine));
        HeadTurnLeft_threshold_line->start->setCoords(QCPRange::minRange, HeadTurnLeft_threshold);
        HeadTurnLeft_threshold_line->end->setCoords(QCPRange::maxRange, HeadTurnLeft_threshold);
    }

    if(ui->HeadTurnRight_spinBox->value() != HeadTurnRight_threshold)
    {
        HeadTurnRight_threshold = ui->HeadTurnRight_spinBox->value();
        ui->HeadTurn_plot->removeItem(HeadTurnRight_threshold_line);
        HeadTurnRight_threshold_line = new QCPItemLine(ui->HeadTurn_plot);
        HeadTurnRight_threshold_line->setPen(QPen(Qt::DashDotLine));
        HeadTurnRight_threshold_line->start->setCoords(QCPRange::minRange, HeadTurnRight_threshold);
        HeadTurnRight_threshold_line->end->setCoords(QCPRange::maxRange, HeadTurnRight_threshold);
    }

    if(ui->Blink_spinBox->value() != Blink_threshold)
    {
        Blink_threshold = ui->Blink_spinBox->value();
        ui->Blink_plot->removeItem(Blink_threshold_line);
        Blink_threshold_line = new QCPItemLine(ui->Blink_plot);
        Blink_threshold_line->setPen(QPen(Qt::DashDotLine));
        Blink_threshold_line->start->setCoords(QCPRange::minRange, Blink_threshold);
        Blink_threshold_line->end->setCoords(QCPRange::maxRange, Blink_threshold);
    }

    if(ui->YawnMouth_spinBox->value() != YawnMouth_threshold)
    {
        YawnMouth_threshold = ui->YawnMouth_spinBox->value();
        ui->Yawn_plot->removeItem(YawnMouth_threshold_line);
        YawnMouth_threshold_line = new QCPItemLine(ui->Yawn_plot);
        YawnMouth_threshold_line->setPen(QPen(Qt::DashDotLine));
        YawnMouth_threshold_line->start->setCoords(QCPRange::minRange, YawnMouth_threshold);
        YawnMouth_threshold_line->end->setCoords(QCPRange::maxRange, YawnMouth_threshold);
    }

    YawnEyes_threshold = ui->YawnEyes_spinBox->value();

    //Instance data plot update
    ui->HeadTurn_plot->graph(0)->setData(xAxis_PlotData, HeadTurn_PlotData);
    ui->HeadTurn_plot->xAxis->setRange((HeadTurn_PlotData.size()), 100, Qt::AlignRight);
    ui->HeadTurn_plot->replot();
    ui->HeadTurn_plot->update();

    ui->Blink_plot->graph(0)->setData(xAxis_PlotData, LeftEye_PlotData);
    ui->Blink_plot->graph(1)->setData(xAxis_PlotData, RightEye_PlotData);
    ui->Blink_plot->xAxis->setRange((xAxis_PlotData.size()), 100, Qt::AlignRight);
    ui->Blink_plot->replot();
    ui->Blink_plot->update();

    ui->Yawn_plot->graph(0)->setData(xAxis_PlotData, Mouth_PlotData);
    ui->Yawn_plot->xAxis->setRange((xAxis_PlotData.size()), 100, Qt::AlignRight);
    ui->Yawn_plot->replot();
    ui->Yawn_plot->update();

    //Instance trigger indicator
    if(FaceLeft_instanceTrigger == true || FaceRight_instanceTrigger == true)
        ui->HeadTurn_trigger_indicator->setStyleSheet("QLabel { background-color : lime; }");
    else
        ui->HeadTurn_trigger_indicator->setStyleSheet("QLabel { background-color : darkgreen; }");

    if(Blink_instanceTrigger == true)
        ui->Blink_trigger_indicator->setStyleSheet("QLabel { background-color : lime; }");
    else
        ui->Blink_trigger_indicator->setStyleSheet("QLabel { background-color : darkgreen; }");

    if(Yawn_instanceTrigger == true)
        ui->Yawn_trigger_indicator->setStyleSheet("QLabel { background-color : lime; }");
    else
        ui->Yawn_trigger_indicator->setStyleSheet("QLabel { background-color : darkgreen; }");

    //Instance refresh rate
    HeadTurn_refreshRate = ui->RefreshRate_spinBox->value();
    Blink_refreshRate = ui->RefreshRate_spinBox->value();
    SlowBlink_refreshRate = ui->RefreshRate_spinBox->value();
    Yawn_refreshRate = ui->RefreshRate_spinBox->value();

    //Instance threshold time
    HeadTurn_thresholdTime = ui->HeadTurn_ThresholdTime_spinBox->value();
    Blink_thresholdTime = ui->Blink_ThresholdTime_spinBox->value();
    SlowBlink_thresholdTime = ui->SlowBlink_ThresholdTime_spinBox->value();
    Yawn_thresholdTime = ui->Yawn_ThresholdTime_spinBox->value();

    //Text displays
//    ui->textBrowser->setText(main_string);
//    QScrollBar *instance_string_sb = ui->textBrowser->verticalScrollBar();
//    instance_string_sb->setValue(instance_string_sb->maximum());

    //Instance rate displays
    QString HeadTurn_rate_string;
    HeadTurn_rate_string += "Head turn rate(";
    HeadTurn_rate_string += QString::number(HeadTurn_refreshRate);
    HeadTurn_rate_string += "s): ";
    HeadTurn_rate_string += QString::number(HeadTurn_rate.size());
    ui->HeadTurn_rate_text->setText(HeadTurn_rate_string);

    QString Blink_rate_string;
    Blink_rate_string.append("Blink rate (");
    Blink_rate_string.append(QString::number(Blink_refreshRate));
    Blink_rate_string.append("s): ");
    Blink_rate_string.append(QString::number(Blink_rate.size()));
    ui->Blink_rate_text->setText(Blink_rate_string);

    QString SlowBlink_rate_string;
    SlowBlink_rate_string.append("Slow Blink rate (");
    SlowBlink_rate_string.append(QString::number(SlowBlink_refreshRate));
    SlowBlink_rate_string.append("s): ");
    SlowBlink_rate_string.append(QString::number(SlowBlink_rate.size()));
    ui->SlowBlink_rate_text->setText(SlowBlink_rate_string);

    QString Yawn_rate_string;
    Yawn_rate_string.append("Yawn rate (");
    Yawn_rate_string.append(QString::number(Yawn_refreshRate));
    Yawn_rate_string.append("s): ");
    Yawn_rate_string.append(QString::number(Yawn_rate.size()));
    ui->Yawn_rate_text->setText(Yawn_rate_string);

    //Face layout color settings
    ui->blue_horizontalSlider->setRange(0, 255);
    blue = ui->blue_horizontalSlider->value();

    ui->green_horizontalSlider->setRange(0, 255);
    green = ui->green_horizontalSlider->value();

    ui->red_horizontalSlider->setRange(0, 255);
    red = ui->red_horizontalSlider->value();

    //Current time label
    QTime time = QTime::currentTime();
    QString time_string = time.toString("hh:mm:ss ap");
    ui->CurrentTime_label->setText(time_string);

    //QTableWidgetItem font size
    QFont font;
    font.setPointSize(8);

    for(int i = 0; i < (ui->Main_tableWidget->rowCount()); i++)
    {
        for(int j = 0; j < (ui->Main_tableWidget->columnCount()); j++)
        {
            QTableWidgetItem *item = ui->Main_tableWidget->item(i, j);
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    for(int i = 0; i < (ui->HeadTurn_tableWidget->rowCount()); i++)
    {
        for(int j = 0; j < (ui->HeadTurn_tableWidget->columnCount()); j++)
        {
            QTableWidgetItem *item = ui->HeadTurn_tableWidget->item(i, j);
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    for(int i = 0; i < (ui->Blink_tableWidget->rowCount()); i++)
    {
        for(int j = 0; j < (ui->Blink_tableWidget->columnCount()); j++)
        {
            QTableWidgetItem *item = ui->Blink_tableWidget->item(i, j);
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);
        }
    }

    for(int i = 0; i < (ui->Yawn_tableWidget->rowCount()); i++)
    {
        for(int j = 0; j < (ui->Yawn_tableWidget->columnCount()); j++)
        {
            QTableWidgetItem *item = ui->Yawn_tableWidget->item(i, j);
            item->setFont(font);
            item->setTextAlignment(Qt::AlignCenter);
        }
    }


    int tableWidget_width = ui->Main_tableWidget->horizontalHeader()->length()
                          + ui->Main_tableWidget->verticalScrollBar()->size().width()
                          + ui->Main_tableWidget->verticalHeader()->width();

    ui->Main_tableWidget->setMinimumWidth(tableWidget_width);
    ui->HeadTurn_tableWidget->setMinimumWidth(tableWidget_width);
    ui->Blink_tableWidget->setMinimumWidth(tableWidget_width);
    ui->Yawn_tableWidget->setMinimumWidth(tableWidget_width);

    ui->tabWidget->setMinimumWidth(tableWidget_width + 20);

    //QTableWidget auto scroll
    ui->Main_tableWidget->scrollToBottom();
    ui->HeadTurn_tableWidget->scrollToBottom();
    ui->Blink_tableWidget->scrollToBottom();
    ui->Yawn_tableWidget->scrollToBottom();
}

void MainWindow::show_frame(Mat &image)
{
//    Mat resized_image = image.clone();

//    int width_of_label = ui->label_camera->width();
//    int height_of_label = ui->label_camera->height();

//    Size size(width_of_label, height_of_label);

//    cv::resize(image, resized_image, size);
    cvtColor(image, image, CV_BGR2RGB);
    QImage qt_image((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);
    ui->label_camera->setPixmap(QPixmap::fromImage(qt_image));
}

void MainWindow::face_layout(const dlib::full_object_detection& shape, Mat image)
{
    std::vector<cv::Point> points;
    int start_point[] = {0, 17, 22, 27, 30, 36, 42, 48, 60};
    int end_point[] = {16, 21, 26, 30, 35, 41, 47, 59, 67};
    bool isClosed;

    for(int j = 0; j <= 8 ; j++)
    {
        for(int i = start_point[j]; i <= end_point[j]; ++i)
        {
            points.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
        }
        if(j < 4)
            isClosed = false;
        else
            isClosed = true;
        cv::polylines(image, points, isClosed, cv::Scalar(blue, green, red), 1, 16);
        points.clear();
    }
}

void MainWindow::ResetData()
{
    FaceLeft_instanceTrigger = false;
    FaceRight_instanceTrigger = false;
    Blink_instanceTrigger = false;
    Yawn_instanceTrigger = false;

    HeadTurn_count = 0;
    Blink_count = 0;
    Yawn_count = 0;

    HeadTurn_rate.clear();
    Blink_rate.clear();
    SlowBlink_rate.clear();
    Yawn_rate.clear();

    HeadTurn_PlotData.clear();
    LeftEye_PlotData.clear();
    RightEye_PlotData.clear();
    Mouth_PlotData.clear();
    xAxis_PlotData.clear();

    for(int i = ui->Main_tableWidget->rowCount(); i >= 0; --i)
    {
        ui->Main_tableWidget->removeRow(i);
    }

    for(int i = ui->HeadTurn_tableWidget->rowCount(); i >= 0; --i)
    {
        ui->HeadTurn_tableWidget->removeRow(i);
    }

    for(int i = ui->Blink_tableWidget->rowCount(); i >= 0; --i)
    {
        ui->Blink_tableWidget->removeRow(i);
    }

    for(int i = ui->Yawn_tableWidget->rowCount(); i >= 0; --i)
    {
        ui->Yawn_tableWidget->removeRow(i);
    }
}

void MainWindow::on_pushButton_reset_clicked()
{
    if(ui->ResetData_checkBox->isChecked() == true)
    {
        ResetData();
    }

    if(ui->ResetSettings_checkBox->isChecked() == true)
    {
        LeftEye_max = 0;
        RightEye_max = 0;
        LeftEye_min = 100;
        RightEye_min = 100;

        ui->HeadTurnLeft_spinBox->setValue(65);
        ui->HeadTurnRight_spinBox->setValue(35);
        ui->Blink_spinBox->setValue(25);
        ui->YawnEyes_spinBox->setValue(35);
        ui->YawnMouth_spinBox->setValue(30);

        ui->HeadTurn_ThresholdTime_spinBox->setValue(0);
        ui->Blink_ThresholdTime_spinBox->setValue(0);
        ui->SlowBlink_ThresholdTime_spinBox->setValue(200);
        ui->Yawn_ThresholdTime_spinBox->setValue(200);

        ui->HeadTurn_TimeLimit_spinBox->setValue(2000);
        ui->SlowBlink_ThresholdRate_spinBox->setValue(3);
        ui->ClosedEyes_TimeLimit_spinBox->setValue(2000);
        ui->Yawn_ThresholdRate_spinBox->setValue(3);

        ui->RefreshRate_spinBox->setValue(10);
    }
}

void MainWindow::on_SaveOutputData_pushButton_clicked()
{
    QString FolderPath = ui->FolderPath_lineEdit->text();
    QString FileName = ui->FileName_lineEdit->text();
    int ExistingFileIndex = 0;
    QString FilePath;
    while(1)
    {
        FilePath.clear();
        FilePath += FolderPath;
        FilePath += FileName;
        if(ExistingFileIndex != 0)
            FilePath += QString::number(ExistingFileIndex);
        FilePath += ".csv";
        QFileInfo *check_file = new QFileInfo(FilePath);
        if(!check_file->exists())
            break;
        ExistingFileIndex++;
        cout << "File exists" << endl;
    }

    QFile file(FilePath);
    QTextStream out(&file);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "could not open" << endl;
        return;
    }
    for(int i = 0; i < ui->Main_tableWidget->rowCount(); i++)
    {
        if(ui->includeCount_checkBox->isChecked())
            out << i + 1 << ", ";
        for(int j = 0; j < ui->Main_tableWidget->columnCount(); j++)
        {
            out << ui->Main_tableWidget->item(i, j)->text() << ", ";
        }
        out << "\n";
    }
    file.flush();
    file.close();
}



void MainWindow::on_PauseVideo_pushButton_clicked()
{
    PauseVideo = true;
}

void MainWindow::on_StopVideo_pushButton_clicked()
{
    StopVideo = true;
}
