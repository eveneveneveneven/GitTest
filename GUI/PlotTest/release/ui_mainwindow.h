/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QCustomPlot *velocityPlot;
    QCustomPlot *headingPlot;
    QCustomPlot *positionPlot;
    QPushButton *clearPlotButton;
    QPushButton *generateAreaButton;
    QPushButton *posToPDFbutton;
    QPushButton *sendWaypointsButton;
    QPushButton *clearWaypointsButton;
    QSpinBox *depthSpinBox;
    QLabel *label_14;
    QDoubleSpinBox *LOSspinBox;
    QPushButton *LOSbutton;
    QCustomPlot *leftMotorPlot;
    QCustomPlot *rightMotorPlot;
    QTextEdit *textBox;
    QPushButton *stopButton;
    QPushButton *startButton;
    QPushButton *connectButton;
    QDoubleSpinBox *speedKpSpinBox;
    QDoubleSpinBox *speedKdSpinBox;
    QDoubleSpinBox *speedKiSpinBox;
    QPushButton *speedGainButton;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QDoubleSpinBox *headingKdSpinBox;
    QLabel *label_5;
    QDoubleSpinBox *headingKiSpinBox;
    QDoubleSpinBox *headingKpSpinBox;
    QPushButton *headingGainButton;
    QLabel *label_6;
    QDoubleSpinBox *speedRefSpinBox;
    QLabel *label_7;
    QLineEdit *speedDisplay;
    QLabel *label_8;
    QPushButton *speedRefApply;
    QDoubleSpinBox *headingRefSpinBox;
    QLabel *label_9;
    QLineEdit *headingDisplay;
    QLabel *label_10;
    QPushButton *headingRefApply;
    QFrame *frame;
    QLabel *label_11;
    QFrame *frame_2;
    QLabel *label_13;
    QFrame *frame_3;
    QFrame *frame_4;
    QRadioButton *wpChase;
    QRadioButton *headingChase;
    QRadioButton *zigsagMode;
    QRadioButton *arrowControl;
    QLabel *label_15;
    QFrame *frame_5;
    QLabel *label_16;
    QPushButton *arrowUp;
    QPushButton *arrowStop;
    QPushButton *arrowLeft;
    QPushButton *arrowRight;
    QLabel *label_17;
    QGraphicsView *graphicsView;
    QGraphicsView *graphicsView_2;
    QFrame *frame_6;
    QGroupBox *groupBox;
    QGraphicsView *connectionLight;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1600, 800);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        velocityPlot = new QCustomPlot(centralWidget);
        velocityPlot->setObjectName(QStringLiteral("velocityPlot"));
        velocityPlot->setGeometry(QRect(0, 410, 731, 161));
        headingPlot = new QCustomPlot(centralWidget);
        headingPlot->setObjectName(QStringLiteral("headingPlot"));
        headingPlot->setGeometry(QRect(0, 580, 731, 161));
        positionPlot = new QCustomPlot(centralWidget);
        positionPlot->setObjectName(QStringLiteral("positionPlot"));
        positionPlot->setGeometry(QRect(740, 0, 851, 741));
        clearPlotButton = new QPushButton(positionPlot);
        clearPlotButton->setObjectName(QStringLiteral("clearPlotButton"));
        clearPlotButton->setGeometry(QRect(710, 540, 121, 31));
        generateAreaButton = new QPushButton(positionPlot);
        generateAreaButton->setObjectName(QStringLiteral("generateAreaButton"));
        generateAreaButton->setGeometry(QRect(710, 600, 121, 31));
        posToPDFbutton = new QPushButton(positionPlot);
        posToPDFbutton->setObjectName(QStringLiteral("posToPDFbutton"));
        posToPDFbutton->setGeometry(QRect(710, 660, 121, 31));
        sendWaypointsButton = new QPushButton(positionPlot);
        sendWaypointsButton->setObjectName(QStringLiteral("sendWaypointsButton"));
        sendWaypointsButton->setGeometry(QRect(710, 630, 121, 31));
        clearWaypointsButton = new QPushButton(positionPlot);
        clearWaypointsButton->setObjectName(QStringLiteral("clearWaypointsButton"));
        clearWaypointsButton->setGeometry(QRect(710, 570, 121, 31));
        depthSpinBox = new QSpinBox(positionPlot);
        depthSpinBox->setObjectName(QStringLiteral("depthSpinBox"));
        depthSpinBox->setGeometry(QRect(790, 10, 41, 21));
        depthSpinBox->setMinimum(1);
        depthSpinBox->setValue(3);
        label_14 = new QLabel(positionPlot);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(660, 10, 131, 21));
        LOSspinBox = new QDoubleSpinBox(positionPlot);
        LOSspinBox->setObjectName(QStringLiteral("LOSspinBox"));
        LOSspinBox->setGeometry(QRect(80, 10, 61, 22));
        LOSspinBox->setDecimals(4);
        LOSspinBox->setSingleStep(0.5);
        LOSspinBox->setValue(3);
        LOSbutton = new QPushButton(positionPlot);
        LOSbutton->setObjectName(QStringLiteral("LOSbutton"));
        LOSbutton->setGeometry(QRect(150, 10, 181, 21));
        leftMotorPlot = new QCustomPlot(centralWidget);
        leftMotorPlot->setObjectName(QStringLiteral("leftMotorPlot"));
        leftMotorPlot->setGeometry(QRect(0, 240, 251, 161));
        rightMotorPlot = new QCustomPlot(centralWidget);
        rightMotorPlot->setObjectName(QStringLiteral("rightMotorPlot"));
        rightMotorPlot->setGeometry(QRect(260, 240, 251, 161));
        textBox = new QTextEdit(centralWidget);
        textBox->setObjectName(QStringLiteral("textBox"));
        textBox->setGeometry(QRect(320, 10, 191, 51));
        stopButton = new QPushButton(centralWidget);
        stopButton->setObjectName(QStringLiteral("stopButton"));
        stopButton->setGeometry(QRect(220, 10, 91, 51));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(8);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(9);
        stopButton->setFont(font);
        stopButton->setStyleSheet(QLatin1String("background-color: rgb(255, 0, 0);\n"
"color: rgb(255, 255, 255);\n"
"background-color: rgb(170, 0, 0);\n"
"font: 75 8pt \"MS Shell Dlg 2\";"));
        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setGeometry(QRect(120, 10, 91, 51));
        startButton->setStyleSheet(QStringLiteral("background-color: rgb(0, 170, 0);"));
        connectButton = new QPushButton(centralWidget);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setGeometry(QRect(10, 20, 51, 31));
        speedKpSpinBox = new QDoubleSpinBox(centralWidget);
        speedKpSpinBox->setObjectName(QStringLiteral("speedKpSpinBox"));
        speedKpSpinBox->setGeometry(QRect(20, 110, 62, 22));
        speedKpSpinBox->setDecimals(4);
        speedKpSpinBox->setValue(6);
        speedKdSpinBox = new QDoubleSpinBox(centralWidget);
        speedKdSpinBox->setObjectName(QStringLiteral("speedKdSpinBox"));
        speedKdSpinBox->setGeometry(QRect(160, 110, 62, 22));
        speedKdSpinBox->setDecimals(4);
        speedKiSpinBox = new QDoubleSpinBox(centralWidget);
        speedKiSpinBox->setObjectName(QStringLiteral("speedKiSpinBox"));
        speedKiSpinBox->setGeometry(QRect(90, 110, 62, 22));
        speedKiSpinBox->setDecimals(4);
        speedKiSpinBox->setValue(1);
        speedGainButton = new QPushButton(centralWidget);
        speedGainButton->setObjectName(QStringLiteral("speedGainButton"));
        speedGainButton->setGeometry(QRect(20, 140, 201, 31));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 90, 16, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(110, 90, 21, 16));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(180, 90, 21, 16));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(440, 90, 21, 16));
        headingKdSpinBox = new QDoubleSpinBox(centralWidget);
        headingKdSpinBox->setObjectName(QStringLiteral("headingKdSpinBox"));
        headingKdSpinBox->setGeometry(QRect(420, 110, 62, 22));
        headingKdSpinBox->setDecimals(4);
        headingKdSpinBox->setMinimum(-99.99);
        headingKdSpinBox->setMaximum(99.99);
        headingKdSpinBox->setValue(50);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(370, 90, 21, 16));
        headingKiSpinBox = new QDoubleSpinBox(centralWidget);
        headingKiSpinBox->setObjectName(QStringLiteral("headingKiSpinBox"));
        headingKiSpinBox->setGeometry(QRect(350, 110, 62, 22));
        headingKiSpinBox->setDecimals(4);
        headingKiSpinBox->setValue(0.5);
        headingKpSpinBox = new QDoubleSpinBox(centralWidget);
        headingKpSpinBox->setObjectName(QStringLiteral("headingKpSpinBox"));
        headingKpSpinBox->setGeometry(QRect(280, 110, 61, 22));
        headingKpSpinBox->setDecimals(4);
        headingKpSpinBox->setValue(40);
        headingGainButton = new QPushButton(centralWidget);
        headingGainButton->setObjectName(QStringLiteral("headingGainButton"));
        headingGainButton->setGeometry(QRect(280, 140, 201, 31));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(300, 90, 16, 16));
        speedRefSpinBox = new QDoubleSpinBox(centralWidget);
        speedRefSpinBox->setObjectName(QStringLiteral("speedRefSpinBox"));
        speedRefSpinBox->setGeometry(QRect(20, 200, 81, 21));
        speedRefSpinBox->setStyleSheet(QStringLiteral(""));
        speedRefSpinBox->setValue(4);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(20, 180, 81, 16));
        speedDisplay = new QLineEdit(centralWidget);
        speedDisplay->setObjectName(QStringLiteral("speedDisplay"));
        speedDisplay->setGeometry(QRect(120, 200, 51, 20));
        speedDisplay->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(130, 180, 41, 16));
        speedRefApply = new QPushButton(centralWidget);
        speedRefApply->setObjectName(QStringLiteral("speedRefApply"));
        speedRefApply->setGeometry(QRect(180, 182, 41, 41));
        headingRefSpinBox = new QDoubleSpinBox(centralWidget);
        headingRefSpinBox->setObjectName(QStringLiteral("headingRefSpinBox"));
        headingRefSpinBox->setGeometry(QRect(280, 198, 81, 21));
        headingRefSpinBox->setMaximum(360);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(280, 178, 91, 16));
        headingDisplay = new QLineEdit(centralWidget);
        headingDisplay->setObjectName(QStringLiteral("headingDisplay"));
        headingDisplay->setGeometry(QRect(380, 198, 51, 20));
        headingDisplay->setStyleSheet(QStringLiteral("color: rgb(0, 0, 255);"));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(390, 178, 61, 16));
        headingRefApply = new QPushButton(centralWidget);
        headingRefApply->setObjectName(QStringLiteral("headingRefApply"));
        headingRefApply->setGeometry(QRect(440, 180, 41, 41));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(0, 70, 251, 161));
        frame->setStyleSheet(QLatin1String("background-color: rgb(0, 85, 0);\n"
"background-color: rgb(128, 128, 128);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_11 = new QLabel(frame);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(0, 0, 251, 21));
        QFont font1;
        font1.setPointSize(11);
        font1.setBold(true);
        font1.setWeight(75);
        label_11->setFont(font1);
        label_11->setAlignment(Qt::AlignCenter);
        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setGeometry(QRect(260, 70, 251, 161));
        frame_2->setStyleSheet(QStringLiteral("background-color: rgb(128, 128, 128);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        label_13 = new QLabel(frame_2);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(0, 0, 251, 21));
        label_13->setFont(font1);
        label_13->setAlignment(Qt::AlignCenter);
        frame_3 = new QFrame(centralWidget);
        frame_3->setObjectName(QStringLiteral("frame_3"));
        frame_3->setGeometry(QRect(-1, -1, 1601, 771));
        frame_3->setStyleSheet(QLatin1String("color: rgb(89, 89, 89);\n"
"background-color: rgb(89, 89, 89);"));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        frame_4 = new QFrame(frame_3);
        frame_4->setObjectName(QStringLiteral("frame_4"));
        frame_4->setGeometry(QRect(520, 240, 211, 71));
        frame_4->setStyleSheet(QStringLiteral("background-color: rgb(126, 126, 126);"));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        wpChase = new QRadioButton(frame_4);
        wpChase->setObjectName(QStringLiteral("wpChase"));
        wpChase->setGeometry(QRect(10, 20, 101, 20));
        wpChase->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        headingChase = new QRadioButton(frame_4);
        headingChase->setObjectName(QStringLiteral("headingChase"));
        headingChase->setGeometry(QRect(10, 40, 101, 17));
        headingChase->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        zigsagMode = new QRadioButton(frame_4);
        zigsagMode->setObjectName(QStringLiteral("zigsagMode"));
        zigsagMode->setGeometry(QRect(120, 20, 82, 17));
        zigsagMode->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        arrowControl = new QRadioButton(frame_4);
        arrowControl->setObjectName(QStringLiteral("arrowControl"));
        arrowControl->setGeometry(QRect(120, 40, 121, 17));
        arrowControl->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_15 = new QLabel(frame_4);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(0, 0, 211, 21));
        label_15->setFont(font1);
        label_15->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_15->setAlignment(Qt::AlignCenter);
        frame_5 = new QFrame(frame_3);
        frame_5->setObjectName(QStringLiteral("frame_5"));
        frame_5->setGeometry(QRect(520, 320, 211, 80));
        frame_5->setStyleSheet(QStringLiteral("background-color: rgb(120, 120, 120);"));
        frame_5->setFrameShape(QFrame::StyledPanel);
        frame_5->setFrameShadow(QFrame::Raised);
        label_16 = new QLabel(frame_5);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setGeometry(QRect(100, 0, 111, 21));
        label_16->setFont(font1);
        label_16->setStyleSheet(QStringLiteral("color: rgb(0, 0, 0);"));
        label_16->setAlignment(Qt::AlignCenter);
        arrowUp = new QPushButton(frame_5);
        arrowUp->setObjectName(QStringLiteral("arrowUp"));
        arrowUp->setGeometry(QRect(40, 10, 31, 31));
        arrowUp->setStyleSheet(QStringLiteral("background-color: rgb(216, 216, 216);"));
        arrowStop = new QPushButton(frame_5);
        arrowStop->setObjectName(QStringLiteral("arrowStop"));
        arrowStop->setGeometry(QRect(40, 40, 31, 31));
        arrowStop->setStyleSheet(QStringLiteral("background-color: rgb(216, 216, 216);"));
        arrowLeft = new QPushButton(frame_5);
        arrowLeft->setObjectName(QStringLiteral("arrowLeft"));
        arrowLeft->setGeometry(QRect(10, 40, 31, 31));
        arrowLeft->setStyleSheet(QStringLiteral("background-color: rgb(216, 216, 216);"));
        arrowRight = new QPushButton(frame_5);
        arrowRight->setObjectName(QStringLiteral("arrowRight"));
        arrowRight->setGeometry(QRect(70, 40, 31, 31));
        arrowRight->setStyleSheet(QStringLiteral("background-color: rgb(216, 216, 216);"));
        label_17 = new QLabel(frame_5);
        label_17->setObjectName(QStringLiteral("label_17"));
        label_17->setGeometry(QRect(100, 20, 111, 20));
        label_17->setStyleSheet(QLatin1String("font: 6pt \"MS Shell Dlg 2\";\n"
"font: 8pt \"MS Shell Dlg 2\";\n"
"color: rgb(0, 0, 0);"));
        label_17->setAlignment(Qt::AlignCenter);
        graphicsView = new QGraphicsView(frame_3);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setGeometry(QRect(520, 10, 211, 161));
        graphicsView->setStyleSheet(QStringLiteral("border-image: url(:/images/kongsberglogo.jpg);"));
        graphicsView_2 = new QGraphicsView(frame_3);
        graphicsView_2->setObjectName(QStringLiteral("graphicsView_2"));
        graphicsView_2->setGeometry(QRect(530, 180, 191, 41));
        graphicsView_2->setStyleSheet(QStringLiteral("border-image: url(:/images/FFIlogo.png);"));
        frame_6 = new QFrame(frame_3);
        frame_6->setObjectName(QStringLiteral("frame_6"));
        frame_6->setGeometry(QRect(520, 170, 211, 61));
        frame_6->setStyleSheet(QStringLiteral("background-color: rgb(255, 255, 255);"));
        frame_6->setFrameShape(QFrame::StyledPanel);
        frame_6->setFrameShadow(QFrame::Raised);
        groupBox = new QGroupBox(frame_3);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(1, 12, 110, 49));
        frame_4->raise();
        frame_5->raise();
        graphicsView->raise();
        frame_6->raise();
        graphicsView_2->raise();
        groupBox->raise();
        connectionLight = new QGraphicsView(centralWidget);
        connectionLight->setObjectName(QStringLiteral("connectionLight"));
        connectionLight->setGeometry(QRect(70, 20, 31, 31));
        connectionLight->setStyleSheet(QStringLiteral("background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.698864 rgba(255, 0, 0, 255), stop:0.761364 rgba(255, 255, 255, 0));"));
        MainWindow->setCentralWidget(centralWidget);
        frame_3->raise();
        frame_2->raise();
        frame->raise();
        velocityPlot->raise();
        headingPlot->raise();
        positionPlot->raise();
        leftMotorPlot->raise();
        rightMotorPlot->raise();
        textBox->raise();
        stopButton->raise();
        startButton->raise();
        speedKpSpinBox->raise();
        speedKdSpinBox->raise();
        speedKiSpinBox->raise();
        speedGainButton->raise();
        label->raise();
        label_2->raise();
        label_3->raise();
        label_4->raise();
        headingKdSpinBox->raise();
        label_5->raise();
        headingKiSpinBox->raise();
        headingKpSpinBox->raise();
        headingGainButton->raise();
        label_6->raise();
        speedRefSpinBox->raise();
        label_7->raise();
        speedDisplay->raise();
        label_8->raise();
        speedRefApply->raise();
        headingRefSpinBox->raise();
        label_9->raise();
        headingDisplay->raise();
        label_10->raise();
        headingRefApply->raise();
        connectButton->raise();
        connectionLight->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1600, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        clearPlotButton->setText(QApplication::translate("MainWindow", "Clear position history", 0));
        generateAreaButton->setText(QApplication::translate("MainWindow", "Generate scan area", 0));
        posToPDFbutton->setText(QApplication::translate("MainWindow", "Save position to PDF", 0));
        sendWaypointsButton->setText(QApplication::translate("MainWindow", "Send waypoints", 0));
        clearWaypointsButton->setText(QApplication::translate("MainWindow", "Clear waypoints", 0));
        label_14->setText(QApplication::translate("MainWindow", "Minimum operation depth:", 0));
        LOSbutton->setText(QApplication::translate("MainWindow", "Apply new Line of Sight Distance", 0));
        stopButton->setText(QApplication::translate("MainWindow", "STOP", 0));
        startButton->setText(QApplication::translate("MainWindow", "START", 0));
        connectButton->setText(QApplication::translate("MainWindow", "Connect", 0));
        speedGainButton->setText(QApplication::translate("MainWindow", "Apply speed controller gains", 0));
        label->setText(QApplication::translate("MainWindow", "Kp", 0));
        label_2->setText(QApplication::translate("MainWindow", "Ki", 0));
        label_3->setText(QApplication::translate("MainWindow", "Kd", 0));
        label_4->setText(QApplication::translate("MainWindow", "Kd", 0));
        label_5->setText(QApplication::translate("MainWindow", "Ki", 0));
        headingGainButton->setText(QApplication::translate("MainWindow", "Apply heading controller gains", 0));
        label_6->setText(QApplication::translate("MainWindow", "Kp", 0));
        label_7->setText(QApplication::translate("MainWindow", "Speed ref [km/h]", 0));
        speedDisplay->setText(QString());
        label_8->setText(QApplication::translate("MainWindow", "Speed", 0));
        speedRefApply->setText(QApplication::translate("MainWindow", "Apply", 0));
        label_9->setText(QApplication::translate("MainWindow", "Heading ref [deg]", 0));
        label_10->setText(QApplication::translate("MainWindow", "Heading", 0));
        headingRefApply->setText(QApplication::translate("MainWindow", "Apply", 0));
        label_11->setText(QApplication::translate("MainWindow", "Speed controller", 0));
        label_13->setText(QApplication::translate("MainWindow", "Heading controller", 0));
        wpChase->setText(QApplication::translate("MainWindow", "Waypoint chase", 0));
        headingChase->setText(QApplication::translate("MainWindow", "Heading chase", 0));
        zigsagMode->setText(QApplication::translate("MainWindow", "Zig-zag mode", 0));
        arrowControl->setText(QApplication::translate("MainWindow", "Arrow control", 0));
        label_15->setText(QApplication::translate("MainWindow", "Choose mode of operation", 0));
        label_16->setText(QApplication::translate("MainWindow", "Direct control", 0));
        arrowUp->setText(QApplication::translate("MainWindow", "^", 0));
        arrowStop->setText(QApplication::translate("MainWindow", "-", 0));
        arrowLeft->setText(QApplication::translate("MainWindow", "<-", 0));
        arrowRight->setText(QApplication::translate("MainWindow", "->", 0));
        label_17->setText(QApplication::translate("MainWindow", "(select Arrow control)", 0));
        groupBox->setTitle(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
