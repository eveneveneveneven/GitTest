
#ifndef GPSPARSER_H
#define GPSPARSER_H

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <winsock2.h>
#include <QPointF>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define GPS_UDP_PORT		31036
#define CONTROLLER_UDP_PORT 32123

struct GPSdata {
    float Longitude;
    float Latitude;
    float Heading;
    float RateOfTurn;
    int Time;
    int FixQuality;
    float TrueTrack;
    float MagneticTrack;
    float GroundSpeedKmh;
};
struct CTRLdata {
    double     leftInput;
    double     rightInput;
    float   headingRef;
    float   speedRef;
    double  heading;
    double  nextWPlong;
    double  nextWPlat;
    double  Kps;
    double  Kis;
    double  Kds;
    double  Kph;
    double  Kih;
    double  Kdh;
    double  LOSdistance;
};

SOCKET UDPrxSocketConstructor( int Port );
void receiveGPSdata(SOCKET rxSocket, GPSdata & gpsData);
void receiveCTRLdata(SOCKET rxSocket, CTRLdata & ctrlData);
void printData(GPSdata & gpsData);

#endif
