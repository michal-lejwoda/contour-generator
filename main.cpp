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
double cellsize = 0.5;
int minx;
int miny;
int halfminx;
int halfminy;
std::vector<std::vector<Cell>> tab;
std::vector<std::vector<LineCell>> arr;
std::vector<Line> array_with_lines;
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
    halfminx = minx/2;
    halfminy = miny/2;
    tab = vector<vector<Cell>>( minx , vector<Cell> (miny));
    arr = vector<vector<LineCell>> (halfminx, vector<LineCell>(halfminy));

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
