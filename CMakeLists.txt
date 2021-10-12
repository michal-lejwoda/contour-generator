cmake_minimum_required(VERSION 3.20)
project(master)
find_package(libLAS REQUIRED)
find_package(GDAL REQUIRED)
set(CMAKE_CXX_STANDARD 17)

add_executable(master main.cpp idw.h idw.cpp grid.h grid.cpp cell.h cell.cpp)

target_link_libraries (master ${libLAS_LIBRARIES} ${GDAL_LIBRARIES})

