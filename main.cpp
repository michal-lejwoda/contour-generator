#include <iostream>
#include <string>
#include <fstream>
#include <liblas/liblas.hpp>
#include <filesystem>
#include <gdal/gdal.h>
#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h" // for CPLMalloc()
#include "grid.h"
#include "structures.h"
#include "gdal/cpl_string.h"
#include "gdal/ogrsf_frmts.h"
#include <cmath>
#include "json.hpp"
#include "omp.h"


using json = nlohmann::json;
double cellsize = 0.5;
int x_length;
int y_length;
std::vector<std::vector<Cell>> cell_array;
std::vector<std::vector<LineCell>> linecell_array;
std::vector<Line> array_with_lines;
std::vector<Linev2> temp_array_with_lines;
GDALDataset *poDStest;
double isoline_value = 2;
const char *pszDriverNametest = "ESRI Shapefile";
GDALDriver *poDrivertest;
//poDrivertest = GetGDALDriverManager()->GetDriverByName(pszDriverNametest);
//poDStest = poDrivertest->Create("/home/saxatachi/Desktop/testjednejlinii12.shp", 0, 0, 0, GDT_Unknown, NULL);
OGRLayer *poLayertest;
//poLayertest = poDStest->CreateLayer("line_out", NULL,
//                                    double isoline_value = 2;

//int dominant_value(std::vector<Line> temp_array) {
//    std::vector<double> allvalues;
//    allvalues.push_back(temp_array[0].value);
//    for (int k = 0; k < allvalues.size(); k++) {
//        int count = 0;
//        for (int i = 0; i < temp_array.size(); i++) {
//            if (temp_array[i].value == allvalues[k]) {
//                count++;
//            }else{
//                for(int j=0;j<allvalues.size();j++){
//                    if(allvalues[j]==temp_array[i].value){
//                        break;
//                    }
//                }
//                allvalues.push_back(temp_array[i].value);
//            }
//        }
//        if (count > (temp_array.size() / 2)) {
//            return allvalues[k];
//        }
//    }
//}

