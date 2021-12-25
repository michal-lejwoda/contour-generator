#include<iostream>
#include <cmath>
#include "grid.h"
#include "cell.h"

using namespace std;
extern double cellsize;
extern int minx;
extern int miny;
extern int halfminx;
extern int halfminy;
double R;
extern std::vector<std::vector<Cell>> tab;
extern std::vector<std::vector<LineCell>> arr;
extern std::vector<Line> array_with_lines;
extern double isoline_value;

//void checkvalues(){
//    for (int i = 0; i < minx; i++) {
//        for (int j = 0; j < miny; j++) {
//            cout<<"tab["<<i<<"]["<<j<<"] = "<<tab[i][j].value<<endl;
//        }
//    }
//}


void Grid::generateGrid(liblas::Header header, liblas::Reader reader) {
    for (int i = 0; i < minx; i++) {
        for (int j = 0; j < miny; j++) {
            tab[i][j].centerx = header.GetMinX() + (i * (cellsize)) + (cellsize / 2);
            tab[i][j].centery = header.GetMaxY() - ((j * (cellsize)) + (cellsize / 2));
        }
    }
    distance_beetween_points(header, reader);
    idw();
    get_data_of_every_cell(header);
}

void Grid::distance_beetween_points(liblas::Header header, liblas::Reader reader) {
    R = (cellsize * sqrt(2)) / 2;
    liblas::Point const &p = reader.GetPoint();
    int count = 0;
    while (reader.ReadNextPoint()) {

//        if (p.GetClassification().GetClass() == 0 or p.GetClassification().GetClass() == 2) {
//            count++;
//        } else {
//            continue;
//        }
        if (p.GetClassification().GetClass() == 2) {
            count++;
        } else {
            continue;
        }
        int x = floor((p.GetX() - header.GetMinX()) / cellsize);
        int y = floor((header.GetMaxY() - p.GetY()) / cellsize);
        checkneighbours(x, y, p);
    }
}

void Grid::checkneighbours(int x, int y, liblas::Point p) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x - i > 0 && x - i < minx && y - j > 0 && y - j < miny) {
                double result = sqrt(
                        pow((tab[x - i][y - j].centerx) - p.GetX(), 2) +
                        pow((tab[x - i][y - j].centery) - p.GetY(), 2));
                if (result < R) {
                    PointsDistance temp_distance(p, result);
                    tab[x - i][y - j].pointsdistance.push_back(temp_distance);
                }
            }
        }
    }
}


double Grid::neighbours(int x, int y) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x - i > 0 && x - i < minx && y - j > 0 && y - j < miny) {
                if (tab[x - i][y - j].pointsdistance.size() > 0) {
                    tab[x][y].value = tab[x - i][y - j].pointsdistance[0].point.GetZ();
                    return tab[x][y].value;
                }
            }
        }
    }
    return tab[x][y].value = 77;
}

void line(Point point1, Point point2, double firstpoint,double secondpoint) {
    if(floor(firstpoint/isoline_value) == ceil(secondpoint/isoline_value)){
        cout<<"Pierwsza petla = "<<(floor(firstpoint/isoline_value)*isoline_value)<<endl;
        Line temp_line = Line(point1, point2,(floor(firstpoint/isoline_value)*isoline_value));
        array_with_lines.push_back(temp_line);
    }else if(floor(secondpoint/isoline_value) == ceil(firstpoint/isoline_value)){
        cout<<"Druga petla = "<<(floor(secondpoint/isoline_value)*2)<<endl;
        Line temp_line = Line(point1, point2,(floor(firstpoint/isoline_value)*isoline_value));
        array_with_lines.push_back(temp_line);
    }else{
        //Za duża róznica
//        cout<<"za duża różnica "<<firstpoint<<" "<<secondpoint<<endl;
    ;
    }
}

void generateLines(int state, LineCell cell) {
    switch (state) {
        case 1:
            line(cell.pointc, cell.pointd,cell.bottomleft,cell.topright);
            break;
        case 2:
            line(cell.pointb, cell.pointc,cell.topleft,cell.bottomright);
            break;
        case 3:
            line(cell.pointb, cell.pointd,cell.topright,cell.bottomleft);
            break;
        case 4:
            line(cell.pointa, cell.pointb,cell.topright,cell.bottomleft);
            break;
        case 5:
            line(cell.pointa, cell.pointd,cell.topleft,cell.bottomright);
            line(cell.pointb, cell.pointc,cell.bottomright,cell.topleft);
            break;
        case 6:
            line(cell.pointa, cell.pointc,cell.topleft,cell.topright);
            break;
        case 7:
            line(cell.pointa, cell.pointd,cell.topleft,cell.topright);
            break;
        case 8:
            line(cell.pointa, cell.pointd,cell.topleft,cell.bottomright);
            break;
        case 9:
            line(cell.pointa, cell.pointc,cell.topleft,cell.bottomright);
            break;
        case 10:
            line(cell.pointa, cell.pointb,cell.topright,cell.bottomleft);
            line(cell.pointc, cell.pointd,cell.bottomleft,cell.topright);
            break;
        case 11:
            line(cell.pointa, cell.pointb,cell.topright,cell.bottomleft);
            break;
        case 12:
            line(cell.pointb, cell.pointd,cell.topright,cell.bottomright);
            break;

        case 13:
            line(cell.pointb, cell.pointc,cell.bottomright,cell.topleft);
            break;

        case 14:
            line(cell.pointc, cell.pointd,cell.bottomleft,cell.topright);
            break;
    }
}


