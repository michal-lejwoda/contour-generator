#ifndef MASTER_GRID_H
#define MASTER_GRID_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
#include "cell.h"

using namespace std;
class Grid{
public:
    void generateGrid(liblas::Header,liblas::Reader,Cell[268][180]);
    void distance_beetween_points(liblas::Header,liblas::Reader,Cell[268][180]);
    void idw(Cell[268][180]);
};
#endif
