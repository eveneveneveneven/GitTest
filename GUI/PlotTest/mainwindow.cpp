#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <time.h>
#include <iostream>
#include <string>
#include "pathDeciding.h"
#include "TCPcommandTransmitter.h"
#include "gpstools.h"

using namespace std;

#define REDLIGHT  "background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.698864 rgba(255, 0, 0, 255), stop:0.761364 rgba(255, 255, 255, 0))"
#define GREENLIGHT    "background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0.698864 rgba(0, 255, 0, 255), stop:0.761364 rgba(255, 255, 255, 0))"

struct cornerPoints {
    QPointF  lowerLeft;
    QPointF  upperRight;
};

cornerPoints    satelitePhotoCorners;
vector<QPointF> markedPositions;

void makeRealtimeGraph(QCustomPlot *myPlot, QString title, QString ylabel);
void setupRealTimeCurve(QCPCurve *myCurve, QCustomPlot *myPlot);
QPointF pixelPosToGPSPos(QPoint pixelPoint);








MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupSateliteView(workArea currentArea){
    ui->positionPlot->setBackground(Qt::darkGray);
    ui->positionPlot->axisRect()->setBackground(currentArea.SatelitePhoto);
    ui->positionPlot->axisRect()->setBackgroundScaled(true);
    ui->positionPlot->axisRect()->setBackgroundScaledMode(Qt::IgnoreAspectRatio);

    positionCurve = new QCPCurve(ui->positionPlot->xAxis, ui->positionPlot->yAxis);
    ui->positionPlot->addPlottable(positionCurve);
    QPen positionPen = QPen(Qt::red);
    positionPen.setWidth(0);
    positionCurve->setPen(positionPen);
    positionCurve->setBrush(Qt::NoBrush);
    ui->positionPlot->addGraph();
    ui->positionPlot->graph(0)->setPen(QPen(Qt::red));
    ui->positionPlot->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->positionPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->positionPlot->axisRect()->setupFullAxesBox();

    positionTitle = new QCPPlotTitle(ui->positionPlot);
    positionTitle->setText("Position");
    positionTitle->setFont(QFont("sans", 12, QFont::Bold));
    ui->positionPlot->plotLayout()->insertRow(0);
    ui->positionPlot->plotLayout()->addElement(0, 0, positionTitle);
    ui->positionPlot->yAxis->setLabel("Latitude");
    ui->positionPlot->xAxis->setLabel("Longitude");
    ui->positionPlot->xAxis->setRange(currentArea.LongitudeMin, currentArea.LongitudeMax);
    ui->positionPlot->yAxis->setRange(currentArea.LatitudeMin, currentArea.LatitudeMax);

    scanPlan = new QCPCurve(ui->positionPlot->xAxis, ui->positionPlot->yAxis);
    ui->positionPlot->addPlottable(scanPlan);
    QPen scanTrackPen = QPen(QColor(255,165,0));
    scanTrackPen.setWidth(0);
    scanPlan->setPen(scanTrackPen);
    scanPlan->setLineStyle(QCPCurve::lsLine);
    scanPlan->setScatterStyle(QCPScatterStyle::ssDisc);

    jolnersPlan = new QCPCurve(ui->positionPlot->xAxis, ui->positionPlot->yAxis);
    ui->positionPlot->addPlottable(jolnersPlan);
    QPen jolnerPen = QPen(Qt::green);
    jolnerPen.setWidth(0);
    jolnersPlan->setPen(jolnerPen);
    jolnersPlan->setBrush(Qt::NoBrush);
    jolnersPlan->setLineStyle(QCPCurve::lsNone);
    int pixelsPrMeter = 6;
    int acceptanceRadius = 2;
    jolnersPlan->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, acceptanceRadius*pixelsPrMeter));

    connect(ui->positionPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->positionPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->positionPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->positionPlot->yAxis2, SLOT(setRange(QCPRange)));

}

