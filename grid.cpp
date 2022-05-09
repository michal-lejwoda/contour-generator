#include <cmath>
#include "grid.h"
#include "structures.h"
#include <omp.h>
#include <gdal/gdal.h>
#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h"
#include "gdal/ogrsf_frmts.h"

//using namespace std;
extern double cellsize;
extern int x_length;
extern int y_length;
double R;
extern std::vector<std::vector<Cell>> cell_array;
extern std::vector<std::vector<LineCell>> linecell_array;
extern double isoline_value;
extern std::vector<Line> array_with_lines;
extern OGRLayer *poLayertest;

int dominant_value(){
    int max = 0;
    int max_result = 0;
    map<int, int> mp;
    for (int i = 0; i < array_with_lines.size(); i++) {
        mp[array_with_lines[i].value] = mp[array_with_lines[i].value]+=1;
    }
    for (map<int,int>::iterator itr = mp.begin();itr != mp.end();++itr){
        if(itr->second > max){
            max = itr->second;
            max_result = itr->first;
        }
        return max_result;
    }
}

void Grid::mainfunctions(liblas::Header header, liblas::Reader reader) {
    generateGrid(header);
    distance_beetween_points(header, reader);
    inverse_distance_weighting_algorithm();
    check_if_every_cell_has_value();
    create_raster(header);
    set_important_values_for_every_linecell(header);
    check_how_it_looks();
}

void Grid::generateGrid(liblas::Header header) {
    double minx = header.GetMinX();
    double maxy = header.GetMaxY();
    double start = omp_get_wtime();
    #pragma omp parallel num_threads(12)
    {
    #pragma omp for schedule(static)
        for (int i = 0; i < x_length; i++) {
            for (int j = 0; j < y_length; j++) {
                cell_array[i][j].centerx = minx + (i * (cellsize)) + (cellsize / 2);
                cell_array[i][j].centery = maxy - ((j * (cellsize)) + (cellsize / 2));
            }
        }
    };
    double end = omp_get_wtime();
    double elapsed = end - start;
    std::cout << "elapsed generate grid = " << elapsed << std::endl;
}

