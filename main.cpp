#include <iostream>
#include <string>
//#include <fstream>
#include <liblas/liblas.hpp>
//#include <filesystem>
#include <gdal/gdal.h>
#include "gdal/gdal_priv.h"
//#include "gdal/cpl_conv.h" // for CPLMalloc()
#include "grid.h"
#include "structures.h"
//#include "gdal/cpl_string.h"
#include "gdal/ogrsf_frmts.h"
#include <cmath>
#include "json.hpp"
//#include "omp.h"


using json = nlohmann::json;
double cellsize = 0.5;
int x_length;
int y_length;
std::vector<std::vector<Cell>> cell_array;
std::vector<std::vector<LineCell>> linecell_array;
std::vector<Line> array_with_lines;
GDALDataset *poDStest;
double isoline_value = 3;
GDALDriver *poDrivertest;
OGRLayer *poLayertest;

int main() {
    GDALAllRegister();
    cout.precision(15);
    std::ifstream ifs;
//    ifs.open("/home/saxatachi/las_data/points13v2.las", std::ios::in | std::ios::binary);
//    ifs.open("/home/saxatachi/las_data/points800v2.las", std::ios::in | std::ios::binary);
    ifs.open("/home/saxatachi/las_data/punkty_z_domami.las", std::ios::in | std::ios::binary);
//    ifs.open("/home/saxatachi/las_data/test_lidar.las", std::ios::in | std::ios::binary);
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
    liblas::Header const &header = reader.GetHeader();
    x_length = ceil((header.GetMaxX() - header.GetMinX()) / cellsize);
    y_length = ceil((header.GetMaxY() - header.GetMinY()) / cellsize);
    cell_array = vector<vector<Cell>>(x_length, vector<Cell>(y_length));
    linecell_array = vector<vector<LineCell>>(x_length - 1, vector<LineCell>(y_length - 1));
    const char *pszDriverNametest = "ESRI Shapefile";
    poDrivertest = GetGDALDriverManager()->GetDriverByName(pszDriverNametest);
    poDStest = poDrivertest->Create("/home/saxatachi/Desktop/lines3.shp", 0, 0, 0, GDT_Unknown, NULL);
    poLayertest = poDStest->CreateLayer("first_layer", NULL, wkbLineString, NULL);
    OGRFieldDefn oFieldtest("Value", OFTString);
    oFieldtest.SetWidth(32);
    poLayertest->CreateField(&oFieldtest);
    Grid grid;
    grid.mainfunctions(header, reader);
    GDALClose(poDStest);
    return 0;

}

