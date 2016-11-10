#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define _WINSOCKAPI_ // Trengs akkurat her for å fjerne en rar bug med winsock2.h

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include "gpsparser.h"


struct workArea {
    double  LongitudeMin;
    double  LongitudeMax;
    double  LatitudeMin;
    double  LatitudeMax;
    QPixmap SatelitePhoto;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updatecurrentGPSdata(GPSdata newData);
    void GPSplotInit();
    void setupRealtimeDataDemo();

    QCPPlotTitle *headingTitle;
    QCPPlotTitle *speedTitle;
    QCPPlotTitle *leftMotorTitle;
    QCPPlotTitle *rightMotorTitle;
    QCPPlotTitle *positionTitle;

private slots:
    void plotGPSdata();
    void dataFetch();
    void checkStatus();
    void clearPositionPlot();
    void saveToPDF();
    void positionClick( QMouseEvent *event);
    void mouseWheel();
    void clearWaypoints();
    void generateScanArea();
    void connectToJolner();
    void sendWaypoints();
    void sendStop();
    void sendStart();
    void sendSpeedGains();
    void sendSpeedRef();
    void sendHeadingGains();
    void sendHeadingRef();
    void sendNewMode();
    void sendDirLeft();
    void sendDirRight();
    void sendDirUp();
    void sendDirStop();
    void sendLOSdistance();


private:
    void setupSateliteView(workArea currentArea);
    GPSdata currentGPSdata;
    CTRLdata currentCTRLdata;
    Ui::MainWindow *ui;
    QTimer dataFetchTimer;
    QTimer plotTimer;
    QTimer statusTimer;
    QCPCurve *positionCurve;
    QCPCurve *scanPlan;
    QCPCurve *jolnersPlan;

};

#endif // MAINWINDOW_H
