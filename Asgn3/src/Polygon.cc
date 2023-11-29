#include "../include/Circle.h"
#include "../include/Polygon.h"
#include "../include/Reuleaux.h"
#include "../include/StaticFunctions.h"

Polygon::Polygon(std::vector<Point> v){
    vertices = v;
}

bool Polygon::ContainedBy(Circle &circle){
    for(size_t i = 0; i < vertices.size(); i++){
        if(dist(circle.center, vertices[i]) > circle.radius){
            return false;
        }
    }
    return true;
}

bool Polygon::ContainedBy(Polygon &polygon){
    for(size_t i = 0; i < vertices.size(); i++){
        if(pointInsidePolygon(vertices[i], polygon) == false){
            return false;
        }
    }
    for(size_t i = 0; i < vertices.size(); i++){
        for(size_t x = 0; x < polygon.vertices.size(); x++){
            Point p1 = vertices[i];
            Point p2 = vertices[i+1%vertices.size()];
            Point q1 = vertices[x];
            Point q2 = vertices[x+1%polygon.vertices.size()];
            if(lineSegmentsIntersect(p1,p2,q1,q2) == true){
                return false;
            }
        }
    }
    return true;
}

bool Polygon::ContainedBy(ReuleauxTriangle &rt){
    for(size_t i = 0; i < vertices.size(); i++){
        if(pointInsideTriangle(vertices[i], rt) == false){
            return false;
        }
    }
    for(size_t i = 0; i < vertices.size(); i++){
            Point p1 = vertices[i];
            Point p2 = vertices[i+1%vertices.size()];
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