void makeRealtimeGraph(QCustomPlot *myPlot, QString title, QString ylabel){
    myPlot->setBackground(Qt::darkGray);
    myPlot->axisRect()->setBackground(Qt::white);
    myPlot->addGraph();
    myPlot->graph(0)->setPen(QPen(Qt::blue));
    myPlot->graph(0)->setBrush(Qt::NoBrush);
    myPlot->addGraph();
    myPlot->graph(1)->setPen(QPen(Qt::blue));
    myPlot->graph(1)->setLineStyle(QCPGraph::lsNone);
    myPlot->graph(1)->setScatterStyle(QCPScatterStyle::ssDisc);
    myPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    myPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    myPlot->xAxis->setAutoTickStep(false);
    myPlot->xAxis->setTickStep(4);

    myPlot->axisRect()->setupFullAxesBox();
    QCPPlotTitle *titleObj = new QCPPlotTitle(myPlot);
    titleObj->setText(title);
    titleObj->setFont(QFont("sans", 12, QFont::Bold));
    myPlot->plotLayout()->insertRow(0);
    myPlot->plotLayout()->addElement(0, 0, titleObj);
    myPlot->yAxis->setLabel(ylabel);
    myPlot->xAxis->setLabel("Time");
}

void MainWindow::GPSplotInit(){
    workArea testAreaBig = {
        10.465358,
        10.471861,
        59.425918,
        59.428036,
        QPixmap(":/images/Kart_Testarea.PNG"),
    };

    workArea testAreaSmall = {
        10.466020,
        10.470137,
        59.426879,
        59.428482,
        QPixmap(":/images/Kart_Testarea_small.PNG"),
    };

    workArea testAreaKMlogo = {
        10.464631,
        10.469189,
        59.427232,
        59.428829,
        QPixmap(":/images/Kart_Testarea_KMlogo.PNG"),
    };

    workArea IndreHavn = {
        10.462880,
        10.489146,
        59.423137,
        59.429957,
        QPixmap(":/images/Kart_Horten.PNG"),
    };

    workArea Pollen = {
        -20,
        40,
        -20,
        40,
        QPixmap(":/images/Kart_Pollen4.PNG"),
    };

    // Choose which area to show in satelite view:
    workArea currentArea = Pollen;

    satelitePhotoCorners.lowerLeft = QPointF(currentArea.LongitudeMin, currentArea.LatitudeMin);
    satelitePhotoCorners.upperRight = QPointF(currentArea.LongitudeMax, currentArea.LatitudeMax);
    setupSateliteView(currentArea);

    makeRealtimeGraph(ui->leftMotorPlot, "Surge Force", "input [N]");
    makeRealtimeGraph(ui->rightMotorPlot, "Yaw Moment", "input [Nm]");

    makeRealtimeGraph(ui->velocityPlot, "Speed", "Speed [km/h]");
    ui->velocityPlot->addGraph();
    ui->velocityPlot->graph(2)->setPen(QPen(Qt::red));
    ui->velocityPlot->graph(2)->setBrush(Qt::NoBrush);
    ui->velocityPlot->addGraph();
    ui->velocityPlot->graph(3)->setPen(QPen(Qt::red));
    ui->velocityPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->velocityPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->velocityPlot->graph(0)->removeFromLegend();
    ui->velocityPlot->graph(2)->removeFromLegend();
    ui->velocityPlot->graph(1)->setName("Speed");
    ui->velocityPlot->graph(3)->setName("Reference");
    ui->velocityPlot->legend->setFont(QFont(font().family(), 7));
    ui->velocityPlot->legend->setVisible(true);
    ui->velocityPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);



    makeRealtimeGraph(ui->headingPlot, "Heading", "Heading [deg]");
    ui->headingPlot->addGraph();
    ui->headingPlot->graph(2)->setPen(QPen(Qt::red));
    ui->headingPlot->graph(2)->setBrush(Qt::NoBrush);
    ui->headingPlot->addGraph();
    ui->headingPlot->graph(3)->setPen(QPen(Qt::red));
    ui->headingPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
    ui->headingPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);
    ui->headingPlot->graph(0)->removeFromLegend();
    ui->headingPlot->graph(2)->removeFromLegend();
    ui->headingPlot->graph(1)->setName("Heading");
    ui->headingPlot->graph(3)->setName("Reference");
    ui->headingPlot->legend->setFont(QFont(font().family(), 7));
    ui->headingPlot->legend->setVisible(true);
    ui->headingPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->velocityPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->velocityPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->velocityPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->velocityPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->headingPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->headingPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->headingPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->headingPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->leftMotorPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->leftMotorPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->leftMotorPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->leftMotorPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->rightMotorPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->rightMotorPlot->yAxis2, SLOT(setRange(QCPRange)));
    connect(ui->rightMotorPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->rightMotorPlot->xAxis2, SLOT(setRange(QCPRange)));

    //connect(ui->positionPlot, SIGNAL(mousePress(QMouseEvent*)), SLOT(positionClick( QMouseEvent *)));
    connect(ui->positionPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
    ui->positionPlot->setInteraction(QCP::iRangeDrag, true);
    ui->positionPlot->setInteraction(QCP::iRangeZoom, true);
    // setup a timer that reads GPS data from UDP:
    connect(&dataFetchTimer, SIGNAL(timeout()), this, SLOT(dataFetch()));
    dataFetchTimer.start(0); // Interval 0 means to refresh as fast as possible
    connect(&plotTimer, SIGNAL(timeout()), this, SLOT(plotGPSdata()));
    plotTimer.start(0); // Interval 0 means to refresh as fast as possible
    connect(&statusTimer, SIGNAL(timeout()), this, SLOT(checkStatus()));
    statusTimer.start(500); //ms

    // setup buttons:
    connect(ui->posToPDFbutton, SIGNAL (released()), this, SLOT (saveToPDF()));
    connect(ui->clearPlotButton, SIGNAL (released()), this, SLOT (clearPositionPlot()));
    connect(ui->clearWaypointsButton, SIGNAL (released()), this, SLOT (clearWaypoints()));
    connect(ui->generateAreaButton, SIGNAL (released()), this, SLOT (generateScanArea()));
    connect(ui->connectButton, SIGNAL (released()), this, SLOT (connectToJolner()));
    connect(ui->sendWaypointsButton, SIGNAL (released()), this, SLOT (sendWaypoints()));
    connect(ui->stopButton, SIGNAL (released()), this, SLOT (sendStop()));
    connect(ui->startButton, SIGNAL (released()), this, SLOT (sendStart()));
    connect(ui->speedGainButton, SIGNAL (released()), this, SLOT (sendSpeedGains()));
    connect(ui->speedRefApply, SIGNAL (released()), this, SLOT (sendSpeedRef()));
    connect(ui->headingGainButton, SIGNAL (released()), this, SLOT (sendHeadingGains()));
    connect(ui->headingRefApply, SIGNAL (released()), this, SLOT (sendHeadingRef()));
    connect(ui->arrowControl, SIGNAL (clicked()), this, SLOT (sendNewMode()));
    connect(ui->zigsagMode, SIGNAL (clicked()), this, SLOT (sendNewMode()));
    connect(ui->headingChase, SIGNAL (clicked()), this, SLOT (sendNewMode()));
    connect(ui->wpChase, SIGNAL (clicked()), this, SLOT (sendNewMode()));
    connect(ui->arrowLeft, SIGNAL (released()), this, SLOT (sendDirLeft()));
    connect(ui->arrowRight, SIGNAL (released()), this, SLOT (sendDirRight()));
    connect(ui->arrowUp, SIGNAL (released()), this, SLOT (sendDirUp()));
    connect(ui->arrowStop, SIGNAL (released()), this, SLOT (sendDirStop()));
    connect(ui->LOSbutton, SIGNAL (released()), this, SLOT (sendLOSdistance()));
}


