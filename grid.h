#ifndef MASTER_GRID_H
#define MASTER_GRID_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
#include "cell.h"

using namespace std;
class Grid{
public:
    void generateGrid(liblas::Header,liblas::Reader,Cell[391][307]);
    void distance_beetween_points(liblas::Reader,Cell[391][307]);
    void idw(Cell[391][307]);
};
#endif
