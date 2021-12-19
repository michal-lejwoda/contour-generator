#ifndef MASTER_CELL_H
#define MASTER_CELL_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
//#include <vector>

using namespace std;


struct PointsDistance{
    liblas::Point point;
    double distance;
    PointsDistance(const liblas::Point& point_,double distance_):point(point_), distance(distance_){}
};

struct Point{
    double x;
    double y;
};
struct Line{
    Point point1;
    Point point2;
};
struct Cell {
    double centerx;
    double centery;
    vector<PointsDistance> pointsdistance;
//    vector<liblas::Point> points;
//    vector<double> distance;
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
};


#endif //MASTER_CELL_H
