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

QMAKE_CXXFLAGS_RELEASE += -mavx

DISTFILES +=

RESOURCES += \
    resources.qrc

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += dlib-1
