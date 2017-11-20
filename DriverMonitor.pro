#-------------------------------------------------
#
# Project created by QtCreator 2017-08-13T12:44:42
#
#-------------------------------------------------

QT       += core gui printsupport multimedia bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DriverMonitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    driver_monitor.cpp \
    qcustomplot.cpp \
    chat.cpp \
    chatclient.cpp \
    chatserver.cpp \
    remoteselector.cpp

HEADERS  += mainwindow.h \
    driver_monitor.h \
    qcustomplot.h \
    chat.h \
    chatclient.h \
    chatserver.h \
    remoteselector.h

FORMS    += mainwindow.ui \
    chat.ui \
    remoteselector.ui

#INCLUDEPATH += C:\opencv\QT_opencv\install\include
#LIBS += -LC:\opencv\QT_opencv\install\x86\mingw\lib \
#    -lopencv_core320.dll \
#    -lopencv_highgui320.dll \
#    -lopencv_imgcodecs320.dll \
#    -lopencv_imgproc320.dll \
#    -lopencv_features2d320.dll \
#    -lopencv_calib3d320.dll \
#    -lopencv_videoio320.dll \
#    -lopencv_objdetect320.dll

#QMAKE_CXXFLAGS_RELEASE += -mavx

#INCLUDEPATH += C:\opencv\QT_dlib\build\include
#LIBS += -LC:\opencv\QT_dlib\dlib
#LIBS += -ldlib

DISTFILES +=

RESOURCES += \
    resources.qrc


unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += dlib-1
