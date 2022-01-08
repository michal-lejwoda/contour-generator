#include <cmath>
#include "grid.h"
#include "structures.h"
using namespace std;
extern double cellsize;
extern int x_length;
extern int y_length;
double R;
extern std::vector<std::vector<Cell>> cell_array;
extern std::vector<std::vector<LineCell>> linecell_array;
extern std::vector<Line> array_with_lines;
extern double isoline_value;

void Grid::mainfunctions(liblas::Header header,liblas::Reader reader) {
    generateGrid(header);
    distance_beetween_points(header, reader);
    inverse_distance_weighting_algorithm();
    checkeveryvalue();
    set_important_values_for_every_linecell(header);
}

void Grid::generateGrid(liblas::Header header) {
    for (int i = 0; i < x_length; i++) {
        for (int j = 0; j < y_length; j++) {
            cell_array[i][j].centerx = header.GetMinX() + (i * (cellsize)) + (cellsize / 2);
            cell_array[i][j].centery = header.GetMaxY() - ((j * (cellsize)) + (cellsize / 2));
        }
    }
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
        check_if_point_belongs_to_neighbours(x, y, p);
    }
}

void Grid::check_if_point_belongs_to_neighbours(int x, int y, liblas::Point p) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x - i > 0 && x - i < x_length && y - j > 0 && y - j < y_length) {
                double result = sqrt(
                        pow((cell_array[x - i][y - j].centerx) - p.GetX(), 2) +
                        pow((cell_array[x - i][y - j].centery) - p.GetY(), 2));
                if (result < R) {
                    PointsDistance temp_distance(p, result);
                    cell_array[x - i][y - j].pointsdistance.push_back(temp_distance);
                }
            }
        }
    }
}

double Grid::neighbours(int x, int y) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (x - i > 0 && x - i < x_length && y - j > 0 && y - j < y_length) {
                if (cell_array[x - i][y - j].pointsdistance.size() > 0) {
                    cell_array[x][y].value = cell_array[x - i][y - j].pointsdistance[0].point.GetZ();
                    return cell_array[x][y].value;
                }
            }
        }
    }
    return cell_array[x][y].value = 77;
}

void generate_line(Point point1, Point point2, double firstpoint,double secondpoint,int i,int j) {
    if(floor(firstpoint/isoline_value) == ceil(secondpoint/isoline_value)){
        Line temp_line = Line(point1, point2,(floor(firstpoint/isoline_value)*isoline_value),i,j);
        array_with_lines.push_back(temp_line);
    }else if(floor(secondpoint/isoline_value) == ceil(firstpoint/isoline_value)){
        Line temp_line = Line(point1, point2,(floor(firstpoint/isoline_value)*isoline_value),i,j);
        array_with_lines.push_back(temp_line);
    }
}

void recognize_type_of_line(int state, LineCell cell,int i,int j) {
    switch (state) {
        case 1:
            generate_line(cell.pointc, cell.pointd,cell.bottomleft,cell.topright,i,j);
            break;
        case 2:
            generate_line(cell.pointb, cell.pointc,cell.topleft,cell.bottomright,i,j);
            break;
        case 3:
            generate_line(cell.pointb, cell.pointd,cell.topright,cell.bottomleft,i,j);
            break;
        case 4:
            generate_line(cell.pointa, cell.pointb,cell.topright,cell.bottomleft,i,j);
            break;
        case 5:
            generate_line(cell.pointa, cell.pointd,cell.topleft,cell.bottomleft,i,j);
            generate_line(cell.pointb, cell.pointc,cell.bottomright,cell.topright,i,j);
            break;
        case 6:
            generate_line(cell.pointa, cell.pointc,cell.topleft,cell.topright,i,j);
            break;
        case 7:
            generate_line(cell.pointa, cell.pointd,cell.topleft,cell.topright,i,j);
            break;
        case 8:
            generate_line(cell.pointa, cell.pointd,cell.topleft,cell.bottomright,i,j);
            break;
        case 9:
            generate_line(cell.pointa, cell.pointc,cell.topleft,cell.bottomright,i,j);
            break;
        case 10:
            generate_line(cell.pointa, cell.pointb,cell.topright,cell.bottomright,i,j);
            generate_line(cell.pointc, cell.pointd,cell.bottomleft,cell.topleft,i,j);
            break;
        case 11:
            generate_line(cell.pointa, cell.pointb,cell.topright,cell.bottomleft,i,j);
            break;
        case 12:
            generate_line(cell.pointb, cell.pointd,cell.topright,cell.bottomright,i,j);
            break;

        case 13:
            generate_line(cell.pointb, cell.pointc,cell.bottomright,cell.topleft,i,j);
            break;

        case 14:
            generate_line(cell.pointc, cell.pointd,cell.bottomleft,cell.topright,i,j);
            break;
    }
}

