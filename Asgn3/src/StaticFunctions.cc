#include <math.h> 
#include <iostream>
#include "../include/StaticFunctions.h"
#include "../include/Point.h"


double dist(Point a, Point b){
    double xDist = b.x - a.x;
    double yDist = b.y - a.y;
    return sqrt((xDist*xDist) + (yDist*yDist));
}

//Code for minimum distance from a line segment to a point
//is adapted from GeeksForGeeks
//https://www.geeksforgeeks.org/minimum-distance-from-a-point-to-the-line-segment-using-vectors/
double minDist(Point a, Point b, Point e){
    double AB[2] = {b.x - a.x, b.y - a.y};
    double BE[2] = {e.x - b.x, e.y - b.y};
    double AE[2] = {e.x - a.x, e.y - a.y};
    double AB_BE = AB[0]*BE[0] + AB[1]*BE[1];
    double AB_AE = AB[0]*AE[0] + AB[1]*AE[1];
    double minDist = 0;

    if(AB_BE > 0){
        double y = e.y - b.y;
        double x = e.x - b.x;
        minDist = sqrt(x*x + y*y);
    } else if(AB_AE < 0){
        double y = e.y - a.y;
        double x = e.x - a.x;
        minDist = sqrt(x*x + y*y);
    } else {
        double x1 = AB[0];
        double y1 = AB[1];
        double x2 = AE[0];
        double y2 = AE[1];
        double mod = sqrt(x1*x1 + y1*y1);
        minDist = abs(x1*y2 - y1*x2)/mod;
    }
    return minDist;
}

//Ray casting algorithm
//Implementation of algorithm found online
//at wikipedia and other places 
bool pointInsidePolygon(Point p, Polygon poly){
    bool inside = false;
    double x1, x2, y1, y2;
    for(size_t i = 0; i < poly.vertices.size(); i++){
        x1 = poly.vertices[i].x;
        x2 = poly.vertices[i+1%poly.vertices.size()].x;
        y1 = poly.vertices[i].y;
        y2 = poly.vertices[i+1%poly.vertices.size()].y;
        if((p.y < y1) != (p.y < y2) && p.x < (x2-x1) * (p.y-y1) / (y2-y1) + x1){
            //std::cout << "x1 = " << x1 << " x2 = " << x2 << " y1 = " << y1 << " y2 = " << y2 << std::endl;
            inside = !inside;
        }
    }
    return inside;
}

//Method for finding point within a triangle found online,
//I don't remember exactly where

double area(Point p1, Point p2, Point p3){
    return abs((p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y)+ p3.x*(p1.y-p2.y))/2.0);
}

bool pointInsideTriangle(Point p, ReuleauxTriangle triangle){
    Point p1 = triangle.vertices[0];
    Point p2 = triangle.vertices[1];
    Point p3 = triangle.vertices[2];
   double A = area (p1,p2,p3);
   double A1 = area (p,p2, p3);
   double A2 = area (p1, p, p3);
   double A3 = area (p1, p2, p);
   return (A == A1 + A2 + A3);
}


//LineSegmentIntersection and associated code adpated from
//Geek to Geeks and other places
//https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
int orientation(Point p, Point q, Point r)
{
    double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;
    return (val > 0)? 1: 2; 
}

bool lineSegmentsIntersect(Point p1, Point p2, Point q1, Point q2){
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
    if (o1 != o2 && o3 != o4) { return true; }
    return false;
}