#include "../include/Polygon.h"
#include "../include/Reuleaux.h"
#include <gtest/gtest.h>

TEST(PolyTriangle, Contained){
    std::vector<Point> v;
    v.push_back(Point());
    v.push_back(Point(4,0));
    v.push_back(Point(4,4));
    v.push_back(Point(0,4));
    Polygon inner = Polygon(v);
    Point p1 = Point(0,7);
    Point p2 = Point(12,-1);
    Point p3 = Point(-2,-1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(PolyTriangle, Uncontained_Overlap){
    std::vector<Point> v;
    v.push_back(Point());
    v.push_back(Point(4,0));
    v.push_back(Point(4,4));
    v.push_back(Point(0,4));
    Polygon inner = Polygon(v);
    Point p1 = Point(2,2);
    Point p2 = Point(12,-1);
    Point p3 = Point(-2,-1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(PolyTriangle, Uncontained_NoOverlap){
    std::vector<Point> v;
    v.push_back(Point());
    v.push_back(Point(4,0));
    v.push_back(Point(4,4));
    v.push_back(Point(0,4));
    Polygon inner = Polygon(v);
    Point p1 = Point(5,0);
    Point p2 = Point(-2,-1);
    Point p3 = Point(-3,-1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(PolyTriangle, Contained_Tanget){
    std::vector<Point> v;
    v.push_back(Point());
    v.push_back(Point(4,0));
    v.push_back(Point(4,4));
    v.push_back(Point(0,4));
    Polygon inner = Polygon(v);
    Point p1 = Point(0,7);
    Point p2 = Point(12,-1);
    Point p3 = Point(0,-1);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(PolyTriangle, Uncontained_Tanget){
    std::vector<Point> v;
    v.push_back(Point());
    v.push_back(Point(4,0));
    v.push_back(Point(4,4));
    v.push_back(Point(0,4));
    Polygon inner = Polygon(v);
    Point p1 = Point(0,7);
    Point p2 = Point(0,-1);
    Point p3 = Point(-1, 3);
    Point v2[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}






