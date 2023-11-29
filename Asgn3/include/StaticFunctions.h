#include "Point.h"
#include "Polygon.h" 
#include "Reuleaux.h"

//distance between 2 points
double dist(Point a, Point b); 

//minDist between line segment AB and E
double minDist(Point a, Point b, Point e);

//raycasting algorithm for testing if a point is in a polygon
bool pointInsidePolygon(Point p, Polygon poly);

bool pointInsideTriangle(Point p, ReuleauxTriangle triangle);

//Function to check if 2 line segements intersect with eachother
//does not count intersection if a line segment ends on the other line
bool lineSegmentsIntersect(Point a1, Point a2, Point b1, Point b2);