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
    get_center_of_every_cell(header);
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
//        double result = sqrt(pow((tab[x][y].centerx) - p.GetX(), 2) + pow((tab[x][y].centery) - p.GetY(), 2));
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

void line(Point point1, Point point2) {
    Line temp_line = Line(point1, point2);
    array_with_lines.push_back(temp_line);
//    printf("generowanie lini");
//    printf("punkt 1 X = %f Y = %f ", point1.x, point1.y);
//    printf("punkt 2 X = %f Y = %f \n", point2.x, point2.y);
}

void generateLines(int state, Point a, Point b, Point c, Point d) {
    switch (state) {
        case 1:
            line(c, d);
            break;
        case 2:
            line(b, c);
            break;
        case 3:
            line(b, d);
            break;
        case 4:
            line(a, b);
            break;
        case 5:
            line(a, d);
            line(b, c);
            break;
        case 6:
            line(a, c);
            break;
        case 7:
            line(a, d);
            break;
        case 8:
            line(a, d);
            break;
        case 9:
            line(a, c);
            break;

        case 10:
            line(a, b);
            line(c, d);
            break;
        case 11:
            line(a, b);
            break;
        case 12:
            line(b, d);
            break;

        case 13:
            line(b, c);
            break;

        case 14:
            line(c, d);
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
    double linediff = 0.5;
    double topl = floor(cell.topleft / linediff);
    double topr = floor(cell.topright / linediff);
    double botl = floor(cell.bottomleft / linediff);
    double botr = floor(cell.bottomright / linediff);
    if (topl == topr && topr == botl && botl == botr) { ;
    } else {
        int minvalue = findMin(topl, topr, botl, botr);
        int a = ceil(topl / minvalue) - 1;
        int b = ceil(topr / minvalue) - 1;
        int c = ceil(botl / minvalue) - 1;
        int d = ceil(botr / minvalue) - 1;
        int result = getState(a, b, c, d);
        generateLines(result, cell.pointa, cell.pointb, cell.pointc, cell.pointd);
    }
}

void Grid::get_center_of_every_cell(liblas::Header header) {
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