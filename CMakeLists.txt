cmake_minimum_required (VERSION 2.8)
add_definitions(-std=c++11)

project (GDSCanvas)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

find_package(Qt4 REQUIRED QtGui)
add_executable(example main.cpp)
ADD_LIBRARY(GDSCanvas
	canvas.cpp canvas.h
	boundaryitem.cpp boundaryitem.h
	pathitem.cpp pathitem.h
	)
ADD_SUBDIRECTORY(GDS)
TARGET_LINK_LIBRARIES(GDSCanvas libGDS)
target_link_libraries(GDSCanvas Qt4::QtGui)
target_link_libraries(example Qt4::QtGui)
TARGET_LINK_LIBRARIES(example GDSCanvas)
