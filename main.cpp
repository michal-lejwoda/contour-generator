#include <iostream>
#include <string>
#include <fstream>
#include <liblas/liblas.hpp>
#include <filesystem>
#include <gdal/gdal.h>
#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h" // for CPLMalloc()
#include "idw.h"
#include "grid.h"
#include "cell.h"
#include "gdal/cpl_string.h"
#include "gdal/ogrsf_frmts.h"
#include <cmath>
double cellsize = 0.5;
int minx;
int miny;
std::vector<std::vector<Cell>> tab;
std::vector<std::vector<LineCell>> arr;
std::vector<Line> array_with_lines;
std::vector<OGRFeature> features;
double isoline_value = 2;
bool compareLine(Line l1, Line l2)
{
    return (l1.value < l2.value);
}
int main() {
    GDALAllRegister();
    cout.precision(15);
    std::ifstream ifs;
//    ifs.open("/home/saxatachi/las_data/points.las", std::ios::in | std::ios::binary);
    ifs.open("/home/saxatachi/las_data/punkty_z_domami.las", std::ios::in | std::ios::binary);
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
    liblas::Header const& header = reader.GetHeader();
    minx = ceil((header.GetMaxX() - header.GetMinX())/cellsize);
    miny = ceil((header.GetMaxY() - header.GetMinY())/cellsize);
    tab = vector<vector<Cell>>( minx , vector<Cell> (miny));
    arr = vector<vector<LineCell>> (minx-1, vector<LineCell>(miny-1));
    clock_t start = clock();
    Grid grid;
    grid.generateGrid(header,reader);
//    grid.distance_beetween_points(header,reader,tab);
//    grid.idw(tab);
//    grid.get_center_of_every_cell(header,tab,array);
//    grid.generateLines(array);
    clock_t end = clock();
    double elapsed = double(end - start)/CLOCKS_PER_SEC;
    printf("Time measured: %.3f seconds.\n", elapsed);


//    sort(array_with_lines.begin(), array_with_lines.end(), compareLine);
//
    GDALDataset *poDS1;
    poDS1 = (GDALDataset*) GDALOpenEx( "/home/saxatachi/Desktop/testaa.shp", GDAL_OF_VECTOR, NULL, NULL, NULL );
    if( poDS1 == NULL )
    {
        printf( "Open failed.\n" );
        exit( 1 );
    }
    OGRLayer  *poLayer1;
    poLayer1 = poDS1->GetLayerByName("testaa");
//    cout<<"name "<<poDS->GetLayer(0)->GetName()<<endl;
//    cout<<"lines"<<poDS->GetLayerCount()<<endl;
    for( auto& poFeature: poLayer1 )
    {
        cout<<"feature"<<endl;
        cout<<poFeature->GetGeometryRef()->exportToJson()<<endl;
    }
    std::vector<Line> temp_array;
    temp_array.push_back(array_with_lines[0]);
    vector <Line>::iterator it;
    it = array_with_lines.begin();
    array_with_lines.erase(it);
    const char *pszDriverName = "ESRI Shapefile";
    GDALDriver *poDriver;
    poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName );
    GDALDataset *poDS;
    poDS = poDriver->Create( "/home/saxatachi/Desktop/testjednejlinii.shp", 0, 0, 0, GDT_Unknown, NULL );
    OGRLayer *poLayer;
    poLayer = poDS->CreateLayer( "line_out", NULL, wkbLineString, NULL );
    OGRFieldDefn oField("Value",OFTString);
    oField.SetWidth(32);
    poLayer->CreateField(&oField);
    while(array_with_lines.size()!= 0) {
        Line var = temp_array.back();
        Line var1 = temp_array.front();
        int temp_size = temp_array.size();
        for (int i = 0; i < array_with_lines.size(); i++) {

            if(abs(var.i - array_with_lines[i].i) <= 1 && abs(var.j - array_with_lines[i].j) <= 1) {
//                cout<<"var i "<<var.i<<endl;
//                cout<<"var j "<<var.j<<endl;
//                cout<<"array_with_Lines i "<<array_with_lines[i].i<<endl;
//                cout<<"array_with_Lines j "<<array_with_lines[i].j<<endl;
//                cout<<"Var points 1"<<var.point1.x<<" "<<var.point1.y<<endl;
//                cout<<"Var points 2"<<var.point2.x<<" "<<var.point2.y<<endl;
//                cout<<"array points 1"<<array_with_lines[i].point1.x<<" "<<array_with_lines[i].point1.y<<endl;
//                cout<<"array points 2"<<array_with_lines[i].point2.x<<" "<<array_with_lines[i].point2.y<<endl;
//                cout<<"test1 = "<<sqrt(pow((var.point1.x-array_with_lines[i].point1.x),2)+pow(var.point1.y-array_with_lines[i].point1.y,2))<<endl;
//                cout<<"test2 = "<<sqrt(pow((var.point1.x-array_with_lines[i].point2.x),2)+pow(var.point1.y-array_with_lines[i].point2.y,2))<<endl;
//                cout<<"test3 = "<<sqrt(pow((var.point2.x-array_with_lines[i].point1.x),2)+pow(var.point2.y-array_with_lines[i].point1.y,2))<<endl;
//                cout<<"test4 = "<<sqrt(pow((var.point2.x-array_with_lines[i].point2.x),2)+pow(var.point2.y-array_with_lines[i].point2.y,2))<<endl;

//                if(sqrt(pow((var.point1.x-array_with_lines[i].point1.x),2)+pow(var.point1.y-array_with_lines[i].point1.y,2))<0.1){
//                    it = array_with_lines.begin() + i;
////                    cout<<"testtt "<<it->point1.x<<" "<<it->point1.y<<" "<<it->point2.x<<it->point2.y<<endl;
////                    cout<<"testt2 "<<array_with_lines[i].point1.x<<" "<<array_with_lines[i].point1.y<<" "<<array_with_lines[i].point2.x<<array_with_lines[i].point2.y<<endl;
//                    temp_array.push_back(array_with_lines[i]);
//                    array_with_lines.erase(it);
//                    break;
//                }
//                if(sqrt(pow((var.point1.x-array_with_lines[i].point2.x),2)+pow(var.point1.y-array_with_lines[i].point2.y,2))<0.1){
//                    Point temp_var = array_with_lines[i].point2;
//                    array_with_lines[i].point2 = array_with_lines[i].point1;
//                    array_with_lines[i].point1 = temp_var;
//                    it = array_with_lines.begin() + i;
//                    temp_array.push_back(array_with_lines[i]);
//                    int tescik1 = temp_array.size();
//                    array_with_lines.erase(it);
//                    break;
//                }
//                if(sqrt(pow((var.point2.x-array_with_lines[i].point1.x),2)+pow(var.point2.y-array_with_lines[i].point1.y,2))<0.1){
//                    it = array_with_lines.begin() + i;
//                    temp_array.insert(temp_array.begin(),1,array_with_lines[i]);
//                    array_with_lines.erase(it);
//                    break;
//                }
//                if(sqrt(pow((var.point2.x-array_with_lines[i].point2.x),2)+pow(var.point2.y-array_with_lines[i].point2.y,2))<0.1){
//                    Point temp_var = array_with_lines[i].point2;
//                    array_with_lines[i].point2 = array_with_lines[i].point1;
//                    array_with_lines[i].point1 = temp_var;
//                    it = array_with_lines.begin() + i;
//                    temp_array.insert(temp_array.begin(),1,array_with_lines[i]);
//                    int tescik3 = temp_array.size();
//                    array_with_lines.erase(it);
//                    break;
//                }
                if (sqrt(pow((var.point2.x - array_with_lines[i].point1.x), 2) +
                         pow(var.point2.y - array_with_lines[i].point1.y, 2)) < 0.05) {
                    it = array_with_lines.begin() + i;
                    temp_array.push_back(array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
                if (sqrt(pow((var.point2.x - array_with_lines[i].point2.x), 2) +
                         pow(var.point2.y - array_with_lines[i].point2.y, 2)) < 0.05) {
                    Point temp_var = array_with_lines[i].point2;
                    array_with_lines[i].point2 = array_with_lines[i].point1;
                    array_with_lines[i].point1 = temp_var;
                    it = array_with_lines.begin() + i;
                    temp_array.push_back(array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
                if(sqrt(pow((var1.point1.x-array_with_lines[i].point1.x),2)+pow(var1.point1.y-array_with_lines[i].point1.y,2))<0.05){
                    Point temp_var = array_with_lines[i].point2;
                    array_with_lines[i].point2 = array_with_lines[i].point1;
                    array_with_lines[i].point1 = temp_var;
                    it = array_with_lines.begin() + i;
                    temp_array.insert(temp_array.begin(),1,array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
                if(sqrt(pow((var1.point1.x-array_with_lines[i].point2.x),2)+pow(var1.point1.y-array_with_lines[i].point2.y,2))<0.05){
                    it = array_with_lines.begin() + i;
                    temp_array.insert(temp_array.begin(),1,array_with_lines[i]);
                    array_with_lines.erase(it);
                    break;
                }
            }
        }

//        cout<<"temp_size"<<temp_size<<endl;
//        cout<<"temp_array_size"<<temp_array.size()<<endl;
        if(temp_size == temp_array.size()){
            cout<<"Wielkość tablicy"<< temp_array.size()<<endl;
            OGRLineString ls;
            OGRFeature *poFeature3;
            poFeature3 = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
            poFeature3->SetField( "Value", temp_array[0].value);
            for(int i=0;i<temp_array.size();i++){
                ls.addPoint(temp_array[i].point1.x,temp_array[i].point1.y);
                ls.addPoint(temp_array[i].point2.x,temp_array[i].point2.y);
            }
            poFeature3->SetGeometry(&ls);
            poLayer->CreateFeature(poFeature3);
            cout<<poFeature3->GetGeometryRef()->exportToJson()<<endl;

            cout<<"Liczba "<<poLayer->GetFeatureCount()<<endl;
            cout<<ls.exportToJson()<<endl;
            temp_array.clear();
            if(array_with_lines.size()>0) {
                temp_array.push_back(array_with_lines[0]);
                it = array_with_lines.begin();
                array_with_lines.erase(it);
            }
        }
    }









//    const char *pszDriverName = "ESRI Shapefile";
//    GDALDriver *poDriver;
//    poDriver = GetGDALDriverManager()->GetDriverByName(pszDriverName );
//    GDALDataset *poDS;
//    poDS = poDriver->Create( "/home/saxatachi/Desktop/testjednejlinii.shp", 0, 0, 0, GDT_Unknown, NULL );
//    OGRLayer *poLayer;
//    poLayer = poDS->CreateLayer( "line_out", NULL, wkbLineString, NULL );
//    OGRFieldDefn oField("Value",OFTString);
//    oField.SetWidth(32);
//    poLayer->CreateField(&oField);
//    OGRFeature *poFeature;
//    OGRFeature *poFeature1;
//    poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
//    poFeature1 = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
//    poLayer->SetFeature(poFeature);
////    poLayer->SetFeature(poFeature1);
//    OGRMultiLineString mls;
//    OGRMultiLineString mls2;



//    OGRLineString ls;
//    for(int i=0;i<temp_array.size();i++){
//        OGRFeature *poFeature2;
//        poFeature2 = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
//        poLayer->SetFeature(poFeature2);
//        cout<<"ile "<<temp_array[i].point1.x <<" "<<temp_array[i].point1.y<<" "<<temp_array[i].point2.x <<" "<<temp_array[i].point2.y<<endl;
//        if(i-1>0){
//            if(temp_array[i-1].point1.x == temp_array[i].point1.x && temp_array[i-1].point1.y == temp_array[i].point1.y){
//                ls.addPoint(temp_array[i].point1.x,temp_array[i].point1.y);
//                ls.addPoint(temp_array[i].point2.x,temp_array[i].point2.y);
//            }else{
//                ls.addPoint(temp_array[i].point2.x,temp_array[i].point2.y);
//                ls.addPoint(temp_array[i].point1.x,temp_array[i].point1.y);
//            }
//        }
////        ls.addPoint(array_with_lines[i].point1.x,array_with_lines[i].point1.y);
////        ls.addPoint(array_with_lines[i].point2.x,array_with_lines[i].point2.y);
//        mls2.addGeometry(&ls);
//        poFeature2->SetGeometry(&mls2);
//
////        cout<<"x i y = "<<temp_array[i].point1.x<<" "<<temp_array[i].point1.y<<endl;
////        cout<<"x2 i y2 = "<<temp_array[i].point2.x<<" "<<temp_array[i].point2.y<<endl;
//    }

//    for(int i=0;i<array_with_lines.size();i++){
//
//        OGRFeature *poFeature2;
//        poFeature2 = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
//        poLayer->SetFeature(poFeature2);
//        OGRLineString ls;
//        ls.addPoint(array_with_lines[i].point1.x,array_with_lines[i].point1.y);
//        ls.addPoint(array_with_lines[i].point2.x,array_with_lines[i].point2.y);
//        mls2.addGeometry(&ls);
//        poFeature2->SetGeometry(&mls2);
////        cout<<"Feature2"<< poFeature2->GetGeometryRef()->exportToJson()<<endl;
//
//        OGRFeature::DestroyFeature( poFeature2 );
//        poFeature1->SetField( "Value", array_with_lines[i].value);
////        poLayer->CreateFeature(poFeature1);
//    }
//    poFeature1->SetGeometry(&ls);
//    poFeature->SetGeometry(&mls);
//    poFeature1->SetGeometry(&mls2);
//    cout<<"Feature2"<< poFeature2->GetGeometryRef()->exportToJson()<<endl;

//    cout<<"OField "<<oField.GetFieldTypeName<<endl;
//    poLayer->CreateFeature(poFeature);
//    poLayer->CreateFeature(poFeature1);
//    cout<<"gtf " <<poLayer->GetFeatureCount()<<endl;

//    ls.addPoint(64,45);
//    poFeature->SetGeometry(&ls);

//    if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
//    {
//        printf( "Failed to create feature in shapefile.\n" );
//        exit( 1 );
//    }
//    cout<<"GEt Features"<<poDS->GetFeatures()<<endl;
//    cout<<poFeature->GetGeometryRef()->exportToJson()<<endl;
//    cout<<poFeature1->GetGeometryRef()->exportToJson()<<endl;

//    OGRFeature::DestroyFeature( poFeature );
//    OGRFeature::DestroyFeature( poFeature1 );
//    OGRFeature::DestroyFeature( poFeature2 );
    GDALClose( poDS );

















//    poDS = (GDALDataset*) GDALOpenEx( "/home/saxatachi/Desktop/line.shp", GDAL_OF_VECTOR, NULL, NULL, NULL );
//    OGRLayer  *poLayer = poDS->GetLayerByName( "line" );
//    OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();
//    poLayer->ResetReading();
//    OGRFeature *poFeature;
//    while( (poFeature = poLayer->GetNextFeature()) != NULL )
//    {
//        cout<<"wynik "<<poFeature->GetFieldAsDouble(0)<<endl;
//        cout<<"wynik2 "<<poFeature->GetFieldAsDouble(1)<<endl;
//        cout<<"wynik3 "<<poFeature->GetGeomFieldRef(1)<<endl;
//        cout<<poFeature->GetGeometryRef()->getGeometryType()<<endl;
//        cout<<"kk "<<poFeature->GetGeometryRef()<<endl;
//        cout<<"wynik4 "<<poFeature->GetGeometryRef()->getCurveGeometry()<<endl;
//        cout<<"MultiLineString "<<wkbMultiLineString<<endl;
//        cout<<"LineString "<<wkbLineString<<endl;
//        OGRLineString *poLine = poFeature->GetGeometryRef()->toLineString();
//        cout<<"poline "<<poLine->exportToJson()<<endl;
//        char const *p = (char*) malloc (sizeof(char)*40);
//        char **tablicaa = new char * [0];
//        cout<<"poline wkt "<<poLine->exportToWkt(tablicaa)<<endl;
//        cout<<"poline wkt "<<poLine->exportToKML()<<endl;
//        for( int iField = 0; iField < poFDefn->GetFieldCount(); iField++ )
//        {
//            OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );

//            switch( poFieldDefn->GetType() )
//            {
//                case OFTInteger:
//                    printf( "%d,", poFeature->GetFieldAsInteger( iField ) );
//                    break;
//                case OFTInteger64:
//                    printf( CPL_FRMT_GIB ",", poFeature->GetFieldAsInteger64( iField ) );
//                    break;
//                case OFTReal:
//                    printf( "%.3f,", poFeature->GetFieldAsDouble(iField) );
//                    break;
//                case OFTString:
//                    printf( "%s,", poFeature->GetFieldAsString(iField) );
//                    break;
//                default:
//                    printf( "%s,", poFeature->GetFieldAsString(iField) );
//                    break;
//            }
//        }

//        OGRGeometry *poGeometry = poFeature->GetGeometryRef();
//        cout<<wkbFlatten(poGeometry->getGeometryType())<<endl;
//        cout<<"wkbLineString = "<<wkbLineString<<endl;


//        OGRMultiLineString *poLine = (OGRMultiLineString *) poGeometry;
//        OGRLineString *testLine = (OGRLineString *)poFeature->getGeometryRef(0);
//        cout<<"area "<< testLine->get_Area()<<endl;
//            printf( "%.3f,%3.f\n", poPoint->getX(), poPoint->getY() );
//            poLine->


//        OGRFeature::DestroyFeature( poFeature );
//    }

    //    poDS = poDriver->Create( "point_out.shp", 0, 0, 0, GDT_Unknown, NULL );
//    if( poDS == NULL )
//    {
//        printf( "Creation of output file failed.\n" );
//        exit( 1 );
//    }


    //tworzenie rastra
//    GDALDataset  *poDataset,*pNewDS;
//    GDALDriver *pDriverTiff;
//    char const * pszFilename = "/home/saxatachi/Desktop/inter_raster.tif";
//    char const * output = "/home/saxatachi/Desktop/neww.tif";
////    GDALAllRegister();
//    poDataset = (GDALDataset *) GDALOpen( pszFilename, GA_ReadOnly );
//    if( poDataset == NULL )
//    {
//        cout<<"null"<<endl;
//    }else{
//
//        int nRows, nCols;
//        int rows = 268;
//        int columns = 180;
//        double noData;
//        double transform[6];
//        nCols = poDataset->GetRasterBand(1)->GetXSize();
//        nRows = poDataset->GetRasterBand(1)->GetYSize();
//        noData = poDataset->GetRasterBand(1)->GetNoDataValue();
//        cout<<"nCols "<<nCols<<endl;
//        cout<<"nRows "<<nRows<<endl;
//        poDataset->GetGeoTransform(transform);
//        transform[1] = 0.5;
//        transform[5] = 0.5;
//        for(int i=0;i<6;i++){
//            cout<<"element"<<transform[i]<<endl;
//        }
//        pDriverTiff = GetGDALDriverManager()->GetDriverByName("GTiff");
//        pNewDS = pDriverTiff->Create(output,rows,columns,1,GDT_Float32,NULL);
//        pNewDS->SetGeoTransform(transform);
//        float *oldRow = (float*) CPLMalloc(sizeof(float)*nCols);
//        float *newRow = (float*) CPLMalloc(sizeof(float)*nCols);
//        cout<<"alokowanie"<<endl;
//        float *newwRow = (float*) CPLMalloc(sizeof(float)*columns);


//        for(int i=0;i<nRows;i++){
//            poDataset->GetRasterBand(1)->RasterIO(GF_Read,0,i,nCols,1,oldRow,nCols,1,GDT_Float32,0,0);
//            for(int j=0;j<nCols;j++){
//                if(oldRow[j] == noData){
//                    newRow[j] = noData;
//                }else{
////                    cout<<oldRow[j]<<endl;
//                    newRow[j] = oldRow[j] + 10;
//                }
//            }
//            pNewDS->GetRasterBand(1)->RasterIO(GF_Write,0,i,nCols,1,newRow,nCols,1,GDT_Float32,0,0);
//        }

// Uzupełnianie komórek rastra
//        for(int i=0;i<rows;i++){
//            for(int j=0;j<columns;j++){
//                newwRow[j] = tab[i][j].value;
//            }
//            pNewDS->GetRasterBand(1)->RasterIO(GF_Write,0,i,rows,1,newwRow,columns,1,GDT_Float32,0,0);
//        }

//Zamkniecie rastra
//        GDALClose(poDataset);
//        GDALClose(pNewDS);
//        GDALDestroyDriverManager();
//    }


    //    GDALDataset  *poDataset;
//    char const *p = "/home/saxatachi/las_data/raster.tif";
//    poDataset = (GDALDataset *) GDALOpen( p, GA_ReadOnly );
//    std::cout<<poDataset->GetDriverName()<<std::endl;
//    std::cout<<poDataset->GetBands().size()<<std::endl;
//    double adfGeoTransform[6];
//    if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None )
//    {
//        printf( "Origin = (%.6f,%.6f)\n",
//                adfGeoTransform[0], adfGeoTransform[3] );
//        printf( "Pixel Size = (%.6f,%.6f)\n",
//                adfGeoTransform[1], adfGeoTransform[5] );
//    }
//    double adfGeoTransform2[8];
//    std::cout<<adfGeoTransform2<<std::endl;
//    if( poDataset->GetGeoTransform( adfGeoTransform2 ) == CE_None )
//    {
//        printf( "Origin = (%.6f,%.6f)\n",
//                adfGeoTransform2[0], adfGeoTransform2[3] );
//        printf( "Pixel Size = (%.6f,%.6f)\n",
//                adfGeoTransform2[1], adfGeoTransform2[5] );
//    }
//    const char *pszFormat = "GTiff";
//    GDALDriver *poDriver;
//    char **papszMetadata;
//    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
//    if( poDriver == NULL )
//        exit( 1 );
//    papszMetadata = poDriver->GetMetadata();
//    if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
//        printf( "Driver %s supports Create() method.\n", pszFormat );
//    if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATECOPY, FALSE ) )
//        printf( "Driver %s supports CreateCopy() method.\n", pszFormat );
//    GDALDataset *poDstDS;
//    GDALDriver *poDriver;
//    char const *z = "/home/saxatachi/las_data/raster12";
//    char **papszOptions = NULL;
//
//    poDstDS = poDriver->Create( z, 512, 512, 1, GDT_Byte,NULL);
//    GDALClose(poDstDS);
//    ifs.close();
    return 0;

}

