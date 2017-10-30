#include "mainwindow.h"
#include "ui_mainwindow.h"

QCPItemLine *HeadTurnLeft_ThresholdLine;
QCPItemLine *HeadTurnRight_ThresholdLine;
QCPItemLine *Blink_ThresholdLine;
QCPItemLine *YawnMouth_ThresholdLine;
//QCPItemLine *MouthWidth_ThresholdLine;
QCPItemLine *SideMouth_ThresholdLine;

QCPItemLine *SmileBlink_ThresholdLine;
QCPItemLine *HeadTurnBlink_ThresholdLine;

QString VideoFilePath;

QTime VideoTimer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chat = new Chat(this);
    chat->show();
    connect(chat, SIGNAL(Distracted(QString)), this, SLOT(fromBluetooth(QString)));
    connect(this, SIGNAL(passToBluetooth(QString)), chat, SLOT(sendClicked(QString)));

    this->setWindowState(Qt::WindowMaximized);
    timer = new QTimer(this);

//    ui->Smile_widget->addGraph();
//    ui->Smile_widget->yAxis->setRange(0, 250);
//    ui->Smile_widget->setInteractions(QCP::iSelectItems | QCP::iRangeDrag);
//    ui->Smile_widget->axisRect()->setRangeDrag(Qt::Horizontal);
//    ui->Smile_widget->graph(0)->setLineStyle(QCPGraph::lsLine);

//    MouthWidth_ThresholdLine = new QCPItemLine(ui->Smile_widget);
//    MouthWidth_ThresholdLine->setSelectable(true);
//    MouthWidth_ThresholdLine->setPen(QPen(Qt::DashDotLine));
//    MouthWidth_ThresholdLine->start->setCoords(QCPRange::minRange, 190);
//    MouthWidth_ThresholdLine->end->setCoords(QCPRange::maxRange, 190);

    ui->SideMouth_plot->addGraph();
    ui->SideMouth_plot->addGraph();
    ui->SideMouth_plot->yAxis->setRange(-20, 70);
    ui->SideMouth_plot->setInteractions(QCP::iSelectItems | QCP::iRangeDrag);
    ui->SideMouth_plot->axisRect()->setRangeDrag(Qt::Horizontal);

    ui->SideMouth_plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->SideMouth_plot->graph(1)->setLineStyle(QCPGraph::lsLine);

    ui->SideMouth_plot->graph(0)->setPen(QPen(Qt::red));
    ui->SideMouth_plot->graph(1)->setPen(QPen(Qt::green));

    SideMouth_ThresholdLine = new QCPItemLine(ui->SideMouth_plot);
    SideMouth_ThresholdLine->setSelectable(true);
    SideMouth_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    SideMouth_ThresholdLine->start->setCoords(QCPRange::minRange, ui->LiptoSideMouth_spinBox->value());
    SideMouth_ThresholdLine->end->setCoords(QCPRange::maxRange, ui->LiptoSideMouth_spinBox->value());

    SmileBlink_ThresholdLine = new QCPItemLine(ui->Blink_plot);
    SmileBlink_ThresholdLine->setSelectable(true);
    SmileBlink_ThresholdLine->setPen(QPen(Qt::darkMagenta));
    SmileBlink_ThresholdLine->start->setCoords(QCPRange::minRange, ui->SmileBlink_spinBox->value());
    SmileBlink_ThresholdLine->end->setCoords(QCPRange::maxRange, ui->SmileBlink_spinBox->value());

    HeadTurnBlink_ThresholdLine = new QCPItemLine(ui->Blink_plot);
    HeadTurnBlink_ThresholdLine->setSelectable(true);
    HeadTurnBlink_ThresholdLine->setPen(QPen(Qt::blue));
    HeadTurnBlink_ThresholdLine->start->setCoords(QCPRange::minRange, ui->HeadTurnBlink_spinBox->value());
    HeadTurnBlink_ThresholdLine->end->setCoords(QCPRange::maxRange, ui->HeadTurnBlink_spinBox->value());




    ui->HeadTurn_plot->addGraph();

    ui->HeadTurn_plot->yAxis->setRange(0, 100);
    ui->HeadTurn_plot->setInteractions(QCP::iSelectItems | QCP::iRangeDrag);
    ui->HeadTurn_plot->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->HeadTurn_plot->graph(0)->setLineStyle(QCPGraph::lsLine);

    HeadTurnLeft_ThresholdLine = new QCPItemLine(ui->HeadTurn_plot);
    HeadTurnLeft_ThresholdLine->setSelectable(true);
    HeadTurnLeft_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    HeadTurnLeft_ThresholdLine->start->setCoords(QCPRange::minRange, ui->HeadTurnLeft_spinBox->value());
    HeadTurnLeft_ThresholdLine->end->setCoords(QCPRange::maxRange, ui->HeadTurnLeft_spinBox->value());

    HeadTurnRight_ThresholdLine = new QCPItemLine(ui->HeadTurn_plot);
    HeadTurnRight_ThresholdLine->setSelectable(true);
    HeadTurnRight_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    HeadTurnRight_ThresholdLine->start->setCoords(QCPRange::minRange, ui->HeadTurnRight_spinBox->value());
    HeadTurnRight_ThresholdLine->end->setCoords(QCPRange::maxRange, ui->HeadTurnRight_spinBox->value());

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

    Blink_ThresholdLine = new QCPItemLine(ui->Blink_plot);
    Blink_ThresholdLine->setSelectable(true);
    Blink_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    Blink_ThresholdLine->start->setCoords(QCPRange::minRange, ui->Blink_spinBox->value());
    Blink_ThresholdLine->end->setCoords(QCPRange::maxRange, ui->Blink_spinBox->value());

    ui->Yawn_plot->addGraph();

    ui->Yawn_plot->yAxis->setRange(0, 100);
    ui->Yawn_plot->setInteractions(QCP::iSelectItems | QCP::iRangeDrag);
    ui->Yawn_plot->axisRect()->setRangeDrag(Qt::Horizontal);

    ui->Yawn_plot->graph(0)->setLineStyle((QCPGraph::lsLine));
    ui->Yawn_plot->graph(0)->setPen(QPen(Qt::green));

    YawnMouth_ThresholdLine = new QCPItemLine(ui->Yawn_plot);
    YawnMouth_ThresholdLine->setSelectable(true);
    YawnMouth_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    YawnMouth_ThresholdLine->start->setCoords(QCPRange::minRange, ui->YawnMouth_spinBox->value());
    YawnMouth_ThresholdLine->end->setCoords(QCPRange::maxRange, ui->YawnMouth_spinBox->value());

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

    try
    {
    deserialize("shape_predictor_68_face_landmarks.dat") >> shape_model;
    }

    catch (serialization_error &e)
    {
        while(true)
        {
            ui->statusBar->showMessage("Failed to load shape predictor", 0);
            QString shapePredictorPath = QFileDialog::getOpenFileName(this, tr("Open Shape Predictor File"), VideoFilePath, "(*.dat)");
            try
            {
                deserialize(shapePredictorPath.toStdString()) >> shape_model;
                ui->statusBar->clearMessage();
                break;
            }
            catch(serialization_error &e)
            {

            }
        }
    }

    connect(timer, SIGNAL(timeout()), this, SLOT(DisplayCurrentTime()));
    timer->start(20);

    alert->setMedia(QUrl("qrc:/sound/alarm.mp3"));

    ui->comboBox->setCurrentIndex(0);
    connect(timer, SIGNAL(timeout()), this, SLOT(uiFunctions()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::passToBluetooth(const QString &message)
{
    if(message == "Open Camera\n")
    {
        on_OpenCamera_pushButton_clicked();
    }
    else if(message == "Close Camera\n")
    {
        on_CloseCamera_pushButton_clicked();
    }
    else if(message == "Enable Distracted\n")
    {
        enableDistracted = true;
    }
    else if(message == "Disable Distracted\n")
    {
        enableDistracted = false;
    }
}

void MainWindow::on_OpenCamera_pushButton_clicked()
{
    cap.open(ui->comboBox->currentIndex());
    if(!cap.isOpened())
    {
        ui->statusBar->showMessage("Failed to open camera", 2000);
    }
    else
    {
        cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
        ui->statusBar->showMessage("Camera is open", 2000);
        ui->OriginalFrameDelay_label->setText("0");

        connect(timer, SIGNAL(timeout()), this, SLOT(ProcessCameraFrame()));
        timer->start(20);
        ui->CloseCamera_pushButton->setEnabled(true);
    }
}

void MainWindow::on_CloseCamera_pushButton_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(ProcessCameraFrame()));
    cap.release();
    Mat image = Mat::zeros(frame.size(), CV_8UC3);
    show_frame(image);

    ui->statusBar->showMessage("Camera is closed", 2000);

    ResetTriggers();
}

