#ifndef MASTER_GRID_H
#define MASTER_GRID_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
#include "cell.h"

using namespace std;
class Grid{
public:
    void generateGrid(liblas::Header,Cell[]);
};
#endif //MASTER_GRID_H