void Grid::distance_beetween_points(liblas::Header header, liblas::Reader reader) {
    R = (cellsize * sqrt(2)) / 2;
    liblas::Point const &p = reader.GetPoint();
    int count = 0;
    clock_t start = clock();
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
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "elapsed distance_beetween_points = " << elapsed << std::endl;
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

void generate_line(Point point1, Point point2, double firstpoint, double secondpoint, int i, int j, string pt1,
                      string pt2) {

    if (floor(firstpoint / isoline_value) == ceil(secondpoint / isoline_value)) {
        Line temp_line = Line(point1, point2, (floor(firstpoint / isoline_value) * isoline_value), pt1, pt2);
        linecell_array[i][j].lines.push_back(temp_line);
    } else if (floor(secondpoint / isoline_value) == ceil(firstpoint / isoline_value)) {
        Line temp_line = Line(point1, point2, (floor(firstpoint / isoline_value) * isoline_value), pt1, pt2);
        linecell_array[i][j].lines.push_back(temp_line);
    }
}

void recognize_type_of_line(int state, LineCell cell, int i, int j) {
    switch (state) {
        case 1:
            generate_line(cell.pointc, cell.pointd, cell.bottomleft, cell.topright, i, j, "c", "d");
            break;
        case 2:
            generate_line(cell.pointb, cell.pointc, cell.topleft, cell.bottomright, i, j, "b", "c");
            break;
        case 3:
            generate_line(cell.pointb, cell.pointd, cell.topright, cell.bottomleft, i, j, "b", "d");
            break;
        case 4:
            generate_line(cell.pointa, cell.pointb, cell.topright, cell.bottomleft, i, j, "a", "b");
            break;
        case 5:
            generate_line(cell.pointa, cell.pointd, cell.topleft, cell.bottomleft, i, j, "a", "d");
            generate_line(cell.pointb, cell.pointc, cell.bottomright, cell.topright, i, j, "b", "c");
            break;
        case 6:
            generate_line(cell.pointa, cell.pointc, cell.topleft, cell.topright, i, j, "a", "c");
            break;
        case 7:
            generate_line(cell.pointa, cell.pointd, cell.topleft, cell.topright, i, j, "a", "d");
            break;
        case 8:
            generate_line(cell.pointa, cell.pointd, cell.topleft, cell.bottomright, i, j, "a", "d");
            break;
        case 9:
            generate_line(cell.pointa, cell.pointc, cell.topleft, cell.bottomright, i, j, "a", "c");
            break;
        case 10:
            generate_line(cell.pointa, cell.pointb, cell.topright, cell.bottomright, i, j, "a", "b");
            generate_line(cell.pointc, cell.pointd, cell.bottomleft, cell.topleft, i, j, "c", "d");
            break;
        case 11:
            generate_line(cell.pointa, cell.pointb, cell.topright, cell.bottomleft, i, j, "a", "b");
            break;
        case 12:
            generate_line(cell.pointb, cell.pointd, cell.topright, cell.bottomright, i, j, "b", "d");
            break;
        case 13:
            generate_line(cell.pointb, cell.pointc, cell.bottomright, cell.topleft, i, j, "b", "c");
            break;
        case 14:
            generate_line(cell.pointc, cell.pointd, cell.bottomleft, cell.topright, i, j, "c", "d");
            break;
    }
}

void calculate_average_cell_value(int x, int y, vector<double> temp_array) {
    double temp = 0;
    for (int i = 0; i < temp_array.size(); i++) {
        temp += temp_array[i];
    }
    double result = temp / temp_array.size();
    cell_array[x][y].value = result;
}

double get_cell_value_from_the_closest_cells_with_value(int x, int y) {
    int i = 0;
    vector<double> temp_array;
    while (true) {
        if (temp_array.size() > 0) {
            calculate_average_cell_value(x, y, temp_array);
            break;
        }
        #pragma omp parallel
        #pragma omp for
        for (int j = -i; j <= i; j++) {
            for (int k = -i; k <= i; k++) {
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

void Grid::check_if_every_cell_has_value() {
    double start = omp_get_wtime();
    #pragma omp parallel
    #pragma omp for schedule(dynamic)
    for (int i = 0; i < x_length; i++) {
        for (int j = 0; j < y_length; j++) {
            if (cell_array[i][j].value == 0) {
                get_cell_value_from_the_closest_cells_with_value(i, j);
            }
        }
    }
    double end = omp_get_wtime();
    double elapsed = end - start;
    std::cout << "elapsed push_back_line_to_array every value " << elapsed << std::endl;
}

void Grid::inverse_distance_weighting_algorithm() {
    double start = omp_get_wtime();
#pragma omp parallel
#pragma omp for schedule(static)
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
                            result1 + (cell_array[i][j].pointsdistance[k].point.GetZ() /
                                       cell_array[i][j].pointsdistance[k].distance);
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
    double end = omp_get_wtime();
    double elapsed = end - start;
    std::cout << "elapsed idw " << elapsed << std::endl;
}

int findMin(int a, int b, int c, int d) {
    return min(a, min(b, min(c, d)));
}

int getState(int a, int b, int c, int d) {
    return a * 8 + b * 4 + c * 2 + d * 1;
}

void checkValuesv2(LineCell cell, int i, int j) {
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
        recognize_type_of_line(result, cell, i, j);
    }
}

void push_back_line_to_array(int i,int j,string pt1,string pt2,int index){
    if (j - 1 >= 0 && (pt1 == "a" || pt2 == "a")) {
        for (int k = 0; k < linecell_array[i][j - 1].lines.size(); k++) {
            if (linecell_array[i][j - 1].lines[k].pt1 == "c" ||
                linecell_array[i][j - 1].lines[k].pt2 == "c") {
                if (linecell_array[i][j - 1].lines[k].pt1 == "c") {
                    array_with_lines.push_back(linecell_array[i][j - 1].lines[k]);
                    linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    if (j - 1 > 0) {
                        return push_back_line_to_array(i, j - 1,linecell_array[i][j - 1].lines[k].pt1,linecell_array[i][j - 1].lines[k].pt2,k);
                    }
                } else {
                    Point temp;
                    temp = linecell_array[i][j - 1].lines[k].point1;
                    linecell_array[i][j - 1].lines[k].point1 = linecell_array[i][j - 1].lines[k].point2;
                    linecell_array[i][j - 1].lines[k].point2 = temp;
                    array_with_lines.push_back(linecell_array[i][j - 1].lines[k]);
                    linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    if (j - 1 > 0) {
                        return push_back_line_to_array(i, j - 1,linecell_array[i][j - 1].lines[k].pt2,linecell_array[i][j - 1].lines[k].pt1,k);
                    }
                }


            }
        }
    }
    if (i + 1 < x_length - 1 &&
        (pt1 == "b" || pt2 == "b")) {
        for (int k = 0; k < linecell_array[i + 1][j].lines.size(); k++) {
            if (linecell_array[i + 1][j].lines[k].pt1 == "d" ||
                linecell_array[i + 1][j].lines[k].pt2 == "d") {
                if (linecell_array[i + 1][j].lines[k].pt1 == "d") {
                    array_with_lines.push_back(linecell_array[i + 1][j].lines[k]);
                    linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    if (i + 1 < x_length - 1) {
                        return push_back_line_to_array(i + 1, j,linecell_array[i + 1][j].lines[k].pt1,linecell_array[i + 1][j].lines[k].pt2,k);
                    }
                } else {
                    Point temp;
                    temp = linecell_array[i + 1][j].lines[k].point1;
                    linecell_array[i + 1][j].lines[k].point1 = linecell_array[i + 1][j].lines[k].point2;
                    linecell_array[i + 1][j].lines[k].point2 = temp;
                    array_with_lines.push_back(linecell_array[i + 1][j].lines[k]);
                    linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);

                    if (i + 1 < x_length - 1) {
                        return push_back_line_to_array(i + 1, j,linecell_array[i + 1][j].lines[k].pt2,linecell_array[i + 1][j].lines[k].pt1,k);
                    }
                }


            }
        }
    }
    if (j + 1 < y_length - 1 &&
        (pt1 == "c" || pt2 == "c")) {
        for (int k = 0; k < linecell_array[i][j + 1].lines.size(); k++) {
            if (linecell_array[i][j + 1].lines[k].pt1 == "a" ||
                linecell_array[i][j + 1].lines[k].pt2 == "a") {
                if (linecell_array[i][j + 1].lines[k].pt1 == "a") {
                    array_with_lines.push_back(linecell_array[i][j + 1].lines[k]);
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    if (j + 1 < y_length - 1) {
                        return push_back_line_to_array(i, j + 1,linecell_array[i][j + 1].lines[k].pt1,linecell_array[i][j + 1].lines[k].pt2,k);
                    }
                } else {
                    Point temp;
                    temp = linecell_array[i][j + 1].lines[k].point1;
                    linecell_array[i][j + 1].lines[k].point1 = linecell_array[i][j + 1].lines[k].point2;
                    linecell_array[i][j + 1].lines[k].point2 = temp;
                    array_with_lines.push_back(linecell_array[i][j + 1].lines[k]);
                    linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    if (j + 1 < y_length - 1) {
                        return push_back_line_to_array(i, j + 1,linecell_array[i][j + 1].lines[k].pt2,linecell_array[i][j + 1].lines[k].pt1,k);
                    }
                }

            }
        }
    }
    if (i - 1 >= 0 && (pt1 == "d" || pt2 == "d")) {

        for (int k = 0; k < linecell_array[i - 1][j].lines.size(); k++) {
            if (linecell_array[i - 1][j].lines[k].pt1 == "b" ||
                linecell_array[i - 1][j].lines[k].pt2 == "b") {
                if (linecell_array[i - 1][j].lines[k].pt1 == "b") {
                    array_with_lines.push_back(linecell_array[i - 1][j].lines[k]);
                    if (linecell_array[i][j].lines.size() > 0) {
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    return push_back_line_to_array(i - 1, j,linecell_array[i - 1][j].lines[k].pt1,linecell_array[i - 1][j].lines[k].pt2,k);
                } else {
                    Point temp;
                    temp = linecell_array[i - 1][j].lines[k].point1;
                    linecell_array[i - 1][j].lines[k].point1 = linecell_array[i - 1][j].lines[k].point2;
                    linecell_array[i - 1][j].lines[k].point2 = temp;
                    array_with_lines.push_back(linecell_array[i - 1][j].lines[k]);
                    linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    return push_back_line_to_array(i - 1, j,linecell_array[i - 1][j].lines[k].pt2,linecell_array[i - 1][j].lines[k].pt1,k);
                }


            }
        }
    }
    if(linecell_array[i][j].lines.size()>0) {
        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + 0);
    }
}

void push_front_line_to_array(int i,int j,string pt1,string pt2,int index){
    if (j - 1 >= 0 && (pt1 == "a" || pt2 == "a")) {
        for (int k = 0; k < linecell_array[i][j - 1].lines.size(); k++) {
            if (linecell_array[i][j - 1].lines[k].pt1 == "c" ||
                linecell_array[i][j - 1].lines[k].pt2 == "c") {
                if (linecell_array[i][j - 1].lines[k].pt2 == "c") {
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i][j-1].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    if (j - 1 > 0) {
                        return push_front_line_to_array(i, j - 1,linecell_array[i][j - 1].lines[k].pt1,linecell_array[i][j - 1].lines[k].pt2,k);
                    }
                } else {
                    Point temp;
                    temp = linecell_array[i][j - 1].lines[k].point1;
                    linecell_array[i][j - 1].lines[k].point1 = linecell_array[i][j - 1].lines[k].point2;
                    linecell_array[i][j - 1].lines[k].point2 = temp;
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i][j-1].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    if (j - 1 > 0) {
                        return push_front_line_to_array(i, j - 1,linecell_array[i][j - 1].lines[k].pt2,linecell_array[i][j - 1].lines[k].pt1,k);
                    }
                }
            }
        }
    }
    if (i + 1 < x_length - 1 &&
        (pt1 == "b" || pt2 == "b")) {
        for (int k = 0; k < linecell_array[i + 1][j].lines.size(); k++) {
            if (linecell_array[i + 1][j].lines[k].pt1 == "d" ||
                linecell_array[i + 1][j].lines[k].pt2 == "d") {
                if (linecell_array[i + 1][j].lines[k].pt2 == "d") {
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i+1][j].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    if (i + 1 < x_length - 1) {
                        return push_front_line_to_array(i + 1, j,linecell_array[i + 1][j].lines[k].pt1,linecell_array[i + 1][j].lines[k].pt2,k);
                    }
                } else {
                    Point temp;
                    temp = linecell_array[i + 1][j].lines[k].point1;
                    linecell_array[i + 1][j].lines[k].point1 = linecell_array[i + 1][j].lines[k].point2;
                    linecell_array[i + 1][j].lines[k].point2 = temp;
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i+1][j].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    if (i + 1 < x_length - 1) {
                        return push_front_line_to_array(i + 1, j,linecell_array[i + 1][j].lines[k].pt2,linecell_array[i + 1][j].lines[k].pt1,k);
                    }
                }


            }
        }
    }
    if (j + 1 < y_length - 1 &&
        (pt1 == "c" || pt2 == "c")) {
        for (int k = 0; k < linecell_array[i][j + 1].lines.size(); k++) {
            if (linecell_array[i][j + 1].lines[k].pt1 == "a" ||
                linecell_array[i][j + 1].lines[k].pt2 == "a") {
                if (linecell_array[i][j + 1].lines[k].pt2 == "a") {
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i][j + 1].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    if (j + 1 < y_length - 1) {
                        return push_front_line_to_array(i, j + 1,linecell_array[i][j + 1].lines[k].pt1,linecell_array[i][j + 1].lines[k].pt2,k);
                    }
                }
                else {
                    Point temp;
                    temp = linecell_array[i][j + 1].lines[k].point1;
                    linecell_array[i][j + 1].lines[k].point1 = linecell_array[i][j + 1].lines[k].point2;
                    linecell_array[i][j + 1].lines[k].point2 = temp;
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i][j + 1].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    if (j + 1 < y_length - 1) {
                        return push_front_line_to_array(i, j + 1,linecell_array[i][j + 1].lines[k].pt2,linecell_array[i][j + 1].lines[k].pt1,k);
                    }
                }

            }
        }
    }
    if (i - 1 >= 0 && (pt1 == "d" || pt2 == "d")) {

        for (int k = 0; k < linecell_array[i - 1][j].lines.size(); k++) {
            if (linecell_array[i - 1][j].lines[k].pt1 == "b" ||
                linecell_array[i - 1][j].lines[k].pt2 == "b") {
                if (linecell_array[i - 1][j].lines[k].pt2 == "b") {
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i-1][j].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);
                    }
                    return push_front_line_to_array(i - 1, j,linecell_array[i - 1][j].lines[k].pt1,linecell_array[i - 1][j].lines[k].pt2,k);
                } else {
                    Point temp;
                    temp = linecell_array[i - 1][j].lines[k].point1;
                    linecell_array[i - 1][j].lines[k].point1 = linecell_array[i - 1][j].lines[k].point2;
                    linecell_array[i - 1][j].lines[k].point2 = temp;
                    array_with_lines.insert(array_with_lines.begin(),linecell_array[i-1][j].lines[k]);
                    if(linecell_array[i][j].lines.size()>0){
                        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + index);

                    }
                    return push_front_line_to_array(i - 1, j,linecell_array[i - 1][j].lines[k].pt2,linecell_array[i - 1][j].lines[k].pt1,k);
                }
            }
        }
    }
    if(linecell_array[i][j].lines.size()>0) {
        linecell_array[i][j].lines.erase(linecell_array[i][j].lines.begin() + 0);
    }
}