void MainWindow::sendLOSdistance(){
    double LOSdistance = ui->LOSspinBox->value();
    string newLOSCommand = "$NDLOS," + to_string(LOSdistance) + "|\n";
    qInfo(newLOSCommand.c_str());
    if( !sendCommand(newLOSCommand.c_str()) ){
        ui->textBox->setText("Socket error while sending new LOS distance.");
    }
}

void MainWindow::checkStatus(){
    bool connectionOK = sendCommand("");

    // Update connection lamp and button:
    if( connectionOK ){
        ui->connectButton->setEnabled(false);
        ui->connectionLight->setStyleSheet(GREENLIGHT);
    }
    else{
        ui->connectButton->setEnabled(true);
        ui->connectionLight->setStyleSheet(REDLIGHT);
    }

    // Update controller input validation color:
    if( ui->speedKpSpinBox->value() == currentCTRLdata.Kps) ui->speedKpSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->speedKpSpinBox->setStyleSheet("background-color: pink");

    if( ui->speedKiSpinBox->value() == currentCTRLdata.Kis) ui->speedKiSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->speedKiSpinBox->setStyleSheet("background-color: pink");

    if( ui->speedKdSpinBox->value() == currentCTRLdata.Kds) ui->speedKdSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->speedKdSpinBox->setStyleSheet("background-color: pink");

    if( ui->headingKpSpinBox->value() == currentCTRLdata.Kph) ui->headingKpSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->headingKpSpinBox->setStyleSheet("background-color: pink");

    if( ui->headingKiSpinBox->value() == currentCTRLdata.Kih) ui->headingKiSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->headingKiSpinBox->setStyleSheet("background-color: pink");

    if( ui->headingKdSpinBox->value() == currentCTRLdata.Kdh) ui->headingKdSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->headingKdSpinBox->setStyleSheet("background-color: pink");

    if( ui->headingKdSpinBox->value() == currentCTRLdata.Kdh) ui->headingKdSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->headingKdSpinBox->setStyleSheet("background-color: pink");

    if( ui->speedRefSpinBox->value() == currentCTRLdata.speedRef) ui->speedRefSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->speedRefSpinBox->setStyleSheet("background-color: pink");

    if( ui->headingRefSpinBox->value() == currentCTRLdata.headingRef) ui->headingRefSpinBox->setStyleSheet("background-color: lightgreen");
    else ui->headingRefSpinBox->setStyleSheet("background-color: pink");

    if( ui->LOSspinBox->value() == currentCTRLdata.LOSdistance) ui->LOSspinBox->setStyleSheet("background-color: lightgreen");
    else ui->LOSspinBox->setStyleSheet("background-color: pink");


    // Green circle around Jolners next WayPoint:
    static Position nextWP = {
        currentCTRLdata.nextWPlat,
        currentCTRLdata.nextWPlong,
    };
    if (nextWP.Latitude != currentCTRLdata.nextWPlat || nextWP.Longitude != currentCTRLdata.nextWPlong){
        // jolnersPlan->clearData();
        nextWP = {
            currentCTRLdata.nextWPlat,
            currentCTRLdata.nextWPlong,
        };
        jolnersPlan->addData(nextWP.Longitude, nextWP.Latitude);

    }

    // Update mode of operation box:
    if( !connectionOK ){
        ui->arrowControl->setAutoExclusive(false);
        ui->arrowControl->setChecked(false);
        ui->arrowControl->setAutoExclusive(true);

        ui->zigsagMode->setAutoExclusive(false);
        ui->zigsagMode->setChecked(false);
        ui->zigsagMode->setAutoExclusive(true);

        ui->headingChase->setAutoExclusive(false);
        ui->headingChase->setChecked(false);
        ui->headingChase->setAutoExclusive(true);

        ui->wpChase->setAutoExclusive(false);
        ui->wpChase->setChecked(false);
        ui->wpChase->setAutoExclusive(true);
    }
}