int main() {
    cout<<omp_get_num_procs()<<endl;
    GDALAllRegister();
    cout.precision(15);
    std::ifstream ifs;
//    ifs.open("/home/saxatachi/las_data/points2.las", std::ios::in | std::ios::binary);
    clock_t start = clock();
    ifs.open("/home/saxatachi/las_data/punkty_z_domami.las", std::ios::in | std::ios::binary);
//    ifs.open("/home/saxatachi/las_data/test_lidar.las", std::ios::in | std::ios::binary);
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
    liblas::Header const &header = reader.GetHeader();
    x_length = ceil((header.GetMaxX() - header.GetMinX()) / cellsize);
    y_length = ceil((header.GetMaxY() - header.GetMinY()) / cellsize);
    cell_array = vector<vector<Cell>>(x_length, vector<Cell>(y_length));
    linecell_array = vector<vector<LineCell>>(x_length - 1, vector<LineCell>(y_length - 1));


//    GDALDataset *poDStest;
    const char *pszDriverNametest = "ESRI Shapefile";
//    GDALDriver *poDrivertest;
    poDrivertest = GetGDALDriverManager()->GetDriverByName(pszDriverNametest);
    poDStest = poDrivertest->Create("/home/saxatachi/Desktop/testjednejlinii13.shp", 0, 0, 0, GDT_Unknown, NULL);
//    OGRLayer *poLayertest;
    poLayertest = poDStest->CreateLayer("line_jeden", NULL, wkbLineString, NULL);
    OGRFieldDefn oFieldtest("Value", OFTString);
    oFieldtest.SetWidth(32);
    poLayertest->CreateField(&oFieldtest);
    Grid grid;
    grid.mainfunctions(header, reader);
    GDALClose(poDStest);
    cout<<"test"<<endl;
//    OGRLineString ls;
//    OGRFeature *poFeature3;
//    poFeature3 = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
//    poFeature3->SetField("Value", temp_array_with_lines[0].value);
//    for (int i = 0; i < temp_array_with_lines.size(); i++) {
//        ls.addPoint(temp_array_with_lines[i].point1.x, temp_array_with_lines[i].point1.y);
//        ls.addPoint(temp_array_with_lines[i].point2.x, temp_array_with_lines[i].point2.y);
//    }


    GDALDataset *poDS1;
    poDS1 = (GDALDataset *) GDALOpenEx("/home/saxatachi/Desktop/testaa.shp", GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (poDS1 == NULL) {
        printf("Open failed.\n");
        exit(1);
    }
    OGRLayer *poLayer1;
    poLayer1 = poDS1->GetLayerByName("testaa");
    std::vector<Line> temp_array;
    temp_array.push_back(array_with_lines[0]);
    vector<Line>::iterator it;
    it = array_with_lines.begin();
    array_with_lines.erase(it);
    const char *pszDriverName = "ESRI Shapefile";
    GDALDriver *poDriver;
    poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName);
    GDALDataset *poDS;
    poDS = poDriver->Create("/home/saxatachi/Desktop/testjednejlinii.shp", 0, 0, 0, GDT_Unknown, NULL);
    OGRLayer *poLayer;
    poLayer = poDS->CreateLayer("line_out", NULL, wkbLineString, NULL);
    OGRFieldDefn oField("Value", OFTString);
    oField.SetWidth(32);
    poLayer->CreateField(&oField);
    while (array_with_lines.size() != 0) {
        Line var = temp_array.back();
        Line var1 = temp_array.front();
        int temp_size = temp_array.size();
        for (int i = 0; i < array_with_lines.size(); i++) {
            if (abs(var.i - array_with_lines[i].i) <= 1 && abs(var.j - array_with_lines[i].j) <= 1 ||
                abs(var1.i - array_with_lines[i].i) <= 1 && abs(var1.j - array_with_lines[i].j) <= 1) {
                if (sqrt(pow((var.point2.x - array_with_lines[i].point1.x), 2) +
                         pow(var.point2.y - array_with_lines[i].point1.y, 2)) < 0.005) {
                    it = array_with_lines.begin() + i;
                    temp_array.push_back(array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
                if (sqrt(pow((var.point2.x - array_with_lines[i].point2.x), 2) +
                         pow(var.point2.y - array_with_lines[i].point2.y, 2)) < 0.005) {
                    Point temp_var = array_with_lines[i].point2;
                    array_with_lines[i].point2 = array_with_lines[i].point1;
                    array_with_lines[i].point1 = temp_var;
                    it = array_with_lines.begin() + i;
                    temp_array.push_back(array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
                if (sqrt(pow((var1.point1.x - array_with_lines[i].point1.x), 2) +
                         pow(var1.point1.y - array_with_lines[i].point1.y, 2)) < 0.005) {
                    Point temp_var = array_with_lines[i].point2;
                    array_with_lines[i].point2 = array_with_lines[i].point1;
                    array_with_lines[i].point1 = temp_var;
                    it = array_with_lines.begin() + i;
                    temp_array.insert(temp_array.begin(), 1, array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
                if (sqrt(pow((var1.point1.x - array_with_lines[i].point2.x), 2) +
                         pow(var1.point1.y - array_with_lines[i].point2.y, 2)) < 0.005) {
                    it = array_with_lines.begin() + i;
                    temp_array.insert(temp_array.begin(), 1, array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
            }
        }
        if (temp_size == temp_array.size()) {
            OGRLineString ls;
            OGRFeature *poFeature3;
            poFeature3 = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
            poFeature3->SetField("Value", temp_array[0].value);
            for (int i = 0; i < temp_array.size(); i++) {
                ls.addPoint(temp_array[i].point1.x, temp_array[i].point1.y);
                ls.addPoint(temp_array[i].point2.x, temp_array[i].point2.y);
            }
            poFeature3->SetGeometry(&ls);
            poLayer->CreateFeature(poFeature3);
//            cout<<poFeature3->GetGeometryRef()->exportToJson()<<endl;

            temp_array.clear();
            if (array_with_lines.size() > 0) {
                temp_array.push_back(array_with_lines[0]);
                it = array_with_lines.begin();
                array_with_lines.erase(it);
            }
        }
    }
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    printf("Time measured: %.3f seconds.\n", elapsed);
    GDALDriver *poDriver2;
    poDriver2 = GetGDALDriverManager()->GetDriverByName(pszDriverName);
    GDALDataset *poDS2;
    poDS2 = poDriver2->Create("/home/saxatachi/Desktop/smooth1.shp", 0, 0, 0, GDT_Unknown, NULL);
    OGRLayer *poLayer2;
    poLayer2 = poDS2->CreateLayer("out", NULL, wkbLineString, NULL);
    OGRFieldDefn oField1("Value", OFTString);
    oField1.SetWidth(32);
    poLayer2->CreateField(&oField1);
    for (auto &poFeature1: poLayer1) {
        string string1 = poFeature1->GetGeometryRef()->exportToJson();
        json second = json::parse(string1);
        OGRLineString ls;
        OGRFeature *poFeature4;
        poFeature4 = OGRFeature::CreateFeature(poLayer2->GetLayerDefn());

        double temp_x = second["coordinates"][0][0];
        double temp_y = second["coordinates"][0][1];
        ls.addPoint(temp_x, temp_y);
        for (int i = 1; i < second["coordinates"].size(); i++) {
            double temp2_x = second["coordinates"][i][0];
            double temp2_y = second["coordinates"][i][1];
            double mid_x = (temp_x + temp2_x) / 2;
            double mid_y = (temp_y + temp2_y) / 2;
            ls.addPoint((temp_x + mid_x) / 2, (temp_y + mid_y) / 2);
            ls.addPoint((temp2_x + mid_x) / 2, (temp2_y + mid_y) / 2);
            temp_x = temp2_x;
            temp_y = temp2_y;
            if (i == second["coordinates"].size() - 1) {
                ls.addPoint(temp_x, temp_y);
            }
        }
        poFeature4->SetGeometry(&ls);
//        poFeature4->SetField( "Value", poFeature1->GetFieldAsString("Value"));
//        cout<<"feature4"<<endl;
//        cout<<poFeature4->GetGeometryRef()->exportToJson()<<endl;
        poLayer2->CreateFeature(poFeature4);
        poLayer2->SetFeature(poFeature4);
    }
//    for (auto &poFeature1: poLayer1) {
//        cout << "wynik" << poFeature1->GetGeometryRef()->exportToJson() << endl;
//    }
//    for (auto &poFeature2: poLayer2) {
//        cout << "wynik" << poFeature2->GetGeometryRef()->exportToJson() << endl;
//    }
//    for (auto &poFeature2: poLayer) {
//        cout << "wynik" << poFeature2->GetGeometryRef()->exportToJson() << endl;
//    }
    GDALClose(poDS);
    GDALClose(poDS2);

    return 0;

}

