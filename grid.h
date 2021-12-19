#ifndef MASTER_GRID_H
#define MASTER_GRID_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
#include "cell.h"

using namespace std;
class Grid{
public:
    void generateGrid(liblas::Header,liblas::Reader,std::vector<std::vector<Cell>>);
    void distance_beetween_points(liblas::Header,liblas::Reader,std::vector<std::vector<Cell>>);
    void idw(std::vector<std::vector<Cell>>);
//    void generateLines(int result,Point a,Point b,Point c,Point d);
    void get_center_of_every_cell(liblas::Header,std::vector<std::vector<Cell>>,std::vector<std::vector<LineCell>>);
    double neighbours(int x, int y,std::vector<std::vector<Cell>>);

};
#endif