void add_feature_to_shapefile(){
    OGRLineString ls;
    OGRFeature *poFeature9;
    poFeature9 = OGRFeature::CreateFeature(poLayertest->GetLayerDefn());
    poFeature9->SetField("Value", dominant_value());
    for (int i = 0; i < array_with_lines.size(); i++) {
        ls.addPoint(array_with_lines[i].point1.x, array_with_lines[i].point1.y);
        ls.addPoint(array_with_lines[i].point2.x, array_with_lines[i].point2.y);
    }
    poFeature9->SetGeometry(&ls);
    poLayertest->CreateFeature(poFeature9);
    poLayertest->SetFeature(poFeature9);
}

void Grid::check_how_it_looks() {
    double start = omp_get_wtime();
    for (int i = 0; i < x_length - 1; i++) {
        for (int j = 0; j < y_length - 1; j++) {
            int temp_var = 0;
                if (linecell_array[i][j].lines.size() > 0) {
                    array_with_lines.push_back(linecell_array[i][j].lines[0]);
                    if(linecell_array[i][j].lines[0].pt1 == "a" ||linecell_array[i][j].lines[0].pt2 == "a"){
                        push_back_line_to_array(i, j,linecell_array[i][j].lines[0].pt1,linecell_array[i][j].lines[0].pt2,0);
                        temp_var++;
                    }
                    if(linecell_array[i][j].lines[0].pt1 == "b" ||linecell_array[i][j].lines[0].pt2 == "b"){
                        if(temp_var>0 && temp_var<2){
                            push_front_line_to_array(i, j,linecell_array[i][j].lines[0].pt1,linecell_array[i][j].lines[0].pt2,0);
                        }else {
                            push_back_line_to_array(i, j, linecell_array[i][j].lines[0].pt1, linecell_array[i][j].lines[0].pt2, 0);
                        }
                        temp_var++;
                    }
                    if(linecell_array[i][j].lines[0].pt1 == "c" ||linecell_array[i][j].lines[0].pt2 == "c"){
                        if(temp_var>0 && temp_var<2){
                            push_front_line_to_array(i, j,linecell_array[i][j].lines[0].pt1,linecell_array[i][j].lines[0].pt2,0);

                        }else {
                            push_back_line_to_array(i, j, linecell_array[i][j].lines[0].pt1, linecell_array[i][j].lines[0].pt2, 0);
                        }
                        temp_var++;
                    }
                    if(linecell_array[i][j].lines[0].pt1 == "d" ||linecell_array[i][j].lines[0].pt2 == "d"){
                        if(temp_var>0 && temp_var<2){
                            push_front_line_to_array(i, j,linecell_array[i][j].lines[0].pt1,linecell_array[i][j].lines[0].pt2,0);
                        }else {
                            push_back_line_to_array(i, j, linecell_array[i][j].lines[0].pt1, linecell_array[i][j].lines[0].pt2, 0);
                        }
                        temp_var++;
                    }
                    add_feature_to_shapefile();
                    array_with_lines.clear();
            }
        }
    }
    double end = omp_get_wtime();
    double elapsed = end - start;
    std::cout << "elapsed check_how_it_looks " << elapsed << std::endl;
}