void MainWindow::on_LoadVideo_pushButton_clicked()
{
    //QString VideoPath;
    VideoPath = QFileDialog::getOpenFileName(this, tr("Open Video File"), VideoFilePath, "MPEG-4 (*.mp4)");
    VideoFilePath = QFileInfo(VideoPath).path();
    ui->VideoInputPath_lineEdit->setText(QFileInfo(VideoPath).fileName());
    cap.open(VideoPath.toStdString());
    if(!cap.isOpened())
        ui->statusBar->showMessage("Video file is not loaded", 2000);
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
    if((VideoDuration - trunc(VideoDuration / 60) * 60) < 10)
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
    ui->Video_horizontalSlider->setMaximum(VideoDuration * 1000);

    int OriginalFrameDelay = 1000 / cap.get(CV_CAP_PROP_FPS);
    ui->OriginalFrameDelay_label->setText(QString::number(OriginalFrameDelay));

    ui->statusBar->showMessage("Finished loading video", 2000);
    cap.release();
    ResetTriggers();
}

void MainWindow::on_PlayVideo_pushButton_clicked()
{
    cap.open(VideoPath.toStdString());
    if(!cap.isOpened())
    {
        ui->statusBar->showMessage("Failed to open video file", 2000);
    }
    else
    {
        connect(timer, SIGNAL(timeout()), this, SLOT(ProcessVideoFrame()));
        timer->start(20);
        if(PauseVideo == true)
        {
            cap.set(CV_CAP_PROP_POS_MSEC, PauseTime);
            PauseVideo = false;
        }
        else if(ui->ProcessVideo_checkBox->isChecked())
        {
            ResetData();
            ResetTriggers();
        }
    }
}

