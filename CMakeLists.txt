cmake_minimum_required(VERSION 2.8)

project(DisplayDICOMSeries)

find_package(VTK REQUIRED)

include(${VTK_USE_FILE})

add_executable(DisplayDICOMSeries DisplayDICOMSeries.cxx)

target_link_libraries(DisplayDICOMSeries ${VTK_LIBRARIES})