void Grid::set_important_values_for_every_linecell(liblas::Header header) {
    double start = omp_get_wtime();
//#pragma omp parallel private (array_with_lines1)
//    {
//#pragma omp for schedule(static)
        for (int i = 0; i < x_length - 1; i++) {
            for (int j = 0; j < y_length - 1; j++) {
                linecell_array[i][j].topleft = cell_array[i][j].value;
                linecell_array[i][j].topright = cell_array[i + 1][j].value;
                linecell_array[i][j].bottomleft = cell_array[i][j + 1].value;
                linecell_array[i][j].bottomright = cell_array[i + 1][j + 1].value;

                linecell_array[i][j].pointa.x = header.GetMinX() + (i * (cellsize) + (cellsize));
                linecell_array[i][j].pointa.y = header.GetMaxY() - (j * (cellsize) + (cellsize / 2));

                linecell_array[i][j].pointb.x = header.GetMinX() + (i * (cellsize) + (cellsize + (cellsize / 2)));
                linecell_array[i][j].pointb.y = header.GetMaxY() - (j * (cellsize) + (cellsize));

                linecell_array[i][j].pointc.x = header.GetMinX() + (i * (cellsize) + (cellsize));
                linecell_array[i][j].pointc.y = header.GetMaxY() - (j * (cellsize) + (cellsize + (cellsize / 2)));

                linecell_array[i][j].pointd.x = header.GetMinX() + (i * (cellsize) + (cellsize / 2));
                linecell_array[i][j].pointd.y = header.GetMaxY() - (j * (cellsize) + (cellsize));

                checkValuesv2(linecell_array[i][j], i, j);
            }
        }
    double end = omp_get_wtime();
    double elapsed = double(end - start);
    std::cout << "elapsed set_important_values_for_every_linecell = " << elapsed << std::endl;
}