void MainWindow::ProcessVideoFrame()
{
    VideoTimer.restart();

    cap >> frame;
    RotateImage();
//    ResizeImage();
//    int FrameWidth = ui->FrameWidth_spinBox->value();
//    int FrameHeight = ui->FrameHeight_spinBox->value();
//    if(Rotate == 1 || Rotate == 3)
//        cv::resize(frame, frame, Size(FrameHeight, FrameWidth), 0, 0, INTER_CUBIC);
//    else
//        cv::resize(frame, frame, Size(FrameWidth, FrameHeight), 0, 0, INTER_CUBIC);
    if(Rotate == 0 || Rotate == 2)
    {
        double resizedHeight;
        resizedHeight = 640 * (cap.get(CV_CAP_PROP_FRAME_HEIGHT) / cap.get(CV_CAP_PROP_FRAME_WIDTH));
        cv::resize(frame, frame, Size(640, resizedHeight), 0, 0, INTER_CUBIC);
    }
    else if(Rotate == 1 || Rotate == 3)
    {
        double resizedWidth;
        resizedWidth = 640 * (cap.get(CV_CAP_PROP_FRAME_HEIGHT) / cap.get(CV_CAP_PROP_FRAME_WIDTH));
        cv::resize(frame, frame, Size(resizedWidth, 640), 0, 0, INTER_AREA);
    }

    double VideoTime = trunc(cap.get(CV_CAP_PROP_POS_MSEC));
    ui->Video_horizontalSlider->setValue(VideoTime);

    QString VideoTime_minute;
    if(trunc((VideoTime / 1000) / 60) < 10)
    {
        VideoTime_minute += QString::number(0);
        VideoTime_minute += QString::number(trunc((VideoTime / 1000) / 60));
    }
    else
    {
        VideoTime_minute += QString::number(trunc((VideoTime / 1000) / 60));
    }
    QString VideoTime_second;
    if((trunc(VideoTime / 1000) - trunc((VideoTime / 1000) / 60) * 60) < 10)
    {
        VideoTime_second += QString::number(0);
        VideoTime_second += QString::number(trunc(VideoTime / 1000) - trunc((VideoTime / 1000) / 60) * 60);
    }
    else
    {
        VideoTime_second += QString::number(trunc(VideoTime / 1000) - trunc((VideoTime / 1000) / 60) * 60);
    }
    QString VideoTime_string;
    VideoTime_string += VideoTime_minute;
    VideoTime_string += ':';
    VideoTime_string += VideoTime_second;
    ui->VideoTime_label->setText(VideoTime_string);

    if(cap.get(CV_CAP_PROP_FRAME_COUNT) > 0 && cap.get(CV_CAP_PROP_POS_FRAMES) == cap.get(CV_CAP_PROP_FRAME_COUNT) - 1)
    {
        cap.release();
        disconnect(timer, SIGNAL(timeout()), this, SLOT(ProcessVideoFrame()));
        timer->start(20);
    }

    if(ui->ProcessVideo_checkBox->isChecked())
        {
        array2d<bgr_pixel> dlib_image;
        assign_image(dlib_image, cv_image<bgr_pixel>(frame));

        std::vector<dlib::rectangle> faces = detector(dlib_image);

        std::vector<full_object_detection> shapes;
        for(unsigned long i = 0; i < faces.size(); i++)
        {
            full_object_detection shape = shape_model(dlib_image, faces[i]);
            shapes.push_back(shape);

            if(faces.size() > 1)
            {
                for(int i = 0; faces.size() > 1; i++)
                {
//                    if(faces.front().area() > faces.back().area())
//                        faces.pop_back();
//                    else
//                        faces.erase(faces.begin());
                    int face1, face2;
                    face1 = abs(shapes.front().part(30).x() - ui->label_camera->width()/2);
                    face2 = abs(shapes.back().part(30).x() - ui->label_camera->width()/2);

                    if(face1 < face2)
                        faces.pop_back();
                    else
                        faces.erase(faces.begin());
                }
            }


            face_layout(shape, frame);

            driver_monitor HeadTurnLeft(shape);
            HeadTurnLeft.measure(30, 36, 'x', 35, 31, 'x', 10);

            driver_monitor HeadTurnRight(shape);
            HeadTurnRight.measure(45, 30, 'x', 35, 31, 'x', 10);

            HeadTurnLeft.head_parameters(HeadTurnLeft.facial_feature, HeadTurnRight.facial_feature);
            HeadTurnLeft.instance('h', HeadTurnLeft_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurnLeft.percent, HeadTurnLeft_ThresholdLevel);
            HeadTurnLeft.classify('l', HeadTurnLeft_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurn_thresholdTime, HeadTurnLeft_displayTrigger);
            HeadTurnLeft.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, HeadTurnLeft_displayTrigger, VideoTime);
            HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, HeadTurnLeft_displayTrigger, HeadTurn_count, VideoTime_string);
            HeadTurnLeft.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), VideoTime, HeadTurnStartTime, HeadTurn_count);
            HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);

            HeadTurnRight.head_parameters(HeadTurnRight.facial_feature, HeadTurnLeft.facial_feature);
            HeadTurnRight.instance('h', HeadTurnRight_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurnRight.percent, 100 - HeadTurnRight_ThresholdLevel);
            HeadTurnRight.classify('r', HeadTurnRight_instanceTrigger, VideoTime, HeadTurnStartTime, HeadTurn_thresholdTime, HeadTurnRight_displayTrigger);
            HeadTurnRight.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, HeadTurnRight_displayTrigger, VideoTime);
            HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, HeadTurnRight_displayTrigger, HeadTurn_count, VideoTime_string);
            HeadTurnRight.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), VideoTime, HeadTurnStartTime, HeadTurn_count);
            HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);

            //cout << HeadTurn_count << endl;

            driver_monitor LeftEye(shape);
            LeftEye.measure(41, 37, 'y', 39, 36,'x', 100);
            LeftEye.eye_parameters(LeftEye_max, LeftEye_min);

            driver_monitor RightEye(shape);
            RightEye.measure(46, 44, 'y', 45, 42, 'x', 100);
            RightEye.eye_parameters(RightEye_max, RightEye_min);

            driver_monitor Blink(shape);

            if(HeadTurnLeft_instanceTrigger == true)
            {
                LeftEye.percent = RightEye.percent;
                Blink_ThresholdLevel = HeadTurnBlink_ThresholdLevel;
            }
            if(HeadTurnRight_instanceTrigger == true)
            {
                RightEye.percent = LeftEye.percent;
                Blink_ThresholdLevel = HeadTurnBlink_ThresholdLevel;
            }
            if(SmileStatusIndicator == true || Yawn_instanceTrigger == true)
            {
                if(LeftEye.percent > RightEye.percent)
                    RightEye.percent = LeftEye.percent;
                else
                    LeftEye.percent = RightEye.percent;

                Blink_ThresholdLevel = SmileBlink_ThresholdLevel;
            }

            Blink.instance('b', Blink_instanceTrigger, VideoTime, BlinkStartTime, LeftEye.percent, Blink_ThresholdLevel, RightEye.percent, Blink_ThresholdLevel);

            if(HeadTurnLeft_instanceTrigger == true || HeadTurnRight_instanceTrigger == true || SmileStatusIndicator == true || Yawn_instanceTrigger == true)
            {
                Blink_ThresholdLevel = ui->Blink_spinBox->value();
            }

            if(ui->checkBox_BlinkYawn->isChecked())
            {
                if(Yawn_instanceTrigger == true && Blink_instanceTrigger == true)
                        Blink_instanceTrigger = false;
            }

            Blink.classify('b', Blink_instanceTrigger, VideoTime, BlinkStartTime, Blink_thresholdTime, Blink_displayTrigger);
            Blink.instance_rate(Blink_rate, Blink_refreshRate, Blink_thresholdTime, Blink_displayTrigger, VideoTime);
            Blink.instance_rate(SlowBlink_rate, SlowBlink_refreshRate, SlowBlink_thresholdTime, Blink_displayTrigger, VideoTime);
            Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->Blink_tableWidget, Blink_displayTrigger, Blink_count, VideoTime_string);
            Blink.DriverStatus_Drowsy(ui->Blink_ThresholdRate_spinBox->value(), Blink_rate);
            Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);

            Blink.DriverStatus_Drowsy(ui->SlowBlink_ThresholdRate_spinBox->value(), SlowBlink_rate);
            Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);
            Blink.DriverStatus_Asleep(ui->ClosedEyes_TimeLimit_spinBox->value(), VideoTime, BlinkStartTime, Blink_count);
            Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, VideoTime_string);

            driver_monitor Yawn(shape);
            Yawn.measure(66, 62, 'y', 64, 60, 'x', 100);
            Yawn.instance('y', Yawn_instanceTrigger, VideoTime, YawnStartTime, Yawn.facial_feature, YawnMouth_ThresholdLevel, LeftEye.percent, YawnEyes_ThresholdLevel, RightEye.percent, YawnEyes_ThresholdLevel);
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


            driver_monitor SmileY1(shape);
            SmileY1.measure(66, 54, 'y', 35, 31, 'x', 100);
            driver_monitor SmileY2(shape);
            SmileY2.measure(66, 48, 'y', 35, 31, 'x', 100);

