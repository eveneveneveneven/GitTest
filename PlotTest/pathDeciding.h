#pragma once
#include <vector>
#include <QPointF>

//Depth, coverage angle of sonar and coverage overlap are currently set as constants in the beginning of pathdecider.cpp.

/*
ConvexHull returns a vector of points in a convex hull, defined counter clockwise.
The first point is the one with the lowest y-value (if two points have equal y-value, the one with the highest x-value is chosen).
If there are more than two unequal points in input vector P (more points than on a line), the first point is also added to the back of the vector.
*/
std::vector<QPointF> ConvexHull(std::vector<QPointF> P);


//Returns a scan trajectory as a series of waypoints based on the area made up by the points passed as input arguments.
std::vector<QPointF> pointsToWaypoints(std::vector<QPointF> points, int depth);