void Grid::create_raster(liblas::Header header) {
    GDALDataset *pNewDS;
    GDALDriver *pDriverTiff;
    char const * output = "/home/saxatachi/Desktop/neww.tif";
    double transform[6];
    transform[0] = header.GetMinX();
    transform[1] = 0.5;
    transform[2] = 0;
    transform[3] = header.GetMinY();
    transform[4] = 0;
    transform[5] = 0.5;
    pDriverTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
    pNewDS = pDriverTiff->Create(output,x_length,y_length,1,GDT_Float32,NULL);
    pNewDS->SetGeoTransform(transform);
    float *newwRow = (float*) CPLMalloc(sizeof(float)*x_length);
    for(int i=0;i<y_length;i++){
        for(int j=0;j<x_length;j++){
            newwRow[j] = cell_array[i][j].value;
        }
        pNewDS->GetRasterBand(1)->RasterIO(GF_Write,0,i,x_length,1,newwRow,y_length,1,GDT_Float32,0,0);
    }
//    for(int i=0;i<y_length;i++){
//        for(int j=0;j<x_length;j++){
//            newwRow[j] = cell_array[i][j].value;
//        }
//        pNewDS->GetRasterBand(1)->RasterIO(GF_Write,0,i,x_length,1,newwRow,y_length,1,GDT_Float32,0,0);
//    }
//    GDALClose(poDataset);
    GDALClose(pNewDS);
}