//            cout << SmileY1.facial_feature << ' '
//               << SmileY2.facial_feature << ' '
//               << LeftEye.percent << ' '
//               << RightEye.percent << ' ';
               //<< endl;

            if(SmileY1.facial_feature > SmileThresholdLevel && SmileY2.facial_feature > SmileThresholdLevel)
            {
                SmileStatusIndicator = true;
                cout << "Smiling" << endl;
            }
            else
            {
                SmileStatusIndicator = false;
                cout << "Not Smiling" << endl;
            }

//            MouthWidth_PlotData.append(Smile.facial_feature);

            SideMouth1_PlotData.append(SmileY1.facial_feature);
            SideMouth2_PlotData.append(SmileY2.facial_feature);

//        if(Smile.facial_feature > 200)
//            cout << "Smiling" << endl;
//        else
//            cout << "Not Smiling" << endl;

//            uiFunctions();
        }
    }
    show_frame(frame);
    ui->EffectiveFrameDelay_label->setText(QString::number(VideoTimer.elapsed()));
}

void MainWindow::ProcessCameraFrame()
{
    VideoTimer.restart();
    cap >> frame;

    array2d<bgr_pixel> dlib_image;
    assign_image(dlib_image, cv_image<bgr_pixel>(frame));

    std::vector<dlib::rectangle> faces = detector(dlib_image);

    std::vector<full_object_detection> shapes;
    for(unsigned long i = 0; i < faces.size(); i++)
    {
        full_object_detection shape = shape_model(dlib_image, faces[i]);
        shapes.push_back(shape);

        face_layout(shape, frame);

        if(faces.size() > 1)
        {
            for(int i = 0; faces.size() > 1; i++)
            {
//                    if(faces.front().area() > faces.back().area())
//                        faces.pop_back();
//                    else
//                        faces.erase(faces.begin());
                int face1, face2;
                face1 = abs(shapes.front().part(30).x() - ui->label_camera->width()/2);
                face2 = abs(shapes.back().part(30).x() - ui->label_camera->width()/2);

                if(face1 < face2)
                    faces.pop_back();
                else
                    faces.erase(faces.begin());
            }
        }

        driver_monitor HeadTurnLeft(shape);
        HeadTurnLeft.measure(30, 36, 'x', 35, 31, 'x', 10);

        driver_monitor HeadTurnRight(shape);
        HeadTurnRight.measure(45, 30, 'x', 35, 31, 'x', 10);

        HeadTurnLeft.head_parameters(HeadTurnLeft.facial_feature, HeadTurnRight.facial_feature);
        HeadTurnLeft.instance('h', HeadTurnLeft_instanceTrigger, HeadTurn_timer, HeadTurnLeft.percent, HeadTurnLeft_ThresholdLevel);
        HeadTurnLeft.classify('l', HeadTurnLeft_instanceTrigger, HeadTurn_timer, HeadTurn_thresholdTime, HeadTurnLeft_displayTrigger);
        HeadTurnLeft.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, HeadTurnLeft_displayTrigger);
        HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, HeadTurnLeft_displayTrigger, HeadTurn_count);

        if(enableDistracted == true)
        {
            HeadTurnLeft.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), HeadTurn_timer, HeadTurn_count);
            if(!HeadTurnLeft.DriverStatus_string.isEmpty())
                emit passToBluetooth("Alarm");
        }
        HeadTurnLeft.DisplayTo_QTableWidget(ui->Main_tableWidget);

        HeadTurnRight.head_parameters(HeadTurnRight.facial_feature, HeadTurnLeft.facial_feature);
        HeadTurnRight.instance('h', HeadTurnRight_instanceTrigger, HeadTurn_timer, HeadTurnRight.percent, 100 - HeadTurnRight_ThresholdLevel);
        HeadTurnRight.classify('r', HeadTurnRight_instanceTrigger, HeadTurn_timer, HeadTurn_thresholdTime, HeadTurnRight_displayTrigger);
        HeadTurnRight.instance_rate(HeadTurn_rate, HeadTurn_refreshRate, HeadTurn_thresholdTime, HeadTurnRight_displayTrigger);
        HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->HeadTurn_tableWidget, HeadTurnRight_displayTrigger, HeadTurn_count);

        if(enableDistracted == true)
        {
            HeadTurnRight.DriverStatus_Distracted(ui->HeadTurn_TimeLimit_spinBox->value(), HeadTurn_timer, HeadTurn_count);
            if(!HeadTurnRight.DriverStatus_string.isEmpty())
                emit passToBluetooth("Alarm");
        }
        HeadTurnRight.DisplayTo_QTableWidget(ui->Main_tableWidget);

        if(!HeadTurnLeft.DriverStatus_string.isEmpty() || !HeadTurnRight.DriverStatus_string.isEmpty())
        {
            if(alert->state() == QMediaPlayer::PlayingState)
                alert->setPosition(0);
            else
                alert->play();
        }

        driver_monitor LeftEye(shape);
        LeftEye.measure(41, 37, 'y', 39, 36,'x', 100);
        LeftEye.eye_parameters(LeftEye_max, LeftEye_min);

        driver_monitor RightEye(shape);
        RightEye.measure(46, 44, 'y', 45, 42, 'x', 100);
        RightEye.eye_parameters(RightEye_max, RightEye_min);

        driver_monitor Blink(shape);

        if(HeadTurnLeft_instanceTrigger == true)
        {
            LeftEye.percent = RightEye.percent;
            Blink_ThresholdLevel = HeadTurnBlink_ThresholdLevel;
        }
        if(HeadTurnRight_instanceTrigger == true)
        {
            RightEye.percent = LeftEye.percent;
            Blink_ThresholdLevel = HeadTurnBlink_ThresholdLevel;
        }
        if(SmileStatusIndicator == true || Yawn_instanceTrigger == true)
        {
            if(LeftEye.percent > RightEye.percent)
                RightEye.percent = LeftEye.percent;
            else
                LeftEye.percent = RightEye.percent;

            Blink_ThresholdLevel = SmileBlink_ThresholdLevel;
        }


