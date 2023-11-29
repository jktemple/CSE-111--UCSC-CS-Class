#include "../include/Circle.h"
#include "../include/Polygon.h"
#include "../include/Reuleaux.h"
#include "../include/StaticFunctions.h"
#include <iostream>


Circle::Circle(const Point &c, double r){
    center = Point(c.x, c.y);
    radius = r;
}

bool Circle::ContainedBy(Circle &circle){
    if(radius > circle.radius){ 
        //to be contained this must have
        //smaller radius than the circle 
        //containing it
        return false;
    } else if (dist(center, circle.center) >= circle.radius) {
        //center of this cirlce must be inside of the containg circle
        return false;
    }
    if(center.x + radius > circle.center.x + circle.radius){
        return false;
    } else if(center.x - radius < circle.center.x - circle.radius) {
        return false;
    } else if(center.y + radius > circle.center.y + circle.radius) {
        return false;
    } else if(center.y - radius < circle.center.y - circle.radius) {
        return false;
    }
    return true;
}

bool Circle::ContainedBy(Polygon &polygon){

    if(!pointInsidePolygon(center, polygon)){
        // std::cout << "point inside" << std::endl;
        return false;
    }

    for(Point p : polygon.vertices){
        if(dist(center, p) < radius){
            //std::cout << "fail from for each" << std::endl;
            return false;
        }
    }

    for(size_t x = 0; x < polygon.vertices.size() - 1; x ++){
        if(minDist(polygon.vertices[x], polygon.vertices[x+1], center) < radius){
            //::cout<<"min distance = " << minDist(polygon.vertices[x], polygon.vertices[x+1], center) 
           // << " radius = " << radius << std::endl;
            return false;
        }
    }
    if(minDist(polygon.vertices[polygon.vertices.size() - 1], polygon.vertices[0], center) < radius){
        //std::cout<<"min distance = " << minDist(polygon.vertices[polygon.vertices.size() - 1], polygon.vertices[0], center) 
            //<< " radius = " << radius << std::endl;
        return false;
    }
    return true;
}

bool Circle::ContainedBy(ReuleauxTriangle &rt){

    if(pointInsideTriangle(center, rt) == false){
        std::cout << "fail from pointInsideTriangle" << std::endl;
        return false;
    }   

    if(minDist(rt.vertices[0], rt.vertices[1], center) < radius){
        return false;
    } else if(minDist(rt.vertices[1], rt.vertices[2], center) < radius){
        return false;
    } else if(minDist(rt.vertices[2], rt.vertices[0], center) < radius){
        return false;
    }
    return true;
}