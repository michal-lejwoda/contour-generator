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

void Grid::generateGrid(liblas::Header header,liblas::Reader reader,std::vector<std::vector<Cell>> tab){
    for(int i=0;i<minx;i++){
        for(int j=0;j<miny;j++){
            tab[i][j].centerx = header.GetMinX() + (i*(cellsize)) + (cellsize/2);
            tab[i][j].centery = header.GetMaxY() - ((j*(cellsize))+ (cellsize/2));
        }
    }
}

void Grid::distance_beetween_points(liblas::Header header,liblas::Reader reader,std::vector<std::vector<Cell>> tab){
//    cout.precision(15);
//    double cellsize = 0.5;
    double R = (cellsize * sqrt(2))/2;
//    reader.ReadPointAt(1002);
//    liblas::Point const& r = reader.GetPoint();
//    int x = floor((r.GetX() - header.GetMinX())/cellsize);
//    int y = floor((header.GetMaxY() - r.GetY())/cellsize);
//    double result = sqrt(pow((tab[x][y].centerx)-r.GetX(),2)+pow((tab[x][y].centery)-r.GetY(),2));
//    cout<<tab[x][y].centerx - r.GetX()<<endl;
//    cout<<tab[x][y].centery - r.GetY()<<endl;
//    if(result>R){
//
//        for(int el=1;el<=ceil(1/cellsize);el++){
//            for(int el2=1;el2<=ceil(1/cellsize);el2++){
//                float result2 = sqrt(pow((tab[x+el][y+el2].centerx)-r.GetX(),2)+pow((tab[x+el][y+el2].centery)-r.GetY(),2));
//                if(result2<=R){
//                    cout<<"wynik w poprawnym for"<<result2<<"tab["<<x+el<<"]["<<y+el2<<"]"<<endl;
//                    break;
//                }
//            }
//        }
//    }else{
//        cout<<"wynik w pierwszym"<<result<<"tab["<<x<<"]["<<y<<"]"<<endl;
//    }


    liblas::Point const& p = reader.GetPoint();
    int count = 0;
    while (reader.ReadNextPoint()) {
//        if (p.GetClassification().GetClass() == 0 or p.GetClassification().GetClass() == 2) {
//            count++;
//        } else {
//            continue;
//        }
        if(p.GetClassification().GetClass() == 2){
            count++;
        }else{
            continue;
        }
        double testx = (p.GetX() - header.GetMinX()) / cellsize;
        int x = floor((p.GetX() - header.GetMinX()) / cellsize);
        int y = floor((header.GetMaxY() - p.GetY()) / cellsize);
        double result = sqrt(pow((tab[x][y].centerx) - p.GetX(), 2) + pow((tab[x][y].centery) - p.GetY(), 2));
//        if (result > R) {
//            cout<<x<<" oraz "<<y<<endl;
//            for (int el = 1; el <= ceil(1 / cellsize); el++) {
//                for (int el2 = 1; el2 <= ceil(1 / cellsize); el2++) {
//                    if(x+el>268 or y+el2>180){
//                        continue;
//                    }
//                    float result2 = sqrt(pow((tab[x + el][y + el2].centerx) - p.GetX(), 2) +
//                                         pow((tab[x + el][y + el2].centery) - p.GetY(), 2));
//                    if (result2 <= R) {
//                        cout<<x + el<<" oraz "<<y + el2<<endl;
//                        tab[x + el][y + el2].points.push_back(p);
//                        tab[x + el][y + el2].distance.push_back(result2);
//                        break;
//                    }
//                }
//            }
//        } else {

            PointsDistance temp_distance(p,result);
            tab[x][y].pointsdistance.push_back(temp_distance);


//            tab[x][y].pointsdistance.point.push_back(p);
//            tab[x][y].pointsdistance.distance.push_back(result);
    }
}
double Grid::neighbours(int x,int y,std::vector<std::vector<Cell>> tab){
    if(tab[x-1][y-1].pointsdistance.size()>0){
        tab[x][y].value = tab[x-1][y-1].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    if(tab[x][y-1].pointsdistance.size()>0){
        tab[x][y].value = tab[x][y-1].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    if(tab[x+1][y-1].pointsdistance.size()>0){
        tab[x][y].value = tab[x+1][y-1].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    if(tab[x-1][y].pointsdistance.size()>0){
        tab[x][y].value = tab[x-1][y].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    if(tab[x+1][y].pointsdistance.size()>0){
        tab[x][y].value = tab[x+1][y].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    if(tab[x-1][y+1].pointsdistance.size()>0){
        tab[x][y].value = tab[x-1][y+1].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    if(tab[x][y+1].pointsdistance.size()>0){
        tab[x][y].value = tab[x][y+1].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    if(tab[x+1][y+1].pointsdistance.size()>0){
        tab[x][y].value = tab[x+1][y+1].pointsdistance[0].point.GetZ();
        return tab[x][y].value;
    }
    else{
        return tab[x][y].value = 77;
    }

}

void line(Point point1,Point point2){
    printf("generowanie lini");
    printf("punkt 1 X = %f Y = %f ",point1.x,point1.y);
    printf("punkt 2 X = %f Y = %f \n",point2.x,point2.y);
}

void generateLines(int state,Point a,Point b,Point c,Point d){
    printf("Line generator");
    switch(state)
    {
        case 1:
            line(c,d);
            break;
        case 2:
            line(b,c);
            break;
        case 3:
            line(b,d);
            break;
        case 4:
            line(a,b);
            break;
        case 5:
            line(a,d);
            line(b,c);
            break;
        case 6:
            line(a,c);
            break;
        case 7:
            line(a,d);
            break;
        case 8:
            line(a,d);
            break;
        case 9:
            line(a,c);
            break;

        case 10:
            line(a,b);
            line(c,d);
            break;
        case 11:
            line(a,b);
            break;
        case 12:
            line(b,d);
            break;

        case 13:
            line(b,c);
            break;

        case 14:
            line(c,d);
            break;
    }
}


void Grid::idw(std::vector<std::vector<Cell>> tab){
    cout<<"algorytm idw"<<endl;
    for(int i=0;i<minx;i++){
        for(int j=0;j<miny;j++){
            double result = 0;
            double result1 = 0;
            double result2 = 0;
            double temp = 0;
            int size = tab[i][j].pointsdistance.size();
            if(size>0) {
                for (int k = 0; k < size; k++) {
                    result1 = result1 + (tab[i][j].pointsdistance[k].point.GetZ() / tab[i][j].pointsdistance[k].distance);
                    result2 = result2 + (1 / tab[i][j].pointsdistance[k].distance);
                    if(tab[i][j].pointsdistance[k].distance == 0){
                        temp = tab[i][j].pointsdistance[k].point.GetZ();
                    }
                }
                result = result1 / result2;
                if(temp != 0){
                    tab[i][j].value = temp;
                }else {
                    tab[i][j].value = result;
                }
            }else{
                neighbours(i,j,tab);
            }
        }
    }

}
int findMin(int a,int b,int c,int d){
    return min(a,min(b,min(c,d)));
}
int getState(int a,int b, int c, int d){
    return a*8+b*4+c*2+d*1;
}
void checkValues(LineCell cell){
    double linediff = 0.5;
    double topl = floor(cell.topleft/linediff);
    double topr = floor(cell.topright/linediff);
    double botl = floor(cell.bottomleft/linediff);
    double botr = floor(cell.bottomright/linediff);
    if(topl == topr && topr == botl && botl == botr){
        ;
    }else{
        int minvalue = findMin(topl,topr,botl,botr);
        int a = ceil(topl/minvalue)-1;
        int b = ceil(topr/minvalue)-1;
        int c = ceil(botl/minvalue)-1;
        int d = ceil(botr/minvalue)-1;
        int result = getState(a,b,c,d);
        generateLines(result,cell.pointa,cell.pointb,cell.pointc,cell.pointd);
    }
}

void Grid::get_center_of_every_cell(liblas::Header header,std::vector<std::vector<Cell>> tab,std::vector<std::vector<LineCell>> arr){
    //134 90
//    double cellsize= 0.5;
    double bigcellsize = cellsize *2;
    int minx = (header.GetMaxX() - header.GetMinX())/cellsize;
    int miny = (header.GetMaxY() - header.GetMinY())/cellsize;
    for(int i=0;i<halfminx;i++){
        for(int j=0;j<halfminy;j++){
            arr[i][j].topleft=tab[i*2][j*2].value;
            arr[i][j].topright=tab[i*2+1][j*2].value;
            arr[i][j].bottomleft=tab[i*2][j*2+1].value;
            arr[i][j].bottomright=tab[i*2+1][j*2+1].value;
            arr[i][j].pointa.x = header.GetMinX() + (i*(bigcellsize)) + (cellsize);
            arr[i][j].pointa.y = header.GetMaxY() - (j*(bigcellsize));
            arr[i][j].pointb.x = header.GetMinX() + (i*(bigcellsize)) + (cellsize*2);
            arr[i][j].pointb.y = header.GetMaxY() - ((j*(bigcellsize))+ (cellsize));
            arr[i][j].pointc.x = header.GetMinX() + (i*(bigcellsize)) + (cellsize);
            arr[i][j].pointc.y = header.GetMaxY() - ((j*(bigcellsize))+ (cellsize*2));
            arr[i][j].pointd.x = header.GetMinX() + (i*(bigcellsize));
            arr[i][j].pointd.y = header.GetMaxY() - ((j*(bigcellsize))+ (cellsize));
            checkValues(arr[i][j]);
        }
    }
}