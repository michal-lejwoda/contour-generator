cmake_minimum_required(VERSION 3.16)
project(master)
find_package(libLAS REQUIRED)
find_package(GDAL REQUIRED)
find_package(OpenMP REQUIRED)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
add_executable(master main.cpp grid.h grid.cpp structures.h unused.cpp)

target_link_libraries (master ${libLAS_LIBRARIES} ${GDAL_LIBRARIES})

