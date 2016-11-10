#-------------------------------------------------
#
# Project created by QtCreator 2016-07-04T12:50:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = PlotTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot.cpp \
    gpsparser.cpp \
    pathDeciding.cpp \
    tcpcommandtransmitter.cpp \
    gpstools.cpp

HEADERS  += mainwindow.h \
        qcustomplot.h \
    gpsparser.h \
    pathDeciding.h \
    TCPcommandTransmitter.h \
    tcpcommandtransmitter.h \
    gpstools.h

FORMS    += mainwindow.ui

DISTFILES += \
    Kart_Horten.PNG

RESOURCES += \
    resources.qrc