//        cout << Blink_ThresholdLevel << ' ';

        Blink.instance('b', Blink_instanceTrigger, Blink_timer, LeftEye.percent, Blink_ThresholdLevel, RightEye.percent, Blink_ThresholdLevel);

        if(HeadTurnLeft_instanceTrigger == true || HeadTurnRight_instanceTrigger == true || SmileStatusIndicator == true || Yawn_instanceTrigger == true)
        {
            Blink_ThresholdLevel = ui->Blink_spinBox->value();
        }

//        cout << Blink_ThresholdLevel << endl;
        if(ui->checkBox_BlinkYawn->isChecked())
        {
            if(Yawn_instanceTrigger == true && Blink_instanceTrigger == true)
                    Blink_instanceTrigger = false;
        }

        Blink.classify('b', Blink_instanceTrigger, Blink_timer, Blink_thresholdTime, Blink_displayTrigger);
        Blink.instance_rate(Blink_rate, Blink_refreshRate, Blink_thresholdTime, Blink_displayTrigger);
        Blink.instance_rate(SlowBlink_rate, SlowBlink_refreshRate, SlowBlink_thresholdTime, Blink_displayTrigger);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget, ui->Blink_tableWidget, Blink_displayTrigger, Blink_count);
        Blink.DriverStatus_Drowsy(ui->Blink_ThresholdRate_spinBox->value(), Blink_rate);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget);

        Blink.DriverStatus_Drowsy(ui->SlowBlink_ThresholdRate_spinBox->value(), SlowBlink_rate);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget);
        Blink.DriverStatus_Asleep(ui->ClosedEyes_TimeLimit_spinBox->value(), Blink_timer, Blink_count);
        Blink.DisplayTo_QTableWidget(ui->Main_tableWidget);

        driver_monitor Yawn(shape);
        Yawn.measure(66, 62, 'y', 64, 60, 'x', 100);
        Yawn.instance('y', Yawn_instanceTrigger, Yawn_timer, Yawn.facial_feature, YawnMouth_ThresholdLevel, LeftEye.percent, YawnEyes_ThresholdLevel, RightEye.percent, YawnEyes_ThresholdLevel);
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

//        cout << HeadTurn_count
//             << ' '
//             << Blink_count
//             << ' '
//             << Yawn_count
//             << endl;


          driver_monitor SmileY1(shape);
          SmileY1.measure(66, 54, 'y', 35, 31, 'x', 100);
          driver_monitor SmileY2(shape);
          SmileY2.measure(66, 48, 'y', 35, 31, 'x', 100);



          if(SmileY1.facial_feature > SmileThresholdLevel && SmileY2.facial_feature > SmileThresholdLevel)
          {
              SmileStatusIndicator = true;
              cout << "Smiling" << endl;
          }
          else
          {
              SmileStatusIndicator = false;
              cout << "Not Smiling" << endl;
          }

//        MouthWidth_PlotData.append(Smile.facial_feature);

        SideMouth1_PlotData.append(SmileY1.facial_feature);
        SideMouth2_PlotData.append(SmileY2.facial_feature);

//        if(Smile.facial_feature > 200)
//            cout << "Smiling" << endl;
//        else
//            cout << "Not Smiling" << endl;

//        uiFunctions();
    }
    show_frame(frame);
    ui->EffectiveFrameDelay_label->setText(QString::number(VideoTimer.elapsed()));
}

void MainWindow::uiFunctions()
{
    HeadTurnBlink_ThresholdLevel = ui->HeadTurnBlink_spinBox->value();
    SmileBlink_ThresholdLevel = ui->SmileBlink_spinBox->value();
    SmileThresholdLevel = ui->LiptoSideMouth_spinBox->value();


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

//    ui->Smile_widget->graph(0)->setData(xAxis_PlotData, MouthWidth_PlotData);
//    ui->Smile_widget->xAxis->setRange(xAxis_PlotData.size(), 100, Qt::AlignRight);
//    ui->Smile_widget->replot();
//    ui->Smile_widget->update();

    ui->SideMouth_plot->graph(0)->setData(xAxis_PlotData, SideMouth1_PlotData);
    ui->SideMouth_plot->graph(1)->setData(xAxis_PlotData, SideMouth2_PlotData);
    ui->SideMouth_plot->xAxis->setRange(xAxis_PlotData.size(), 100, Qt::AlignRight);
    ui->SideMouth_plot->replot();
    ui->SideMouth_plot->update();

    //Instance trigger indicator
    if(HeadTurnLeft_instanceTrigger == true || HeadTurnRight_instanceTrigger == true)
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


    if(SmileStatusIndicator == true)
        ui->SmileIndicator->setStyleSheet("QLabel { background-color : lime; }");
    else
        ui->SmileIndicator->setStyleSheet("QLabel { background-color : darkgreen; }");

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
//    ui->blue_horizontalSlider->setRange(0, 255);
//    blue = ui->blue_horizontalSlider->value();

//    ui->green_horizontalSlider->setRange(0, 255);
//    green = ui->green_horizontalSlider->value();

//    ui->red_horizontalSlider->setRange(0, 255);
//    red = ui->red_horizontalSlider->value();


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

    ui->Main_tableWidget->scrollToBottom();
    ui->HeadTurn_tableWidget->scrollToBottom();
    ui->Blink_tableWidget->scrollToBottom();
    ui->Yawn_tableWidget->scrollToBottom();
}

