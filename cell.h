#ifndef MASTER_CELL_H
#define MASTER_CELL_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
//#include <vector>

using namespace std;
struct Cell {
    float centerx;
    float centery;
    vector<liblas::Point> points;
    vector<float> distance;
    float value;
};

#endif //MASTER_CELL_H
