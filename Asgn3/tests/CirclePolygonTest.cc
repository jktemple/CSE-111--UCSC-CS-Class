#include "../include/Circle.h"
#include "../include/Polygon.h"
#include <gtest/gtest.h>

TEST(CirclePolygon, Contained){
    Circle inner = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(-3,-3));
    v.push_back(Point(3,-3));
    v.push_back(Point(3,3));
    v.push_back(Point(-3, 3));
    Polygon outer = Polygon(v);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(CirclePolygon, Uncontained){
    Circle inner = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(0, 0));
    v.push_back(Point(0,-4));
    v.push_back(Point(4,-3));
    v.push_back(Point(4, 1));
    Polygon outer = Polygon(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(CirclePolygon, Contained_Tangent){
    Circle inner = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(2, 2));
    v.push_back(Point(2, -2));
    v.push_back(Point(-2,-2));
    v.push_back(Point(-2, 2));
    Polygon outer = Polygon(v);
    ASSERT_TRUE(inner.ContainedBy(outer));
}

TEST(CirclePolygon, Uncontained_Overlap){
    Circle inner = Circle(Point(0,0), 2);
    std::vector<Point> v;
    v.push_back(Point(0, 0));
    v.push_back(Point(0,-4));
    v.push_back(Point(4,-3));
    v.push_back(Point(4, 1));
    Polygon outer = Polygon(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}

TEST(CirclePolygon, Uncontained_PointsOnCircumfrence){
    Circle inner = Circle(Point(0,0), 3);
    std::vector<Point> v;
    v.push_back(Point(0, 3));
    v.push_back(Point(3, 0));
    v.push_back(Point(0,-3));
    v.push_back(Point(-3, 0));
    Polygon outer = Polygon(v);
    ASSERT_FALSE(inner.ContainedBy(outer));
}


