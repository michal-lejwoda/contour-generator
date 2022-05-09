#ifndef MASTER_GRID_H
#define MASTER_GRID_H
#include<iostream>
#include <liblas/liblas.hpp>
#include <liblas/header.hpp>
#include "structures.h"

using namespace std;
class Grid{
public:
    void mainfunctions(liblas::Header,liblas::Reader);
    void generateGrid(liblas::Header);
    void distance_beetween_points(liblas::Header,liblas::Reader);
    void inverse_distance_weighting_algorithm();
    void set_important_values_for_every_linecell(liblas::Header);
    void check_if_point_belongs_to_neighbours(int x,int y,liblas::Point);
    void check_if_every_cell_has_value();
    void check_how_it_looks();
    void create_raster(liblas::Header);

};
#endif
