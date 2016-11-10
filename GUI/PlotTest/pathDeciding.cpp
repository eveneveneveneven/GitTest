#include "pathDeciding.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <QPointF>


const double PI = 3.141592653589793238;


const double DOBLE_CMP_EPSILON = 1e-12;
bool cmpDouble(double a, double b) {
    return (fabs(a - b) < DOBLE_CMP_EPSILON);
}


/*BEGINNING OF GENERAL GEOMETRY PART*/

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}
    bool operator ==(const Point & other) const {
        return cmpDouble(x,other.x) && cmpDouble(y,other.y);
    }
    bool operator <(const Point & other) const {
        if (y < other.y) {
            return true;
        }
        else if(cmpDouble(y,other.y)){
            return x > other.x; //the smallest point is the one with smallest y-value. If equal y-value, the one with the highest x-value
        }
        else {
            return false;
        }
    }
};

struct Line {	//ax + by + c = 0
    double a, b, c;
};

struct Vec {
    double x, y;
    Vec(double _x, double _y) : x(_x), y(_y) {}
};

struct lineIntersection {
    bool doIntersect;
    Point point;
};

Line pointsToLine(Point p1, Point p2);
struct LineSeg{
    Point startPoint;
    Point endPoint;
    Line line;
    LineSeg() : startPoint({ 0,0 }), endPoint({ 0,0 }) {
        line = pointsToLine(startPoint, endPoint);
    }
    LineSeg(Point _p1, Point _p2) : startPoint(_p1), endPoint(_p2) {
        line = pointsToLine(_p1, _p2);
    }
};

double dist(Point p1, Point p2) {
    return hypot(p1.x - p2.x, p1.y - p2.y);
}
Line pointsToLine(Point p1, Point p2) {
    Line l;
    if (cmpDouble(p1.x,p2.x)) {
        l.a = 1;
        l.b = 0;
        l.c = -p1.x;
    }
    else {
        l.a = -static_cast<double>(p1.y - p2.y) / (p1.x - p2.x);
        l.b = 1.0;
        l.c = -static_cast<double>(l.a * p1.x) - p1.y;
    }
    return l;
}
Vec toVec(Point a, Point b) {
    return Vec(b.x - a.x, b.y - a.y);
}
Vec scale(Vec v, double s) {
    return Vec(v.x*s, v.y*s);
}
Point translate(Point p, Vec v) {
    return Point(p.x + v.x, p.y + v.y);
}
double dot(Vec a, Vec b) {
    return (a.x * b.x) + (a.y * b.y);
}
double norm_sq(Vec v) {
    return dot(v, v);
}
double norm(Vec v) {
    return std::sqrt(norm_sq(v));
}
double cross(Vec a, Vec b) {
    return a.x * b.y - a.y * b.x;
}
bool ccw(Point p, Point q, Point r) {
    return cross(toVec(p, q), toVec(p, r)) > 0;
}
bool collinear(Point p, Point q, Point r) {
    return cmpDouble(cross(toVec(p, q), toVec(p, r)), 0);
}
Point pivot;
bool angleCmp(Point a, Point b) {
    if (collinear(pivot, a, b)) {
        return dist(pivot, a) < dist(pivot, b);
    }
    double d1x = a.x - pivot.x;
    double d1y = a.y - pivot.y;
    double d2x = b.x - pivot.x;
    double d2y = b.y - pivot.y;
    return (atan2(d1y, d1x) - atan2(d2y, d2x)) < 0;
}
Vec normalVecCCW(Vec v)
{
    return Vec(-v.y,v.x);
}
Vec unity(Vec v)
{
    return Vec(v.x/norm(v),v.y/norm(v));
}
Line makeParallelLine(LineSeg ls, double displacement) {
    Vec normal = normalVecCCW(toVec(ls.startPoint, ls.endPoint));
    Vec unityNormal = unity(normal);
    Point p1 = translate(ls.startPoint, scale(unityNormal,displacement));
    Point p2 = translate(ls.endPoint, scale(unityNormal,displacement));
    return pointsToLine(p1, p2);
}
LineSeg findLongestLineSeg(std::vector<LineSeg> lineSegs) {
    int indexLongestLineSeg = 0;
    double longestLength = 0;
    double length = 0;

    for (unsigned int i = 0; i < lineSegs.size(); i++) {
        length = dist(lineSegs[i].startPoint, lineSegs[i].endPoint);
        if (length > longestLength) {
            longestLength = length;
            indexLongestLineSeg = i;
        }
    }

    return lineSegs[indexLongestLineSeg];
}
//check if point within line segment "rectangle" (min,max)
bool isPointBetweenStartAndEndPointValue(Point p, LineSeg ls) {
    return (p.x > std::min(ls.startPoint.x, ls.endPoint.x) || cmpDouble(p.x, std::min(ls.startPoint.x, ls.endPoint.x)))
        && (p.x < std::max(ls.startPoint.x, ls.endPoint.x) || cmpDouble(p.x, std::max(ls.startPoint.x, ls.endPoint.x)))
        && (p.y > std::min(ls.startPoint.y, ls.endPoint.y) || cmpDouble(p.y, std::min(ls.startPoint.y, ls.endPoint.y)))
        && (p.y < std::max(ls.startPoint.y, ls.endPoint.y) || cmpDouble(p.y, std::max(ls.startPoint.y, ls.endPoint.y)));
}
lineIntersection intersectionBetweenLineAndLineSeg(Line l, LineSeg ls) {
    lineIntersection intersection;
    double determinant = l.a * ls.line.b - ls.line.a * l.b;
    if (determinant == 0) { //parallell line
        intersection.doIntersect = false;
        intersection.point = { 0,0 };
        return intersection;
    }
    else {
        intersection.point.x = (l.b * ls.line.c - ls.line.b * l.c) / determinant;
        intersection.point.y = (ls.line.a * l.c - l.a * ls.line.c) / determinant;
        //check if intersection point within line segment "rectangle" (min,max)
        if (isPointBetweenStartAndEndPointValue(intersection.point, ls)) {
            intersection.doIntersect = true;
        }
        else {
            intersection.doIntersect = false;
        }
        return intersection;
    }
}
std::vector<LineSeg> pointVectorToLineSegVector(std::vector<Point> points) {
    std::vector<LineSeg> lineSegs;
    for (unsigned int i = 0; i < points.size()-1; i++) {
        lineSegs.push_back(LineSeg(points[i], points[i + 1]));
    }
    return lineSegs;
}

