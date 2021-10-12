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
int main() {
    GDALAllRegister();
    std::cout << "Hello, World!" << std::endl;
    std::ifstream ifs;
//    ifs.open("/home/saxatachi/las_data/lidar.laz", std::ios::in | std::ios::binary);
    ifs.open("/home/saxatachi/las_data/points.las", std::ios::in | std::ios::binary);
    liblas::ReaderFactory f;
    liblas::Reader reader = f.CreateWithStream(ifs);
//    liblas::Reader reader;
    std::cout << "Hello, World!" << std::endl;
//    std::cout << "Hello, World!" << std::endl;
    liblas::Header const& header = reader.GetHeader();
//    float Cellsize = 1.5;
//    std::cout << "Compressed: " << (header.Compressed() == true) ? "true":"false";
//    std::cout << "Signature: " << header.GetFileSignature() << '\n';
//    std::cout << "Points count: " << header.GetPointRecordsCount() << '\n';
//    std::cout << "Min x "<< header.GetMinX()<<std::endl;
//    std::cout << "Max x "<< header.GetMaxX()<<std::endl;
//    std::cout << std::fixed <<"Min y " << header.GetMinY()<<std::endl;
//    std::cout << std::fixed << "Max y " << header.GetMaxY()<<std::endl;
//    std::cout << std::fixed <<"Min z " << header.GetMinZ()<<std::endl;
//    std::cout << std::fixed << "Max z " << header.GetMaxZ()<<std::endl;
//    Test test;
//    test.wczytaj();
    cout<<"lidar x"<<header.GetExtent().minx()<<" "<<header.GetExtent().maxx()<<endl;
    int minx = header.GetMaxX() - header.GetMinX();
    int miny = header.GetMaxY() - header.GetMinY();
    int result = minx*miny;
    Cell tab[result];
    cout<<"array"<<sizeof(tab)<<endl;
//    cout<<"minx*miny"<<minx*miny<<endl;
// To jest ważne
    Grid grid;
    grid.generateGrid(header,tab);
    //    GDALDataset  *poDataset;
    Cell cell={
            10.0,
            12.0,
            14.0,
            16.0
    };
    tab[0]=cell;
    tab[1]=cell;
    cout<<"tab"<<tab<<endl;


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
////    poDstDS = poDriver->Create( z, 512, 512, 1, GDT_Byte,NULL);
//    GDALClose(poDstDS);


    //    std::cout<<"test"<<std::endl;
//    if( poDstDS == NULL )
//    {
//        std::cout<<"Nie działa"<<std::endl;
//    }else{
//        std::cout<<"Działa"<<std::endl;
//    }

    ifs.close();
    return 0;

}
