#include<iostream>
#include "grid.h"
#include "cell.h"
using namespace std;

void Grid::generateGrid(liblas::Header header,liblas::Reader reader,Cell tab[268][180]){
    cout.precision(15);
    double cellsize = 0.5;
    double R = (cellsize * sqrt(2))/2;
    cout<<"generate grid"<<endl;
    int minx = (header.GetMaxX() - header.GetMinX())/0.5;
    int miny = (header.GetMaxY() - header.GetMinY())/0.5;

    for(int i=0;i<minx;i++){
        for(int j=0;j<miny;j++){
            tab[i][j].centerx = header.GetMinX() + (i*(cellsize)) + (cellsize/2);
            tab[i][j].centery = header.GetMaxY() - ((j*(cellsize))+ (cellsize/2));
        }
    }
}

void Grid::distance_beetween_points(liblas::Header header,liblas::Reader reader,Cell tab[268][180]){
    cout.precision(15);
    double cellsize = 0.5;
    double R = (cellsize * sqrt(2))/2;
    reader.ReadPointAt(1002);


    liblas::Point const& r = reader.GetPoint();
    int x = floor((r.GetX() - header.GetMinX())/cellsize);
    int y = floor((header.GetMaxY() - r.GetY())/cellsize);
    cout<<"minus x"<<x<<endl;
    cout<<"minus y"<<y<<endl;
    double result = sqrt(pow((tab[x][y].centerx)-r.GetX(),2)+pow((tab[x][y].centery)-r.GetY(),2));
    cout<<tab[x][y].centerx - r.GetX()<<endl;
    cout<<tab[x][y].centery - r.GetY()<<endl;
    if(result>R){
        cout<<"Jest Większy"<<endl;
        for(int el=1;el<=ceil(1/cellsize);el++){
            for(int el2=1;el2<=ceil(1/cellsize);el2++){
                float result2 = sqrt(pow((tab[x+el][y+el2].centerx)-r.GetX(),2)+pow((tab[x+el][y+el2].centery)-r.GetY(),2));
                if(result2<=R){
                    cout<<"wynik w poprawnym for"<<result2<<"tab["<<x+el<<"]["<<y+el2<<"]"<<endl;
                    break;
                }
            }
        }
    }else{
        cout<<"wynik w pierwszym"<<result<<"tab["<<x<<"]["<<y<<"]"<<endl;
    }


    liblas::Point const& p = reader.GetPoint();
    int count = 0;
    while (reader.ReadNextPoint()) {
//        if (p.GetClassification().GetClass() == 0 or p.GetClassification().GetClass() == 2) {
//            count++;
//        } else {
//            continue;
//        }
        int x = floor((p.GetX() - header.GetMinX()) / cellsize);
        int y = floor((header.GetMaxY() - p.GetY()) / cellsize);
        if(x == 268){
            x= 267;
        }
        double result = sqrt(pow((tab[x][y].centerx) - p.GetX(), 2) + pow((tab[x][y].centery) - p.GetY(), 2));
//        if (result > R) {
//            cout<<x<<" oraz "<<y<<endl;
//            for (int el = 1; el <= ceil(1 / cellsize); el++) {
//                cout<<"el"<<el<<endl;
//                for (int el2 = 1; el2 <= ceil(1 / cellsize); el2++) {
//                    if(x+el>268 or y+el2>180){
//                        cout<<"jest Większe"<<endl;
//                        continue;
//                    }
//                    float result2 = sqrt(pow((tab[x + el][y + el2].centerx) - p.GetX(), 2) +
//                                         pow((tab[x + el][y + el2].centery) - p.GetY(), 2));
//                    if (result2 <= R) {
//                        cout << "wynik w poprawnym for" << result2 << "tab[" << x + el << "][" << y + el2 << "]"
//                             << endl;
//                        cout<<x + el<<" oraz "<<y + el2<<endl;
//                        tab[x + el][y + el2].points.push_back(p);
//                        tab[x + el][y + el2].distance.push_back(result2);
//                        break;
//                    }
//                }
//            }
//        } else {
            cout<<"x = "<<x<<endl;
            cout<<"y = "<<y<<endl;
            tab[x][y].points.push_back(p);
            tab[x][y].distance.push_back(result);
//        }
//        cout<<"count = "<<count<<endl;
    }
//    cout<<"liczba elementow w tab = "<<tab[56][76].points.size()<<endl;
}
void Grid::idw(Cell tab[268][180]){
    cout<<"algorytm idw"<<endl;
    cout<<"tab[268][179]"<<tab[178][80].points.size()<<endl;
    for(int i=0;i<268;i++){
        for(int j=0;j<180;j++){
            cout<<"tab["<<i<<"]"<<"["<<j<<"]"<<tab[i][j].points.size()<<endl;
            double result = 0;
            double result1 = 0;
            double result2 = 0;
            int size = tab[i][j].points.size();
            cout<<"size = "<<size<<endl;
            if(size>0) {
                for (int k = 0; k < size; k++) {
//                cout<<tab[i][j].points[k].GetZ()<<endl;
                    result1 = result1 + (tab[i][j].points[k].GetZ() / tab[i][j].distance[k]);
                    result2 = result2 + (1 / tab[i][j].distance[k]);
                }
                result = result1 / result2;
                tab[i][j].value = result;
                cout << "tab[" << i << "]" << "[" << j << "] = " << result << endl;
            }else{
                tab[i][j].value = 75;
                cout << "tab[" << i << "]" << "[" << j << "] = " << 0 << endl;
            }
        }
    }
//    for(int i=0;i<size;i++){
//        cout<<tab[24][12].points[i].GetZ()<<endl;
//        cout<<"odleglosc "<<tab[24][12].distance[i]<<endl;
//        ((tab[24][12].points[i].GetZ()/tab[24][12].distance[i]))
//        result1 = result1 +(tab[24][12].points[i].GetZ()/tab[24][12].distance[i]);
//        result2 = result2 + (1/tab[24][12].distance[i]);
//    }
//    result = result1/result2;
//    tab[24][12].value = result;
//    cout<<"wynik"<<result<<endl;
}
