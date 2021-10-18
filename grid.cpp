#include<iostream>
#include "grid.h"
#include "cell.h"
using namespace std;

void Grid::generateGrid(liblas::Header header,liblas::Reader reader,Cell tab[391][307]){
    float cellsize = 0.5;
    float R = (cellsize * sqrt(2))/2;
    cout<<"R"<<R<<endl;
    cout<<"generate grid"<<endl;
    int minx = (header.GetMaxX() - header.GetMinX())/0.5;
    int miny = (header.GetMaxY() - header.GetMinY())/0.5;

    cout.precision(15);
    cout<<"wynik max x" <<header.GetMaxX()<<endl;
    cout<<"wynik min x" <<header.GetMinX()<<endl;
    cout<<"wynik max y "<< header.GetMaxY()<<endl;
    cout<<"wynik min y "<< header.GetMinY()<<endl;
    for(int i=0;i<miny;i++){
        for(int j=0;j<minx;j++){
            float centerx;
            float centery;
            centerx = header.GetMinX() + (i*(cellsize)) + (cellsize/2);
            centery = header.GetMaxY() - ((j*(cellsize))+ (cellsize/2));
            tab[i][j].centerx = centerx;
            tab[i][j].centery = centery;
        }
    }
    reader.ReadPointAt(2);
    liblas::Point const& p = reader.GetPoint();
    cout<<"minx = "<<minx<<endl;
    cout<<"miny = "<<miny<<endl;
    distance_beetween_points(reader,tab);
    idw(tab);
    cout<<"Generated grid"<<endl;
}

void Grid::distance_beetween_points(liblas::Reader reader,Cell tab[391][307]){
    cout.precision(100);
    float R = (0.5 * sqrt(2))/2;
    cout<<"distance"<<endl;
    cout<<tab[0][0].centerx<<endl;
    float min = 50;
    reader.ReadPointAt(2);
    liblas::Point const& p = reader.GetPoint();
    int count = 0;

//    for(int i=0;i<391;i++){
//        for(int j=0;j<307;j++){
//            cout<<"x = "<<tab[i][j].centerx<<endl;
//            cout<<"y = "<<tab[i][j].centery<<endl;
//
//        }
//    }
    while (reader.ReadNextPoint())
    {
        if(p.GetClassification().GetClass() == 0 or p.GetClassification().GetClass() == 2){

        }else{
            continue;
        }
        liblas::Point p = reader.GetPoint();
        cout<<"klasyfikacja"<<unsigned(p.GetClassification().GetClass())<<endl;
//        cout<<p.GetHeader()->GetExtent()<<endl;

        float result = sqrt(pow((tab[24][12].centerx)-p.GetX(),2)+pow((tab[24][12].centery)-p.GetY(),2));
        if(result<=R){
            cout<<"result "<<result<<endl;
            tab[24][12].points.push_back(p);
            tab[24][12].distance.push_back(result);
        }
//        for(int i=0;i<391;i++){
//            for(int j=0;j<307;j++){
//                float result = sqrt(pow((tab[i][j].centerx)-p.GetX(),2)+pow((tab[i][j].centery)-p.GetY(),2));
////                cout<<"rezultat"<<result<<endl;
//                if(result<=R){
//                    tab[i][j].points.push_back(p);
//                    continue;
//                }
//            }
//        }
//        float result = sqrt(pow((tab[0][0].centerx)-p.GetX(),2)+pow((tab[0][0].centery)-p.GetY(),2));
//        if(result<R){
//            cout<<
//        }
//        cout<<"result "<<result<<endl;
        count++;

    }
    cout<<"liczba elementow w tab = "<<tab[24][12].points.size()<<endl;
}
void Grid::idw(Cell tab[391][307]){
    cout<<"algorytm idw"<<endl;
    float result = 0;
    float result1 = 0;
    float result2 = 0;
    int size = tab[24][12].points.size();
    for(int i=0;i<size;i++){
        cout<<tab[24][12].points[i].GetZ()<<endl;
        cout<<"odleglosc "<<tab[24][12].distance[i]<<endl;
//        ((tab[24][12].points[i].GetZ()/tab[24][12].distance[i]))
        result1 = result1 +(tab[24][12].points[i].GetZ()/tab[24][12].distance[i]);
        result2 = result2 + (1/tab[24][12].distance[i]);
    }
    result = result1/result2;
    tab[24][12].value = result;
    cout<<"wynik"<<result<<endl;
}