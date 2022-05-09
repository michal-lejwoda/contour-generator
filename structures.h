#ifndef MASTER_STRUCTURES_H
#define MASTER_STRUCTURES_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>

struct Point{
    double x;
    double y;
};

struct PointsDistance{
    liblas::Point point;
    double distance;
    PointsDistance(const liblas::Point& point_,double distance_):point(point_), distance(distance_){}
};

struct Line{
    Point point1;
    Point point2;
    double value;
    std::string pt1;
    std::string pt2;
    Line(Point point1_,Point point2_,double value_,std::string pt1_,std::string pt2_):point1(point1_), point2(point2_), value(value_), pt1(pt1_),pt2(pt2_) {}
};

struct Cell {
    double centerx;
    double centery;
    std::vector<PointsDistance> pointsdistance;
    double value;
};

struct LineCell {
    double topleft;
    double topright;
    double bottomleft;
    double bottomright;
    Point pointa;
    Point pointb;
    Point pointc;
    Point pointd;
    std::vector<Line> lines;
};

#endif
