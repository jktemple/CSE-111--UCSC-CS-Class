#include "../include/Circle.h"
#include "../include/Reuleaux.h"
#include <gtest/gtest.h>

TEST(CircleTriangle, Contained){
    Circle inner = Circle(Point(), 2);
    Point p1 = Point(0,4);
    Point p2 = Point(5,-3);
    Point p3 = Point(-5,-3);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(CircleTriangle, Contained_Tangent){
      Circle inner = Circle(Point(0,0), 2);
    Point p1 = Point(2,5);
    Point p2 = Point(2,-5);
    Point p3 = Point(-4, 0);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(CircleTriangle, Uncontained_Tangent){
      Circle inner = Circle(Point(0,0), 2);
    Point p1 = Point(2,5);
    Point p2 = Point(8,0);
    Point p3 = Point(2, -5);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(CircleTriangle, Uncontained_NoOverlap){
      Circle inner = Circle(Point(0,0), 2);
    Point p1 = Point(3,5);
    Point p2 = Point(3,-5);
    Point p3 = Point(5, 0);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(CircleTriangle, Uncontained_Circumfrence){
      Circle inner = Circle(Point(0,0), 2);
    Point p1 = Point(0,2);
    Point p2 = Point(2, 0);
    Point p3 = Point(0, -2);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(CircleTriangle, Uncontained_Overlap){
      Circle inner = Circle(Point(0,0), 2);
    Point p1 = Point(0, 0);
    Point p2 = Point(0, 1);
    Point p3 = Point(4, 4);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle outer = ReuleauxTriangle(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

