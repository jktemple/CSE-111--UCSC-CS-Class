#include "../include/Circle.h"
#include "../include/Polygon.h"
#include <gtest/gtest.h>


TEST(PolygonCircle, Uncontained_Overlap){
    Circle outer = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(0, 0));
    v.push_back(Point(0,-4));
    v.push_back(Point(4,-3));
    v.push_back(Point(4, 1));
    Polygon inner = Polygon(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(PolygonCircle, Contained){
    Circle outer = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(0, 0));
    v.push_back(Point(0, 1));
    v.push_back(Point(1,1));
    v.push_back(Point(1, 0));
    Polygon inner = Polygon(v);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(PolygonCircle, Contained_Circumfrence){
    Circle outer = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(0, 2));
    v.push_back(Point(2, 0));
    v.push_back(Point(0,-2));
    v.push_back(Point(-2, 0));
    Polygon inner = Polygon(v);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(PolygonCircle, Uncontained_Tangent){
    Circle outer = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(-1, 2));
    v.push_back(Point(2, 2));
    v.push_back(Point(2, 4));
    v.push_back(Point(-2, 4));
    Polygon inner = Polygon(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}








