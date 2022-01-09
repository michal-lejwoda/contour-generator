//openmp

//int nthreads, tid;
//
//    // Begin of parallel region
//#pragma omp parallel private(nthreads, tid)
//    {
//        // Getting thread number
//        tid = omp_get_thread_num();
//        printf("Welcome to GFG from thread = %d\n",
//               tid);
//
//        if (tid == 0) {
//
//            // Only master thread does this
//            nthreads = omp_get_num_threads();
//            printf("Number of threads = %d\n",
//                   nthreads);
//        }
//    }


//
// Created by saxatachi on 08.01.2022.
//

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
//                newwRow[j] = cell_array[i][j].value;
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