void calculate_average_cell_value(int x,int y,vector<double>temp_array){
    double temp = 0;
    for(int i=0;i<temp_array.size();i++){
        temp += temp_array[i];
    }
    double result = temp/temp_array.size();
    cell_array[x][y].value = result;
}

double get_cell_value_from_the_closest_cells_with_value(int x, int y){
    int i = 0;
    vector <double> temp_array;
    while(true){
        if(temp_array.size()>0){
            calculate_average_cell_value(x,y,temp_array);
            break;
        }
        for(int j = -i; j<= i;j++){
            for(int k = -i; k<=i;k++){
                if (x - j > 0 && x - j < x_length && y - k > 0 && y - k < y_length) {
                    if (cell_array[x - j][y - k].pointsdistance.size() > 0) {
                        temp_array.push_back(cell_array[x - j][y - k].value);
                    }
                }
            }
        }
        i++;
    }
}

void Grid::checkeveryvalue(){
    for (int i = 0; i < x_length; i++) {
        for (int j = 0; j < y_length; j++) {
            if(cell_array[i][j].value == 0){
                get_cell_value_from_the_closest_cells_with_value(i,j);
            }
        }
    }
}

void Grid::inverse_distance_weighting_algorithm() {
    for (int i = 0; i < x_length; i++) {
        for (int j = 0; j < y_length; j++) {
            double result = 0;
            double result1 = 0;
            double result2 = 0;
            double temp = 0;
            int size = cell_array[i][j].pointsdistance.size();
            if (size > 0) {
                for (int k = 0; k < size; k++) {
                    result1 =
                            result1 + (cell_array[i][j].pointsdistance[k].point.GetZ() / cell_array[i][j].pointsdistance[k].distance);
                    result2 = result2 + (1 / cell_array[i][j].pointsdistance[k].distance);

                    if (cell_array[i][j].pointsdistance[k].distance == 0) {
                        temp = cell_array[i][j].pointsdistance[k].point.GetZ();
                    }
                }
                result = result1 / result2;

                if (temp != 0) {
                    cell_array[i][j].value = temp;
                } else {
                    cell_array[i][j].value = result;
                }
            }
        }
    }
    checkeveryvalue();
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
        int minvalue = findMin(topl, topr, botl, botr);
        int topleft = ceil(topl / minvalue) - 1;
        int topright = ceil(topr / minvalue) - 1;
        int bottomleft = ceil(botl / minvalue) - 1;
        int bottomright = ceil(botr / minvalue) - 1;
        int result = getState(topleft, topright, bottomright, bottomleft);
        recognize_type_of_line(result, cell, i,j);
    }
}

void Grid::set_important_values_for_every_linecell(liblas::Header header) {
    for (int i = 0; i < x_length-1; i++) {
        for (int j = 0; j < y_length-1; j++) {
            linecell_array[i][j].topleft = cell_array[i][j].value;
            linecell_array[i][j].topright = cell_array[i+1][j].value;
            linecell_array[i][j].bottomleft = cell_array[i][j+1].value;
            linecell_array[i][j].bottomright = cell_array[i+1][j+1].value;

            linecell_array[i][j].pointa.x = header.GetMinX() + (i * (cellsize) + (cellsize));
            linecell_array[i][j].pointa.y = header.GetMaxY() - (j * (cellsize) + (cellsize/2));

            linecell_array[i][j].pointb.x = header.GetMinX() + (i * (cellsize) + (cellsize+(cellsize/2)));
            linecell_array[i][j].pointb.y = header.GetMaxY() - (j * (cellsize) + (cellsize));

            linecell_array[i][j].pointc.x = header.GetMinX() + (i * (cellsize) + (cellsize));
            linecell_array[i][j].pointc.y = header.GetMaxY() - (j * (cellsize) + (cellsize+(cellsize/2)));

            linecell_array[i][j].pointd.x = header.GetMinX() + (i * (cellsize) + (cellsize/2));
            linecell_array[i][j].pointd.y = header.GetMaxY() - (j * (cellsize) + (cellsize));

            checkValues(linecell_array[i][j],i,j);
        }
    }
}



