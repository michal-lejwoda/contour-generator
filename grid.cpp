#include<iostream>
#include "grid.h"
#include "cell.h"
using namespace std;
void Grid::generateGrid(liblas::Header header,Cell tab[]){
    float cellsize = 0.5;
    cout<<"generate grid"<<endl;
//    cout<<header.GetMinX()<<endl;
//    cout<<header.GetMaxX()<<endl;
//    cout<<header.GetMinY()<<endl;
//    cout<<header.GetMaxY()<<endl;
    int minx = (header.GetMaxX() - header.GetMinX())/0.5;
    int miny = (header.GetMaxY() - header.GetMinY())/0.5;
//    cout<<"wynik x "<< minx<<endl;
//    cout<<"wynik y "<< miny<<endl;
//    cout<<"minx"<<minx<<endl;
    cout.precision(15);
//    cout<<"wynik max x" <<header.GetMaxX()<<endl;
//    cout<<"wynik min x" <<header.GetMinX()<<endl;
//    cout<<"wynik max y "<< header.GetMaxY()<<endl;
//    cout<<"wynik min y "<< header.GetMinY()<<endl;
//    cout<<"sadasd "<<header.GetMinX() + cellsize/2<<endl;
    int count = 0;
    for(int i=0;i<miny;i++){
        if(i==6){
            break;
        }
        for(int j=0;j<minx;j++){
            cout<<"tab["<<i<<"]"<<"["<<j<<"]"<<endl;
            cout<<"x = "<<(i*cellsize)<<endl;
            cout<<"y = "<<(j*cellsize)<<endl;
            cout<<"center x = "<<header.GetMinX() + (i*(cellsize)) + (cellsize/2)<<endl;
            cout<<"center y = "<<header.GetMaxY() - ((j*(cellsize))+ (cellsize/2))<<endl;
//            Cell cell={
//                    10.0,
//                    12.0,
//                    14.0,
//                    16.0
//            };
//            tab[count] = cell;
            count++;

        }
    }
    cout<<"size"<<sizeof(tab)<<endl;
    for(int el=0;el<sizeof(tab);el++){
        cout<<tab[el].maxx<<endl;
    }
    cout<<count<<endl;
    cout<<"Generated grid"<<endl;
}