void MainWindow::sendDirLeft(){
    string newCommand = "$DCOMM,LEFT|\n";
    if( !sendCommand(newCommand.c_str()) ){
        ui->textBox->setText(("Socket error while sending " + newCommand).c_str());
    }
    qInfo(newCommand.c_str());
}

void MainWindow::sendDirRight(){
    string newCommand = "$DCOMM,RIGHT|\n";
    if( !sendCommand(newCommand.c_str()) ){
        ui->textBox->setText(("Socket error while sending " + newCommand).c_str());
    }
    qInfo(newCommand.c_str());
}

void MainWindow::sendDirUp(){
    string newCommand = "$DCOMM,FORWARD|\n";
    if( !sendCommand(newCommand.c_str()) ){
        ui->textBox->setText(("Socket error while sending " + newCommand).c_str());
    }
    qInfo(newCommand.c_str());
}

void MainWindow::sendDirStop(){
    string newCommand = "$DCOMM,STOP|\n";
    if( !sendCommand(newCommand.c_str()) ){
        ui->textBox->setText(("Socket error while sending " + newCommand).c_str());
    }
    qInfo(newCommand.c_str());
}

void MainWindow::sendNewMode(){
    string newMode = "$MODES,";
    if( ui->arrowControl->isChecked()){
        newMode += "DIRECTCONTROL|\n";
        if( !sendCommand(newMode.c_str()) ){
            ui->textBox->setText("Socket error while sending DIRECTCONTROL.");
            ui->arrowControl->setAutoExclusive(false);
            ui->arrowControl->setChecked(false);
            ui->arrowControl->setAutoExclusive(true);
        }
    }
    else if( ui->zigsagMode->isChecked()){
        newMode += "ZIGZAG|\n";
        if( !sendCommand(newMode.c_str()) ){
            ui->textBox->setText("Socket error while sending ZIGZAG.");
            ui->zigsagMode->setAutoExclusive(false);
            ui->zigsagMode->setChecked(false);
            ui->zigsagMode->setAutoExclusive(true);
        }
    }
    else if( ui->headingChase->isChecked()){
        newMode += "HEADING|\n";
        if( !sendCommand(newMode.c_str()) ){
            ui->textBox->setText("Socket error while sending HEADING.");
            ui->headingChase->setAutoExclusive(false);
            ui->headingChase->setChecked(false);
            ui->headingChase->setAutoExclusive(true);
        }
    }
    else if( ui->wpChase->isChecked()){
        newMode += "WP|\n";
        if( !sendCommand(newMode.c_str()) ){
            ui->textBox->setText("Socket error while sending WP.");
            ui->wpChase->setAutoExclusive(false);
            ui->wpChase->setChecked(false);
            ui->wpChase->setAutoExclusive(true);
        }
    }
    qInfo(newMode.c_str());
}

