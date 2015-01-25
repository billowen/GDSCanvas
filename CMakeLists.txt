cmake_minimum_required (VERSION 3.0)
add_definitions(-std=c++11)

project (GDSCanvas)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
add_executable(example main.cpp)

#target_link_libraries(example Qt5:Widgets)
qt5_use_modules(example Widgets)
