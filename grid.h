#ifndef MASTER_GRID_H
#define MASTER_GRID_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
#include "cell.h"

using namespace std;
class Grid{
public:
    void generateGrid(liblas::Header,liblas::Reader);
    void distance_beetween_points(liblas::Header,liblas::Reader);
    void idw();
//    void single_cell_idw();
//    void generateLines(int result,Point a,Point b,Point c,Point d);
    void get_data_of_every_cell(liblas::Header);
    double neighbours(int x, int y);
//    double neighboursv2(int x, int y);
    void checkneighbours(int x,int y,liblas::Point);

};
#endif
