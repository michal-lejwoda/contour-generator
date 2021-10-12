#ifndef MASTER_CELL_H
#define MASTER_CELL_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>

using namespace std;
struct Cell {
//    float centerx;
//    float centery;
    float minx;
    float miny;
    float maxx;
    float maxy;
};

#endif //MASTER_CELL_H