/*END OF GENERAL GEOMETRY PART*/


std::vector<Point> ConvexHull(std::vector<Point> P) {
    //Remove duplicate points (and also set starting point):
    sort(P.begin(), P.end());
    P.erase(std::unique(P.begin(), P.end()), P.end());

    int i, j;
    int n = static_cast<int>(P.size());
    if (n < 3) {
        return P;
    }

    pivot = P[0];
    sort(++P.begin(), P.end(), angleCmp); //sort points counter clockwise

    std::vector<Point> S;
    S.push_back(P[n - 1]);
    S.push_back(P[0]);
    S.push_back(P[1]);
    i = 2;
    while (i < n) { //removes points inside hull
        j = static_cast<int>(S.size()) - 1;
        if (ccw(S[j - 1], S[j], P[i]))
            S.push_back(P[i++]);
        else
            S.pop_back();
    }
    S.erase(S.begin());
    S.push_back(S[0]);  //push first element to back to avoid corner case

    return S;
}
std::vector<Point> pointsToWaypoints(std::vector<Point> points, int depth){
    const double COVERAGE_ANGLE = 70;
    const double SONAR_COVERAGE = 2*depth*tan(COVERAGE_ANGLE*(PI/180.0));
    const double COVERAGE_OVERLAP = 0.5;    //overlap of sonar coverage between two driving lines
    const double DIST_BETWEEN_DRIVING_LINES = (1-COVERAGE_OVERLAP)*SONAR_COVERAGE; //(13.74 with 5 m depth and 70 deg coverage angle)

    std::vector<Point>convexhullPoints = ConvexHull(points);
    if (convexhullPoints.size() < 3) {
        return convexhullPoints;
    }

    //pointvectorToLineSegVector used on chPoints gives us all line segments (including segment back to start)
    std::vector<LineSeg> lineSegs = pointVectorToLineSegVector(convexhullPoints);
    LineSeg longestLineSeg = findLongestLineSeg(lineSegs);


    std::vector<Point> waypoints;
    Line parallelLine;
    std::vector<Point> tempIntersections;
    int lineNo = 0;
    int intersectCountOld = -1;
    int intersectCountNew = 0;

    while (intersectCountNew > intersectCountOld) {
        intersectCountOld = intersectCountNew;
        lineNo++;
        if (lineNo == 1) {
            parallelLine = makeParallelLine(longestLineSeg, 0.5 * SONAR_COVERAGE);
        }
        else if (lineNo > 1) {
            parallelLine = makeParallelLine(longestLineSeg, 0.5 * SONAR_COVERAGE + DIST_BETWEEN_DRIVING_LINES * (lineNo - 1));
        }
        for (LineSeg ls : lineSegs) {
            lineIntersection inter = intersectionBetweenLineAndLineSeg(parallelLine, ls);
            if (inter.doIntersect) {
                tempIntersections.push_back(inter.point);
                intersectCountNew++;
            }
        }
        if (intersectCountOld == 0) {	//No previous intersects
            for (Point intersectionPoint : tempIntersections) {
                waypoints.push_back(intersectionPoint);
            }
        }
        else if (tempIntersections.size() == 1) {
            waypoints.push_back(tempIntersections[0]);
        }
        else if (tempIntersections.size() == 2) {   //we want to go to the closest intersection first
            if (dist(waypoints.back(), tempIntersections[0]) < dist(waypoints.back(), tempIntersections[1])) {
                waypoints.push_back(tempIntersections[0]);
                waypoints.push_back(tempIntersections[1]);
            }
            else {
                waypoints.push_back(tempIntersections[1]);
                waypoints.push_back(tempIntersections[0]);
            }
        }
        tempIntersections.clear();
    }

    if (waypoints.size() == 0 && lineNo == 1){ //first parallel line didn't intersect any points
        //all points within sonar reach if we drive along longest line segment
        waypoints.push_back(longestLineSeg.startPoint);
        waypoints.push_back(longestLineSeg.endPoint);
        return waypoints;
    }

    //check if the parallel line 0.5 * SONAR_COVERAGE away from last parallel line intersects any lineSegs.
    //If it does, the sonar was not within reach on last driving line, and we add two more waypoints
    Line checkSonarNotInReachLine = makeParallelLine(longestLineSeg, 0.5 * SONAR_COVERAGE + DIST_BETWEEN_DRIVING_LINES * (lineNo - 2) + 0.5 * SONAR_COVERAGE);
    bool intersectsCheckLine = false;
    for (LineSeg ls : lineSegs) {
        lineIntersection inter = intersectionBetweenLineAndLineSeg(checkSonarNotInReachLine, ls);
        if (inter.doIntersect) {
            intersectsCheckLine = true;
        }
    }
    if (intersectsCheckLine && (waypoints.size() >= 2)) { //not within sonar reach
        Point lastPoint = waypoints[waypoints.size() - 1];
        Point secondToLast = waypoints[waypoints.size() - 2];
        Vec unityNormal = unity(normalVecCCW(toVec(secondToLast, lastPoint)));
        Point newSecondToLast = translate(lastPoint, scale(unityNormal, DIST_BETWEEN_DRIVING_LINES));
        Point newLastPoint = translate(secondToLast, scale(unityNormal, DIST_BETWEEN_DRIVING_LINES));
        waypoints.push_back(newSecondToLast);
        waypoints.push_back(newLastPoint);
    }

    return waypoints;
}


std::vector<QPointF> toQPointFVec(std::vector<Point> points){
    std::vector<QPointF> qpoints;
    for(Point p : points){
        qpoints.push_back(QPointF(p.x,p.y));
    }
    return qpoints;
}

std::vector<Point> toPointVec(std::vector<QPointF> qpoints){
    std::vector<Point> points;
    for(QPointF qPoint : qpoints){
        points.push_back(Point(qPoint.x(),qPoint.y()));
    }
    return points;
}

std::vector<QPointF> ConvexHull(std::vector<QPointF> P){
    std::vector<Point> chPoints = ConvexHull(toPointVec(P));
    return toQPointFVec(chPoints);
}

std::vector<QPointF> pointsToWaypoints(std::vector<QPointF> points, int depth){
    std::vector<Point> wayPoints = pointsToWaypoints(toPointVec(points), depth);
    return toQPointFVec(wayPoints);
}



