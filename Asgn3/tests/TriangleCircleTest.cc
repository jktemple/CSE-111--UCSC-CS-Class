#include "../include/Circle.h"
#include "../include/Reuleaux.h"
#include <gtest/gtest.h>

TEST(TriangleCircle, Contained){
    Circle outer = Circle(Point(2,2), 4);
    Point p1 = Point();
    Point p2 = Point(4,0);
    Point p3 = Point(2,4);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(TriangleCircle, Unontained_Overlap){
    Circle outer = Circle(Point(5,0), 2);
    Point p1 = Point();
    Point p2 = Point(4,0);
    Point p3 = Point(2,4);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TriangleCircle, Uncontained_NoOverlap){
    Circle outer = Circle(Point(6, 0), 1);
    Point p1 = Point();
    Point p2 = Point(4,0);
    Point p3 = Point(2,4);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TriangleCircle, Uncontained_Tangent){
    Circle outer = Circle(Point(2, -2), 2);
    Point p1 = Point();
    Point p2 = Point(4,0);
    Point p3 = Point(2,4);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TriangleCircle, Contained_Circumfrence){
    Circle outer = Circle(Point(), 6);
    Point p1 = Point();
    Point p2 = Point(6,0);
    Point p3 = Point(0,6);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}