void Grid::idw() {
    for (int i = 0; i < minx; i++) {
        for (int j = 0; j < miny; j++) {
            double result = 0;
            double result1 = 0;
            double result2 = 0;
            double temp = 0;
            int size = tab[i][j].pointsdistance.size();
            if (size > 0) {
                for (int k = 0; k < size; k++) {
                    result1 =
                            result1 + (tab[i][j].pointsdistance[k].point.GetZ() / tab[i][j].pointsdistance[k].distance);
                    result2 = result2 + (1 / tab[i][j].pointsdistance[k].distance);

                    if (tab[i][j].pointsdistance[k].distance == 0) {
                        temp = tab[i][j].pointsdistance[k].point.GetZ();
                    }
                }
                result = result1 / result2;

                if (temp != 0) {
                    tab[i][j].value = temp;
                } else {
                    tab[i][j].value = result;
                }
            } else {
                neighbours(i, j);
            }
        }
    }

}

int findMin(int a, int b, int c, int d) {
    return min(a, min(b, min(c, d)));
}

int getState(int a, int b, int c, int d) {
    return a * 8 + b * 4 + c * 2 + d * 1;
}

void checkValues(LineCell cell) {
    double topl = floor(cell.topleft / isoline_value);
    double topr = floor(cell.topright / isoline_value);
    double botl = floor(cell.bottomleft / isoline_value);
    double botr = floor(cell.bottomright / isoline_value);
    if (topl == topr && topr == botl && botl == botr) { ;
    } else {
//        cout<<cell.topleft<<" "<<cell.topright<<" "<<cell.bottomleft<<" "<<cell.bottomright<<endl;
        int minvalue = findMin(topl, topr, botl, botr);
        int topleft = ceil(topl / minvalue) - 1;
        int topright = ceil(topr / minvalue) - 1;
        int bottomleft = ceil(botl / minvalue) - 1;
        int bottomright = ceil(botr / minvalue) - 1;
        int result = getState(topleft, topright, bottomright, bottomleft);
//        if(result == 2){
//            cout<<"a = "<<a<<"b = "<<b<<"c = "<<c<<"d = "<<d<<endl;
//            cout<<"a = "<<topl<<"b = "<<topr<<"c = "<<botl<<"d ="<<botr<<endl;
//            cout<<"a = "<<cell.topleft<<"b = "<<cell.topright<<"c = "<<cell.bottomleft<<"d ="<<cell.bottomright<<endl;
//        }
        generateLines(result, cell);
    }
}

void Grid::get_data_of_every_cell(liblas::Header header) {
    double bigcellsize = cellsize * 2;
    for (int i = 0; i < halfminx; i++) {
        for (int j = 0; j < halfminy; j++) {
            arr[i][j].topleft = tab[i * 2][j * 2].value;
            arr[i][j].topright = tab[i * 2 + 1][j * 2].value;
            arr[i][j].bottomleft = tab[i * 2][j * 2 + 1].value;
            arr[i][j].bottomright = tab[i * 2 + 1][j * 2 + 1].value;
            arr[i][j].pointa.x = header.GetMinX() + (i * (bigcellsize)) + (cellsize);
            arr[i][j].pointa.y = header.GetMaxY() - (j * (bigcellsize));
            arr[i][j].pointb.x = header.GetMinX() + (i * (bigcellsize)) + (cellsize * 2);
            arr[i][j].pointb.y = header.GetMaxY() - ((j * (bigcellsize)) + (cellsize));
            arr[i][j].pointc.x = header.GetMinX() + (i * (bigcellsize)) + (cellsize);
            arr[i][j].pointc.y = header.GetMaxY() - ((j * (bigcellsize)) + (cellsize * 2));
            arr[i][j].pointd.x = header.GetMinX() + (i * (bigcellsize));
            arr[i][j].pointd.y = header.GetMaxY() - ((j * (bigcellsize)) + (cellsize));
            checkValues(arr[i][j]);
        }
    }
}

//void Grid::single_cell_idw(){
//    double result = 0;
//    double result1 = 0;
//    double result2 = 0;
//    double temp = 0;
//    int size = tab[i][j].pointsdistance.size();
//    if (size > 0) {
//        for (int k = 0; k < size; k++) {
//
//            result1 =
//                    result1 + (tab[i][j].pointsdistance[k].point.GetZ() / tab[i][j].pointsdistance[k].distance);
//            result2 = result2 + (1 / tab[i][j].pointsdistance[k].distance);
//
//            if (tab[i][j].pointsdistance[k].distance == 0) {
//                temp = tab[i][j].pointsdistance[k].point.GetZ();
//            }
//        }
//        result = result1 / result2;
//
//        if (temp != 0) {
//            tab[i][j].value = temp;
//        } else {
//            tab[i][j].value = result;
//        }
//    }
//}

//double Grid::neighboursv2(int x, int y){
//    int i =0;
//    vector <double> temp_array;
//    while(i){
//        if(temp_array.size()>0){
//
//            break;
//        }
//        for(int j = -i; j<= i;j++){
//            for(int k = -i; k<=i;k++){
//                if (tab[x - j][y - k].pointsdistance.size() > 0){
//                    temp_array.push_back(tab[x - j][y - k].value);
//                }
//            }
//        }
//    }
//}