void MainWindow::show_frame(Mat &image)
{
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
        blue = 0;
        green = 255;
        red = 0;
        cv::polylines(image, points, isClosed, cv::Scalar(blue, green, red), 1, 16);
        points.clear();
    }
}

void MainWindow::ResetData()
{
    HeadTurnLeft_instanceTrigger = false;
    HeadTurnRight_instanceTrigger = false;
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

//    MouthWidth_PlotData.clear();
    SideMouth1_PlotData.clear();
    SideMouth2_PlotData.clear();

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

void MainWindow::DisplayCurrentTime()
{
    QTime time = QTime::currentTime();
    QString time_string = time.toString("hh:mm:ss ap");
    ui->CurrentTime_label->setText(time_string);
}

//void MainWindow::on_pushButton_reset_clicked()
//{
//    if(ui->ResetData_checkBox->isChecked() == true)
//    {
//        ResetData();
//    }

//    if(ui->ResetSettings_checkBox->isChecked() == true)
//    {
//        LeftEye_max = 0;
//        RightEye_max = 0;
//        LeftEye_min = 100;
//        RightEye_min = 100;

//        ui->HeadTurnLeft_spinBox->setValue(65);
//        ui->HeadTurnRight_spinBox->setValue(35);
//        ui->Blink_spinBox->setValue(25);
//        ui->YawnEyes_spinBox->setValue(35);
//        ui->YawnMouth_spinBox->setValue(30);

//        ui->HeadTurn_ThresholdTime_spinBox->setValue(0);
//        ui->Blink_ThresholdTime_spinBox->setValue(0);
//        ui->SlowBlink_ThresholdTime_spinBox->setValue(200);
//        ui->Yawn_ThresholdTime_spinBox->setValue(200);

//        ui->HeadTurn_TimeLimit_spinBox->setValue(2000);
//        ui->SlowBlink_ThresholdRate_spinBox->setValue(3);
//        ui->ClosedEyes_TimeLimit_spinBox->setValue(2000);
//        ui->Yawn_ThresholdRate_spinBox->setValue(3);

//        ui->RefreshRate_spinBox->setValue(10);
//    }
//}

void MainWindow::on_SaveOutputData_pushButton_clicked()
{
    QString FilePath = QFileDialog::getSaveFileName(this, tr("Save Output Data File"), "C:/Users/Joseph/Desktop", "Comma-Separated Values File (*.csv)");
    ui->FileNameOutput_lineEdit->setText(FilePath);

    QFile file(FilePath);
    QTextStream out(&file);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "could not open" << endl;
        return;
    }
    for(int i = 0; i < ui->Main_tableWidget->rowCount(); i++)
    {
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
    PauseTime = cap.get(CV_CAP_PROP_POS_MSEC);
    cap.release();
    disconnect(timer, SIGNAL(timeout()), this, SLOT(ProcessVideoFrame()));
    timer->start(20);
}

void MainWindow::on_StopVideo_pushButton_clicked()
{
    if(PauseVideo == true)
        PauseVideo = false;
    cap.release();
    ui->Video_horizontalSlider->setValue(ui->Video_horizontalSlider->minimum());
    ui->VideoTime_label->setText("00:00");
    disconnect(timer, SIGNAL(timeout()), this, SLOT(ProcessVideoFrame()));
    timer->start(20);
}

void MainWindow::on_Video_horizontalSlider_sliderMoved(int position)
{
    if(cap.isOpened())
    {
        cap.set(CV_CAP_PROP_POS_MSEC, position);
    }
    else if(PauseVideo == true)
    {
        PauseTime = position;
    }
}

void MainWindow::on_LoadSettings_pushButton_clicked()
{
    QString FilePath = QFileDialog::getOpenFileName(this, tr("Open Settings File"), "C:/Users/Joseph/Desktop", "Comma-Separated Values File (*.csv)");
    ui->FileNameSettings_lineEdit->setText(FilePath);

    ifstream file(FilePath.toStdString());

    if(!file.is_open())
    {
        ui->statusBar->showMessage("Cannot find settings file");
        return;
    }
    string value;
    string label;
    std::vector<int> values;
    QString value_qstring;
    while(file.good())
    {
        getline(file, value, ',');
        getline(file, label, '\n');
        value_qstring = QString::fromStdString(value);
        values.push_back(value_qstring.toInt());
    }
    file.close();

    ui->HeadTurnLeft_spinBox->setValue(values.at(0));
    ui->HeadTurnRight_spinBox->setValue(values.at(1));
    ui->HeadTurn_ThresholdTime_spinBox->setValue(values.at(2));
    ui->HeadTurn_TimeLimit_spinBox->setValue(values.at(3));
    ui->Blink_spinBox->setValue(values.at(4));
    ui->Blink_ThresholdTime_spinBox->setValue(values.at(5));
    ui->SlowBlink_ThresholdTime_spinBox->setValue(values.at(6));
    ui->SlowBlink_ThresholdRate_spinBox->setValue(values.at(7));
    ui->ClosedEyes_TimeLimit_spinBox->setValue(values.at(8));
    ui->YawnEyes_spinBox->setValue(values.at(9));
    ui->YawnMouth_spinBox->setValue(values.at(10));
    ui->Yawn_ThresholdTime_spinBox->setValue(values.at(11));
    ui->Yawn_ThresholdRate_spinBox->setValue(values.at(12));
    ui->RefreshRate_spinBox->setValue(values.at(13));
    LeftEye_max = values.at(14);
    RightEye_max = values.at(15);
    ui->Blink_ThresholdRate_spinBox->setValue(values.at(16));
    ui->HeadTurnBlink_spinBox->setValue(values.at(17));
    ui->SmileBlink_spinBox->setValue(values.at(18));
    ui->LiptoSideMouth_spinBox->setValue(values.at(19));

    ui->statusBar->showMessage("Settings loaded successfully", 2000);
}

void MainWindow::on_SaveSettings_pushButton_clicked()
{
    QString FilePath = QFileDialog::getSaveFileName(this, tr("Save File"), "C:/Users/Joseph/Desktop", "Comma-Separated Values File (*.csv)");
    ui->FileNameSettings_lineEdit->setText(FilePath);

    QFile file(FilePath);
    QTextStream out(&file);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        ui->statusBar->showMessage("Cannot find settings file", 2000);
        return;
    }
    out << ui->HeadTurnLeft_spinBox->value()
        << ","
        << "Head Turn Left threshold level"
        << "\n";
    out << ui->HeadTurnRight_spinBox->value()
        << ","
        << "Head Turn Right threshold level"
        << "\n";
    out << ui->HeadTurn_ThresholdTime_spinBox->value()
        << ","
        << "Head Turn threshold time"
        << "\n";
    out << ui->HeadTurn_TimeLimit_spinBox->value()
        << ","
        << "Head Turn time limit"
        << "\n";
    out << ui->Blink_spinBox->value()
        << ","
        << "Blink threshold level"
        << "\n";
    out << ui->Blink_ThresholdTime_spinBox->value()
        << ","
        << "Blink threshold time"
        << "\n";
    out << ui->SlowBlink_ThresholdTime_spinBox->value()
        << ","
        << "Slow Blink threshold time"
        << "\n";
    out << ui->SlowBlink_ThresholdRate_spinBox->value()
        << ","
        << "Slow Blink threshold rate"
        << "\n";
    out << ui->ClosedEyes_TimeLimit_spinBox->value()
        << ","
        << "Closed Eyes time limit"
        << "\n";
    out << ui->YawnEyes_spinBox->value()
        << ","
        << "Yawn Eye threshold level"
        << "\n";
    out << ui->YawnMouth_spinBox->value()
        << ","
        << "Yawn Mouth threshold level"
        << "\n";
    out << ui->Yawn_ThresholdTime_spinBox->value()
        << ","
        << "Yawn threshold time"
        << "\n";
    out << ui->Yawn_ThresholdRate_spinBox->value()
        << ","
        << "Yawn threshold rate"
        << "\n";
    out << ui->RefreshRate_spinBox->value()
        << ","
        << "Refresh rate"
        << "\n";
    out << LeftEye_max
        << ","
        << "Left Eye max"
        << "\n";
    out << RightEye_max
        << ","
        << "Right Eye max"
        << "\n";
    out << ui->Blink_ThresholdRate_spinBox->value()
        << ","
        << "Blink threshold rate"
        << "\n";
    out << ui->HeadTurnBlink_spinBox->value()
        << ","
        << "Head Turn Blink threshold level"
        << "\n";
    out << ui->SmileBlink_spinBox->value()
        << ","
        << "Smile Blink threshold level"
        << "\n";
    out << ui->LiptoSideMouth_spinBox->value()
        << ","
        << "Smile threshold level"
        << "\n";

    file.flush();
    file.close();

    ui->statusBar->showMessage("Settings saved successfully", 2000);
}

