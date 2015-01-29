cmake_minimum_required (VERSION 3.0)
add_definitions(-std=c++11)

project (GDSCanvas)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Gui REQUIRED)
add_executable(example main.cpp)
ADD_LIBRARY(GDSCanvas
	canvas.cpp canvas.h
	boundaryitem.cpp boundaryitem.h
	#textitem.cpp textitem.h
	pathitem.cpp pathitem.h
	)
ADD_SUBDIRECTORY(GDS)
#target_link_libraries(example Qt5:Widgets)
qt5_use_modules(example Widgets)
qt5_use_modules(example Gui)
qt5_use_modules(GDSCanvas Widgets)
TARGET_LINK_LIBRARIES(GDSCanvas libGDS)
TARGET_LINK_LIBRARIES(example GDSCanvas)
