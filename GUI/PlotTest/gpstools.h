#ifndef GPSTOOLS_H
#define GPSTOOLS_H

#include <QPointF>

struct Position {
    double Latitude;
    double Longitude;
};

double DistanceBetweenPositions(Position DesiredPosition, Position CurrentPosition);


QPointF GPStoMeterCoord(QPointF gpsPos);

QPointF meterCoordToGPS(QPointF meterPos);

#endif // GPSTOOLS_H