void MainWindow::on_Rotate_pushButton_clicked()
{
    if(Rotate == 3)
        Rotate = 0;
    else
        Rotate++;
}

//void MainWindow::ResizeImage()
//{
//    if(Rotate == 0)
//        cv::resize(frame, frame, Size(640, 480), 0, 0, INTER_CUBIC);
//}

void MainWindow::RotateImage()
{
    if(Rotate == 1)
        cv::rotate(frame, frame, ROTATE_90_COUNTERCLOCKWISE);
    else if(Rotate == 2)
        cv::rotate(frame, frame, ROTATE_180);
    else if(Rotate == 3)
        cv::rotate(frame, frame, ROTATE_90_CLOCKWISE);
}

void MainWindow::ResetTriggers()
{
    HeadTurnLeft_instanceTrigger = false;
    HeadTurnRight_instanceTrigger = false;
    Blink_instanceTrigger = false;
    Yawn_instanceTrigger = false;

    HeadTurnLeft_displayTrigger = false;
    HeadTurnRight_displayTrigger = false;
    Blink_displayTrigger = false;
    Yawn_displayTrigger = false;
}

void MainWindow::on_HeadTurnLeft_spinBox_valueChanged(int arg1)
{
    HeadTurnLeft_ThresholdLevel = arg1;
    ui->HeadTurn_plot->removeItem(HeadTurnLeft_ThresholdLine);
    HeadTurnLeft_ThresholdLine = new QCPItemLine(ui->HeadTurn_plot);
    HeadTurnLeft_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    HeadTurnLeft_ThresholdLine->start->setCoords(QCPRange::minRange, HeadTurnLeft_ThresholdLevel);
    HeadTurnLeft_ThresholdLine->end->setCoords(QCPRange::maxRange, HeadTurnLeft_ThresholdLevel);
    ui->HeadTurn_plot->replot();
}

void MainWindow::on_HeadTurnRight_spinBox_valueChanged(int arg1)
{
    HeadTurnRight_ThresholdLevel = arg1;
    ui->HeadTurn_plot->removeItem(HeadTurnRight_ThresholdLine);
    HeadTurnRight_ThresholdLine = new QCPItemLine(ui->HeadTurn_plot);
    HeadTurnRight_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    HeadTurnRight_ThresholdLine->start->setCoords(QCPRange::minRange, HeadTurnRight_ThresholdLevel);
    HeadTurnRight_ThresholdLine->end->setCoords(QCPRange::maxRange, HeadTurnRight_ThresholdLevel);
    ui->HeadTurn_plot->replot();
}