void MainWindow::sendHeadingRef(){
    double ref = ui->headingRefSpinBox->value();
    string newRefCommand = "$HDREF," + to_string(ref) + "|\n";
    qInfo(newRefCommand.c_str());
    if( !sendCommand(newRefCommand.c_str()) ){
        ui->textBox->setText("Socket error while sending new heading ref.");
    }
}

void MainWindow::sendHeadingGains(){
    double Kp = ui->headingKpSpinBox->value();
    double Ki = ui->headingKiSpinBox->value();
    double Kd = ui->headingKdSpinBox->value();
    string newGainsCommand = "$NGHDC," + to_string(Kp) + "," + to_string(Ki) + "," + to_string(Kd) + "|\n";
    qInfo(newGainsCommand.c_str());
    if( !sendCommand(newGainsCommand.c_str()) ){
        ui->textBox->setText("Socket error while sending new heading gains.");
    }
}

void MainWindow::sendSpeedRef(){
    double ref = ui->speedRefSpinBox->value();
    string newRefCommand = "$SPREF," + to_string(ref) + "|\n";
    qInfo(newRefCommand.c_str());
    if( !sendCommand(newRefCommand.c_str()) ){
        ui->textBox->setText("Socket error while sending new speed ref.");
    }
}

void MainWindow::sendSpeedGains(){
    double Kp = ui->speedKpSpinBox->value();
    double Ki = ui->speedKiSpinBox->value();
    double Kd = ui->speedKdSpinBox->value();
    string newGainsCommand = "$NGSPD," + to_string(Kp) + "," + to_string(Ki) + "," + to_string(Kd) + "|\n";
    qInfo(newGainsCommand.c_str());
    if( !sendCommand(newGainsCommand.c_str()) ){
        ui->textBox->setText("Socket error while sending new speed gains.");
    }
}

void MainWindow::connectToJolner(){
    if( !ConnectToHost(COMMAND_RCV_PORT, COMMAND_RCV_IP) ){
        ui->connectionLight->setStyleSheet(REDLIGHT);
    }
    else{
        ui->connectionLight->setStyleSheet(GREENLIGHT);
    }
}

void MainWindow::sendStop(){
    if( !sendCommand("$STOPP|\n") ){
        ui->textBox->setText("Socket error while sending 'STOP'.");
    }
}

void MainWindow::sendStart(){
    if( !sendCommand("$START|\n") ){
        ui->textBox->setText("Socket error while sending 'START'.");
    }
}

void MainWindow::sendWaypoints(){
    int wpCount = 0;
    for (int i = 0; i < markedPositions.size(); i++){
        string newCommand = "$WPGGA,";
        newCommand += to_string(markedPositions[i].y()) + ',' + to_string(markedPositions[i].x()) + "|\n";
        if( !sendCommand(newCommand.c_str()) ){
        }
        else wpCount++;
    }
    QString resultMsg = QString::number(wpCount) + " waypoints sent to Jolner.";
    ui->textBox->setText(resultMsg);
}

