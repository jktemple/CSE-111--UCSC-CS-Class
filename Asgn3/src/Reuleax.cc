#include "../include/Circle.h"
#include "../include/Polygon.h"
#include "../include/Reuleaux.h"
#include "../include/StaticFunctions.h"

ReuleauxTriangle::ReuleauxTriangle(const Point v[3]){
    vertices[0] = v[1];
    vertices[1] = v[1];
    vertices[2] = v[2];
}

bool ReuleauxTriangle::ContainedBy(Circle &circle){
    for(int i = 0; i < 3; i++){
        if(dist(circle.center, vertices[i]) > circle.radius){
            return false;
        }
    }
    return true;
}
bool ReuleauxTriangle::ContainedBy(Polygon &polygon){
    for(int i = 0; i < 3; i++){
        if(pointInsidePolygon(vertices[i], polygon) == false){
            return false;
        }
    }
    for(int i = 0; i < 3; i++){
        for(size_t x = 0; x < polygon.vertices.size(); x++){
            Point p1 = vertices[i];
            Point p2 = vertices[i+1%3];
            Point q1 = vertices[x];
            Point q2 = vertices[x+1%polygon.vertices.size()];
            if(lineSegmentsIntersect(p1,p2,q1,q2) == true){
                return false;
            }
        }
    }
    return true;
}
bool ReuleauxTriangle::ContainedBy(ReuleauxTriangle &rt){
    for(int i = 0; i < 3; i++){
        if(pointInsideTriangle(vertices[i], rt) == false){
            return false;
        }
    }
    for(int i = 0; i < 3; i++){
            Point p1 = vertices[i];
            Point p2 = vertices[i+1%3];
            if(lineSegmentsIntersect(p1,p2,rt.vertices[0],rt.vertices[1]) == true){
                return false;
            } else if(lineSegmentsIntersect(p1,p2,rt.vertices[1],rt.vertices[2]) == true){
                return false;
            } else if(lineSegmentsIntersect(p1,p2,rt.vertices[2],rt.vertices[0]) == true){
                return false;
            }
    }
    return true;
}