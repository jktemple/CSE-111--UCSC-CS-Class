#include "../include/Reuleaux.h"
#include <gtest/gtest.h>

TEST(TriangleTriangle, Contained){
    Point p1 = Point();
    Point p2 = Point(2,1);
    Point p3 = Point(1,2);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);

    Point p4 = Point(-2,-1);
    Point p5 = Point(4,1);
    Point p6 = Point(3,-1);
    Point v2[3] = {p4,p5,p6};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);   
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(TriangleTriangle, Uncontained_Overlap){
    Point p1 = Point();
    Point p2 = Point(2,1);
    Point p3 = Point(1,2);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);

    Point p4 = Point(-2,-1);
    Point p5 = Point(4,1);
    Point p6 = Point(1,-1);
    Point v2[3] = {p4,p5,p6};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);   
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TriangleTriangle, Uncontained_NoOverlap){
    Point p1 = Point();
    Point p2 = Point(2,1);
    Point p3 = Point(1,2);
   Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);

    Point p4 = Point(-2,-1);
    Point p5 = Point(4,1);
    Point p6 = Point(-2,1);
    Point v2[3] = {p4,p5,p6};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);   
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TriangleTriangle, Uncontained_Tangent){
    Point p1 = Point();
    Point p2 = Point(2,1);
    Point p3 = Point(1,2);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);

    Point p4 = Point(-2, 0);
    Point p5 = Point(3,0);
    Point p6 = Point(-3, -4);
    Point v2[3] = {p4,p5,p6};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);   
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(TriangleTriangle, Contained_Tangent){
    Point p1 = Point();
    Point p2 = Point(2,1);
    Point p3 = Point(1,2);
    Point v[3] = {p1,p2,p3};
    ReuleauxTriangle inner = ReuleauxTriangle(v);

    Point p4 = Point(-2, 0);
    Point p5 = Point(3,0);
    Point p6 = Point(4, 1);
    Point v2[3] = {p4,p5,p6};
    ReuleauxTriangle outer = ReuleauxTriangle(v2);   
    ASSERT_TRUE(inner.ContainedBy(outer));
}