void MainWindow::generateScanArea(){
    vector<QPointF> areaCorners = ConvexHull(markedPositions);
    scanPlan->clearData();
    for(int i = 0; i < areaCorners.size(); i++){
        areaCorners[i] = GPStoMeterCoord(areaCorners[i]);
    }

    markedPositions.clear();
    int depth = ui->depthSpinBox->value();
    vector<QPointF> scanTrajectory = pointsToWaypoints(areaCorners, depth);
    for(int i = 0; i < scanTrajectory.size(); i++){
        scanTrajectory[i] = meterCoordToGPS(scanTrajectory[i]);
        scanPlan->addData(scanTrajectory[i].x(), scanTrajectory[i].y());
        markedPositions.push_back(scanTrajectory[i]);
    }

}

void MainWindow::clearWaypoints(){
    markedPositions.clear();
    scanPlan->clearData();
    jolnersPlan->clearData();
    if( !sendCommand("$CLRWP|\n") ){
        ui->textBox->setText("Socket error while sending $CLRWP|.");
    }
}

void MainWindow::positionClick(QMouseEvent *event){
    QPointF GPSpoint = pixelPosToGPSPos(event->pos());

    // Check if outside valid range
    if  (GPSpoint.x() < satelitePhotoCorners.lowerLeft.x()
      || GPSpoint.x() > satelitePhotoCorners.upperRight.x()
      || GPSpoint.y() < satelitePhotoCorners.lowerLeft.y()
      || GPSpoint.y() > satelitePhotoCorners.upperRight.y() )
    {
        // disregard mouseclick:
        return;
    }
    qInfo() << GPSpoint.x() << ", " << GPSpoint.y();
    markedPositions.push_back(GPSpoint);
    markedPositions = ConvexHull(markedPositions);
    scanPlan->clearData();
    for(int i = 0; i < markedPositions.size(); i++){
        scanPlan->addData(markedPositions[i].x(), markedPositions[i].y());
    }
}
void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->positionPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->positionPlot->axisRect()->setRangeZoom(ui->positionPlot->xAxis->orientation());
  else if (ui->positionPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->positionPlot->axisRect()->setRangeZoom(ui->positionPlot->yAxis->orientation());
  else
    ui->positionPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::clearPositionPlot(){
    positionCurve->clearData();
}

void MainWindow::saveToPDF(){
    time_t rawtime;
    struct tm * timeinfo;
    char fileName [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (fileName,80,"Positionplot %d%h%G %Hh%Mm%Ss .PDF",timeinfo);
    ui->positionPlot->savePdf(fileName,false);
}

void MainWindow::dataFetch(){
    static bool initialized = false;
    static SOCKET GPSrxSocket;
    static SOCKET CTRLrxSocket;
    if(!initialized){
        GPSrxSocket = UDPrxSocketConstructor( GPS_UDP_PORT );
        CTRLrxSocket = UDPrxSocketConstructor( CONTROLLER_UDP_PORT );
        initialized = true;
    }

    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0)
    {
        receiveGPSdata(GPSrxSocket, currentGPSdata);
        receiveCTRLdata(CTRLrxSocket, currentCTRLdata);
        lastPointKey = key;
    }
}

void MainWindow::plotGPSdata(){
    int timeHistory = 20;
    // calculate new data points:
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    static double lastPointKey = 0;
    QString titleParam;
    if (key-lastPointKey > 0.01) // at most add point every 10 ms
    {
        // add data to lines:
        ui->velocityPlot->graph(0)->addData(key, currentGPSdata.GroundSpeedKmh);
        // set data of dots:
        ui->velocityPlot->graph(1)->clearData();
        ui->velocityPlot->graph(1)->addData(key, currentGPSdata.GroundSpeedKmh);
        // remove data of lines that's outside visible range:
        ui->velocityPlot->graph(0)->removeDataBefore(key-timeHistory);
        // rescale value (vertical) axis to fit the current data:
        ui->velocityPlot->graph(0)->rescaleValueAxis();
        ui->velocityPlot->graph(2)->addData(key, currentCTRLdata.speedRef);
        // set data of dots:
        ui->velocityPlot->graph(3)->clearData();
        ui->velocityPlot->graph(3)->addData(key, currentCTRLdata.speedRef);
        // remove data of lines that's outside visible range:
        ui->velocityPlot->graph(2)->removeDataBefore(key-timeHistory);
        ui->velocityPlot->graph(2)->rescaleValueAxis(true);
        ui->speedDisplay->setText(QString::number(currentGPSdata.GroundSpeedKmh));

        ui->headingPlot->graph(0)->addData(key, currentCTRLdata.heading);
        // set data of dots:
        ui->headingPlot->graph(1)->clearData();
        ui->headingPlot->graph(1)->addData(key, currentCTRLdata.heading);
        // remove data of lines that's outside visible range:
        ui->headingPlot->graph(0)->removeDataBefore(key-timeHistory);
        // rescale value (vertical) axis to fit the current data:
        ui->headingPlot->graph(0)->rescaleValueAxis();
        ui->headingPlot->graph(2)->addData(key, currentCTRLdata.headingRef);
        // set data of dots:
        ui->headingPlot->graph(3)->clearData();
        ui->headingPlot->graph(3)->addData(key, currentCTRLdata.headingRef);
        // remove data of lines that's outside visible range:
        ui->headingPlot->graph(2)->removeDataBefore(key-timeHistory);
        ui->headingPlot->graph(2)->rescaleValueAxis(true);
        ui->headingDisplay->setText(QString::number(currentCTRLdata.heading));

        ui->rightMotorPlot->graph(0)->addData(key, currentCTRLdata.rightInput);
        // set data of dots:
        ui->rightMotorPlot->graph(1)->clearData();
        ui->rightMotorPlot->graph(1)->addData(key, currentCTRLdata.rightInput);
        // remove data of lines that's outside visible range:
        ui->rightMotorPlot->graph(0)->removeDataBefore(key-timeHistory/2);
        // rescale value (vertical) axis to fit the current data:
        ui->rightMotorPlot->graph(0)->rescaleValueAxis();

        ui->leftMotorPlot->graph(0)->addData(key, currentCTRLdata.leftInput);
        // set data of dots:
        ui->leftMotorPlot->graph(1)->clearData();
        ui->leftMotorPlot->graph(1)->addData(key, currentCTRLdata.leftInput);
        // remove data of lines that's outside visible range:
        ui->leftMotorPlot->graph(0)->removeDataBefore(key-timeHistory/2);
        // rescale value (vertical) axis to fit the current data:
        ui->leftMotorPlot->graph(0)->rescaleValueAxis();

        positionCurve->addData(currentGPSdata.Longitude, currentGPSdata.Latitude);
        QPixmap boat = QPixmap(":/images/JolnerIcon.png");
        boat = boat.scaledToWidth(20);
        boat = boat.scaledToHeight(40);
        QMatrix rm;
        rm.rotate(currentCTRLdata.heading);
        boat = boat.transformed(rm);
        // set data of dots:
        ui->positionPlot->graph(0)->clearData();
        ui->positionPlot->graph(0)->addData(currentGPSdata.Longitude, currentGPSdata.Latitude);
        ui->positionPlot->graph(0)->setScatterStyle(QCPScatterStyle(boat));
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of timeHistory):
    ui->velocityPlot->xAxis->setRange(key+0.25, timeHistory, Qt::AlignRight);
    ui->headingPlot->xAxis->setRange(key+0.25, timeHistory, Qt::AlignRight);
    ui->leftMotorPlot->xAxis->setRange(key+0.25, timeHistory/2, Qt::AlignRight);
    ui->rightMotorPlot->xAxis->setRange(key+0.25, timeHistory/2, Qt::AlignRight);
    ui->velocityPlot->replot();
    ui->headingPlot->replot();
    ui->positionPlot->replot();
    ui->leftMotorPlot->replot();
    ui->rightMotorPlot->replot();
}

QPointF pixelPosToGPSPos(QPoint pixelPoint){
    int leftOffset =  74;
    int topOffset  = 44;
    int pixelWidth = 762;
    int pixelHeight = 657;
    float GPSwidth = satelitePhotoCorners.upperRight.x() - satelitePhotoCorners.lowerLeft.x();
    float GPSheight = satelitePhotoCorners.upperRight.y() - satelitePhotoCorners.lowerLeft.y();

    float xFraction = (float)(pixelPoint.x() - leftOffset)/pixelWidth;
    float yFraction = (float)(pixelPoint.y() - topOffset)/pixelHeight;

    QPointF gpsPos;
    gpsPos.setX(satelitePhotoCorners.lowerLeft.x() + xFraction*GPSwidth);
    gpsPos.setY(satelitePhotoCorners.upperRight.y() - yFraction*GPSheight); // (-) because (0,0) is in upper left corner.
    return gpsPos;
}