void MainWindow::on_Blink_spinBox_valueChanged(int arg1)
{
    Blink_ThresholdLevel = arg1;
    ui->Blink_plot->removeItem(Blink_ThresholdLine);
    Blink_ThresholdLine = new QCPItemLine(ui->Blink_plot);
    Blink_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    Blink_ThresholdLine->start->setCoords(QCPRange::minRange, Blink_ThresholdLevel);
    Blink_ThresholdLine->end->setCoords(QCPRange::maxRange, Blink_ThresholdLevel);
    ui->Blink_plot->replot();
}

void MainWindow::on_YawnMouth_spinBox_valueChanged(int arg1)
{
    YawnMouth_ThresholdLevel = arg1;
    ui->Yawn_plot->removeItem(YawnMouth_ThresholdLine);
    YawnMouth_ThresholdLine = new QCPItemLine(ui->Yawn_plot);
    YawnMouth_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    YawnMouth_ThresholdLine->start->setCoords(QCPRange::minRange, YawnMouth_ThresholdLevel);
    YawnMouth_ThresholdLine->end->setCoords(QCPRange::maxRange, YawnMouth_ThresholdLevel);
    ui->Yawn_plot->replot();
}

void MainWindow::on_YawnEyes_spinBox_valueChanged(int arg1)
{
    YawnEyes_ThresholdLevel = arg1;
}

void MainWindow::on_LoadImage_pushButton_clicked()
{
    QString ImageFilePath = QFileDialog::getOpenFileName(this, tr("Open Image File"), ImagePath, "JPEG (*.jpg)");
    ImagePath = QFileInfo(ImageFilePath).path();
    ui->VideoInputPath_lineEdit->setText(QFileInfo(ImageFilePath).fileName());
    Mat Image;
    Image = imread(ImageFilePath.toStdString());

    cv::resize(Image, Image, Size(640, 360));

    array2d<bgr_pixel> dlib_image;
    assign_image(dlib_image, cv_image<bgr_pixel>(Image));

    std::vector<dlib::rectangle> faces = detector(dlib_image);

    std::vector<full_object_detection> shapes;
    for(unsigned long i = 0; i < faces.size(); i++)
    {
        full_object_detection shape = shape_model(dlib_image, faces[i]);
        shapes.push_back(shape);
        face_layout(shape, Image);
    }
    show_frame(Image);
}

void MainWindow::on_LiptoSideMouth_spinBox_valueChanged(int arg1)
{
    SmileThresholdLevel = arg1;
    ui->SideMouth_plot->removeItem(SideMouth_ThresholdLine);
    SideMouth_ThresholdLine = new QCPItemLine(ui->SideMouth_plot);
    SideMouth_ThresholdLine->setPen(QPen(Qt::DashDotLine));
    SideMouth_ThresholdLine->start->setCoords(QCPRange::minRange, SmileThresholdLevel);
    SideMouth_ThresholdLine->end->setCoords(QCPRange::maxRange, SmileThresholdLevel);
    ui->SideMouth_plot->replot();
}

void MainWindow::on_HeadTurnBlink_spinBox_valueChanged(int arg1)
{
    HeadTurnBlink_ThresholdLevel = arg1;
    ui->Blink_plot->removeItem(HeadTurnBlink_ThresholdLine);
    HeadTurnBlink_ThresholdLine = new QCPItemLine(ui->Blink_plot);
    HeadTurnBlink_ThresholdLine->setPen(QPen(Qt::blue));
    HeadTurnBlink_ThresholdLine->start->setCoords(QCPRange::minRange, HeadTurnBlink_ThresholdLevel);
    HeadTurnBlink_ThresholdLine->end->setCoords(QCPRange::maxRange, HeadTurnBlink_ThresholdLevel);
    ui->Blink_plot->replot();
}

void MainWindow::on_SmileBlink_spinBox_valueChanged(int arg1)
{
    SmileBlink_ThresholdLevel = arg1;
    ui->Blink_plot->removeItem(SmileBlink_ThresholdLine);
    SmileBlink_ThresholdLine = new QCPItemLine(ui->Blink_plot);
    SmileBlink_ThresholdLine->setPen(QPen(Qt::darkMagenta));
    SmileBlink_ThresholdLine->start->setCoords(QCPRange::minRange, SmileBlink_ThresholdLevel);
    SmileBlink_ThresholdLine->end->setCoords(QCPRange::maxRange, SmileBlink_ThresholdLevel);
    ui->Blink_plot->replot();
}
void MainWindow::on_ResetData_pushButton_clicked()
{
    ResetData();
}

void MainWindow::on_ResetSettings_pushButton_clicked()
{
    LeftEye_max = 0;
    RightEye_max = 0;
    LeftEye_min = 100;
    RightEye_min = 100;

    ui->HeadTurnLeft_spinBox->setValue(65);
    ui->HeadTurnRight_spinBox->setValue(35);
    ui->HeadTurn_ThresholdTime_spinBox->setValue(0);
    ui->HeadTurn_TimeLimit_spinBox->setValue(2000);

    ui->Blink_spinBox->setValue(25);
    ui->Blink_ThresholdTime_spinBox->setValue(0);
    ui->SlowBlink_ThresholdTime_spinBox->setValue(400);
    ui->Blink_ThresholdRate_spinBox->setValue(10);
    ui->SlowBlink_ThresholdRate_spinBox->setValue(1);
    ui->ClosedEyes_TimeLimit_spinBox->setValue(1000);
    ui->HeadTurnBlink_spinBox->setValue(40);
    ui->SmileBlink_spinBox->setValue(5);

    ui->YawnEyes_spinBox->setValue(35);
    ui->YawnMouth_spinBox->setValue(30);
    ui->Yawn_ThresholdTime_spinBox->setValue(200);
    ui->Yawn_ThresholdRate_spinBox->setValue(3);

    ui->LiptoSideMouth_spinBox->setValue(20);
    ui->RefreshRate_spinBox->setValue(60);
}
