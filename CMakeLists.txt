cmake_minimum_required (VERSION 2.8.11)
add_definitions(-std=c++11)

option(TESTGDSCANVAS "Whether to build the test exectable or not." OFF)

project (GDSCanvas)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

set (headers
    canvas.h
    boundaryitem.h 
    pathitem.h 
    srefitem.h
    gadgets.h
    )
set (src 
    canvas.cpp
    boundaryitem.cpp
    pathitem.cpp
    srefitem.cpp
    gadgets.cpp
    )

find_package(Qt5Widgets)
find_package(Qt5Gui)

add_library(libGDSCanvas ${src} ${headers})
target_link_libraries(libGDSCanvas libGDS libGDSGadgets)
target_link_libraries(libGDSCanvas Qt5::Widgets Qt5::Gui)


if (TESTGDSCANVAS)
    add_executable(testGDSCanvas main.cpp)
    target_link_libraries(testGDSCanvas Qt5::Widgets Qt5::Gui)
    target_link_libraries(testGDSCanvas libGDSCanvas)
else()
endif()
