#define _USE_MATH_DEFINES
#include "gpstools.h"
#include <math.h>
#include<vector>
#include <iomanip>

double D2R(double yawRefDeg) {
    return yawRefDeg*(M_PI / 180);
}

double DistanceBetweenPositions(Position DesiredPosition, Position CurrentPosition) {
    //Calculates the great-circle distance between two points (shortest distance over earth's surface)
    double Phi1 = D2R(CurrentPosition.Latitude);
    double Phi2 = D2R(DesiredPosition.Latitude);
    double Lambda1 = D2R(CurrentPosition.Longitude);
    double Lambda2 = D2R(DesiredPosition.Longitude);
    double DeltaPhi = Phi2 - Phi1;
    double DeltaLambda = Lambda2 - Lambda1;
    double a = sin(DeltaPhi / 2)*sin(DeltaPhi / 2) + cos(Phi1)*cos(Phi2)*sin(DeltaLambda / 2)*sin(DeltaLambda / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double R = 6371000;
    double Distance = R*c;
    //std::cout << "Distance: " << Distance << std::endl;
    return Distance;
}



QPointF GPStoMeterCoord(QPointF gpsPos){
    int metersPrLatitude = 111200;
    int metersPrLongitude = 56600;

    QPointF meterPos;
    meterPos.setX(gpsPos.x()*metersPrLongitude);
    meterPos.setY(gpsPos.y()*metersPrLatitude);

    return meterPos;
}

QPointF meterCoordToGPS(QPointF meterPos){
    int metersPrLatitude = 111200;
    int metersPrLongitude = 56600;

    QPointF GPSpos;
    GPSpos.setX(meterPos.x()/metersPrLongitude);
    GPSpos.setY(meterPos.y()/metersPrLatitude);

    return GPSpos;
}
