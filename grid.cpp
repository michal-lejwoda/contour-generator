#include<iostream>
#include <cmath>
#include "grid.h"
#include "cell.h"

using namespace std;
extern double cellsize;
extern int minx;
extern int miny;
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

void line(Point point1, Point point2, double firstpoint,double secondpoint,int i,int j) {
    if(floor(firstpoint/isoline_value) == ceil(secondpoint/isoline_value)){
//        cout<<"Pierwsza petla = "<<(floor(firstpoint/isoline_value)*isoline_value)<<endl;
        Line temp_line = Line(point1, point2,(floor(firstpoint/isoline_value)*isoline_value),i,j);
        array_with_lines.push_back(temp_line);
    }else if(floor(secondpoint/isoline_value) == ceil(firstpoint/isoline_value)){
//        cout<<"Druga petla = "<<(floor(secondpoint/isoline_value)*2)<<endl;
        Line temp_line = Line(point1, point2,(floor(firstpoint/isoline_value)*isoline_value),i,j);
        array_with_lines.push_back(temp_line);
    }else{
        //Za duża róznica
        cout<<"za duża różnica "<<firstpoint<<" "<<secondpoint<<endl;
        cout<<"Pierwsza petla = "<<(floor(firstpoint/isoline_value)*isoline_value)<<endl;
        cout<<"Druga petla = "<<(floor(secondpoint/isoline_value)*isoline_value)<<endl;

    ;
    }
}

void generateLines(int state, LineCell cell,int i,int j) {
    switch (state) {
        case 1:
            line(cell.pointc, cell.pointd,cell.bottomleft,cell.topright,i,j);
            break;
        case 2:
            line(cell.pointb, cell.pointc,cell.topleft,cell.bottomright,i,j);
            break;
        case 3:
            line(cell.pointb, cell.pointd,cell.topright,cell.bottomleft,i,j);
            break;
        case 4:
            line(cell.pointa, cell.pointb,cell.topright,cell.bottomleft,i,j);
            break;
        case 5:
            line(cell.pointa, cell.pointd,cell.topleft,cell.bottomleft,i,j);
            line(cell.pointb, cell.pointc,cell.bottomright,cell.topright,i,j);
//            cout<<"cell.topleft "<<cell.topleft<<" cell.bottomright "<<cell.bottomright<<" cell.topright "<<cell.topright<<" cell.bottomleft "<<cell.bottomleft<<endl;
            break;
        case 6:
            line(cell.pointa, cell.pointc,cell.topleft,cell.topright,i,j);
            break;
        case 7:
            line(cell.pointa, cell.pointd,cell.topleft,cell.topright,i,j);
            break;
        case 8:
            line(cell.pointa, cell.pointd,cell.topleft,cell.bottomright,i,j);
            break;
        case 9:
            line(cell.pointa, cell.pointc,cell.topleft,cell.bottomright,i,j);
            break;
        case 10:
            line(cell.pointa, cell.pointb,cell.topright,cell.bottomright,i,j);
            line(cell.pointc, cell.pointd,cell.bottomleft,cell.topleft,i,j);
            break;
        case 11:
            line(cell.pointa, cell.pointb,cell.topright,cell.bottomleft,i,j);
            break;
        case 12:
            line(cell.pointb, cell.pointd,cell.topright,cell.bottomright,i,j);
            break;

        case 13:
            line(cell.pointb, cell.pointc,cell.bottomright,cell.topleft,i,j);
            break;

        case 14:
            line(cell.pointc, cell.pointd,cell.bottomleft,cell.topright,i,j);
            break;
    }
}

void single_cell_idw(int x,int y,vector<double>temp_array){
    double temp = 0;
    for(int i=0;i<temp_array.size();i++){
//        cout<<"wartosci "<<temp_array[i]<<endl;
        temp += temp_array[i];
    }
    double result = temp/temp_array.size();
//    cout<<"rezultat = "<<result<<endl;
    tab[x][y].value = result;

}

double neighboursv2(int x, int y){
    int i = 0;
    vector <double> temp_array;
    while(true){
        if(temp_array.size()>0){
//            cout<<"i = "<<i<<endl;
//            cout<<"końcowy wynik "<<temp_array.size()<<endl;
            single_cell_idw(x,y,temp_array);
            break;
        }
        for(int j = -i; j<= i;j++){
            for(int k = -i; k<=i;k++){
                if (x - j > 0 && x - j < minx && y - k > 0 && y - k < miny) {
                    if (tab[x - j][y - k].pointsdistance.size() > 0) {
                        temp_array.push_back(tab[x - j][y - k].value);
                    }
                }
            }
        }
        i++;
    }
}


void checkeveryvalue(){
    for (int i = 0; i < minx; i++) {
        for (int j = 0; j < miny; j++) {
//            cout<<"tablica "<<tab[i][j].value<<endl;
            if(tab[i][j].value == 0){
                neighboursv2(i,j);
            }
        }
    }
}
void checkeveryvaluev2(){
    for (int i = 0; i < minx; i++) {
        for (int j = 0; j < miny; j++) {
            cout<<"koniec"<<tab[i][j].value<<endl;
//            if(tab[i][j].value == 0){
//                neighboursv2(i,j);
//            }
        }
    }
}
void checkarray(){
    for(int i=0;i<array_with_lines.size();i++){
        cout<<"linia "<<array_with_lines[i].value<<endl;
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
//                neighbours(i, j);
//                neighboursv2(i,j);
            }
        }
    }
    checkeveryvalue();
//    checkeveryvaluev2();

}

int findMin(int a, int b, int c, int d) {
    return min(a, min(b, min(c, d)));
}

int getState(int a, int b, int c, int d) {
    return a * 8 + b * 4 + c * 2 + d * 1;
}

void checkValues(LineCell cell,int i,int j) {
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
        generateLines(result, cell, i,j);
    }
}

void Grid::get_data_of_every_cell(liblas::Header header) {
    for (int i = 0; i < minx-1; i++) {
        for (int j = 0; j < miny-1; j++) {
            arr[i][j].topleft = tab[i][j].value;
            arr[i][j].topright = tab[i+1][j].value;
            arr[i][j].bottomleft = tab[i][j+1].value;
            arr[i][j].bottomright = tab[i+1][j+1].value;

            arr[i][j].pointa.x = header.GetMinX() + (i * (cellsize) + (cellsize));
            arr[i][j].pointa.y = header.GetMaxY() - (j * (cellsize) + (cellsize/2));

            arr[i][j].pointb.x = header.GetMinX() + (i * (cellsize) + (cellsize+(cellsize/2)));
            arr[i][j].pointb.y = header.GetMaxY() - (j * (cellsize) + (cellsize));

            arr[i][j].pointc.x = header.GetMinX() + (i * (cellsize) + (cellsize));
            arr[i][j].pointc.y = header.GetMaxY() - (j * (cellsize) + (cellsize+(cellsize/2)));

            arr[i][j].pointd.x = header.GetMinX() + (i * (cellsize) + (cellsize/2));
            arr[i][j].pointd.y = header.GetMaxY() - (j * (cellsize) + (cellsize));
            checkValues(arr[i][j],i,j);
        }
    }
    checkarray();
}



