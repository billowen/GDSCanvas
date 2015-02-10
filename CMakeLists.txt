cmake_minimum_required (VERSION 2.8)
add_definitions(-std=c++11)

project (GDSCanvas)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

find_package(Qt5Widgets)
add_executable(example main.cpp)
add_library(GDSCanvas
	canvas.cpp canvas.h
	boundaryitem.cpp boundaryitem.h
	pathitem.cpp pathitem.h
	)
add_subdirectory(GDS)
target_link_libraries(GDSCanvas libGDS)
target_link_libraries(GDSCanvas Qt5::Widgets)
target_link_libraries(example Qt5::Widgets)
target_link_libraries(example GDSCanvas)
