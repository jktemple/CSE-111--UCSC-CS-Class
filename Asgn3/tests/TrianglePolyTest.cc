#include "../include/Polygon.h"
#include "../include/Reuleaux.h"
#include <gtest/gtest.h>

TEST(TrianglePoly, Contained){
    std::vector<Point> v;
    v.push_back(Point(-1,-1));
    v.push_back(Point(3,0));
    v.push_back(Point(3,5));
    v.push_back(Point(-1,5));
    Polygon outer = Polygon(v);
    Point p1 = Point();
    Point p2 = Point(2,2);
    Point p3 = Point(4,1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v2);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(TrianglePoly, Uncontained_Overlap){
    std::vector<Point> v;
    v.push_back(Point(-1,-1));
    v.push_back(Point(3,0));
    v.push_back(Point(3,5));
    v.push_back(Point(3,1));
    Polygon outer = Polygon(v);
    Point p1 = Point();
    Point p2 = Point(2,2);
    Point p3 = Point(4,1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TrianglePoly, Uncontained_NoOverlap){
    std::vector<Point> v;
    v.push_back(Point(-1,-1));
    v.push_back(Point(3,-1));
    v.push_back(Point(3,-5));
    v.push_back(Point(0,-4));
    Polygon outer = Polygon(v);
    Point p1 = Point();
    Point p2 = Point(2,2);
    Point p3 = Point(4,1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TrianglePoly, Contained_Tangent){
    std::vector<Point> v;
    v.push_back(Point());
    v.push_back(Point(4,2));
    v.push_back(Point(0,6));
    v.push_back(Point(-2,3));
    Polygon outer = Polygon(v);
    Point p1 = Point();
    Point p2 = Point(2,2);
    Point p3 = Point(4,1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v2);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(TrianglePoly, Uncontained_Tangent){
    std::vector<Point> v;
    v.push_back(Point(-2,-1));
    v.push_back(Point(4,2));
    v.push_back(Point(4,0));
    v.push_back(Point(2,-3));
    Polygon outer = Polygon(v);
    Point p1 = Point();
    Point p2 = Point(2,2);
    Point p3 = Point(4,1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}











