#ifndef MASTER_CELL_H
#define MASTER_CELL_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
//#include <vector>

using namespace std;
struct Cell {
    double centerx;
    double centery;
    vector<liblas::Point> points;
    vector<double> distance;
    double value;
};

struct LineCell {
    double topleft;
    double topright;
    double bottomleft;
    double bottomright;
};

#endif //MASTER_CELL_H
