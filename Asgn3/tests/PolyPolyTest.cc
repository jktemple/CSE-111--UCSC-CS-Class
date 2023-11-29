
#include "../include/Polygon.h"
#include <gtest/gtest.h>

TEST(PolyPoly, Contained){
    std::vector<Point> v;
    v.push_back(Point(2, 2));
    v.push_back(Point(2, 0));
    v.push_back(Point(0, 0));
    v.push_back(Point(0, 2));
    Polygon inner = Polygon(v);
    std::vector<Point> v2;
    v2.push_back(Point(3,3));
    v2.push_back(Point(3, -1));
    v2.push_back(Point(-1,-1));
    v2.push_back(Point(-1,3));
    Polygon outer = Polygon(v2);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(PolyPoly, Uncontained_Overlap){
    std::vector<Point> v;
    v.push_back(Point(2, 2));
    v.push_back(Point(2, 0));
    v.push_back(Point(0, 0));
    v.push_back(Point(0, 2));
    Polygon inner = Polygon(v);
    std::vector<Point> v2;
    v2.push_back(Point(3,3));
    v2.push_back(Point(3, -1));
    v2.push_back(Point(-1,-1));
    v2.push_back(Point(0.5,1.75));
    Polygon outer = Polygon(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(PolyPoly, Uncontained_Tangent){
    std::vector<Point> v;
    v.push_back(Point(2, 2));
    v.push_back(Point(2, 0));
    v.push_back(Point(0, 0));
    v.push_back(Point(0, 2));
    Polygon inner = Polygon(v);
    std::vector<Point> v2;
    v2.push_back(Point(3,3));
    v2.push_back(Point(3, -1));
    v2.push_back(Point(2,-1));
    v2.push_back(Point(2,3));
    Polygon outer = Polygon(v2);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(PolyPoly, Contained_Tangent){
    std::vector<Point> v;
    v.push_back(Point(2, 2));
    v.push_back(Point(2, 0));
    v.push_back(Point(0, 0));
    v.push_back(Point(0, 2));
    Polygon inner = Polygon(v);
    std::vector<Point> v2;
    v2.push_back(Point(2,3));
    v2.push_back(Point(2, -1));
    v2.push_back(Point(-1,-1));
    v2.push_back(Point(-1,3));
    Polygon outer = Polygon(v2);
    ASSERT_TRUE(inner.ContainedBy(outer));
}



